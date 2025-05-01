#include "code.h"


/* I2C1 */

    

/* --- */


/* I2C2 */
    #define Delay_us(Time) { \
        while(Time --){ \
            for(i = 0; i < 10; i ++) {} \
        } \
    }

    #define I2C2_SCL_write(BitValue) { \
        HAL_GPIO_WritePin(I2C2_SCL_GPIO_Port, I2C2_SCL_Pin, (GPIO_PinState)BitValue); \
        Delay_us(1); \
    }

    #define I2C2_SDA_write(BitValue) { \
        HAL_GPIO_WritePin(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin, (GPIO_PinState)BitValue); \
        Delay_us(1); \
    }

    #define I2C2_SDA_read() { \
	    uint8_t BitValue; \
        BitValue = HAL_GPIO_ReadPin(I2C2_SDA_GPIO_Port, I2C2_SDA_Pin); \
        Delay_us(1); \
	    return BitValue; \
    }


    #define I2C2_SCL_write_1 I2C2_SCL_write(1)
    #define I2C2_SCL_write_0 I2C2_SCL_write(0)

    #define I2C2_SDA_write_1 I2C2_SDA_write(1)
    #define I2C2_SDA_write_0 I2C2_SDA_write(0)




#if 0
    void MyI2C_W_SCL(uint8_t BitValue)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, (GPIO_PinState)BitValue);
        //HAL_Delay(1);
    }

    void MyI2C_W_SDA(uint8_t BitValue)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, (GPIO_PinState)BitValue);
        //HAL_Delay(1);
    }

    uint8_t MyI2C_R_SDA(void)
    {
        uint8_t BitValue;
        BitValue = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11);
        //HAL_Delay(1);
        return BitValue;
    }
#endif


/* --- */

void MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{

	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}
