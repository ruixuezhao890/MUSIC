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
   uint8_t res=WM8978_Write_Reg(0,0);	//软复位WM8978
    if(res){
        while (1){
            HAL_Delay(500);
            HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
        }
    }			//发送指令失败,WM8978异常
    //以下为通用设置
    WM8978_Write_Reg(1,0X1B);	//R1,MICEN设置为1(MIC使能),BIASEN设置为1(模拟器工作),VMIDSEL[1:0]设置为:11(5K)
    WM8978_Write_Reg(2,0X1B0);	//R2,ROUT1,LOUT1输出使能(耳机可以工作),BOOSTENR,BOOSTENL使能
    WM8978_Write_Reg(3,0X6C);	//R3,LOUT2,ROUT2输出使能(喇叭工作),RMIX,LMIX使能
    WM8978_Write_Reg(6,0);		//R6,MCLK由外部提供
    WM8978_Write_Reg(43,1<<4);	//R43,INVROUT2反向,驱动喇叭
    WM8978_Write_Reg(47,1<<8);	//R47设置,PGABOOSTL,左通道MIC获得20倍增益
    WM8978_Write_Reg(48,1<<8);	//R48设置,PGABOOSTR,右通道MIC获得20倍增益
    WM8978_Write_Reg(49,1<<1);	//R49,TSDEN,开启过热保护
    WM8978_Write_Reg(10,1<<3);	//R10,SOFTMUTE关闭,128x采样,最佳SNR
    WM8978_Write_Reg(14,1<<3);	//R14,ADC 128x采样率
}

void WM8978::WM8978_ADDA_Cfg(u8 dacen, u8 adcen) {
    u16 regval;
    regval=WM8978_Read_Reg(3);	//读取R3
    if(dacen)regval|=3<<0;		//R3最低2个位设置为1,开启DACR&DACL
    else regval&=~(3<<0);		//R3最低2个位清零,关闭DACR&DACL.
    WM8978_Write_Reg(3,regval);	//设置R3
    regval=WM8978_Read_Reg(2);	//读取R2
    if(adcen)regval|=3<<0;		//R2最低2个位设置为1,开启ADCR&ADCL
    else regval&=~(3<<0);		//R2最低2个位清零,关闭ADCR&ADCL.
    WM8978_Write_Reg(2,regval);	//设置R2
}

WM8978::WM8978() {

}

void WM8978::WM8978_Input_Cfg(u8 micen, u8 lineinen, u8 auxen) {
    u16 regval;
    regval=WM8978_Read_Reg(2);	//读取R2
    if(micen)regval|=3<<2;		//开启INPPGAENR,INPPGAENL(MIC的PGA放大)
    else regval&=~(3<<2);		//关闭INPPGAENR,INPPGAENL.
    WM8978_Write_Reg(2,regval);	//设置R2

    regval=WM8978_Read_Reg(44);	//读取R44
    if(micen)regval|=3<<4|3<<0;	//开启LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
    else regval&=~(3<<4|3<<0);	//关闭LIN2INPPGA,LIP2INPGA,RIN2INPPGA,RIP2INPGA.
    WM8978_Write_Reg(44,regval);//设置R44

    if(lineinen)WM8978_LINEIN_Gain(5);//LINE IN 0dB增益
    else WM8978_LINEIN_Gain(0);	//关闭LINE IN
    if(auxen)WM8978_AUX_Gain(7);//AUX 6dB增益
    else WM8978_AUX_Gain(0);	//关闭AUX输入
}

void WM8978::WM8978_Output_Cfg(u8 dacen, u8 bpsen) {
    u16 regval=0;
    if(dacen)regval|=1<<0;	//DAC输出使能
    if(bpsen)
    {
        regval|=1<<1;		//BYPASS使能
        regval|=5<<2;		//0dB增益
    }
    WM8978_Write_Reg(50,regval);//R50设置
    WM8978_Write_Reg(51,regval);//R51设置
}

void WM8978::WM8978_MIC_Gain(u8 gain) {
    gain&=0X3F;
    WM8978_Write_Reg(45,gain);		//R45,左通道PGA设置
    WM8978_Write_Reg(46,gain|1<<8);	//R46,右通道PGA设置
}

