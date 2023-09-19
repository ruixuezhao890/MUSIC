/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : myIIc.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#include "MyIIc.h"
MyIIc::MyIIc(){

}
MyIIc::MyIIc(GPIO *SDA, GPIO *SCL) {
    this->SCL=SCL;this->SDA=SDA;
    this->Initialize();
}
MyIIc::MyIIc(GPIO *SDA, GPIO *SCL, uint8_t SlaveAddress) {
    this->SCL=SCL;this->SDA=SDA;this->SetSlaveAddress(SlaveAddress);
    this->Initialize();
}
MyIIc::MyIIc(GPIO *SDA, GPIO *SCL, uint8_t SlaveAddress, uint8_t REGAddress) {
    this->SCL=SCL;this->SDA=SDA;this->SetSlaveAddress(SlaveAddress);
    this->SetREGAddress(REGAddress);
    this->Initialize();
}
void MyIIc::Initialize() {
    this->stop();
    this->REGAddress=this->WriteFunCounter=0;
    this->ReadFunCounter=0;
    buff.write_index=buff.read_index=buff.data_size=0;
    for (int i = 0; i < RELENTH; ++i)
        buff.buf[i]=0;
    iicUart=new MyUsart(&huart1);
}
//单字节写入
int MyIIc::SingleWrite(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t REGData) {
    if (!start())return 0;
    SendByte(SlaveAddress);
    if (!waitAck()){stop();*iicUart<<"error1"<<endl;return 0;}
    SendByte(REGAddress);
    if (!waitAck()){stop();*iicUart<<"error2"<<endl;return 0;}
    SendByte(REGData);
    if (!waitAck()){stop();*iicUart<<"error3"<<endl;return 0;}
    stop();
#if EEPROMSWITCH
    HAL_Delay_ms(2);
#endif
    return 1;
}

int MyIIc::SingleRead(uint8_t SlaveAddress, uint8_t REGAddress) {
    unsigned char REGData=0;
    if (!start()){stop();*iicUart<<"error40"<<endl;return 0;}
    SendByte(SlaveAddress);
    if (waitAck()==0){stop();*iicUart<<"error41"<<endl;return 0;}
    SendByte(REGAddress);
    waitAck();
    start();
    SendByte(SlaveAddress+1);
    waitAck();
    REGData=ReadByte();
    stop();
    return REGData;
}

int MyIIc::MultipleRead(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t *ptChar, uint8_t size) {
    //uint8_t i;
//    if (size<1)return 0;
//    if (!start())return 0;
//    SendByte(SlaveAddress);
//    if (!waitAck()){stop();return 0;}
//    SendByte(REGAddress);waitAck();

//    start();
//    SendByte(SlaveAddress+1);waitAck();
//    for (i = 1; i <size; ++i) {
//        *ptChar++=SingleRead(SlaveAddress,REGAddress);
//        ack();
//    }
//    *ptChar++=ReadByte();
//    noAck();
//    stop();
   // iicUart->print("iicRead\n");
    if (size<1||ptChar== nullptr)return 0;
    while (size--){
        *ptChar++= SingleRead(SlaveAddress,REGAddress++);
        ack();
    }
//    *ptChar='\0';
    noAck();
    stop();

    this->BeforeREGAddress=0;
    return 1;
}

void MyIIc::delay() {
//    uint8_t i;
//    for (i = 0; i < 10; ++i) {
//
//    }
    HAL_Delay_us(2);
}

int MyIIc::start() {
    SDA->High();SCL->High();delay();
    if (SDA->IsLow())return 0;
    SDA->Low();delay();
    if (SDA->IsHigh())return 0;
    SCL->Low();delay();
    return 1;
}

void MyIIc::stop() {
    SCL->Low();delay();
    SDA->Low();delay();
    SCL->High();delay();
    SDA->High();delay();
}

void MyIIc::ack() {
    SCL->Low();delay();
    SDA->Low();delay();
    SCL->High();delay();
    SCL->Low();delay();
    //SDA->High();delay();//释放时钟线
}

void MyIIc::noAck() {
    SCL->Low();delay();
    SDA->High();delay();
    SCL->High();delay();
    SCL->Low();delay();

}

