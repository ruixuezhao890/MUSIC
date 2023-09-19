/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : WM8978.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/11 
*********************************************************************
*********
*/
//

#include "WM8978.h"
static u16 WM8978_REGVAL_TBL[58]=
        {
                0X0000,0X0000,0X0000,0X0000,0X0050,0X0000,0X0140,0X0000,
                0X0000,0X0000,0X0000,0X00FF,0X00FF,0X0000,0X0100,0X00FF,
                0X00FF,0X0000,0X012C,0X002C,0X002C,0X002C,0X002C,0X0000,
                0X0032,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,0X0000,
                0X0038,0X000B,0X0032,0X0000,0X0008,0X000C,0X0093,0X00E9,
                0X0000,0X0000,0X0000,0X0000,0X0003,0X0010,0X0010,0X0100,
                0X0100,0X0002,0X0001,0X0001,0X0039,0X0039,0X0039,0X0039,
                0X0001,0X0001
        };
WM8978::WM8978(GPIO *SDA, GPIO *SCL, uint8_t SlaveAddress): MyIIc(SDA, SCL,SlaveAddress) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_SPI2_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2S2 GPIO Configuration
    PC2     ------> I2S2_ext_SD
    PC3     ------> I2S2_SD
    PB10     ------> I2S2_CK
    PB12     ------> I2S2_WS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF6_I2S2ext;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   uint8_t res=WM8978_Write_Reg(0,0);	//��λWM8978
    if(res){
        while (1){
            HAL_Delay(500);
            HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
        }
    }			//����ָ��ʧ��,WM8978�쳣
    //����Ϊͨ������
    WM8978_Write_Reg(1,0X1B);	//R1,MICEN����Ϊ1(MICʹ��),BIASEN����Ϊ1(ģ��������),VMIDSEL[1:0]����Ϊ:11(5K)
    WM8978_Write_Reg(2,0X1B0);	//R2,ROUT1,LOUT1���ʹ��(�������Թ���),BOOSTENR,BOOSTENLʹ��
    WM8978_Write_Reg(3,0X6C);	//R3,LOUT2,ROUT2���ʹ��(���ȹ���),RMIX,LMIXʹ��
    WM8978_Write_Reg(6,0);		//R6,MCLK���ⲿ�ṩ
    WM8978_Write_Reg(43,1<<4);	//R43,INVROUT2����,��������
    WM8978_Write_Reg(47,1<<8);	//R47����,PGABOOSTL,��ͨ��MIC���20������
    WM8978_Write_Reg(48,1<<8);	//R48����,PGABOOSTR,��ͨ��MIC���20������
    WM8978_Write_Reg(49,1<<1);	//R49,TSDEN,�������ȱ���
    WM8978_Write_Reg(10,1<<3);	//R10,SOFTMUTE�ر�,128x����,���SNR
    WM8978_Write_Reg(14,1<<3);	//R14,ADC 128x������
}

void WM8978::WM8978_ADDA_Cfg(u8 dacen, u8 adcen) {
    u16 regval;
    regval=WM8978_Read_Reg(3);	//��ȡR3
    if(dacen)regval|=3<<0;		//R3���2��λ����Ϊ1,����DACR&DACL
    else regval&=~(3<<0);		//R3���2��λ����,�ر�DACR&DACL.
    WM8978_Write_Reg(3,regval);	//����R3
    regval=WM8978_Read_Reg(2);	//��ȡR2
    if(adcen)regval|=3<<0;		//R2���2��λ����Ϊ1,����ADCR&ADCL
    else regval&=~(3<<0);		//R2���2��λ����,�ر�ADCR&ADCL.
    WM8978_Write_Reg(2,regval);	//����R2
}

WM8978::WM8978() {

}

void WM8978::WM8978_Input_Cfg(u8 micen, u8 lineinen, u8 auxen) {
    u16 regval;
    regval=WM8978_Read_Reg(2);	//��ȡR2
    if(micen)regval|=3<<2;		//����INPPGAENR,INPPGAENL(MIC��PGA�Ŵ�)
    else regval&=~(3<<2);		//�ر�INPPGAENR,INPPGAENL.
    WM8978_Write_Reg(2,regval);	//����R2

    regval=WM8978_Read_Reg(44);	//��ȡR44
    if(micen)regval|=3<<4|3<<0;	//����LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
    else regval&=~(3<<4|3<<0);	//�ر�LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
    WM8978_Write_Reg(44,regval);//����R44

    if(lineinen)WM8978_LINEIN_Gain(5);//LINE IN 0dB����
    else WM8978_LINEIN_Gain(0);	//�ر�LINE IN
    if(auxen)WM8978_AUX_Gain(7);//AUX 6dB����
    else WM8978_AUX_Gain(0);	//�ر�AUX����
}

void WM8978::WM8978_Output_Cfg(u8 dacen, u8 bpsen) {
    u16 regval=0;
    if(dacen)regval|=1<<0;	//DAC���ʹ��
    if(bpsen)
    {
        regval|=1<<1;		//BYPASSʹ��
        regval|=5<<2;		//0dB����
    }
    WM8978_Write_Reg(50,regval);//R50����
    WM8978_Write_Reg(51,regval);//R51����
}

void WM8978::WM8978_MIC_Gain(u8 gain) {
    gain&=0X3F;
    WM8978_Write_Reg(45,gain);		//R45,��ͨ��PGA����
    WM8978_Write_Reg(46,gain|1<<8);	//R46,��ͨ��PGA����
}

