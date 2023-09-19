/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyI2s.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/15 
*********************************************************************
*********
*/
//

#include "MyI2s.h"
#include "MyI2SIT.h"
#include "malloc.h"
#include "MyUsart.h"
#include "stm32f4xx_it.h"
#include "tim.h"
std::vector<MyI2s *> MyI2sItHandle;
DMA_HandleTypeDef I2S2_TXDMA_HandlerForC;   //I2S2发送DMA句柄为了c调用
const u16 I2S_PSC_TBL[][5]=
        {
                {800 ,256,5,12,1},		//8Khz采样率
                {1102,429,4,19,0},		//11.025Khz采样率
                {1600,213,2,13,0},		//16Khz采样率
                {2205,429,4, 9,1},		//22.05Khz采样率
                {3200,213,2, 6,1},		//32Khz采样率
                {4410,271,2, 6,0},		//44.1Khz采样率
                {4800,258,3, 3,1},		//48Khz采样率
                {8820,316,2, 3,1},		//88.2Khz采样率
                {9600,344,2, 3,1},  	//96Khz采样率
                {17640,361,2,2,0},  	//176.4Khz采样率
                {19200,393,2,2,0},  	//192Khz采样率
        };
MyI2s::MyI2s() {

}

MyI2s::MyI2s(I2S_HandleTypeDef *hi2s) {
    this->m_hi2s=hi2s;
//    MyI2sItHandle.push_back(this);
    m_DocumentAnalysis=new DocumentAnalysis();
    this->MyI2sReInit();
//    I2sBuf0=(uint8_t *)mymalloc(SRAMIN, I2S_TX_DMA_BUF_SIZE);
//    I2sBuf1=(uint8_t *)mymalloc(SRAMIN, I2S_TX_DMA_BUF_SIZE);
    m_DocumentAnalysis->DocumentAnalysisInit(ROOTPATH);

}
/***************************************************************
  *  @brief     函数作用 设置I2S的采样率
  *  @param     参数   采样率
  *  @note      备注 ：在得到一首歌的歌曲信息之后才能使用
  *  @Sample usage:     函数的使用方法 ：直接调用
   * @author    ZEN3
 **************************************************************/

//uint8_t MyI2s::MyI2sSamplerateInit() {
//    HAL_I2S_DeInit(m_hi2s);//去初始化
//    m_hi2s->Instance = SPI2;
//    m_hi2s->Init.Mode = I2S_MODE_MASTER_TX;
//    m_hi2s->Init.Standard = I2S_STANDARD_PHILIPS;
//    m_hi2s->Init.DataFormat = I2S_DATAFORMAT_16B;
//    m_hi2s->Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
//    m_hi2s->Init.AudioFreq = I2S_AUDIOFREQ_8K;
//    m_hi2s->Init.CPOL = I2S_CPOL_LOW;
//    m_hi2s->Init.ClockSource = I2S_CLOCK_PLL;
//    m_hi2s->Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
//    if (HAL_I2S_Init(m_hi2s) != HAL_OK) {
//        Error_Handler();
//    }
//    DMA_HandleTypeDef I2S2_TXDMA_Handler;
//
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
//    uint16_t i;
//    if (m_hi2s->Instance == SPI2) {
//        //初始化采样率
//        for(i=0;i<(sizeof(I2S_PSC_TBL)/10);i++)//看看改采样率是否可以支持
//        {
//            if(((this->m_DocumentAnalysis.getSamplerate())/10)==I2S_PSC_TBL[i][0])break;
//        }
//        if(i==(sizeof(I2S_PSC_TBL)/10))return 1;//搜遍了也找不到
//
//        PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_I2S;	//外设时钟源选择
//        PeriphClkInitStruct.PLLI2S.PLLI2SN=(u32)I2S_PSC_TBL[i][1];    	//设置PLLI2SN
//        PeriphClkInitStruct.PLLI2S.PLLI2SR=(u32)I2S_PSC_TBL[i][2];    	//设置PLLI2SR
//        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
//            Error_Handler();
//        }
//
//        /* I2S2 clock enable */
//        __HAL_RCC_SPI2_CLK_ENABLE();
//
//        __HAL_RCC_GPIOC_CLK_ENABLE();
//        __HAL_RCC_GPIOB_CLK_ENABLE();
//        /**I2S2 GPIO Configuration
//        PC2     ------> I2S2_ext_SD
//        PC3     ------> I2S2_SD
//        PB12     ------> I2S2_WS
//        PB13     ------> I2S2_CK
//        PC6     ------> I2S2_MCK
//        */
//        //初始化对应引脚
//        GPIO_InitStruct.Pin = GPIO_PIN_2;
//        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//        GPIO_InitStruct.Alternate = GPIO_AF6_I2S2ext;
//        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//        GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_6;
//        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//        GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13;
//        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//        GPIO_InitStruct.Pull = GPIO_NOPULL;
//        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
//        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//        HAL_DMA_DeInit(&hdma_spi2_tx);
//        /* I2S2 DMA Init */
//        /* SPI2_TX Init */
//        __HAL_RCC_DMA1_CLK_ENABLE();
//        I2S2_TXDMA_Handler.Instance = DMA1_Stream4;
//        I2S2_TXDMA_Handler.Init.Channel = DMA_CHANNEL_0;
//        I2S2_TXDMA_Handler.Init.Direction = DMA_MEMORY_TO_PERIPH;
//        I2S2_TXDMA_Handler.Init.PeriphInc = DMA_PINC_DISABLE;
//        I2S2_TXDMA_Handler.Init.MemInc = DMA_MINC_ENABLE;
//        I2S2_TXDMA_Handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//        I2S2_TXDMA_Handler.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//        I2S2_TXDMA_Handler.Init.Mode = DMA_CIRCULAR;
//        I2S2_TXDMA_Handler.Init.Priority = DMA_PRIORITY_HIGH;
//        I2S2_TXDMA_Handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//        if (HAL_DMA_Init(&I2S2_TXDMA_Handler) != HAL_OK) {
//            Error_Handler();
//        }
//
//        __HAL_LINKDMA(m_hi2s, hdmatx, I2S2_TXDMA_Handler);
//
//        /* I2S2 interrupt Init */
//        HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
//        HAL_NVIC_EnableIRQ(SPI2_IRQn);
//
//
//
//    }
//    demomyUsart<<"ReInit OK"<<endl;
//    return 0;
//}