void WM8978::WM8978_LINEIN_Gain(u8 gain) {
    u16 regval;
    gain&=0X07;
    regval=WM8978_Read_Reg(47);	//读取R47
    regval&=~(7<<4);			//清除原来的设置
    WM8978_Write_Reg(47,regval|gain<<4);//设置R47
    regval=WM8978_Read_Reg(48);	//读取R48
    regval&=~(7<<4);			//清除原来的设置
    WM8978_Write_Reg(48,regval|gain<<4);//设置R48
}

void WM8978::WM8978_AUX_Gain(u8 gain) {
    u16 regval;
    gain&=0X07;
    regval=WM8978_Read_Reg(47);	//读取R47
    regval&=~(7<<0);			//清除原来的设置
    WM8978_Write_Reg(47,regval|gain<<0);//设置R47
    regval=WM8978_Read_Reg(48);	//读取R48
    regval&=~(7<<0);			//清除原来的设置
    WM8978_Write_Reg(48,regval|gain<<0);//设置R48
}

u8 WM8978::WM8978_Write_Reg(u8 reg, u8 val) {

    this->SingleWrite(reg,val);

    WM8978_REGVAL_TBL[reg]=val;	//保存寄存器值到本地

    return 0;
}

u16 WM8978::WM8978_Read_Reg(u8 reg) {
    return WM8978_REGVAL_TBL[reg];
}

void WM8978::WM8978_HPvol_Set(u8 voll, u8 volr) {
    voll&=0X3F;
    volr&=0X3F;//限定范围
    if(voll==0)voll|=1<<6;//音量为0时,直接mute
    if(volr==0)volr|=1<<6;//音量为0时,直接mute
    WM8978_Write_Reg(52,voll);			//R52,耳机左声道音量设置
    WM8978_Write_Reg(53,volr|(1<<8));	//R53,耳机右声道音量设置,同步更新(HPVU=1)
}

void WM8978::WM8978_SPKvol_Set(u8 volx) {
    volx&=0X3F;//限定范围
    if(volx==0)volx|=1<<6;//音量为0时,直接mute
    WM8978_Write_Reg(54,volx);			//R54,喇叭左声道音量设置
    WM8978_Write_Reg(55,volx|(1<<8));	//R55,喇叭右声道音量设置,同步更新(SPKVU=1)
}

void WM8978::WM8978_I2S_Cfg(u8 fmt, u8 len) {

    fmt&=0X03;
    len&=0X03;//限定范围
    WM8978_Write_Reg(4,(fmt<<3)|(len<<5));	//R4,WM8978工作模式设置

}

void WM8978::WM8978_3D_Set(u8 depth) {
    depth&=0XF;//限定范围
    WM8978_Write_Reg(41,depth);	//R41,3D环绕设置
}

void WM8978::WM8978_EQ_3D_Dir(u8 dir) {
    u16 regval;
    regval=WM8978_Read_Reg(0X12);
    if(dir)regval|=1<<8;
    else regval&=~(1<<8);
    WM8978_Write_Reg(18,regval);//R18,EQ1的第9位控制EQ/3D方向
}

void WM8978::WM8978_EQ1_Set(u8 cfreq, u8 gain) {
    u16 regval;
    cfreq&=0X3;//限定范围
    if(gain>24)gain=24;
    gain=24-gain;
    regval=WM8978_Read_Reg(18);
    regval&=0X100;
    regval|=cfreq<<5;	//设置截止频率
    regval|=gain;		//设置增益
    WM8978_Write_Reg(18,regval);//R18,EQ1设置
}

void WM8978::WM8978_EQ2_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//限定范围
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//设置截止频率
    regval|=gain;		//设置增益
    WM8978_Write_Reg(19,regval);//R19,EQ2设置
}

void WM8978::WM8978_EQ3_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//限定范围
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//设置截止频率
    regval|=gain;		//设置增益
    WM8978_Write_Reg(20,regval);//R20,EQ3设置
}

void WM8978::WM8978_EQ4_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//限定范围
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//设置截止频率
    regval|=gain;		//设置增益
    WM8978_Write_Reg(21,regval);//R21,EQ4设置
}

void WM8978::WM8978_EQ5_Set(u8 cfreq, u8 gain) {
    u16 regval=0;
    cfreq&=0X3;//限定范围
    if(gain>24)gain=24;
    gain=24-gain;
    regval|=cfreq<<5;	//设置截止频率
    regval|=gain;		//设置增益
    WM8978_Write_Reg(22,regval);//R22,EQ5设置
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
