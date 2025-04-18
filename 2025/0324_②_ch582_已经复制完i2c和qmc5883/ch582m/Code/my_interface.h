#ifndef __INTERFACE_H
#define __INTERFACE_H

    #include "CH59x_common.h"


    #ifndef __USART__All__Definition__
    #define __USART__All__Definition__

        // head

        void USART_APP_Init(void);

    #endif

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


#endif // !__INTERFACE_H 