void MyI2s::MyI2sPlayStart() {

   HAL_I2S_Transmit_DMAEx(m_hi2s,(uint16_t *)m_DocumentAnalysis->getBuf0(),(u16 *)
                          m_DocumentAnalysis->getBuf1(),I2S_TX_DMA_BUF_SIZE/2);//开启双缓冲
 //   HAL_I2S_Transmit_DMA(m_hi2s,(uint16_t *)m_DocumentAnalysis->getBuf0(),I2S_TX_DMA_BUF_SIZE);

}

void MyI2s::MyI2sPlayStop() {

    HAL_I2S_DMAStop(&I2S2_Handler);
}

DocumentAnalysis *MyI2s::getMDocumentAnalysis() {
    return m_DocumentAnalysis;
}

//uint8_t *MyI2s::getI2SBuf0()  {
//    return I2sBuf0;
//}
//
//uint8_t *MyI2s::getI2SBuf1()  {
//    return I2sBuf1;
//}

void MyI2s::MyI2sPlayPause() {
    HAL_I2S_DMAPause(&I2S2_Handler);
}

void MyI2s::MyI2sPlayResume() {
    HAL_I2S_DMAResume(&I2S2_Handler);
}

void MyI2s::MyI2sDeStruct() {
//    myfree(SRAMIN,I2sBuf0);
//    myfree(SRAMIN,I2sBuf1);
    delete m_DocumentAnalysis;
}

MyI2s::~MyI2s() {
    MyI2sDeStruct();
}
/***************************************************************
  *  @brief     函数作用 设置I2S的采样率 开启双缓冲模式
  *  @param     参数   无
  *  @note      备注 ：在得到一首歌的歌曲信息之后才能使用 并且使用完
  *                     该函数之后就可以直接开始播放
  *  @Sample usage:     函数的使用方法 ：直接调用
   * @author    ZEN3
 **************************************************************/

