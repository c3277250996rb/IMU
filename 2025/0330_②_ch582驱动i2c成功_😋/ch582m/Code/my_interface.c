#include "my_interface.h"

#ifndef __USART__All__Reference__
#define __USART__All__Reference__

    // head
    uint8_t TxBuff[] = "This is a tx exam\r\n";
    uint8_t RxBuff[100];
    uint8_t trigB;
    // head

    void USART_APP_Init(void){
        /* 配置串口1：先配置IO口模式，再配置串口 */
        GPIOA_SetBits(GPIO_Pin_9);
        GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);      // RXD-配置上拉输入
        GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA); // TXD-配置推挽输出，注意先让IO口输出高电平
        UART1_DefInit();

        // 中断方式：接收数据后发送出去
        UART1_ByteTrigCfg(UART_7BYTE_TRIG);
        trigB = 7;
        UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
        PFIC_EnableIRQ(UART1_IRQn);
    }

    __INTERRUPT
    __HIGH_CODE
    void UART1_IRQHandler(void){
        volatile uint8_t i;

        switch(UART1_GetITFlag()){

            case UART_II_LINE_STAT: // 线路状态错误
                {
                    UART1_GetLinSTA();
                    break;
                }

            case UART_II_RECV_RDY: // 数据达到设置触发点
                for(i = 0; i != trigB; i++){
                    RxBuff[i] = UART1_RecvByte();
                    UART1_SendByte(RxBuff[i]);
                }
                break;

            case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
                i = UART1_RecvString(RxBuff);
                UART1_SendString(RxBuff, i);
                break;

            case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
                break;

            case UART_II_MODEM_CHG: // 只支持串口0
                break;

            default:
                break;
        }
    }

#endif

#ifdef hard_i2c

