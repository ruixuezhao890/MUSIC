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
DMA_HandleTypeDef I2S2_TXDMA_HandlerForC;   //I2S2����DMA���Ϊ��c����
const u16 I2S_PSC_TBL[][5]=
        {
                {800 ,256,5,12,1},		//8Khz������
                {1102,429,4,19,0},		//11.025Khz������
                {1600,213,2,13,0},		//16Khz������
                {2205,429,4, 9,1},		//22.05Khz������
                {3200,213,2, 6,1},		//32Khz������
                {4410,271,2, 6,0},		//44.1Khz������
                {4800,258,3, 3,1},		//48Khz������
                {8820,316,2, 3,1},		//88.2Khz������
                {9600,344,2, 3,1},  	//96Khz������
                {17640,361,2,2,0},  	//176.4Khz������
                {19200,393,2,2,0},  	//192Khz������
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
  *  @brief     �������� ����I2S�Ĳ�����
  *  @param     ����   ������
  *  @note      ��ע ���ڵõ�һ�׸�ĸ�����Ϣ֮�����ʹ��
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

//uint8_t MyI2s::MyI2sSamplerateInit() {
//    HAL_I2S_DeInit(m_hi2s);//ȥ��ʼ��
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
//        //��ʼ��������
//        for(i=0;i<(sizeof(I2S_PSC_TBL)/10);i++)//�����Ĳ������Ƿ����֧��
//        {
//            if(((this->m_DocumentAnalysis.getSamplerate())/10)==I2S_PSC_TBL[i][0])break;
//        }
//        if(i==(sizeof(I2S_PSC_TBL)/10))return 1;//�ѱ���Ҳ�Ҳ���
//
//        PeriphClkInitStruct.PeriphClockSelection=RCC_PERIPHCLK_I2S;	//����ʱ��Դѡ��
//        PeriphClkInitStruct.PLLI2S.PLLI2SN=(u32)I2S_PSC_TBL[i][1];    	//����PLLI2SN
//        PeriphClkInitStruct.PLLI2S.PLLI2SR=(u32)I2S_PSC_TBL[i][2];    	//����PLLI2SR
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
//        //��ʼ����Ӧ����
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
                          m_DocumentAnalysis->getBuf1(),I2S_TX_DMA_BUF_SIZE/2);//����˫����
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
  *  @brief     �������� ����I2S�Ĳ����� ����˫����ģʽ
  *  @param     ����   ��
  *  @note      ��ע ���ڵõ�һ�׸�ĸ�����Ϣ֮�����ʹ�� ����ʹ����
  *                     �ú���֮��Ϳ���ֱ�ӿ�ʼ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint8_t MyI2s::MyI2sReInit() {
    HAL_I2S_DMAStop(m_hi2s);
    HAL_I2S_DeInit(m_hi2s);//ȥ��ʼ��
//    HAL_DMA_Abort(m_hi2s->hdmatx);
//    HAL_DMA_DeInit(m_hi2s->hdmatx);
    m_hi2s->Instance = SPI2;
    m_hi2s->Init.Mode = I2S_MODE_MASTER_TX;
    m_hi2s->Init.Standard = I2S_STANDARD_PHILIPS;
//    m_hi2s->Init.DataFormat = I2S_DATAFORMAT_16B;
    if (m_DocumentAnalysis->getBps()==24){
        m_hi2s->Init.DataFormat=I2S_DATAFORMAT_24B;		//IIS���ݳ���
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
//    I2S2_Handler.Init.Mode=I2S_MODE_MASTER_TX;					//IISģʽ
//    I2S2_Handler.Init.Standard=I2S_STANDARD_PHILIPS;			//IIS��׼
//    if (m_DocumentAnalysis.getBps()==24){
//        I2S2_Handler.Init.DataFormat=I2S_DATAFORMAT_24B;		//IIS���ݳ���
//    }else{
//        I2S2_Handler.Init.DataFormat=I2S_DATAFORMAT_16B_EXTENDED;
//    }
//
//    I2S2_Handler.Init.MCLKOutput=I2S_MCLKOUTPUT_ENABLE;	//��ʱ�����ʹ��
//    I2S2_Handler.Init.AudioFreq=I2S_AUDIOFREQ_DEFAULT;	//IISƵ������
//    I2S2_Handler.Init.CPOL=I2S_CPOL_LOW;			//����״̬ʱ�ӵ�ƽ
//    I2S2_Handler.Init.ClockSource=I2S_CLOCK_PLL;		//IISʱ��ԴΪPLL
//    HAL_I2S_Init(&I2S2_Handler);
    demomyUsart<<"I2S Init OK"<<endl;
//    SPI2->CR2|=1<<1;									//SPI2 TX DMA����ʹ��.
//    __HAL_I2S_ENABLE(&I2S2_Handler);					//ʹ��I2S2

    //���ò�����
    u8 i=0;
    u32 tempreg=0;
    RCC_PeriphCLKInitTypeDef RCCI2S2_ClkInitSture;

    for(i=0;i<(sizeof(I2S_PSC_TBL)/10);i++)//�����Ĳ������Ƿ����֧��
    {
        if(((m_DocumentAnalysis->getSamplerate())/10)==I2S_PSC_TBL[i][0])break;
    }
    if(i==(sizeof(I2S_PSC_TBL)/10))return 1;//�ѱ���Ҳ�Ҳ���

    RCCI2S2_ClkInitSture.PeriphClockSelection=RCC_PERIPHCLK_I2S;	//����ʱ��Դѡ��
    RCCI2S2_ClkInitSture.PLLI2S.PLLI2SN=(u32)I2S_PSC_TBL[i][1];    	//����PLLI2SN
    RCCI2S2_ClkInitSture.PLLI2S.PLLI2SR=(u32)I2S_PSC_TBL[i][2];    	//����PLLI2SR
    HAL_RCCEx_PeriphCLKConfig(&RCCI2S2_ClkInitSture);             	//����ʱ��

    RCC->CR|=1<<26;					//����I2Sʱ��
    while((RCC->CR&1<<27)==0);		//�ȴ�I2Sʱ�ӿ����ɹ�.
    tempreg=I2S_PSC_TBL[i][3]<<0;	//����I2SDIV
    tempreg|=I2S_PSC_TBL[i][4]<<8;	//����ODDλ
    tempreg|=1<<9;					//ʹ��MCKOEλ,���MCK
    SPI2->I2SPR=tempreg;			//����I2SPR�Ĵ���
//    demomyUsart<<"DMA Init start"<<endl;
//    __HAL_RCC_DMA1_CLK_ENABLE();                                    		//ʹ��DMA1ʱ��
//
//
//    I2S2_TXDMA_Handler.Instance=DMA1_Stream4;                       		//DMA1������4
//    I2S2_TXDMA_Handler.Init.Channel=DMA_CHANNEL_0;                  		//ͨ��0
//    I2S2_TXDMA_Handler.Init.Direction=DMA_MEMORY_TO_PERIPH;         		//�洢��������ģʽ
//    I2S2_TXDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;             		//���������ģʽ
//    I2S2_TXDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                 		//�洢������ģʽ
//    I2S2_TXDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_HALFWORD;   	//�������ݳ���:16λ
//    I2S2_TXDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_HALFWORD;    	//�洢�����ݳ���:16λ
//    I2S2_TXDMA_Handler.Init.Mode=DMA_CIRCULAR;                      		//ʹ��ѭ��ģʽ
//    I2S2_TXDMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;             		//�����ȼ�
//    I2S2_TXDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;          		//��ʹ��FIFO
//    I2S2_TXDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;             		//�洢������ͻ������
//    I2S2_TXDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;          		//����ͻ�����δ���
////    I2S2_TXDMA_Handler.XferHalfCpltCallback=DMAEx_XferCpltCallback;
////    I2S2_TXDMA_Handler.XferM1HalfCpltCallback=DMAEx_XferM1CpltCallback;
////    I2S2_TXDMA_Handler.XferErrorCallback=DMAEx_XferErrorCallback;
////    HAL_DMA_DeInit(&I2S2_TXDMA_Handler);                            		//�������ǰ������
//    HAL_DMA_Init(&I2S2_TXDMA_Handler);	                            		//��ʼ��DMA
//    I2S2_TXDMA_HandlerForC=I2S2_TXDMA_Handler;
//    __HAL_DMA_DISABLE(&I2S2_TXDMA_Handler);                         		//�ȹر�DMA
//    HAL_Delay_us(10);                                                   		//10us��ʱ����ֹ-O2�Ż�������
//    __HAL_DMA_ENABLE_IT(&I2S2_TXDMA_Handler,DMA_IT_TC);             		//������������ж�
//    __HAL_DMA_CLEAR_FLAG(&I2S2_TXDMA_Handler,DMA_FLAG_TCIF0_4);     		//���DMA��������жϱ�־λ
//    __HAL_LINKDMA(&I2S2_Handler,hdmatx,I2S2_TXDMA_Handler);         		//��DMA��I2S��ϵ����
//    HAL_NVIC_SetPriority(DMA1_Stream4_IRQn,0,0);                    		//DMA�ж����ȼ�
//    HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
    return 0;
}
