/**
*********************************************************************
*********
* @project_name :lvgl
* @file : LVpp.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/6 
*********************************************************************
*********
*/
//


#include "LVppDisplayer.h"
#include "MyUsart.h"

LvppDisplayer::LvppDisplayer(uint16_t t) {

}
MyUsart LVPPmyUart(&huart1);
void LvppDisplayer::test() {

    this->btn=new Button(scr_act());
    btn->set_pos(300,20);
    btn->set_size(100,100);
    static Label label0=Label(*btn);
    label0.set_text("hit");
    label0.center();
    auto event_cb=[](Event & e){

        if (e.get_code()==LV_EVENT_CLICKED){
//            temp+=10;
            LVPPmyUart<<"hit"<<endl;
//            label0.set_text_fmt("hit:%d",temp);
        }
        if (e.get_code()==LV_EVENT_LONG_PRESSED){
//            temp+=10;
            LVPPmyUart<<"longHit"<<endl;
//            label0.set_text_fmt("hit:%d",temp);
        }
    };
    btn->add_event_cb(event_cb,LV_EVENT_ALL);

}

LvppDisplayer::~LvppDisplayer() {

}
