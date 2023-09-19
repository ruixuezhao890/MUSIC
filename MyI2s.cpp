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
                {   800, 256, 5, 12, 1 },   /* 8Khz������ */
                {  1102, 429, 4, 19, 0 },   /* 11.025Khz������ */
                {  1600, 213, 2, 13, 0 },   /* 16Khz������ */
                {  2205, 429, 4,  9, 1 },   /* 22.05Khz������ */
                {  3200, 213, 2,  6, 1 },   /* 32Khz������ */
                {  4410, 271, 2,  6, 0 },   /* 44.1Khz������ */
                {  4800, 258, 3,  3, 1 },   /* 48Khz������ */
                {  8820, 316, 2,  3, 1 },   /* 88.2Khz������ */
                {  9600, 344, 2,  3, 1 },   /* 96Khz������ */
                { 17640, 361, 2,  2, 0 },   /* 176.4Khz������ */
                { 19200, 393, 2,  2, 0 },   /* 192Khz������ */
        };
MyI2s::MyI2s() {

}

MyI2s::~MyI2s() {

}

void MyI2s::i2s_dma_enable(void) {
    uint32_t tempreg = 0;
    tempreg = I2S_SPI->CR2;     /* �ȶ�����ǰ������ */
    tempreg |= 1 << 1;          /* ʹ��DMA */
    I2S_SPI->CR2 = tempreg;     /* д��CR2�Ĵ����� */
}

uint8_t MyI2s::i2s_samplerate_set(uint32_t samplerate) {
    uint8_t i = 0;
    uint32_t tempreg = 0;
//    HAL_I2SEx_TransmitReceive();
    RCC_PeriphCLKInitTypeDef rcc_i2s_clkinit_struct;

    for (i = 0; i < (sizeof(I2S_PSC_TBL) / 10); i++)    /* �����Ĳ������Ƿ����֧�� */
    {
        if ((samplerate / 10) == I2S_PSC_TBL[i][0])
        {
            break;
        }
    }
    if (i == (sizeof(I2S_PSC_TBL) / 10))
    {
        return 1;   /* �Ҳ��� */
    }

    rcc_i2s_clkinit_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S;        /* ����ʱ��Դѡ�� */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SN = (uint32_t)I2S_PSC_TBL[i][1];    /* ����PLLI2SN */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SR = (uint32_t)I2S_PSC_TBL[i][2];    /* ����PLLI2SR */
    HAL_RCCEx_PeriphCLKConfig(&rcc_i2s_clkinit_struct);                     /* ����ʱ�� */

    RCC->CR |= 1 << 26;                 /* ����I2Sʱ�� */
    while((RCC->CR & 1 << 27) == 0);    /* �ȴ�I2Sʱ�ӿ����ɹ�. */
    tempreg = I2S_PSC_TBL[i][3] << 0;   /* ����I2SDIV */
    tempreg |= I2S_PSC_TBL[i][4] << 8;  /* ����ODDλ */
    tempreg |= 1 << 9;                  /* ʹ��MCKOEλ,���MCK */
    I2S_SPI->I2SPR = tempreg;           /* ����I2SPR�Ĵ��� */

    return 0;
}

