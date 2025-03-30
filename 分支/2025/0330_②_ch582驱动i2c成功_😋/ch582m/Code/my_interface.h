#ifndef __INTERFACE_H
#define __INTERFACE_H

    #include "CH59x_common.h"
    #include <stdbool.h>


    #ifndef __USART__All__Definition__
    #define __USART__All__Definition__

        // head

        void USART_APP_Init(void);

    #endif

#define hard_i2c
#ifdef hard_i2c
    #ifndef __I2C__All__Definition__
    #define __I2C__All__Definition__

        #include <stddef.h>
        #include <stdint.h>
        #include <stdbool.h>

        #define I2C_BUFFER_LENGTH   32
        #define I2C_READ      1
        #define I2C_WRITE     0

        typedef enum {
            I2C_READY,
            I2C_MRX,  
            I2C_MTX,  
            I2C_SRX,  
            I2C_STX,  
        }i2c_state_t;

        typedef enum {
            I2C_NO_MEM = 1,
            I2C_STATE,
            I2C_MT_NACK,
            I2C_ARB_LOST,
            I2C_BUS_ERROR,
            I2C_OVR,
            I2C_PECERR,
            I2C_TIMEOUT,
            I2C_SMBALERT,
        }i2c_error_t;

        /**
         * @brief   User callback function on I2C slave transmitting.
         * 
         * @param data  Pointer to user data to transmit.
         * 
         * @param len   Pointer to user data length.
         */
        typedef void (*i2c_on_slave_transmit)(uint8_t *data, uint8_t *len);

        /**
         * @brief   User callback function on I2C slave received.
         * 
         * @param data  Pointer to current received data.
         * 
         * @param len  Received data length.
         */
        typedef void (*i2c_on_slave_receive)(uint8_t *data, uint8_t len);

        struct i2c_slave_cb {
            i2c_on_slave_transmit on_transmit;
            i2c_on_slave_receive on_receive;
        };

        /**
         * @brief   I2C interrupt routine initialization.
         * 
         * @param address I2C address.
         */
        void i2c_app_init(uint8_t address);

        /**
         * @brief   I2C slave user callback function regiester.
         * 
         * @param cb    Pointer to user callback function.
         */
        void i2c_slave_cb_register(struct i2c_slave_cb *cb);

        /**
         * @brief   I2C master write data to slave.
         * 
         * @param addr_7bit I2C slave 7bit address.
         * @param data      Pointer to the write data.
         * @param length    Write data length.
         * @param wait      Choose to wait for the write process to end or not.
         * @param send_stop Choose to send stop or not.
         * @return          0 If successful. 
         */
        int i2c_write_to(uint8_t addr_7bit, const uint8_t *data, uint8_t length,
                uint8_t wait, uint8_t send_stop);

        /**
         * @brief   I2C master read data to slave
         * 
         * @param addr_7bit I2C slave 7bit address.
         * @param data      Pointer to the read data to put in. 
         * @param length    Read data length.
         * @param send_stop Choose to send stop or not
         * @param timeout   Read process timeout.
         * @return          Negative on error code otherwise indicates the actual read length.
         */
        int i2c_read_from(uint8_t addr_7bit, uint8_t *data, uint8_t length,
                uint8_t send_stop, int timeout);

    #endif
#endif

#define soft_i2c
#ifdef soft_i2c

// #ifndef __I2C__All__Definition__
// #define __I2C__All__Definition__


#ifndef __Struct__I2C__Definition__
#define __Struct__I2C__Definition__
    
    enum
    {
        enum_INPUT = 0,
        enum_OUTPUT
    };

    enum
    {
        enum_NACK = 0,
        enum_ACK
    };

    enum
    {
        enum_GPIOA = 0,
        enum_GPIOB
    };

    typedef struct __I2C_Class_Struct
    {
        uint16_t SCL_Pin;
        uint16_t SDA_Pin;
        uint8_t I2C_Port;
    }I2C_Class_Struct;

    extern I2C_Class_Struct I2C_Class_1;
    // extern I2C_Class_Struct I2C_Class_2;

    #ifndef i2c_direction_write
        #define i2c_direction_write     ((uint8_t)0x00)
    #endif

    #ifndef i2c_direction_read
        #define i2c_direction_read      ((uint8_t)0x01)
    #endif

