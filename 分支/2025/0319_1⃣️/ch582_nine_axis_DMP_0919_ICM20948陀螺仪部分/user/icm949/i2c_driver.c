/********************************** (C) COPYRIGHT *******************************
 * File Name          : app_i2c.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2022/11/04
 * Description        : 
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "i2c_driver.h"
#include "CH58x_common.h"

/**
 * Note: ������ӻ���DEBUG�ӿ���Ҫͬʱ�򿪻�رգ�
 * ��������ʱ�����⡣
 */
// #define CONFIG_I2C_DEBUG

#ifdef CONFIG_I2C_DEBUG
#define I2C_DBG(...)    PRINT(__VA_ARGS__)
#else
#define I2C_DBG(...)
#endif
static volatile uint8_t i2c_state;
static volatile uint8_t i2c_slave_addr_rw;
static volatile uint8_t i2c_send_stop;  // should the transaction end with a stop
static volatile uint8_t i2c_in_repstart;     // in the middle of a repeated start

static uint8_t i2c_master_buffer[I2C_BUFFER_LENGTH];
static volatile uint8_t i2c_master_buffer_index;
static uint8_t i2c_master_buffer_length;

static uint8_t i2c_slave_txbuffer[I2C_BUFFER_LENGTH];
static volatile uint8_t i2c_slave_txbuffer_index;
static uint8_t i2c_slave_txbuffer_length;

static uint8_t i2c_slave_rxbuffer[I2C_BUFFER_LENGTH];
static volatile uint8_t i2c_slave_rxbuffer_index;

static uint8_t is_nack_sent = false;

static volatile uint8_t i2c_error;

static struct i2c_slave_cb *slave_cb = NULL;

#define i2c_read_timerout   10*1000

void i2c_driver_delay(uint16_t delay)
{
    mDelayuS(delay);
}
// 0 ok
// 1 err
int i2c_write_bytes(uint8_t addr, uint8_t reg,uint8_t *value,uint16_t len)
{
    int ret=0;
    uint16_t wait=0;
    ret++;
    while(I2C_GetFlagStatus(I2C_FLAG_BUSY)!=RESET)
    {
        i2c_driver_delay(1);
        wait++;
        if(wait>i2c_read_timerout)
        {
            return ret;
        }
    }
    I2C_GenerateSTART(ENABLE);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
    }
    I2C_Send7bitAddress(addr, I2C_Direction_Transmitter);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
        }
    I2C_SendData(reg);
    ret++;

    while(len--)
    {
        wait=0;
        while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
        {
                i2c_driver_delay(1);
                wait++;
                if(wait>i2c_read_timerout)
                {
                    return ret;
                }
            }
        I2C_SendData(*value++);
    }
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
    }
    I2C_GenerateSTOP(ENABLE);
    return 0;
}
// 0 ok
// 1 err
int i2c_read_bytes(uint8_t addr, uint8_t reg,uint8_t *value,uint16_t len)
{
    int ret=0;
    uint16_t wait=0;
    uint16_t red_cnt=0;



//    I2C_AcknowledgeConfig(ENABLE);
    ret++;
    while(I2C_GetFlagStatus(I2C_FLAG_BUSY)!=RESET)
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
        }
    I2C_GenerateSTART(ENABLE);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
        }

    I2C_Send7bitAddress(addr, I2C_Direction_Transmitter);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
        }
//    while(!I2C_GetFlagStatus(I2C_FLAG_TXE));

    I2C_SendData(reg);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
        }
    I2C_GenerateSTART(ENABLE);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        i2c_driver_delay(1);
        wait++;
        if(wait>i2c_read_timerout)
        {
            return ret;
        }
    }

    I2C_Send7bitAddress(addr, I2C_Direction_Receiver);
    ret++;
    wait=0;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
            i2c_driver_delay(1);
            wait++;
            if(wait>i2c_read_timerout)
            {
                return ret;
            }
    }
    ret++;
    int i=0;
    wait=0;
    for (i = 0; i < len; ++i)
    {
        if(i == len-1)
         I2C_AcknowledgeConfig(DISABLE);         //清除ACK位               主设备为了能在收到最后一个字节后产生一个NACK脉冲，
        wait=0;
        while(!I2C_GetFlagStatus(I2C_FLAG_RXNE))  //获取RxEN的状态，等待收到数据
        {
               i2c_driver_delay(1);
               wait++;
               if(wait>i2c_read_timerout)
               {
                   return ret;
               }
        }
        *value++= I2C_ReceiveData();                                   //获得从机的寄存器中的数据
    }

   I2C_GenerateSTOP(ENABLE);
   I2C_AcknowledgeConfig(ENABLE);

    return 0;
}

