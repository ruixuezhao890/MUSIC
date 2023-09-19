#include "MusicPlay.h"
#include "MyUsart.h"
#include "malloc.h"
#include "MyKey.h"
#define I2S_TX_DMA_BUF_SIZE    8192
//std::vector<MusicPlay *> MusicPlayHandle;
MusicPlay::MusicPlay(I2S_HandleTypeDef *hi2s) {
    PlayStaut=1;
    MusicI2s=new MyI2s(hi2s);
    key=new MyButton(1);
}

MusicPlay::~MusicPlay() {

}

void MusicPlay::MusicPlayStart() {

    MusicI2s->MyI2sPlayStart();
    while (1){

    }
}

MyI2s *MusicPlay::getMusicI2S() {
    return this->MusicI2s;
}

void MusicPlay::MusicPlayASong() {
    MusicI2s->getMDocumentAnalysis()->DocumentAnalysisNameWithPath();
    uint8_t res;
    res= MusicI2s->getMDocumentAnalysis()->DocumentAnalysisJudgeType((char *)(MusicI2s
            ->getMDocumentAnalysis()->getPname()));
    switch (res) {
        case T_WAV:
            this->PlayStartAwave();
        default:
            demomyUsart<<"can't play:"<<(char *)(MusicI2s->getMDocumentAnalysis()->getPname())<<endl;
            //控制暂停
            break;
    }

}

void MusicPlay::MusicPlayDestruct() {
    delete MusicI2s;
}

uint8_t MusicPlay::PlayStartAwave() {
    uint32_t fillnum;uint32_t t=0; uint8_t key=0;
   u8 res=MusicI2s->getMDocumentAnalysis()->DocumentAnalysisASongInit();
   if(res==0){
       MusicI2s->MyI2sReInit();
       MusicI2s->MyI2sPlayStop();
       res= f_open(MusicI2s->getMDocumentAnalysis()->getFile(),
                   (TCHAR*)( MusicI2s->getMDocumentAnalysis()->getPname()), FA_READ);
       if(res==FR_OK){
           f_lseek(MusicI2s->getMDocumentAnalysis()->getFile(),MusicI2s->getMDocumentAnalysis()
           ->getDatastart());

           fillnum=MusicI2s->getMDocumentAnalysis()->DocumentAnalysisTransmitBufFill();
           fillnum=MusicI2s->getMDocumentAnalysis()->DocumentAnalysisTransmitBufFill();
           MusicI2s->MyI2sPlayStart();
           demomyUsart<<"start"<<endl;
           while(res==FR_OK){

               while (wavtransferend==0);
               wavtransferend=0;
               if (fillnum != I2S_TX_DMA_BUF_SIZE)  // 播放结束?
               {
                   demomyUsart<<"play end"<<endl;
                   res = KEY0_PRES;
                   break;
               }
               fillnum=MusicI2s->getMDocumentAnalysis()->DocumentAnalysisTransmitBufFill();
               while (1){
                     key=this->key->key_scan(0);
                   if (key==WKUP_PRES){
                       (PlayStaut&0x01)?(this->PlayStaut&=~(1<<0)):this->PlayStaut|=(0x01);
                   }
                   if (key==KEY2_PRES||key==KEY0_PRES){
                       res=key;
                       break;
                   }
                   //demomyUsart<<(int )(MusicI2s->getMDocumentAnalysis()->DocumentAnalysisGetCurTime());
                   t++;
                   if(t==20){
                       t=0;
                       HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_9);
                   }
                   if((PlayStaut&0x01)==0){
                       HAL_Delay(10);
                   }else{
                       break;
                   }

               }

           }
       }

   }
    //MusicI2s->MyI2sPlayStop();
    demomyUsart<<"stop"<<endl;
}

void MusicPlay::audio_play() {
    u8 res;DIR wavdir;
    MusicI2s->getMDocumentAnalysis()->DocumentAnalysisGetAllMusicNum();
    MusicI2s->getMDocumentAnalysis()->DocumentAnalysisGetIndex();

    res=f_opendir((MusicI2s->getMDocumentAnalysis()->getWavdir()), (const TCHAR*)"0:/MUSIC"); 	//打开目录
    while (res==FR_OK){
        MusicI2s->getMDocumentAnalysis()->DocumentAnalysisNameWithPath();
        this->MusicPlayASong();
    }
}

uint8_t MusicPlay::getPlayStaut() {
    return PlayStaut;
}
