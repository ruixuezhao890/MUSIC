/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MySPI.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/21 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYSPI_H
#define MY_LIBARARY_MYSPI_H
#include "main.h"
#include "MyGPIO.h"
#include "tim.h"
#ifdef HAL_SPI_ERROR_NONE
#include "stm32f4xx_hal_spi.h"
# include "spi.h"
#endif
#define SPISPEED 1
#include "outputStream.h"
#include "inputStream.h"
enum SoftMode{
    SPIMode0=0,
    SPIMode1,
    SPIMode2,
    SPIMode3,
};
typedef enum ModeSelect{
    HardwareSPI=0,
    SoftwareSPI,
}modeSelect;
using namespace stm32f407;
namespace stm32f407 {
    class MySPI :public outputStream,public inputStream{
    public:
        MySPI();
#ifdef HAL_SPI_ERROR_NONE
        MySPI(SPI_HandleTypeDef* SPISelect,GPIO *CS);
#endif
        MySPI(GPIO* CS,GPIO* SCK,GPIO * MISO ,GPIO * MOSI);
        void SoftSPIModeSet(SoftMode Set);
        SoftMode GetSoftSPIMode();
        void SoftSPISpeedSet(uint16_t set=1);
        void SoftSPIInit();
        void SoftGPIOInit();
        uint8_t SoftSPIWR(uint8_t SendData);
        void SoftSPIW(uint8_t SendData);
        uint8_t SoftSPIR();
#ifdef HAL_SPI_ERROR_NONE
        uint8_t SPIWriteRead(uint8_t SendData);
#endif
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *, size_t);
        virtual int read();
        void SetCS(uint8_t set);
        ~MySPI();

    private:
        void SPIDelay();
        void SoftSPIW0(uint8_t SendData);
        void SoftSPIW1(uint8_t SendData);
        void SoftSPIW2(uint8_t SendData);
        void SoftSPIW3(uint8_t SendData);

        uint8_t SoftSPIR0();
        uint8_t SoftSPIR1();
        uint8_t SoftSPIR2();
        uint8_t SoftSPIR3();
        uint8_t SoftSPIMode0WR(uint8_t SendData);
        uint8_t SoftSPIMode1WR(uint8_t SendData);
        uint8_t SoftSPIMode2WR(uint8_t SendData);
        uint8_t SoftSPIMode3WR(uint8_t SendData);
        uint16_t SoftSPISPEED;
        SoftMode SoftSPIMode;
        GPIO *CS;GPIO* SCK;GPIO * MISO ;GPIO * MOSI;
        modeSelect Mode;
#ifdef HAL_SPI_ERROR_NONE
        SPI_HandleTypeDef* g_spi_handler;
#endif
        GPIO_InitTypeDef gpio_init_struct;



    };
}


#endif //MY_LIBARARY_MYSPI_H