void i2c_app_init(uint8_t address)
{
    i2c_state = I2C_READY;
    i2c_send_stop = true;
    i2c_in_repstart = false;

    GPIOB_ModeCfg(GPIO_Pin_13 | GPIO_Pin_12, GPIO_ModeIN_PU);

    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable,
            I2C_AckAddr_7bit, address);
#if  1
    I2C_Cmd(ENABLE);
    I2C_AcknowledgeConfig(ENABLE);
#elif 1

    I2C_ITConfig(I2C_IT_BUF, ENABLE);
    I2C_ITConfig(I2C_IT_EVT, ENABLE);
    I2C_ITConfig(I2C_IT_ERR, ENABLE);

    PFIC_EnableIRQ(I2C_IRQn);
#endif
}

void i2c_slave_cb_register(struct i2c_slave_cb *cb)
{
   slave_cb = cb;
}

int i2c_write_to(uint8_t addr_7bit, const uint8_t *data, uint8_t length,
        uint8_t wait, uint8_t send_stop)
{
    if (length > I2C_BUFFER_LENGTH) {
        return -I2C_NO_MEM;
    }

    if (i2c_state != I2C_READY) {
        return -I2C_STATE;
    }

    if (!length) {
        return 0;
    }

    i2c_state = I2C_MTX;
    i2c_send_stop = send_stop;

    i2c_error = 0;

    // initialize buffer iteration vars
    i2c_master_buffer_index = 0;
    i2c_master_buffer_length = length;

    memcpy(i2c_master_buffer, data, length);

    i2c_slave_addr_rw = I2C_WRITE;
    i2c_slave_addr_rw |= addr_7bit << 1;

    I2C_GenerateSTOP(DISABLE);

    if (i2c_in_repstart == true) {
        i2c_in_repstart = false;

        do {
            I2C_SendData(i2c_slave_addr_rw);
        } while(R16_I2C_CTRL1 & RB_I2C_BTF);

        /* Disabled in IRS */
        I2C_ITConfig(I2C_IT_BUF, ENABLE);
        I2C_ITConfig(I2C_IT_EVT, ENABLE);
        I2C_ITConfig(I2C_IT_ERR, ENABLE);
    } else {
        I2C_GenerateSTART(ENABLE);
    }

    while(wait && (i2c_state == I2C_MTX)) {
        continue;
    }

    if (i2c_error) {
        return -i2c_error;
    }

    return 0;
}

int i2c_read_from(uint8_t addr_7bit, uint8_t *data, uint8_t length,
        uint8_t send_stop, int timeout)
{
    int to = 0;
    uint8_t forever = (timeout == -1);

    if (length > I2C_BUFFER_LENGTH) {
        return -I2C_NO_MEM;
    }

    if (i2c_state != I2C_READY) {
        return -I2C_STATE;
    }

    if (!length) {
        return 0;
    }

    i2c_state = I2C_MRX;
    i2c_send_stop = send_stop;

    i2c_error = 0;

    // initialize buffer iteration vars
    i2c_master_buffer_index = 0;
    i2c_master_buffer_length = length - 1;

    i2c_slave_addr_rw = I2C_READ;
    i2c_slave_addr_rw |= addr_7bit << 1;

    I2C_GenerateSTOP(DISABLE);

    if (i2c_in_repstart == true) {
        i2c_in_repstart = false;

        do {
            I2C_SendData(i2c_slave_addr_rw);
        } while(R16_I2C_CTRL1 & RB_I2C_BTF);

        /* Disabled in IRS */
        I2C_ITConfig(I2C_IT_BUF, ENABLE);
        I2C_ITConfig(I2C_IT_EVT, ENABLE);
        I2C_ITConfig(I2C_IT_ERR, ENABLE);
    } else {
        I2C_GenerateSTART(ENABLE);
    }

    // wait for read operation to complete
    while (i2c_state == I2C_MRX) {
        mDelaymS(1);
        to++;
        if (!forever && (to >= timeout)) {
            break;
        }
    }

    if (i2c_master_buffer_index < length)
        length = i2c_master_buffer_index;

    // copy i2c buffer to data
    memcpy(data, i2c_master_buffer, length);

    return length;
}

