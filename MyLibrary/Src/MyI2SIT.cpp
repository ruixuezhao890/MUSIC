/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyI2SIT.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/16 
*********************************************************************
*********
*/
//

#include "MyI2SIT.h"
#include "MyUsart.h"
#include "MusicPlay.h"
MusicPlay *MusicPlayHandle;
void MusicPlayHandleInit(MusicPlay* temp){
    MusicPlayHandle=temp;
}
HAL_StatusTypeDef HAL_I2S_Transmit_DMAEx(I2S_HandleTypeDef *hi2s, uint16_t *FirstBuffer, uint16_t *SecondBuffer, uint16_t Size){
    uint32_t tmpreg_cfgr;

    if ((FirstBuffer == NULL) || (SecondBuffer == NULL) || (Size == 0U))
    {
        return HAL_ERROR;
    }

    /* Process Locked */
    __HAL_LOCK(hi2s);

    if (hi2s->State != HAL_I2S_STATE_READY)
    {
        __HAL_UNLOCK(hi2s);
        return HAL_BUSY;
    }

    /* Set state and reset error code */
    hi2s->State = HAL_I2S_STATE_BUSY_TX;
    hi2s->ErrorCode = HAL_I2S_ERROR_NONE;
    hi2s->pTxBuffPtr = FirstBuffer;

    tmpreg_cfgr = hi2s->Instance->I2SCFGR
                  & (SPI_I2SCFGR_DATLEN | SPI_I2SCFGR_CHLEN);

    if ((tmpreg_cfgr == I2S_DATAFORMAT_24B)
        || (tmpreg_cfgr == I2S_DATAFORMAT_32B))
    {
        hi2s->TxXferSize = (Size << 1U);
        hi2s->TxXferCount = (Size << 1U);
    }
    else
    {
        hi2s->TxXferSize = Size;
        hi2s->TxXferCount = Size;
    }

    /* Set the I2S Tx DMA Half transfer complete callback */
//    hi2s->hdmatx->XferHalfCpltCallback = NULL;
//    hi2s->hdmatx->XferM1HalfCpltCallback = NULL;

    /* Set the I2S Tx DMA transfer complete callback */
    hi2s->hdmatx->XferHalfCpltCallback = DMAEx_XferCpltCallback;
    hi2s->hdmatx->XferM1HalfCpltCallback = DMAEx_XferM1CpltCallback;

    /* Set the DMA error callback */
    hi2s->hdmatx->XferErrorCallback = DMAEx_XferErrorCallback;

    /* Set the DMA abort callback */
    hi2s->hdmatx->XferAbortCallback = NULL;

    /* Enable the Tx DMA Stream/Channel */
    if (HAL_OK != HAL_DMAEx_MultiBufferStart_IT(hi2s->hdmatx, (uint32_t) FirstBuffer, (uint32_t) &hi2s->Instance->DR, (uint32_t) SecondBuffer,	hi2s->TxXferSize))
    {
        /* Update SPI error code */
        SET_BIT(hi2s->ErrorCode, HAL_I2S_ERROR_DMA);
        hi2s->State = HAL_I2S_STATE_READY;

        __HAL_UNLOCK(hi2s);
        return HAL_ERROR;
    }
    __HAL_I2S_ENABLE_IT(hi2s, (I2S_IT_TXE | I2S_IT_ERR));
    /* Check if the I2S is already enabled */
    if (HAL_IS_BIT_CLR(hi2s->Instance->I2SCFGR, SPI_I2SCFGR_I2SE))
    {
        /* Enable I2S peripheral */
        __HAL_I2S_ENABLE(hi2s);
    }

    /* Check if the I2S Tx request is already enabled */
    if (HAL_IS_BIT_CLR(hi2s->Instance->CR2, SPI_CR2_TXDMAEN))
    {
        /* Enable Tx DMA Request */
        SET_BIT(hi2s->Instance->CR2, SPI_CR2_TXDMAEN);
    }

    __HAL_UNLOCK(hi2s);
    return HAL_OK;

}
#define I2S_TX_DMA_BUF_SIZE    8192
void DMAEx_XferCpltCallback( struct __DMA_HandleTypeDef *hdma){



     demomyUsart<<"BUF0"<<endl;
     MusicPlayHandle->wavwitchbuf=0;

     if ((MusicPlayHandle->getPlayStaut()&0x01)==0){
         for (uint16_t i = 0; i <I2S_TX_DMA_BUF_SIZE ; i++)    /* ФЭНЈ */
         {
             MusicPlayHandle->getMusicI2S()->getMDocumentAnalysis()->getBuf1()[i]=0;                 /* Моід0 */
         }
//         MusicPlayHandle->getMusicI2S()->MyI2sPlayStop();
//        demomyUsart<<"stop0"<<endl;
     }
    MusicPlayHandle->wavtransferend=1;
}
 void DMAEx_XferM1CpltCallback(struct __DMA_HandleTypeDef *hdma){
     demomyUsart<<"\nBUF1"<<endl;
    // demomyUsart<<MusicPlayHandle->getPlayStaut();
     MusicPlayHandle->wavwitchbuf=1;
     if ((MusicPlayHandle->getPlayStaut()&0x01)==0) {

         for (uint16_t i = 0; i <I2S_TX_DMA_BUF_SIZE ; i++)    /* ФЭНЈ */
         {
             MusicPlayHandle->getMusicI2S()->getMDocumentAnalysis()->getBuf0()[i]=0;                 /* Моід0 */
         }
//         MusicPlayHandle->getMusicI2S()->MyI2sPlayStop();
         demomyUsart<<"stop1"<<endl;
     }
     MusicPlayHandle->wavtransferend=1;
//     uint8_t res=(MusicPlayHandle->getMusicI2S()->getMDocumentAnalysis()->DocumentAnalysisTransmitBufFill());
//     demomyUsart<<"res1:"<<res;
//     if (res==0){
//         MusicPlayHandle->getMusicI2S()->MyI2sPlayStop();
//         demomyUsart<<"stop1"<<endl;
//     }
}
 void DMAEx_XferErrorCallback( struct __DMA_HandleTypeDef *hdma) {
    demomyUsart << "DMAEx_XferErrorCallback" << endl;
}
