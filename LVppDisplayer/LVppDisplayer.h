/**
*********************************************************************
*********
* @project_name :lvgl
* @file : LVpp.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/6 
*********************************************************************
*********
*/
#ifndef LVGL_LVPP_H
#define LVGL_LVPP_H
#include "main.h"
#include "IncludeLvpp.h"
using namespace lvgl::core;
using namespace lvgl::widgets;
#ifdef __cplusplus
extern "C"
{
#endif
/*______________________类的声明在这下面________________:*/
class LvppDisplayer{
public:
    LvppDisplayer(uint16_t t);
    void test();
    ~LvppDisplayer();
private:
    Button*  btn;
    Label* label;

};


#ifdef __cplusplus
}
#endif
#endif //LVGL_LVPP_H
