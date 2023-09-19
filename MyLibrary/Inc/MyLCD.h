/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyLCD.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/18 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYLCD_H
#define MY_LIBARARY_MYLCD_H
#include "main.h"
#include "WString.h"
#include "lcd.h"
//#include "lvgl.h"
//#include "lv_port_disp.h"
//#include "lv_port_indev.h"
#include "inputStream.h"
#include "outputStream.h"

class MyLCD{
public:
    MyLCD(uint8_t mode=1);
    //void MyLCDGreatButton(uint16_t x,uint16_t y,String text,uint16_t color,CallbackFun callbackFun,uint16_t wight=(lcddev.width)/6,uint16_t high=(lcddev.height)/6);
    ~MyLCD();
private:
};
#endif //MY_LIBARARY_MYLCD_H