#ifdef CONFIG_I2C_DEBUG
static void print_i2c_irq_sta(uint32_t state)
{
    I2C_DBG("i2c irq: ( ");

    if(state & RB_I2C_SB)
        I2C_DBG("SB ");
    if(state & RB_I2C_ADDR)
        I2C_DBG("ADDR ");
    if(state & RB_I2C_BTF)
        I2C_DBG("BTF ");
    if(state & RB_I2C_ADD10)
        I2C_DBG("ADD10 ");
    if(state & RB_I2C_STOPF)
        I2C_DBG("STOP ");
    if(state & RB_I2C_RxNE)
        I2C_DBG("RxNE ");
    if(state & RB_I2C_TxE)
        I2C_DBG("TxE ");
    if(state & RB_I2C_BERR)
        I2C_DBG("BERR ");
    if(state & RB_I2C_ARLO)
        I2C_DBG("ARLO ");
    if(state & RB_I2C_AF)
        I2C_DBG("AF ");
    if(state & RB_I2C_OVR)
        I2C_DBG("OVR ");
    if(state & RB_I2C_PECERR)
        I2C_DBG("PECERR ");
    if(state & RB_I2C_TIMEOUT)
        I2C_DBG("TIMEOUT ");
    if(state & RB_I2C_SMBALERT)
        I2C_DBG("SMBALERT ");
    if(state & (RB_I2C_MSL << 16))
        I2C_DBG("MSL ");
    if(state & (RB_I2C_BUSY << 16))
        I2C_DBG("BUSY ");
    if(state & (RB_I2C_TRA << 16))
        I2C_DBG("TRA ");
    if(state & (RB_I2C_GENCALL << 16))
        I2C_DBG("GENCALL ");
    if(state & (RB_I2C_SMBDEFAULT << 16))
        I2C_DBG("SMBDEFAULT ");
    if(state & (RB_I2C_SMBHOST << 16))
        I2C_DBG("SMBHOST ");
    if(state & (RB_I2C_DUALF << 16))
        I2C_DBG("DUALF ");

    I2C_DBG(")\n");
}
#else
static inline void print_i2c_irq_sta(uint32_t state)
{
    (void)state;
}
#endif