void MyI2s::i2s_tx_dma_init(uint8_t *buf0, uint8_t *buf1, uint16_t num) {

    I2S_TX_DMA_CLK_ENABLE();        /* ʹ��I2S TX DMAʱ�� */
    __HAL_LINKDMA(&g_i2s_handler, hdmatx, g_i2s_txdma_handler);             /* ��DMA��I2S��ϵ���� */

    g_i2s_txdma_handler.Instance = I2S_TX_DMASx;                            /* ����I2S TX DMA������ */
    g_i2s_txdma_handler.Init.Channel = I2S_TX_DMASx_Channel;                /* ����I2S TX DMAͨ�� */
    g_i2s_txdma_handler.Init.Direction = DMA_MEMORY_TO_PERIPH;              /* �洢��������ģʽ */
    g_i2s_txdma_handler.Init.PeriphInc = DMA_PINC_DISABLE;                  /* ���������ģʽ */
    g_i2s_txdma_handler.Init.MemInc = DMA_MINC_ENABLE;                      /* �洢������ģʽ */
    g_i2s_txdma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* �������ݳ���:16λ */
    g_i2s_txdma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* �洢�����ݳ���:16λ */
    g_i2s_txdma_handler.Init.Mode = DMA_CIRCULAR;                           /* ʹ��ѭ��ģʽ */
    g_i2s_txdma_handler.Init.Priority = DMA_PRIORITY_HIGH;                  /* �����ȼ� */
    g_i2s_txdma_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               /* ��ʹ��FIFO */
    g_i2s_txdma_handler.Init.MemBurst = DMA_MBURST_SINGLE;                  /* �洢������ͻ������ */
    g_i2s_txdma_handler.Init.PeriphBurst = DMA_PBURST_SINGLE;               /* ����ͻ�����δ��� */
    HAL_DMA_DeInit(&g_i2s_txdma_handler);                                   /* �������ǰ������ */
    HAL_DMA_Init(&g_i2s_txdma_handler);                                     /* ��ʼ��DMA */

    HAL_DMAEx_MultiBufferStart(&g_i2s_txdma_handler, (uint32_t)buf0, (uint32_t)&I2S_SPI->DR, (uint32_t)buf1, num);  /* ����˫���� */
    __HAL_DMA_DISABLE(&g_i2s_txdma_handler);;/* �ȹر�DMA */
    HAL_Delay_us(10); /* 10us��ʱ����ֹ-O2�Ż������� */
    __HAL_DMA_ENABLE_IT(&g_i2s_txdma_handler, DMA_IT_TC);                   /* ������������ж� */

    __HAL_DMA_CLEAR_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG);
    /* ���DMA��������жϱ�־λ */
    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);                           /* DMA�ж����ȼ� */
    demomyUsart<<"test5"<<endl;
    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
    demomyUsart<<"test6"<<endl;

}

void MyI2s::i2s_play_start(void) {
    __HAL_DMA_ENABLE(&g_i2s_txdma_handler); /* ����DMA TX���� */
}

void MyI2s::i2s_play_stop(void) {
    __HAL_DMA_DISABLE(&g_i2s_txdma_handler);  /* �ر�DMA TX���� */
}

//void MyI2s::setCallback(void (*i2s_tx_callback)(void)) {
//    this->i2s_tx_callback=i2s_tx_callback;
//}

void MyI2s::CallbackI2s(void) {
    if (__HAL_DMA_GET_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG) != RESET)   /* DMA������� */
    {
        __HAL_DMA_CLEAR_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG);          /* ���DMA��������жϱ�־λ */

//        if (i2s_tx_callback != NULL)
//        {
            wavefunctation[0]->wav_i2s_dma_tx_callback();
//            i2s_tx_callback();  /* ִ�лص�����,��ȡ���ݵȲ����������洦�� */
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
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ��
//
//    DMA_DeInit(DMA1_Stream4);
//    while (DMA_GetCmdStatus(DMA1_Stream4) != DISABLE){}//�ȴ�DMA1_Stream1������
//
//    DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_FEIF4|DMA_IT_DMEIF4|DMA_IT_TEIF4|DMA_IT_HTIF4|DMA_IT_TCIF4);//���DMA1_Stream4�������жϱ�־
//
//    /* ���� DMA Stream */
//    DMA_InitStructure.DMA_Channel =  DMA_Channel_0;  //ͨ��0 SPI2_TXͨ��
//    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI2->DR;//�����ַΪ:(u32)&SPI2->DR
//    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)buffer0;//DMA �洢��0��ַ
//    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
//    DMA_InitStructure.DMA_BufferSize = num;//���ݴ�����
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���:16λ
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//�洢�����ݳ��ȣ�16λ
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
//    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //��ʹ��FIFOģʽ
//    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
//    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
//    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
//    DMA_Init(DMA1_Stream4, &DMA_InitStructure);//��ʼ��DMA Stream
//
//    DMA_DoubleBufferModeConfig(DMA1_Stream4,(uint32_t)buffer0,DMA_Memory_0);//˫����ģʽ����
//    DMA_DoubleBufferModeConfig(DMA1_Stream4,(uint32_t)buffer1,DMA_Memory_1);//˫����ģʽ����
//
//    DMA_DoubleBufferModeCmd(DMA1_Stream4,ENABLE);//˫����ģʽ����
//
//    DMA_ITConfig(DMA1_Stream4,DMA_IT_TC,ENABLE);//������������ж�
//
//    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);//SPI2 TX DMA����ʹ��.
//
//    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�1
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�����ȼ�2
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
//    NVIC_Init(&NVIC_InitStructure);//����
//}
