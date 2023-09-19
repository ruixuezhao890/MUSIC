/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyI2s.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/11 
*********************************************************************
*********
*/
//

#include "MyI2s.h"
#include "tim.h"
#include "wavplay.h"
const uint16_t I2S_PSC_TBL[][5]=
        {
                {   800, 256, 5, 12, 1 },   /* 8Khz采样率 */
                {  1102, 429, 4, 19, 0 },   /* 11.025Khz采样率 */
                {  1600, 213, 2, 13, 0 },   /* 16Khz采样率 */
                {  2205, 429, 4,  9, 1 },   /* 22.05Khz采样率 */
                {  3200, 213, 2,  6, 1 },   /* 32Khz采样率 */
                {  4410, 271, 2,  6, 0 },   /* 44.1Khz采样率 */
                {  4800, 258, 3,  3, 1 },   /* 48Khz采样率 */
                {  8820, 316, 2,  3, 1 },   /* 88.2Khz采样率 */
                {  9600, 344, 2,  3, 1 },   /* 96Khz采样率 */
                { 17640, 361, 2,  2, 0 },   /* 176.4Khz采样率 */
                { 19200, 393, 2,  2, 0 },   /* 192Khz采样率 */
        };
MyI2s::MyI2s() {

}

MyI2s::~MyI2s() {

}

void MyI2s::i2s_dma_enable(void) {
    uint32_t tempreg = 0;
    tempreg = I2S_SPI->CR2;     /* 先读出以前的设置 */
    tempreg |= 1 << 1;          /* 使能DMA */
    I2S_SPI->CR2 = tempreg;     /* 写入CR2寄存器中 */
}

uint8_t MyI2s::i2s_samplerate_set(uint32_t samplerate) {
    uint8_t i = 0;
    uint32_t tempreg = 0;
//    HAL_I2SEx_TransmitReceive();
    RCC_PeriphCLKInitTypeDef rcc_i2s_clkinit_struct;

    for (i = 0; i < (sizeof(I2S_PSC_TBL) / 10); i++)    /* 看看改采样率是否可以支持 */
    {
        if ((samplerate / 10) == I2S_PSC_TBL[i][0])
        {
            break;
        }
    }
    if (i == (sizeof(I2S_PSC_TBL) / 10))
    {
        return 1;   /* 找不到 */
    }

    rcc_i2s_clkinit_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S;        /* 外设时钟源选择 */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SN = (uint32_t)I2S_PSC_TBL[i][1];    /* 设置PLLI2SN */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SR = (uint32_t)I2S_PSC_TBL[i][2];    /* 设置PLLI2SR */
    HAL_RCCEx_PeriphCLKConfig(&rcc_i2s_clkinit_struct);                     /* 设置时钟 */

    RCC->CR |= 1 << 26;                 /* 开启I2S时钟 */
    while((RCC->CR & 1 << 27) == 0);    /* 等待I2S时钟开启成功. */
    tempreg = I2S_PSC_TBL[i][3] << 0;   /* 设置I2SDIV */
    tempreg |= I2S_PSC_TBL[i][4] << 8;  /* 设置ODD位 */
    tempreg |= 1 << 9;                  /* 使能MCKOE位,输出MCK */
    I2S_SPI->I2SPR = tempreg;           /* 设置I2SPR寄存器 */

    return 0;
}