#endif


#ifndef __Map__I2C__Definition__
#define __Map__I2C__Definition__

    uint8_t I2C_1_Read(uint8_t DeviceAddress, uint8_t RegisterAddress);
    void I2C_1_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data);

    uint8_t I2C_2_Read(uint8_t DeviceAddress, uint8_t RegisterAddress);
    void I2C_2_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data);

#endif


#ifndef __I2C__APP__Definition__
#define __I2C__APP__Definition__

    // I2C初始化
    void I2C_APP_Init(void);

    // I2C写单字节数据
    void I2C_APP_Write_Single_Byte(	I2C_Class_Struct input_I2C_Class, 
                                    uint8_t DeviceAddress, 
                                    uint8_t RegisterAddress, 
                                    uint8_t input_Data);

    // I2C读单字节数据
    uint8_t I2C_APP_Read_Single_Byte(	I2C_Class_Struct input_I2C_Class, 
                                        uint8_t DeviceAddress, 
                                        uint8_t RegisterAddress);

#endif

#ifndef __I2C__Driver__Definition__
#define __I2C__Driver__Definition__

    // I2C驱动启动
    void I2C_Driver_Start(I2C_Class_Struct input_I2C_Class);

    // I2C驱动停止
    void I2C_Driver_Stop(I2C_Class_Struct input_I2C_Class);

    // 等待ACK
    bool I2C_Driver_Wait_Ack(I2C_Class_Struct input_I2C_Class);

    // 发送ACK
    void I2C_Driver_Ack(I2C_Class_Struct input_I2C_Class);

    // 发送NAck
    void I2C_Driver_NAck(I2C_Class_Struct input_I2C_Class);

    // I2C写数据
    void I2C_Driver_Write(I2C_Class_Struct input_I2C_Class, uint8_t input_Data);

    // I2C读数据
    uint8_t I2C_Driver_Read(I2C_Class_Struct input_I2C_Class, uint8_t Ack);

#endif 

#ifndef __I2C__GPIO__Definition__
#define __I2C__GPIO__Definition__

    // // I2C SCL写数据
    // void I2C_GPIO_Write_SCL(I2C_Class_Struct input_I2C_Class, GPIO_PinState input_PinState);

    // // I2C SDA写数据
    // void I2C_GPIO_Write_SDA(I2C_Class_Struct input_I2C_Class, GPIO_PinState input_PinState);

    // // I2C SDA模式设置
    void I2C_GPIO_Mode_SDA(I2C_Class_Struct input_I2C_Class, uint8_t input_SDA_Mode);

    // 读取I2C SDA数据
    // GPIO_PinState I2C_GPIO_Read_SDA(I2C_Class_Struct input_I2C_Class);
    #define I2C_GPIO_Read_SDA(input_I2C_Class) GPIOB_ReadPortPin(input_I2C_Class.SDA_Pin)

    #define I2C_GPIO_Write_SCL_1(input_I2C_Class) GPIOB_SetBits(input_I2C_Class.SCL_Pin)
    #define I2C_GPIO_Write_SCL_0(input_I2C_Class) GPIOB_ResetBits(input_I2C_Class.SCL_Pin)
    #define I2C_GPIO_Write_SDA_1(input_I2C_Class) GPIOB_SetBits(input_I2C_Class.SDA_Pin)
    #define I2C_GPIO_Write_SDA_0(input_I2C_Class) GPIOB_ResetBits(input_I2C_Class.SDA_Pin)

    #define I2C_GPIO_Mode_SDA_IN(input_I2C_Class)  I2C_GPIO_Mode_SDA(input_I2C_Class, enum_INPUT)
    #define I2C_GPIO_Mode_SDA_OUT(input_I2C_Class) I2C_GPIO_Mode_SDA(input_I2C_Class, enum_OUTPUT)

#endif


// #endif

#endif

#endif // !__INTERFACE_H 
