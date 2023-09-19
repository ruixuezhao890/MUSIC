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
#define I2S_LRCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB口时钟使能 */

#define I2S_SCLK_GPIO_PORT              GPIOB
#define I2S_SCLK_GPIO_PIN               GPIO_PIN_13
#define I2S_SCLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB口时钟使能 */

#define I2S_SDOUT_GPIO_PORT             GPIOC
#define I2S_SDOUT_GPIO_PIN              GPIO_PIN_2
#define I2S_SDOUT_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC口时钟使能 */

#define I2S_SDIN_GPIO_PORT              GPIOC
#define I2S_SDIN_GPIO_PIN               GPIO_PIN_3
#define I2S_SDIN_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC口时钟使能 */

#define I2S_MCLK_GPIO_PORT              GPIOC
#define I2S_MCLK_GPIO_PIN               GPIO_PIN_6
#define I2S_MCLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC口时钟使能 */

#define I2S_SPI                         SPI2
#define I2S_SPI_CLK_ENABLE()            do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)      /* I2S2时钟使能*/

#define I2S_TX_DMASx                    DMA1_Stream4
#define I2S_TX_DMASx_Channel            DMA_CHANNEL_0
#define I2S_TX_DMASx_Handle             DMA1_Stream4_IRQHandler
#define I2S_TX_DMASx_IRQ                DMA1_Stream4_IRQn
#define I2S_TX_DMASx_FLAG               DMA_FLAG_TCIF0_4
#define I2S_TX_DMA_CLK_ENABLE()         do{ __HAL_RCC_DMA1_CLK_ENABLE(); }while(0)   /* I2S2 TX DMA时钟使能 */
class MyI2s {
private:
public:
    MyI2s();
    MyI2s(uint16_t flag);
   void i2sInit(uint32_t i2s_standard, uint32_t i2s_mode, uint32_t i2s_clock_polarity, uint32_t i2s_dataformat);
    void i2s_dma_enable(void);      /* 开启I2S的DMA功能 */
    uint8_t i2s_samplerate_set(uint32_t samplerate);    /* 设置I2S的采样率 */
    void i2s_tx_dma_init(uint8_t* buf0, uint8_t *buf1, uint16_t num);   /* I2S TX DMA配置 */
//    void I2Sx_TX_DMA_Init(const uint16_t *buffer0,const uint16_t *buffer1,const uint32_t num);
    void i2s_play_start(void);      /* I2S开始播放 */
    void i2s_play_stop(void);       /* I2S停止播放 */
//    void (*i2s_tx_callback)(void);/* I2S TX回调函数指针  */
    void CallbackI2s(void);
protected:
//    void (*i2s_tx_callback)(void);
    I2S_HandleTypeDef g_i2s_handler;         /* I2S句柄 */
    DMA_HandleTypeDef g_i2s_txdma_handler;   /* I2S发送DMA句柄 */
    ~MyI2s();
};


#endif //MY_LIBARARY_MYI2S_H
