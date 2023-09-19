/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyI2SIT.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/16 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYI2SIT_H
#define MY_LIBARARY_MYI2SIT_H
#include "sys.h"



#ifdef __cplusplus
extern "C"
{
#endif

//extern DMA_HandleTypeDef I2S2_TXDMA_HandlerForC;   //I2S2发送DMA句柄为了c调用

HAL_StatusTypeDef HAL_I2S_Transmit_DMAEx(I2S_HandleTypeDef *hi2s, uint16_t *FirstBuffer, uint16_t *SecondBuffer, uint16_t Size);

static void DMAEx_XferCpltCallback( struct __DMA_HandleTypeDef *hdma);
static void DMAEx_XferM1CpltCallback(struct __DMA_HandleTypeDef *hdma);
static void DMAEx_XferErrorCallback( struct __DMA_HandleTypeDef *hdma);
/*______________________类的声明在这下面________________:*/

#ifdef __cplusplus
}
#endif



#endif //MY_LIBARARY_MYI2SIT_H
