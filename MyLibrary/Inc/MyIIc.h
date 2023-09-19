/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : myIIc.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYIIC_H
#define MY_LIBARARY_MYIIC_H
#include "main.h"
#include "tim.h"
#include "MyGPIO.h"
#include "outputStream.h"
#include "inputStream.h"
#include "MyUsart.h"
#include "usart.h"


//#define EEPROMSWITCH 1 //如果是EEPROM将此置为1因为EEPROM速度较慢需要延时
using namespace stm32f407;
namespace stm32f407 {
    class MyIIc: public outputStream,public inputStream{
    public:
        MyIIc();
        MyIIc(GPIO *SDA,GPIO* SCL);
        MyIIc(GPIO *SDA,GPIO* SCL,uint8_t SlaveAddress);
        MyIIc(GPIO *SDA,GPIO* SCL,uint8_t SlaveAddress,uint8_t REGAddress);
        void Initialize();
        int SingleWrite(uint8_t SlaveAddress,uint8_t REGAddress,uint8_t REGData);
        int SingleRead(uint8_t SlaveAddress,uint8_t REGAddress);

        int SingleWrite(uint8_t REGAddress,uint8_t REGData);
        int SingleRead(uint8_t REGAddress);
        int MultipleRead(uint8_t SlaveAddress,uint8_t REGAddress,uint8_t * ptChar,uint8_t size);
        int MultipleWrite(uint8_t SlaveAddress,uint8_t REGAddress,uint8_t * ptChar,uint8_t size);
        virtual size_t write(uint8_t);
        virtual size_t write(const uint8_t *buffer, size_t size);

        virtual int available();
        virtual int read();
        void SetSlaveAddress(uint8_t SetData);
        void SetREGAddress(uint8_t SetData);
        uint8_t GetSlaveAddress();
        uint8_t GetREGAddress();
        size_t GetSize();
        uint8_t GetBeforeREGAddress();
//        virtual size_t write(uint8_t);
//        virtual size_t write(const uint8_t *buffer, size_t size);
    private:
        MyUsart * iicUart;
        GPIO *SDA,*SCL;
        void delay();
        int start();
        void stop();
        void  ack();
        void noAck();
        int waitAck();// 返回为1有Ack 0则没有Ack
        void SendByte(uint8_t SendByte);
        uint8_t ReadByte();
        uint8_t EEPROMSWITCH=0;
        uint8_t SlaveAddress;
        uint8_t REGAddress;
        uint16_t StreamSize;
        uint8_t BeforeREGAddress;
        uint16_t WriteFunCounter;
        uint16_t ReadFunCounter;
    };
}
#endif //MY_LIBARARY_MYIIC_H
