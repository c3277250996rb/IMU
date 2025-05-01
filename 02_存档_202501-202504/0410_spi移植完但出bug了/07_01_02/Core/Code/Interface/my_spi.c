#include "my_spi.h"

SPI_Class_Struct SPI_Class_1 = {
    .CS_Pin = SPI1_CS_PA4_Pin,
    .CS_Port = SPI1_CS_PA4_GPIO_Port
};

// SPI_HandleTypeDef g_spi1_handler;


SPI_HandleTypeDef g_spi1_handler; /* SPI1句柄 */
uint8_t SPI_1_RW(uint8_t input_Data){
    return SPI_Read_Write_Byte(&g_spi1_handler, input_Data);
}


void SPI_APP_Init(SPI_HandleTypeDef *input_SPI_HandleTypeDef){
    // MX_SPI1_Init();
    // __HAL_SPI_ENABLE(input_SPI_HandleTypeDef);
    // SPI_1_RW(0Xff); /* 启动传输, 实际上就是产生8个时钟脉冲, 达到清空DR的作用, 非必需 */


    __HAL_RCC_SPI1_CLK_ENABLE(); /* SPI1时钟使能 */

    g_spi1_handler.Instance = SPI1;                                /* SPI1 */
    g_spi1_handler.Init.Mode = SPI_MODE_MASTER;                        /* 设置SPI工作模式，设置为主模式 */
    g_spi1_handler.Init.Direction = SPI_DIRECTION_2LINES;              /* 设置SPI单向或者双向的数据模式:SPI设置为双线模式 */
    g_spi1_handler.Init.DataSize = SPI_DATASIZE_8BIT;                  /* 设置SPI的数据大小:SPI发送接收8位帧结构 */
    g_spi1_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;               /* 串行同步时钟的空闲状态为高电平 */
    g_spi1_handler.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* 串行同步时钟的第二个跳变沿（上升或下降）数据被采样 */
    g_spi1_handler.Init.NSS = SPI_NSS_SOFT;                            /* NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制 */
    g_spi1_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* 定义波特率预分频的值:波特率预分频值为256 */
    g_spi1_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始 */
    g_spi1_handler.Init.TIMode = SPI_TIMODE_DISABLE;                   /* 关闭TI模式 */
    g_spi1_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* 关闭硬件CRC校验 */
    g_spi1_handler.Init.CRCPolynomial = 7;                             /* CRC值计算的多项式 */
    HAL_SPI_Init(&g_spi1_handler);                                     /* 初始化 */

    __HAL_SPI_ENABLE(&g_spi1_handler); /* 使能SPI1 */
    // g_spi1_handler = g_spi1_handler;
    SPI_1_RW(0Xff); /* 启动传输, 实际上就是产生8个时钟脉冲, 达到清空DR的作用, 非必需 */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    if (hspi->Instance == SPI1)
    {
        // SPI1_SCK_GPIO_CLK_ENABLE();  /* SPI1_SCK脚时钟使能 */
        // SPI1_MISO_GPIO_CLK_ENABLE(); /* SPI1_MISO脚时钟使能 */
        // SPI1_MOSI_GPIO_CLK_ENABLE(); /* SPI1_MOSI脚时钟使能 */

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* SCK引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI1_SCK_PA5_Pin;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI1_SCK_PA5_GPIO_Port, &gpio_init_struct);

        /* MISO引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI1_MISO_PA6_Pin;
        HAL_GPIO_Init(SPI1_MISO_PA6_GPIO_Port, &gpio_init_struct);

        /* MOSI引脚模式设置(复用输出) */
        gpio_init_struct.Pin = SPI1_MOSI_PA7_Pin;
        HAL_GPIO_Init(SPI1_MOSI_PA7_GPIO_Port, &gpio_init_struct);
    }
}









uint8_t SPI_Read_Write_Byte(SPI_HandleTypeDef *input_SPI_HandleTypeDef, uint8_t input_Data){
    uint8_t temp_Data;
    HAL_SPI_TransmitReceive(input_SPI_HandleTypeDef, &input_Data, &temp_Data, 1 , 1000);
    return temp_Data;
}

void SPI_Set_Speed(SPI_HandleTypeDef *input_SPI_HandleTypeDef, uint8_t input_Speed){
    assert_param(IS_SPI_BAUDRATE_PRESCALER(input_Speed));       /* 判断有效性 */
    __HAL_SPI_DISABLE(input_SPI_HandleTypeDef);                 /* 关闭SPI */
    input_SPI_HandleTypeDef->Instance->CR1 &= 0XFFC7;           /* 位3-5清零，用来设置波特率 */
    input_SPI_HandleTypeDef->Instance->CR1 |= input_Speed << 3; /* 设置SPI速度 */
    __HAL_SPI_ENABLE(input_SPI_HandleTypeDef);                  /* 使能SPI */
}


void SPI_GPIO_Write_CS(SPI_Class_Struct *input_SPI_Class, GPIO_PinState input_PinState){
    HAL_GPIO_WritePin(
        input_SPI_Class->CS_Port,
        input_SPI_Class->CS_Pin,
        input_PinState
    );
}