__INTERRUPT
__HIGH_CODE
void I2C_IRQHandler(void)
{
    uint32_t event = I2C_GetLastEvent();
    print_i2c_irq_sta(event);

    /* I2C Master */
    if (event & (RB_I2C_MSL << 16)) {
        if (event & RB_I2C_SB) {
            /* Start condition sent, send address */
            I2C_SendData(i2c_slave_addr_rw);
            I2C_DBG("Master selected, send address\n");
        }

        /* I2C Master transmitter */
        if (event & (RB_I2C_TRA << 16)) {
            I2C_DBG("Master transmitter:\n");
            /* Slave receiver acked address or sent bit */
            if (event & (RB_I2C_ADDR | RB_I2C_BTF | RB_I2C_TxE | (RB_I2C_TRA << 16))) {
                /* if there is data to send, send it, otherwise stop */
                if (i2c_master_buffer_index < i2c_master_buffer_length) {
                    I2C_SendData(i2c_master_buffer[i2c_master_buffer_index++]);
                    I2C_DBG("I2C  send (%#x)\n",
                            i2c_master_buffer[i2c_master_buffer_index - 1]);
                } else {
                    if (i2c_send_stop) {
                        i2c_state = I2C_READY;
                        I2C_GenerateSTOP(ENABLE);
                        I2C_DBG("  send STOP\n");
                    } else {
                        i2c_in_repstart = true;
                        /* we're gonna send the START, don't enable the interrupt. */
                        I2C_ITConfig(I2C_IT_BUF, DISABLE);
                        I2C_ITConfig(I2C_IT_EVT, DISABLE);
                        I2C_ITConfig(I2C_IT_ERR, DISABLE);
                        I2C_GenerateSTART(ENABLE);
                        i2c_state = I2C_READY;
                        I2C_DBG("  restart\n");
                    }
                }
            }

            /* Address or data sent, nack received */
            if (event & RB_I2C_AF) {
                I2C_ClearFlag(I2C_FLAG_AF);

                i2c_error = I2C_MT_NACK;
                i2c_state = I2C_READY;
                I2C_GenerateSTOP(ENABLE);
                I2C_DBG("  NACK received, sent stop\n");
            }
        } else {
        /* I2C Master reveiver */
            I2C_DBG("Master receiver:\n");

            /* address sent, ack received */
            if(event & RB_I2C_ADDR) { 
                /* ack if more bytes are expected, otherwise nack */
                if (i2c_master_buffer_length) {
                    I2C_AcknowledgeConfig(ENABLE);
                    I2C_DBG("  address sent\n");
                    I2C_DBG("  ACK next\n");
                } else {
                    //XXX: Should not delay too match before NACK 
                    I2C_AcknowledgeConfig(DISABLE);
                    is_nack_sent = true;
                    I2C_DBG("  address sent\n");
                    I2C_DBG("  NACK next\n");
                }
            }

            /* data reveived */
            if (event & (RB_I2C_RxNE)) {
                /* put byte into buffer */ 
                i2c_master_buffer[i2c_master_buffer_index++] = I2C_ReceiveData();

                if (i2c_master_buffer_index < i2c_master_buffer_length) {
                    I2C_AcknowledgeConfig(ENABLE);
                    I2C_DBG("  ACK next\n");
                } else {
                    //XXX: Should not delay too match before NACK 
                    I2C_AcknowledgeConfig(DISABLE);
                    I2C_DBG("  NACK next\n");

                    if (is_nack_sent) {
                        is_nack_sent = false;
                        if (i2c_send_stop) {
                            I2C_GenerateSTOP(ENABLE);
                            i2c_state = I2C_READY;
                            I2C_DBG("  send STOP\n");
                        } else {
                            i2c_in_repstart = true;
                            /* we're gonna send the START, don't enable the interrupt. */
                            I2C_ITConfig(I2C_IT_BUF, DISABLE);
                            I2C_ITConfig(I2C_IT_EVT, DISABLE);
                            I2C_ITConfig(I2C_IT_ERR, DISABLE);
                            I2C_GenerateSTART(ENABLE);
                            i2c_state = I2C_READY;
                            I2C_DBG("  restart\n");

                        }
                    } else {
                        is_nack_sent = true;
                    }
                }

                I2C_DBG("  received data (%#x)\n", 
                        i2c_master_buffer[i2c_master_buffer_index - 1]);

            }

            /* nack received */
            if (event & RB_I2C_AF) {
                I2C_ClearFlag(I2C_FLAG_AF);
                /* put final byte into buffer */
                i2c_master_buffer[i2c_master_buffer_index++] = I2C_ReceiveData();

                if (i2c_send_stop) {
                    i2c_state = I2C_READY;
                    I2C_GenerateSTOP(ENABLE);
                    I2C_DBG("  NACK received, send STOP\n");
                } else {
                    i2c_in_repstart = true;
                    /* we're gonna send the START, don't enable the interrupt. */
                    I2C_ITConfig(I2C_IT_BUF, DISABLE);
                    I2C_ITConfig(I2C_IT_EVT, DISABLE);
                    I2C_ITConfig(I2C_IT_ERR, DISABLE);
                    I2C_GenerateSTART(ENABLE);
                    i2c_state = I2C_READY;
                    I2C_DBG("  restart\n");
                }
            }
        }

    } else {
    /* I2C slave */
        /* addressed, returned ack */
        if (event & RB_I2C_ADDR) {

            if (event & ((RB_I2C_TRA << 16) | RB_I2C_TxE)) {
                I2C_DBG("Slave transmitter address matched\n");
                
                i2c_state = I2C_STX;
                i2c_slave_txbuffer_index = 0;
                i2c_slave_txbuffer_length = 0;

                if (slave_cb && slave_cb->on_transmit) {
                    slave_cb->on_transmit(i2c_slave_txbuffer, &i2c_slave_txbuffer_length);
                }
            } else {
                I2C_DBG("Slave reveiver address matched\n");

                i2c_state = I2C_SRX;
                i2c_slave_rxbuffer_index = 0;
            }
        }

        if (event & (RB_I2C_TRA << 16)) { //TODO: STOP?
            /* Slave transmintter */
            I2C_AcknowledgeConfig(ENABLE);
            I2C_DBG("Slave transmitter:\n");

            if (event & RB_I2C_AF) {
                /* Nack received */
                I2C_ClearFlag(I2C_FLAG_AF);
                I2C_AcknowledgeConfig(ENABLE);
                I2C_DBG("  Nack received\n");

                /* leave slave receiver state */
                i2c_state = I2C_READY;
                /* clear status */
                event = 0;
            }

            if (event & (RB_I2C_BTF | RB_I2C_TxE)) {
                /* if there is more to send, ack, otherwise send 0xff */
                if (i2c_slave_txbuffer_index < i2c_slave_txbuffer_length) {
                    /* copy data to output register */
                    I2C_SendData(i2c_slave_txbuffer[i2c_slave_txbuffer_index++]);
                    I2C_DBG("  send (%#x)\n", 
                        i2c_slave_txbuffer[i2c_slave_txbuffer_index - 1]);
                } else {
                    I2C_SendData(0xff);
                    I2C_DBG("  no more data, send 0xff\n");
                }
            }
        } else {
            /* Slave receiver */
            I2C_DBG("Slave receiver:\n");

            if (event & RB_I2C_RxNE) {
                /* if there is still room in the rx buffer */
                if (i2c_slave_rxbuffer_index < I2C_BUFFER_LENGTH) {
                    /* put byte in buffer and ack */
                    i2c_slave_rxbuffer[i2c_slave_rxbuffer_index++] = I2C_ReceiveData();
                    I2C_AcknowledgeConfig(ENABLE);
                    I2C_DBG("  received (%#x)\n", 
                            i2c_slave_rxbuffer[i2c_slave_rxbuffer_index - 1]);
                } else {
                    // otherwise nack
                    I2C_AcknowledgeConfig(DISABLE);
                }
            }

            if (event & RB_I2C_STOPF) {
                /* ack future responses and leave slave receiver state */
                R16_I2C_CTRL1 |= RB_I2C_PE; //clear flag

                I2C_DBG("  reveive stop\n");

                /* callback to user defined callback */
                if (slave_cb && slave_cb->on_receive) {
                    slave_cb->on_receive(i2c_slave_rxbuffer, i2c_slave_rxbuffer_index);
                }
                /* since we submit rx buffer , we can reset it */
                i2c_slave_rxbuffer_index = 0;
            }

            if (event & RB_I2C_AF) {  
                I2C_ClearFlag(I2C_FLAG_AF);

                /* ack future responses */
                I2C_AcknowledgeConfig(ENABLE);
            }
        }
    }

    if (event & RB_I2C_BERR) {
        I2C_ClearFlag(RB_I2C_BERR);
        I2C_GenerateSTOP(ENABLE);

        i2c_error = I2C_BUS_ERROR;
        I2C_DBG("RB_I2C_BERR\n");
    }

    if (event & RB_I2C_ARLO) {
        I2C_ClearFlag(RB_I2C_ARLO);
        
        i2c_error = I2C_ARB_LOST;
        I2C_DBG("RB_I2C_ARLO\n");
    }

    if (event & RB_I2C_OVR) {
        I2C_ClearFlag(RB_I2C_OVR);
        
        i2c_error = I2C_OVR;
        I2C_DBG("RB_I2C_OVR\n");
    }

    if (event & RB_I2C_PECERR) {
        I2C_ClearFlag(RB_I2C_PECERR);
        
        i2c_error = I2C_PECERR;
        I2C_DBG("RB_I2C_PECERR\n");
    }

    if (event & RB_I2C_TIMEOUT) {
        I2C_ClearFlag(RB_I2C_TIMEOUT);
        
        i2c_error = I2C_TIMEOUT;
        I2C_DBG("RB_I2C_TIMEOUT\n");
    }

    if (event & RB_I2C_SMBALERT) {
        I2C_ClearFlag(RB_I2C_SMBALERT);
        
        i2c_error = I2C_SMBALERT;
        I2C_DBG("RB_I2C_SMBALERT\n");
    }

    I2C_DBG("\n");
}
