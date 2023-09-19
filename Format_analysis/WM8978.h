/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : WM8978.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/11 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_WM8978_H
#define MY_LIBARARY_WM8978_H
#include "MyIIc.h"
#include "sys.h"
#define WM8978_ADDR				0X1A	//WM8978��������ַ,�̶�Ϊ0X1A

#define EQ1_80Hz		0X00
#define EQ1_105Hz		0X01
#define EQ1_135Hz		0X02
#define EQ1_175Hz		0X03

#define EQ2_230Hz		0X00
#define EQ2_300Hz		0X01
#define EQ2_385Hz		0X02
#define EQ2_500Hz		0X03

#define EQ3_650Hz		0X00
#define EQ3_850Hz		0X01
#define EQ3_1100Hz		0X02
#define EQ3_14000Hz		0X03

#define EQ4_1800Hz		0X00
#define EQ4_2400Hz		0X01
#define EQ4_3200Hz		0X02
#define EQ4_4100Hz		0X03

#define EQ5_5300Hz		0X00
#define EQ5_6900Hz		0X01
#define EQ5_9000Hz		0X02
#define EQ5_11700Hz		0X03
class WM8978: public MyIIc {
public:
    WM8978();

    WM8978(GPIO *SDA, GPIO *SCL, uint8_t SlaveAddress=WM8978_ADDR);
    void WM8978_ADDA_Cfg(u8 dacen,u8 adcen);
    void WM8978_Input_Cfg(u8 micen,u8 lineinen,u8 auxen);
    void WM8978_Output_Cfg(u8 dacen,u8 bpsen);
    void WM8978_MIC_Gain(u8 gain);
    void WM8978_LINEIN_Gain(u8 gain);
    void WM8978_AUX_Gain(u8 gain);

    void WM8978_HPvol_Set(u8 voll,u8 volr);
    void WM8978_SPKvol_Set(u8 volx);
    void WM8978_I2S_Cfg(u8 fmt,u8 len);
    void WM8978_3D_Set(u8 depth);
    void WM8978_EQ_3D_Dir(u8 dir);
    void WM8978_EQ1_Set(u8 cfreq,u8 gain);
    void WM8978_EQ2_Set(u8 cfreq,u8 gain);
    void WM8978_EQ3_Set(u8 cfreq,u8 gain);
    void WM8978_EQ4_Set(u8 cfreq,u8 gain);
    void WM8978_EQ5_Set(u8 cfreq,u8 gain);
    uint8_t wm8978_Reset(void);
protected:
    u8 WM8978_Write_Reg(u8 reg,u8 val);
    u16 WM8978_Read_Reg(u8 reg);
};


#endif //MY_LIBARARY_WM8978_H
