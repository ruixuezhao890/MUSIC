/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyI2s.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/11 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYI2S_H
#define MY_LIBARARY_MYI2S_H
//#include "MusicPlay.h"
#include "i2s.h"
//#include "stm32f4xx_hal_i2s.h"
#define GPIO_AF_I2S_SPI                 GPIO_AF5_SPI2

#define I2S_LRCK_GPIO_PORT              GPIOB
#define I2S_LRCK_GPIO_PIN               GPIO_PIN_12
#define I2S_LRCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

#define I2S_SCLK_GPIO_PORT              GPIOB
#define I2S_SCLK_GPIO_PIN               GPIO_PIN_13
#define I2S_SCLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

#define I2S_SDOUT_GPIO_PORT             GPIOC
#define I2S_SDOUT_GPIO_PIN              GPIO_PIN_2
#define I2S_SDOUT_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define I2S_SDIN_GPIO_PORT              GPIOC
#define I2S_SDIN_GPIO_PIN               GPIO_PIN_3
#define I2S_SDIN_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define I2S_MCLK_GPIO_PORT              GPIOC
#define I2S_MCLK_GPIO_PIN               GPIO_PIN_6
#define I2S_MCLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define I2S_SPI                         SPI2
#define I2S_SPI_CLK_ENABLE()            do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)      /* I2S2ʱ��ʹ��*/

#define I2S_TX_DMASx                    DMA1_Stream4
#define I2S_TX_DMASx_Channel            DMA_CHANNEL_0
#define I2S_TX_DMASx_Handle             DMA1_Stream4_IRQHandler
#define I2S_TX_DMASx_IRQ                DMA1_Stream4_IRQn
#define I2S_TX_DMASx_FLAG               DMA_FLAG_TCIF0_4
#define I2S_TX_DMA_CLK_ENABLE()         do{ __HAL_RCC_DMA1_CLK_ENABLE(); }while(0)   /* I2S2 TX DMAʱ��ʹ�� */
class MyI2s {
private:
public:
    MyI2s();
    MyI2s(uint16_t flag);
   void i2sInit(uint32_t i2s_standard, uint32_t i2s_mode, uint32_t i2s_clock_polarity, uint32_t i2s_dataformat);
    void i2s_dma_enable(void);      /* ����I2S��DMA���� */
    uint8_t i2s_samplerate_set(uint32_t samplerate);    /* ����I2S�Ĳ����� */
    void i2s_tx_dma_init(uint8_t* buf0, uint8_t *buf1, uint16_t num);   /* I2S TX DMA���� */
//    void I2Sx_TX_DMA_Init(const uint16_t *buffer0,const uint16_t *buffer1,const uint32_t num);
    void i2s_play_start(void);      /* I2S��ʼ���� */
    void i2s_play_stop(void);       /* I2Sֹͣ���� */
//    void (*i2s_tx_callback)(void);/* I2S TX�ص�����ָ��  */
    void CallbackI2s(void);
protected:
//    void (*i2s_tx_callback)(void);
    I2S_HandleTypeDef g_i2s_handler;         /* I2S��� */
    DMA_HandleTypeDef g_i2s_txdma_handler;   /* I2S����DMA��� */
    ~MyI2s();
};


#endif //MY_LIBARARY_MYI2S_H