#ifndef __I2C__All__Reference__
#define __I2C__All__Reference__

    /**
     * Note: 主机与从机的DEBUG接口需要同时打开或关闭，
     * 否则会产生时序问题。
     */
    #define CONFIG_I2C_DEBUG

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

    void i2c_app_init(uint8_t address)
    {
        i2c_state = I2C_READY;
        i2c_send_stop = true;
        i2c_in_repstart = false;

        // GPIOB_ModeCfg(GPIO_Pin_14 | GPIO_Pin_15, GPIO_ModeIN_PU);
        GPIOB_ModeCfg(GPIO_Pin_13 | GPIO_Pin_12, GPIO_ModeOut_PP_5mA);

        // I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable,
        //         I2C_AckAddr_7bit, address);
        I2C_GPIO_Write_SCL_1(I2C_Class_1);
        I2C_GPIO_Write_SDA_1(I2C_Class_1);

        // I2C_ITConfig(I2C_IT_BUF, ENABLE);
        // I2C_ITConfig(I2C_IT_EVT, ENABLE);
        // I2C_ITConfig(I2C_IT_ERR, ENABLE);
        
        // PFIC_EnableIRQ(I2C_IRQn);
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
                        I2C_DBG("  send (%#x)\n", 
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

#endif

#endif

#ifdef soft_i2c

// #ifndef __I2C__All__Reference__
// #define __I2C__All__Reference__

void i2c_delay(void){
        DelayUs(1);
}

#ifndef __Struct__I2C__Reference__
#define __Struct__I2C__Reference__
    
    I2C_Class_Struct I2C_Class_1 = {
        .SCL_Pin = GPIO_Pin_13,
        .SDA_Pin = GPIO_Pin_12,
        .I2C_Port = enum_GPIOB
    };

    // I2C_Class_Struct I2C_Class_2 = {
    //     .SCL_Pin = I2C2_SCL_PB10_Pin,
    //     .SDA_Pin = I2C2_SDA_PB11_Pin,
    //     .I2C_Port = I2C2_SDA_PB11_GPIO_Port 
    // };

#endif


#ifndef __Map__I2C__Reference__
#define __Map__I2C__Reference__

    uint8_t I2C_1_Read(uint8_t DeviceAddress, uint8_t RegisterAddress){
        return I2C_APP_Read_Single_Byte(I2C_Class_1, DeviceAddress, RegisterAddress);
    }
    void I2C_1_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
        I2C_APP_Write_Single_Byte(I2C_Class_1, DeviceAddress, RegisterAddress, input_Data);
    }

    // uint8_t I2C_2_Read(uint8_t DeviceAddress, uint8_t RegisterAddress){
    //     return I2C_APP_Read_Single_Byte(I2C_Class_2, DeviceAddress, RegisterAddress);
    // }
    // void I2C_2_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
    //     I2C_APP_Write_Single_Byte(I2C_Class_2, DeviceAddress, RegisterAddress, input_Data);
    // }

#endif 


#ifndef __I2C__APP__Reference__
#define __I2C__APP__Reference__
/* APP Reference Begin */

    void I2C_APP_Init(void){
        // __HAL_RCC_I2C1_CLK_ENABLE();
        // __HAL_RCC_I2C2_CLK_ENABLE();
    }

    void I2C_APP_Write_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
                                    uint8_t				DeviceAddress, 
                                    uint8_t				RegisterAddress, 
                                    uint8_t				input_Data)
    {
        I2C_Driver_Start(input_I2C_Class);
        I2C_Driver_Write(	input_I2C_Class, 
                            DeviceAddress | i2c_direction_write);
        I2C_Driver_Wait_Ack(input_I2C_Class);
        I2C_Driver_Write(	input_I2C_Class, 
                            RegisterAddress);
        I2C_Driver_Wait_Ack(input_I2C_Class);
        I2C_Driver_Write(	input_I2C_Class, 
                            input_Data);
        I2C_Driver_Wait_Ack(input_I2C_Class);
        I2C_Driver_Stop(input_I2C_Class);
    }

    uint8_t I2C_APP_Read_Single_Byte(	I2C_Class_Struct	input_I2C_Class, 
                                        uint8_t				DeviceAddress, 
                                        uint8_t				RegisterAddress)
    {
        uint8_t output_Data = 0;
    
        I2C_Driver_Start(input_I2C_Class);
        I2C_Driver_Write(	input_I2C_Class, 
                            DeviceAddress | i2c_direction_write);
        I2C_Driver_Wait_Ack(input_I2C_Class);
        I2C_Driver_Write(	input_I2C_Class, 
                            RegisterAddress);
        I2C_Driver_Wait_Ack(input_I2C_Class);
        I2C_Driver_Start(input_I2C_Class);
        I2C_Driver_Write(	input_I2C_Class, 
                            DeviceAddress | i2c_direction_read);
        I2C_Driver_Wait_Ack(input_I2C_Class);
        output_Data = I2C_Driver_Read(	input_I2C_Class, 
                                        enum_NACK);
        I2C_Driver_Stop(input_I2C_Class);
        
        return output_Data;
    }

/* APP Reference End */
#endif

#define OD_for_1_and_PP_for_0 0

