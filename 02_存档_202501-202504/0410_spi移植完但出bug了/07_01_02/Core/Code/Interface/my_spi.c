#include "my_spi.h"

SPI_Class_Struct SPI_Class_1 = {
    .CS_Pin = SPI1_CS_PA4_Pin,
    .CS_Port = SPI1_CS_PA4_GPIO_Port
};

// SPI_HandleTypeDef g_spi1_handler;


SPI_HandleTypeDef g_spi1_handler; /* SPI1��� */
uint8_t SPI_1_RW(uint8_t input_Data){
    return SPI_Read_Write_Byte(&g_spi1_handler, input_Data);
}


void SPI_APP_Init(SPI_HandleTypeDef *input_SPI_HandleTypeDef){
    // MX_SPI1_Init();
    // __HAL_SPI_ENABLE(input_SPI_HandleTypeDef);
    // SPI_1_RW(0Xff); /* ��������, ʵ���Ͼ��ǲ���8��ʱ������, �ﵽ���DR������, �Ǳ��� */


    __HAL_RCC_SPI1_CLK_ENABLE(); /* SPI1ʱ��ʹ�� */

    g_spi1_handler.Instance = SPI1;                                /* SPI1 */
    g_spi1_handler.Init.Mode = SPI_MODE_MASTER;                        /* ����SPI����ģʽ������Ϊ��ģʽ */
    g_spi1_handler.Init.Direction = SPI_DIRECTION_2LINES;              /* ����SPI�������˫�������ģʽ:SPI����Ϊ˫��ģʽ */
    g_spi1_handler.Init.DataSize = SPI_DATASIZE_8BIT;                  /* ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ */
    g_spi1_handler.Init.CLKPolarity = SPI_POLARITY_HIGH;               /* ����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ */
    g_spi1_handler.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* ����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ����� */
    g_spi1_handler.Init.NSS = SPI_NSS_SOFT;                            /* NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ���� */
    g_spi1_handler.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* ���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 */
    g_spi1_handler.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ */
    g_spi1_handler.Init.TIMode = SPI_TIMODE_DISABLE;                   /* �ر�TIģʽ */
    g_spi1_handler.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* �ر�Ӳ��CRCУ�� */
    g_spi1_handler.Init.CRCPolynomial = 7;                             /* CRCֵ����Ķ���ʽ */
    HAL_SPI_Init(&g_spi1_handler);                                     /* ��ʼ�� */

    __HAL_SPI_ENABLE(&g_spi1_handler); /* ʹ��SPI1 */
    // g_spi1_handler = g_spi1_handler;
    SPI_1_RW(0Xff); /* ��������, ʵ���Ͼ��ǲ���8��ʱ������, �ﵽ���DR������, �Ǳ��� */

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    if (hspi->Instance == SPI1)
    {
        // SPI1_SCK_GPIO_CLK_ENABLE();  /* SPI1_SCK��ʱ��ʹ�� */
        // SPI1_MISO_GPIO_CLK_ENABLE(); /* SPI1_MISO��ʱ��ʹ�� */
        // SPI1_MOSI_GPIO_CLK_ENABLE(); /* SPI1_MOSI��ʱ��ʹ�� */

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /* SCK����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI1_SCK_PA5_Pin;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI1_SCK_PA5_GPIO_Port, &gpio_init_struct);

        /* MISO����ģʽ����(�������) */
        gpio_init_struct.Pin = SPI1_MISO_PA6_Pin;
        HAL_GPIO_Init(SPI1_MISO_PA6_GPIO_Port, &gpio_init_struct);

        /* MOSI����ģʽ����(�������) */
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
    assert_param(IS_SPI_BAUDRATE_PRESCALER(input_Speed));       /* �ж���Ч�� */
    __HAL_SPI_DISABLE(input_SPI_HandleTypeDef);                 /* �ر�SPI */
    input_SPI_HandleTypeDef->Instance->CR1 &= 0XFFC7;           /* λ3-5���㣬�������ò����� */
    input_SPI_HandleTypeDef->Instance->CR1 |= input_Speed << 3; /* ����SPI�ٶ� */
    __HAL_SPI_ENABLE(input_SPI_HandleTypeDef);                  /* ʹ��SPI */
}


void SPI_GPIO_Write_CS(SPI_Class_Struct *input_SPI_Class, GPIO_PinState input_PinState){
    HAL_GPIO_WritePin(
        input_SPI_Class->CS_Port,
        input_SPI_Class->CS_Pin,
        input_PinState
    );
}