void MyI2s::i2s_tx_dma_init(uint8_t *buf0, uint8_t *buf1, uint16_t num) {

    I2S_TX_DMA_CLK_ENABLE();        /* 使能I2S TX DMA时钟 */
    __HAL_LINKDMA(&g_i2s_handler, hdmatx, g_i2s_txdma_handler);             /* 将DMA与I2S联系起来 */

    g_i2s_txdma_handler.Instance = I2S_TX_DMASx;                            /* 设置I2S TX DMA数据流 */
    g_i2s_txdma_handler.Init.Channel = I2S_TX_DMASx_Channel;                /* 设置I2S TX DMA通道 */
    g_i2s_txdma_handler.Init.Direction = DMA_MEMORY_TO_PERIPH;              /* 存储器到外设模式 */
    g_i2s_txdma_handler.Init.PeriphInc = DMA_PINC_DISABLE;                  /* 外设非增量模式 */
    g_i2s_txdma_handler.Init.MemInc = DMA_MINC_ENABLE;                      /* 存储器增量模式 */
    g_i2s_txdma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* 外设数据长度:16位 */
    g_i2s_txdma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* 存储器数据长度:16位 */
    g_i2s_txdma_handler.Init.Mode = DMA_CIRCULAR;                           /* 使用循环模式 */
    g_i2s_txdma_handler.Init.Priority = DMA_PRIORITY_HIGH;                  /* 高优先级 */
    g_i2s_txdma_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               /* 不使用FIFO */
    g_i2s_txdma_handler.Init.MemBurst = DMA_MBURST_SINGLE;                  /* 存储器单次突发传输 */
    g_i2s_txdma_handler.Init.PeriphBurst = DMA_PBURST_SINGLE;               /* 外设突发单次传输 */
    HAL_DMA_DeInit(&g_i2s_txdma_handler);                                   /* 先清除以前的设置 */
    HAL_DMA_Init(&g_i2s_txdma_handler);                                     /* 初始化DMA */

    HAL_DMAEx_MultiBufferStart(&g_i2s_txdma_handler, (uint32_t)buf0, (uint32_t)&I2S_SPI->DR, (uint32_t)buf1, num);  /* 开启双缓冲 */
    __HAL_DMA_DISABLE(&g_i2s_txdma_handler);;/* 先关闭DMA */
    HAL_Delay_us(10); /* 10us延时，防止-O2优化出问题 */
    __HAL_DMA_ENABLE_IT(&g_i2s_txdma_handler, DMA_IT_TC);                   /* 开启传输完成中断 */

    __HAL_DMA_CLEAR_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG);
    /* 清除DMA传输完成中断标志位 */
    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);                           /* DMA中断优先级 */
    demomyUsart<<"test5"<<endl;
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
    demomyUsart<<"test6"<<endl;

}

void MyI2s::i2s_play_start(void) {
    __HAL_DMA_ENABLE(&g_i2s_txdma_handler); /* 开启DMA TX传输 */
}

void MyI2s::i2s_play_stop(void) {
    __HAL_DMA_DISABLE(&g_i2s_txdma_handler);  /* 关闭DMA TX传输 */
}

//void MyI2s::setCallback(void (*i2s_tx_callback)(void)) {
//    this->i2s_tx_callback=i2s_tx_callback;
//}

void MyI2s::CallbackI2s(void) {
    if (__HAL_DMA_GET_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG) != RESET)   /* DMA传输完成 */
    {
        __HAL_DMA_CLEAR_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG);          /* 清除DMA传输完成中断标志位 */

//        if (i2s_tx_callback != NULL)
//        {
            wavefunctation[0]->wav_i2s_dma_tx_callback();
//            i2s_tx_callback();  /* 执行回调函数,读取数据等操作在这里面处理 */
//        }
    }
}

void MyI2s::i2sInit(uint32_t i2s_standard, uint32_t i2s_mode, uint32_t i2s_clock_polarity, uint32_t i2s_dataformat) {

}

MyI2s::MyI2s(uint16_t flag) {

}

//void MyI2s::Getcallback(void *) {
//
//}

//void MyI2s::I2Sx_TX_DMA_Init(const uint16_t *buffer0, const uint16_t *buffer1, const uint32_t num) {
//    NVIC_InitTypeDef   NVIC_InitStructure;
//    DMA_InitTypeDef  DMA_InitStructure;
//
//
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能
//
//    DMA_DeInit(DMA1_Stream4);
//    while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}//等待DMA1_Stream1可配置
//
//    DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_FEIF4|DMA_IT_DMEIF4|DMA_IT_TEIF4|DMA_IT_HTIF4|DMA_IT_TCIF4);//清空DMA1_Stream4上所有中断标志
//
//    /* 配置 DMA Stream */
//    DMA_InitStructure.DMA_Channel =  DMA_Channel_0;  //通道0 SPI2_TX通道
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;//外设地址为:(u32)&SPI2->DR
//    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer0;//DMA 存储器0地址
//    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
//    DMA_InitStructure.DMA_BufferSize = num;//数据传输量
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:16位
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度：16位
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用循环模式
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO模式
//    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//外设突发单次传输
//    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//存储器突发单次传输
//    DMA_Init(DMA1_Stream4, &DMA_InitStructure);//初始化DMA Stream
//
//    DMA_DoubleBufferModeConfig(DMA1_Stream4,(uint32_t)buffer0,DMA_Memory_0);//双缓冲模式配置
//    DMA_DoubleBufferModeConfig(DMA1_Stream4,(uint32_t)buffer1,DMA_Memory_1);//双缓冲模式配置
//
//    DMA_DoubleBufferModeCmd(DMA1_Stream4,ENABLE);//双缓冲模式开启
//
//    DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);//开启传输完成中断
//
//    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);//SPI2 TX DMA请求使能.
//
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//子优先级2
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//    NVIC_Init(&NVIC_InitStructure);//配置
//}