#ifndef __I2C__Driver__Reference__
#define __I2C__Driver__Reference__
/* Driver Reference Begin */

    void I2C_Driver_Start(I2C_Class_Struct input_I2C_Class){
        
        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_OUT(input_I2C_Class);
        #endif

        I2C_GPIO_Write_SDA_1(input_I2C_Class);
        I2C_GPIO_Write_SCL_1(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SDA_0(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_0(input_I2C_Class);
        i2c_delay();
    }

    void I2C_Driver_Stop(I2C_Class_Struct input_I2C_Class){
        
        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_OUT(input_I2C_Class);
        #endif
        
        I2C_GPIO_Write_SDA_0(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_1(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SDA_1(input_I2C_Class);
        i2c_delay();
    }

    bool I2C_Driver_Wait_Ack(I2C_Class_Struct input_I2C_Class){
        uint8_t Retry = 0;

        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_IN(input_I2C_Class);
        #endif
        
        I2C_GPIO_Write_SDA_1(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_1(input_I2C_Class);
        i2c_delay();

        while(I2C_GPIO_Read_SDA(input_I2C_Class)){
            Retry++;
            if(Retry > 250){
                I2C_Driver_Stop(input_I2C_Class);
                return false;
            }
        }

        I2C_GPIO_Write_SCL_0(input_I2C_Class);
        i2c_delay();
        
        return true;
    }

    void I2C_Driver_Ack(I2C_Class_Struct input_I2C_Class){
        
        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_OUT(input_I2C_Class);
        #endif

        I2C_GPIO_Write_SDA_0(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_1(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_0(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SDA_1(input_I2C_Class);
    }

    void I2C_Driver_NAck(I2C_Class_Struct input_I2C_Class){
        
        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_OUT(input_I2C_Class);
        #endif

        I2C_GPIO_Write_SDA_1(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_1(input_I2C_Class);
        i2c_delay();
        I2C_GPIO_Write_SCL_0(input_I2C_Class);
        i2c_delay();
    }

    void I2C_Driver_Write(	I2C_Class_Struct	input_I2C_Class,
                            uint8_t				input_Data)
    {
        uint8_t i;
        
        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_OUT(input_I2C_Class);
        #endif

        for(i = 0; i < 8; i ++){
            I2C_GPIO_Write_SCL_0(input_I2C_Class);
            i2c_delay();

            #if 1  // 符合直觉 (推荐)
                if(input_Data & 0x80){
                    I2C_GPIO_Write_SDA_1(input_I2C_Class);
                }else{
                    I2C_GPIO_Write_SDA_0(input_I2C_Class);
                }
            #else  // 晦涩难读 (不推荐)
                I2C_GPIO_Write_SDA(input_I2C_Class, (input_Data & 0x80) >> 7);
            #endif

            i2c_delay();
            I2C_GPIO_Write_SCL_1(input_I2C_Class);
            i2c_delay();
            I2C_GPIO_Write_SCL_0(input_I2C_Class);
            input_Data <<= 1;
        }

        I2C_GPIO_Write_SDA_1(input_I2C_Class);
    }

    uint8_t I2C_Driver_Read(	I2C_Class_Struct	input_I2C_Class,
                                uint8_t				Ack)
    {
        uint8_t i, output_Data = 0;

        #if OD_for_1_and_PP_for_0

        #else
        I2C_GPIO_Mode_SDA_IN(input_I2C_Class);
        #endif

        for(i = 0; i < 8; i++){
            output_Data <<= 1;
            I2C_GPIO_Write_SCL_1(input_I2C_Class);
            i2c_delay();

            if(I2C_GPIO_Read_SDA(input_I2C_Class)){
                output_Data++;
            }

            I2C_GPIO_Write_SCL_0(input_I2C_Class);
            i2c_delay();
        }

        if(enum_ACK == Ack){
            I2C_Driver_Ack(input_I2C_Class);
        }else{
            I2C_Driver_NAck(input_I2C_Class);
        }

        return output_Data;
    }

/* Driver Reference End */
#endif

#ifndef __I2C__GPIO__Reference__
#define __I2C__GPIO__Reference__
/* GPIO Reference Begin */


    // void I2C_GPIO_Write_SCL(	I2C_Class_Struct	input_I2C_Class,
    //                             GPIO_PinState		input_PinState)
    // {	
    //     GPIO_TypeDef* temp_Port  = input_I2C_Class.SCL_Port;
    //     uint16_t temp_Pin       = input_I2C_Class.SCL_Pin;

    //     HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
    // }

    // void I2C_GPIO_Write_SDA(	I2C_Class_Struct	input_I2C_Class,
    //                             GPIO_PinState		input_PinState)
    // {
    //     GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
    //     uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

    //     HAL_GPIO_WritePin(temp_Port, temp_Pin, input_PinState);
    // }

    void I2C_GPIO_Mode_SDA(	I2C_Class_Struct	input_I2C_Class,
                            uint8_t				input_SDA_Mode)						
    {
        // GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
        // uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

        GPIOB_ModeCfg(GPIO_Pin_12, 
            input_SDA_Mode == enum_INPUT ? GPIO_ModeIN_PU : GPIO_ModeOut_PP_5mA
        );
    }

    // GPIO_PinState I2C_GPIO_Read_SDA(I2C_Class_Struct input_I2C_Class){
    //     GPIO_TypeDef* temp_Port	= input_I2C_Class.SDA_Port;
    //     uint16_t temp_Pin       = input_I2C_Class.SDA_Pin;

    //     GPIO_PinState output_PinState;
    //     output_PinState = HAL_GPIO_ReadPin(temp_Port, temp_Pin);
    //     return output_PinState;
    // }

/* GPIO Reference End */
#endif


// #endif

#endif

