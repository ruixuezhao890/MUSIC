/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : ConnectCplusplus.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/11 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_CONNECTCPLUSPLUS_H
#define MY_LIBARARY_CONNECTCPLUSPLUS_H
#ifdef __cplusplus
extern "C"
{
#endif
#include "main.h"
#include "gpio.h"
#include "usart.h"
#include "sdio.h"
#include "ff_gen_drv.h"
#include "fatfs.h"

void Step();
void MyMain();
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx);
void KeyTimeBenchmark();
void I2sCallBackFunction();
void printx();


#ifdef __cplusplus
}
#endif
#endif //MY_LIBARARY_CONNECTCPLUSPLUS_H
