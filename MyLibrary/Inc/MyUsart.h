/**
*********************************************************************
*********
* @project_name :templateDevelopment
* @file : myUsart.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/4/27 
*********************************************************************
*********
*/
//

#ifndef TEMPLATEDEVELOPMENT_MYUSART_H
#define TEMPLATEDEVELOPMENT_MYUSART_H

#include "outputStream.h"
#include "inputStream.h"

#ifdef __cplusplus

extern "C"
{
#endif
#include "main.h"
#include "usart.h"
#define RELENTH 256
#define UART_RX_BUF_SIZE 256
/*______________________类的声明在这下面________________:*/
class MyUsart: public outputStream,public inputStream{
public:
    MyUsart();
    MyUsart(UART_HandleTypeDef *huart);
    MyUsart(uint16_t Enable,UART_HandleTypeDef *huart);
    void begin(uint32_t baudRate=115200);
    void ReceiveInit();
    uint8_t * ReceiveAgain();
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    void SendString(uint8_t * stringBuf);
    void SendString(const char* stringBuf);
    void SendOneInt(uint16_t IntBuf);
    void SendInt(uint16_t* IntBuf,uint16_t times);
    void SendOneFloat(float FloatBuf);
    void SendFloat(float *FloatBuf,uint16_t times);
   void ReceiveDataCallBack(DMA_HandleTypeDef * hdma_usartx_rx);
    void SendReceive();
    void SendCmdOut(char *fmt,...);
    void Myprintf(char *fmt,...);
    uint8_t * Re_GetData();
    uint8_t  GetRECV();
    uint8_t  GetRecvLenth();
    ~MyUsart();
private:
    UART_HandleTypeDef *huart;
    uint8_t   re_len=0;
    uint8_t   recv_end_flag=0;
    uint8_t   tx_Buff[UART_RX_BUF_SIZE];
    uint8_t   re_Buff[UART_RX_BUF_SIZE];
    uint8_t   temp[RELENTH];
};
extern MyUsart demomyUsart;
#ifdef __cplusplus
}
#endif
#endif //TEMPLATEDEVELOPMENT_MYUSART_H