uint8_t MyI2s::MyI2sReInit() {
    HAL_I2S_DMAStop(m_hi2s);
    HAL_I2S_DeInit(m_hi2s);//去初始化
//    HAL_DMA_Abort(m_hi2s->hdmatx);
//    HAL_DMA_DeInit(m_hi2s->hdmatx);
    m_hi2s->Instance = SPI2;
    m_hi2s->Init.Mode = I2S_MODE_MASTER_TX;
    m_hi2s->Init.Standard = I2S_STANDARD_PHILIPS;
//    m_hi2s->Init.DataFormat = I2S_DATAFORMAT_16B;
    if (m_DocumentAnalysis->getBps()==24){
        m_hi2s->Init.DataFormat=I2S_DATAFORMAT_24B;		//IIS数据长度
    }else{
        m_hi2s->Init.DataFormat=I2S_DATAFORMAT_16B_EXTENDED;
    }
    m_hi2s->Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
    m_hi2s->Init.AudioFreq = I2S_AUDIOFREQ_8K;
    m_hi2s->Init.CPOL = I2S_CPOL_LOW;
    m_hi2s->Init.ClockSource = I2S_CLOCK_PLL;
    m_hi2s->Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
    if (HAL_I2S_Init(m_hi2s) != HAL_OK)
    {
        Error_Handler();
    }

//    I2S2_Handler.Instance=SPI2;
//    I2S2_Handler.Init.Mode=I2S_MODE_MASTER_TX;					//IIS模式
//    I2S2_Handler.Init.Standard=I2S_STANDARD_PHILIPS;			//IIS标准
//    if (m_DocumentAnalysis.getBps()==24){
//        I2S2_Handler.Init.DataFormat=I2S_DATAFORMAT_24B;		//IIS数据长度
//    }else{
//        I2S2_Handler.Init.DataFormat=I2S_DATAFORMAT_16B_EXTENDED;
//    }
//
//    I2S2_Handler.Init.MCLKOutput=I2S_MCLKOUTPUT_ENABLE;	//主时钟输出使能
//    I2S2_Handler.Init.AudioFreq=I2S_AUDIOFREQ_DEFAULT;	//IIS频率设置
//    I2S2_Handler.Init.CPOL=I2S_CPOL_LOW;			//空闲状态时钟电平
//    I2S2_Handler.Init.ClockSource=I2S_CLOCK_PLL;		//IIS时钟源为PLL
//    HAL_I2S_Init(&I2S2_Handler);
    demomyUsart<<"I2S Init OK"<<endl;
//    SPI2->CR2|=1<<1;									//SPI2 TX DMA请求使能.
//    __HAL_I2S_ENABLE(&I2S2_Handler);					//使能I2S2

    //设置采样率
    u8 i=0;
    u32 tempreg=0;
    RCC_PeriphCLKInitTypeDef RCCI2S2_ClkInitSture;

    for(i=0;i<(sizeof(I2S_PSC_TBL)/10);i++)//看看改采样率是否可以支持
    {
        if(((m_DocumentAnalysis->getSamplerate())/10)==I2S_PSC_TBL[i][0])break;
    }
    if(i==(sizeof(I2S_PSC_TBL)/10))return 1;//搜遍了也找不到

    RCCI2S2_ClkInitSture.PeriphClockSelection=RCC_PERIPHCLK_I2S;	//外设时钟源选择
    RCCI2S2_ClkInitSture.PLLI2S.PLLI2SN=(u32)I2S_PSC_TBL[i][1];    	//设置PLLI2SN
    RCCI2S2_ClkInitSture.PLLI2S.PLLI2SR=(u32)I2S_PSC_TBL[i][2];    	//设置PLLI2SR
    HAL_RCCEx_PeriphCLKConfig(&RCCI2S2_ClkInitSture);             	//设置时钟

    RCC->CR|=1<<26;					//开启I2S时钟
    while((RCC->CR&1<<27)==0);		//等待I2S时钟开启成功.
    tempreg=I2S_PSC_TBL[i][3]<<0;	//设置I2SDIV
    tempreg|=I2S_PSC_TBL[i][4]<<8;	//设置ODD位
    tempreg|=1<<9;					//使能MCKOE位,输出MCK
    SPI2->I2SPR=tempreg;			//设置I2SPR寄存器
//    demomyUsart<<"DMA Init start"<<endl;
//    __HAL_RCC_DMA1_CLK_ENABLE();                                    		//使能DMA1时钟
//
//
//    I2S2_TXDMA_Handler.Instance=DMA1_Stream4;                       		//DMA1数据流4
//    I2S2_TXDMA_Handler.Init.Channel=DMA_CHANNEL_0;                  		//通道0
//    I2S2_TXDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;         		//存储器到外设模式
//    I2S2_TXDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;             		//外设非增量模式
//    I2S2_TXDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                 		//存储器增量模式
//    I2S2_TXDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;   	//外设数据长度:16位
//    I2S2_TXDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;    	//存储器数据长度:16位
//    I2S2_TXDMA_Handler.Init.Mode=DMA_CIRCULAR;                      		//使用循环模式
//    I2S2_TXDMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;             		//高优先级
//    I2S2_TXDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;          		//不使用FIFO
//    I2S2_TXDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;             		//存储器单次突发传输
//    I2S2_TXDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;          		//外设突发单次传输
////    I2S2_TXDMA_Handler.XferHalfCpltCallback=DMAEx_XferCpltCallback;
////    I2S2_TXDMA_Handler.XferM1HalfCpltCallback=DMAEx_XferM1CpltCallback;
////    I2S2_TXDMA_Handler.XferErrorCallback=DMAEx_XferErrorCallback;
////    HAL_DMA_DeInit(&I2S2_TXDMA_Handler);                            		//先清除以前的设置
//    HAL_DMA_Init(&I2S2_TXDMA_Handler);	                            		//初始化DMA
//    I2S2_TXDMA_HandlerForC=I2S2_TXDMA_Handler;
//    __HAL_DMA_DISABLE(&I2S2_TXDMA_Handler);                         		//先关闭DMA
//    HAL_Delay_us(10);                                                   		//10us延时，防止-O2优化出问题
//    __HAL_DMA_ENABLE_IT(&I2S2_TXDMA_Handler,DMA_IT_TC);             		//开启传输完成中断
//    __HAL_DMA_CLEAR_FLAG(&I2S2_TXDMA_Handler,DMA_FLAG_TCIF0_4);     		//清除DMA传输完成中断标志位
//    __HAL_LINKDMA(&I2S2_Handler,hdmatx,I2S2_TXDMA_Handler);         		//将DMA与I2S联系起来
//    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn,0,0);                    		//DMA中断优先级
//    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
    return 0;
}
