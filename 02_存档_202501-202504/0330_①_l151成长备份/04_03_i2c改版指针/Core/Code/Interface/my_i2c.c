#include "my_i2c.h"

#ifndef __DWT__Delay__
#define __DWT__Delay__

    #ifdef USE_MY_ALIENTEK
        #include "atk_delay.h"
        void DWT_Delay_Init(void) {
            // Stm32_Clock_Init(RCC_PLL_MUL9);
        }
    #else
        #include "core_cm3.h"  // Cortex-M3 内核头文件（如果是M4/M7，包含 core_cm4.h 或 core_cm7.h）

        void DWT_Delay_Init(void) {
            // 允许DWT外设
            CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
            // 使能 DWT 计数器
            DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
        }

        // 微秒级延时
        void DWT_delay_us(uint32_t us) {
            uint32_t startTick = DWT->CYCCNT;
            uint32_t delayTicks = us * (SystemCoreClock / 1000000);  // 计算需要的时钟周期数
            while ((DWT->CYCCNT - startTick) < delayTicks);         // 等待
        }

        void delay_us(uint16_t Time){
            uint8_t i;	
            DWT_delay_us(Time);
            // while(Time --) for(i = 0; i < 10; i ++);
        }

        void delay_ms(uint16_t Time){
            while(Time --){
                delay_us(1000);
            }
        }
    #endif

    void i2c_delay(void){
        // delay_us(1);
    }

#endif   


#ifndef __Struct__I2C__Reference__
#define __Struct__I2C__Reference__
    
    I2C_Class_Struct I2C_Class_1 = {
        .SCL_Pin = I2C1_SCL_PB6_Pin,
        .SCL_Port = I2C1_SCL_PB6_GPIO_Port,
        .SDA_Pin = I2C1_SDA_PB7_Pin,
        .SDA_Port = I2C1_SDA_PB7_GPIO_Port
    };

    // I2C_Class_Struct I2C_Class_2 = {
    //     .SCL_Pin = I2C2_SCL_PB10_Pin,
    //     .SCL_Port = I2C2_SCL_PB10_GPIO_Port,
    //     .SDA_Pin = I2C2_SDA_PB11_Pin,
    //     .SDA_Port = I2C2_SDA_PB11_GPIO_Port 
    // };

#endif


#ifndef __Map__I2C__Reference__
#define __Map__I2C__Reference__

    uint8_t I2C_1_Read(uint8_t DeviceAddress, uint8_t RegisterAddress){
        return I2C_APP_Read_Single_Byte(&I2C_Class_1, DeviceAddress, RegisterAddress);
    }
    void I2C_1_Write(uint8_t DeviceAddress, uint8_t RegisterAddress, uint8_t input_Data){
        I2C_APP_Write_Single_Byte(&I2C_Class_1, DeviceAddress, RegisterAddress, input_Data);
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
        __HAL_RCC_I2C1_CLK_ENABLE();
        // __HAL_RCC_I2C2_CLK_ENABLE();
    }

    void I2C_APP_Write_Single_Byte(	I2C_Class_Struct	*input_I2C_Class, 
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

    uint8_t I2C_APP_Read_Single_Byte(	I2C_Class_Struct	*input_I2C_Class, 
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

#ifndef __I2C__Driver__Reference__
#define __I2C__Driver__Reference__
/* Driver Reference Begin */

    void I2C_Driver_Start(I2C_Class_Struct *input_I2C_Class){
        /**
         * START condition
         * 
         * L|A\  |L
         *  |  \A|
         */
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

    void I2C_Driver_Stop(I2C_Class_Struct *input_I2C_Class){
        /**
         * STOP codition
         * 
         * L|  /A|L
         *  |A/  | 
         */
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

    bool I2C_Driver_Wait_Ack(I2C_Class_Struct *input_I2C_Class){
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

    void I2C_Driver_Ack(I2C_Class_Struct *input_I2C_Class){
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

    void I2C_Driver_NAck(I2C_Class_Struct *input_I2C_Class){
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

    void I2C_Driver_Write(	I2C_Class_Struct	*input_I2C_Class,
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

    uint8_t I2C_Driver_Read(	I2C_Class_Struct	*input_I2C_Class,
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


    void I2C_GPIO_Write_SCL(	I2C_Class_Struct	*input_I2C_Class,
                                GPIO_PinState		input_PinState)
    {	
        HAL_GPIO_WritePin(
            input_I2C_Class ->SCL_Port,
            input_I2C_Class ->SCL_Pin,
            input_PinState
        );
    }

    void I2C_GPIO_Write_SDA(	I2C_Class_Struct	*input_I2C_Class,
                                GPIO_PinState		input_PinState)
    {
        HAL_GPIO_WritePin(
            input_I2C_Class ->SDA_Port,
            input_I2C_Class ->SDA_Pin,
            input_PinState
        );
    }

    void I2C_GPIO_Mode_SDA(	I2C_Class_Struct	*input_I2C_Class,
                            uint8_t				input_SDA_Mode)						
    {
        GPIO_InitTypeDef GPIO_InitStruct;

        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Pin = input_I2C_Class ->SDA_Pin;

        if(enum_OUTPUT == input_SDA_Mode){
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
        }else{
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        }

        HAL_GPIO_Init(
            input_I2C_Class ->SDA_Port, 
            &GPIO_InitStruct
        );
    }

    GPIO_PinState I2C_GPIO_Read_SDA(I2C_Class_Struct *input_I2C_Class){
        return HAL_GPIO_ReadPin(
            input_I2C_Class ->SDA_Port,
            input_I2C_Class ->SDA_Pin
        );
    }

/* GPIO Reference End */
#endif


