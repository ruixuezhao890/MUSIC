/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : MyI2s.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/15 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_MYI2S_H
#define MY_LIBARARY_MYI2S_H
#include "sys.h"
#include "i2s.h"
#include "DocumentAnalysis.h"
#include "vector"
class MyI2s {
public:
    MyI2s();
    MyI2s(I2S_HandleTypeDef * hi2s);

    uint8_t *getI2SBuf0() ;

    uint8_t *getI2SBuf1() ;


    void MyI2sPlayStart();
    void MyI2sPlayPause();
    void MyI2sPlayResume();
    void MyI2sPlayStop();
    void MyI2sDeStruct();
    uint8_t MyI2sReInit();
    ~MyI2s();
protected:

private:
    I2S_HandleTypeDef* m_hi2s;
    I2S_HandleTypeDef I2S2_Handler;			//I2S2¾ä±ú
    DMA_HandleTypeDef I2S2_TXDMA_Handler;   //I2S2·¢ËÍDMA¾ä±ú
    DocumentAnalysis* m_DocumentAnalysis;


public:
     DocumentAnalysis *getMDocumentAnalysis() ;
};

extern std::vector<MyI2s *> MyI2sItHandle;
#endif //MY_LIBARARY_MYI2S_H