void WM8978::WM8978_LINEIN_Gain(u8 gain) {
    u16 regval;
    gain&=0X07;
    regval=WM8978_Read_Reg(47);	//��ȡR47
    regval&=~(7<<4);			//���ԭ��������
    WM8978_Write_Reg(47,regval|gain<<4);//����R47
    regval=WM8978_Read_Reg(48);	//��ȡR48
    regval&=~(7<<4);			//���ԭ��������
    WM8978_Write_Reg(48,regval|gain<<4);//����R48
}

void WM8978::WM8978_AUX_Gain(u8 gain) {
    u16 regval;
    gain&=0X07;
    regval=WM8978_Read_Reg(47);	//��ȡR47
    regval&=~(7<<0);			//���ԭ��������
    WM8978_Write_Reg(47,regval|gain<<0);//����R47
    regval=WM8978_Read_Reg(48);	//��ȡR48
    regval&=~(7<<0);			//���ԭ��������
    WM8978_Write_Reg(48,regval|gain<<0);//����R48
}

u8 WM8978::WM8978_Write_Reg(u8 reg, u8 val) {

    this->SingleWrite(reg,val);

    WM8978_REGVAL_TBL[reg]=val;	//����Ĵ���ֵ������

    return 0;
}

u16 WM8978::WM8978_Read_Reg(u8 reg) {
    return WM8978_REGVAL_TBL[reg];
}

void WM8978::WM8978_HPvol_Set(u8 voll, u8 volr) {
    voll&=0X3F;
    volr&=0X3F;//�޶���Χ
    if(voll==0)voll|=1<<6;//����Ϊ0ʱ,ֱ��mute
    if(volr==0)volr|=1<<6;//����Ϊ0ʱ,ֱ��mute
    WM8978_Write_Reg(52,voll);			//R52,������������������
    WM8978_Write_Reg(53,volr|(1<<8));	//R53,������������������,ͬ������(HPVU=1)
}

void WM8978::WM8978_SPKvol_Set(u8 volx) {
    volx&=0X3F;//�޶���Χ
    if(volx==0)volx|=1<<6;//����Ϊ0ʱ,ֱ��mute
    WM8978_Write_Reg(54,volx);			//R54,������������������
    WM8978_Write_Reg(55,volx|(1<<8));	//R55,������������������,ͬ������(SPKVU=1)
}

void WM8978::WM8978_I2S_Cfg(u8 fmt, u8 len) {

    fmt&=0X03;
    len&=0X03;//�޶���Χ
    WM8978_Write_Reg(4,(fmt<<3)|(len<<5));	//R4,WM8978����ģʽ����

}

void WM8978::WM8978_3D_Set(u8 depth) {
    depth&=0XF;//�޶���Χ
    WM8978_Write_Reg(41,depth);	//R41,3D��������
}

void WM8978::WM8978_EQ_3D_Dir(u8 dir) {
    u16 regval;
    regval=WM8978_Read_Reg(0X12);
    if(dir)regval|=1<<8;
    else regval&=~(1<<8);
    WM8978_Write_Reg(18,regval);//R18,EQ1�ĵ�9λ����EQ/3D����
}

void WM8978::WM8978_EQ1_Set(u8 cfreq, u8 gain) {
    u16 regval;
    cfreq&=0X3;//�޶���Χ
    if(gain>24)gain=24;
    gain=24-gain;
    regval=WM8978_Read_Reg(18);
    regval&=0X100;
    regval|=cfreq<<5;	//���ý�ֹƵ��
    regval|=gain;		//��������
    WM8978_Write_Reg(18,regval);//R18,EQ1����
}

void WM8978::WM8978_EQ2_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//�޶���Χ
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//���ý�ֹƵ��
    regval|=gain;		//��������
    WM8978_Write_Reg(19,regval);//R19,EQ2����
}

void WM8978::WM8978_EQ3_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//�޶���Χ
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//���ý�ֹƵ��
    regval|=gain;		//��������
    WM8978_Write_Reg(20,regval);//R20,EQ3����
}

void WM8978::WM8978_EQ4_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//�޶���Χ
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//���ý�ֹƵ��
    regval|=gain;		//��������
    WM8978_Write_Reg(21,regval);//R21,EQ4����
}

void WM8978::WM8978_EQ5_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//�޶���Χ
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//���ý�ֹƵ��
    regval|=gain;		//��������
    WM8978_Write_Reg(22,regval);//R22,EQ5����
}

uint8_t WM8978::wm8978_Reset(void) {
    const uint16_t reg_default[] = {
            0x000, 0x000, 0x000, 0x000, 0x050, 0x000, 0x140, 0x000,
            0x000, 0x000, 0x000, 0x0FF, 0x0FF, 0x000, 0x100, 0x0FF,
            0x0FF, 0x000, 0x12C, 0x02C, 0x02C, 0x02C, 0x02C, 0x000,
            0x032, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,
            0x038, 0x00B, 0x032, 0x000, 0x008, 0x00C, 0x093, 0x0E9,
            0x000, 0x000, 0x000, 0x000, 0x003, 0x010, 0x010, 0x100,
            0x100, 0x002, 0x001, 0x001, 0x039, 0x039, 0x039, 0x039,
            0x001, 0x001
    };
    uint8_t res;
    uint8_t i;

    res=WM8978_Write_Reg(0x00, 0);

    for (i = 0; i < sizeof(reg_default) / 2; i++)
    {
        WM8978_REGVAL_TBL[i] = reg_default[i];
    }
    return res;
}