int MyIIc::waitAck() {
    //SDA->Initialize(GPIO_MODE_INPUT);
    SCL->Low();delay();
    SDA->High();delay();
    SCL->High();delay();
    if (SDA->IsHigh()){
        SCL->Low();delay();
        return 0;
    }
    SCL->Low();delay();
   //SDA->Initialize(GPIO_MODE_OUTPUT_PP);
    return 1;
}

void MyIIc::SendByte(uint8_t SendByte) {
    uint8_t i=8;
    while (i--){
        SCL->Low();delay();
        if ((SendByte&0x80)>>7)SDA->High();
        else SDA->Low();
        SendByte<<=1;delay();
        SCL->High();delay();

    }
    SCL->Low();
}

uint8_t MyIIc::ReadByte() {
    uint8_t i=8;
    uint8_t receiveByte=0;
    SDA->High();
    while (i--){
        receiveByte<<=1;
        SCL->Low();delay();
        SCL->High();delay();
        if (SDA->IsHigh()){
            receiveByte|=0x01;
        }
    }
    SCL->Low();
    return receiveByte;
}

int MyIIc::MultipleWrite(uint8_t SlaveAddress, uint8_t REGAddress, uint8_t *ptChar, uint8_t size) {
    if (size<1||ptChar== nullptr)return 0;
    if (!this->WriteFunCounter)  this->StreamSize=0;
    this->StreamSize+=size;
    REGAddress=WriteFunCounter>0?BeforeREGAddress:REGAddress;
    BeforeREGAddress+=size;
    while (size--) {
        this->SingleWrite(SlaveAddress,REGAddress,*ptChar);
        REGAddress++;
        ptChar++;
    }
    WriteFunCounter++;
    return 1;
}

size_t MyIIc::write(uint8_t data) {
   this->SingleWrite(this->SlaveAddress,this->REGAddress,data);
    return 1;
}

size_t MyIIc::write(const uint8_t *buffer, size_t size) {

    this->MultipleWrite(SlaveAddress,REGAddress,(uint8_t *)buffer,size);
    return 1;
}

void MyIIc::SetSlaveAddress(uint8_t SetData) {
    if (SetData>=0){
        this->SlaveAddress=SetData;
    }
}

void MyIIc::SetREGAddress(uint8_t SetData) {
    if (SetData>=0){
        this->REGAddress=SetData;
    }
}

uint8_t MyIIc::GetSlaveAddress() {
    return this->SlaveAddress;
}

uint8_t MyIIc::GetREGAddress() {
    return this->REGAddress;
}

size_t MyIIc::GetSize() {
    return (this->StreamSize-1);
}

uint8_t MyIIc::GetBeforeREGAddress() {
    return this->BeforeREGAddress;
}

int MyIIc::read() {
    if(!this->ReadFunCounter){
        MultipleRead(SlaveAddress,REGAddress,buff.buf,StreamSize);
        this->ReadFunCounter=1;
    }
   int temp=buff.buf[buff.read_index++];
    if (buff.read_index>=StreamSize){
        buff.write_index=buff.read_index=buff.data_size=0;
        for (int i = 0; i < RELENTH; ++i)
            buff.buf[i]=0;
        StreamSize=this->ReadFunCounter=0;
    }
    return temp;
}

int MyIIc::available() {
    return this->StreamSize;
}

int MyIIc::SingleWrite(uint8_t REGAddress, uint8_t REGData) {
    if (!start())return 0;
    SendByte((SlaveAddress<<1)|0);
    if (!waitAck()){stop();*iicUart<<"error1"<<endl;return 0;}
    SendByte(REGAddress);
    if (!waitAck()){stop();*iicUart<<"error2"<<endl;return 0;}
    SendByte(REGData);
    if (!waitAck()){stop();*iicUart<<"error3"<<endl;return 0;}
    stop();
#if EEPROMSWITCH
    HAL_Delay_ms(2);
#endif
    return 1;
}

int MyIIc::SingleRead(uint8_t REGAddress) {
    unsigned char REGData=0;
    if (!start()){stop();*iicUart<<"error40"<<endl;return 0;}
    SendByte(SlaveAddress);
    if (waitAck()==0){stop();*iicUart<<"error41"<<endl;return 0;}
    SendByte(REGAddress);
    waitAck();
    start();
    SendByte(SlaveAddress+1);
    waitAck();
    REGData=ReadByte();
    stop();
    return REGData;
}






