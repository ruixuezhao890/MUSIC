/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : Startup.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/11 
*********************************************************************
*********
*/
#include "ConnectCplusplus.h"
#include "ALLfile.h"
#include "touch.h"
#include "LVppDisplayer.h"
#include "MyI2s.h"
#include "MyI2SIT.h"

//using namespace stm32f407;
//using namespace lvgl;
//using namespace lvgl::core;
//using namespace lvgl::misc;
//using namespace lvgl::widgets;

MyUsart myUart(&huart1);
void printx(){
    printf("hello");
    demomyUsart<<"printx"<<endl;
}
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s){
    printf("world");
    demomyUsart<<"callback0"<<endl;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s){
    printf("nihao");
    demomyUsart<<"callback1"<<endl;
}

void demoTest(){
    uint16_t arr[10]={0};
    for (int i = 0; i < 10; ++i) {
        arr[i]=i;
    }













}


//std::vector<MusicPlay> musicCall;

void MyMain(){

    MusicPlay musicPlay(&hi2s2);
    MusicPlayHandleInit(&musicPlay);
//MyI2s demo(&hi2s2);

    for (;;){
//        demo.MyI2sPlayStart();
    musicPlay.MusicPlayASong();
    }


}
void Interrupt_management(DMA_HandleTypeDef * hdma_usartx_rx){
    myUart.ReceiveDataCallBack(hdma_usartx_rx);
}
void KeyTimeBenchmark(){
        buttonManage.buttonTick();
}
void I2sCallBackFunction(){
//    musicCall[0].GetWavplay().Get_i2s_handler()->CallbackI2s();
}