/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : DocumentAnalysis.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/14
*********************************************************************
*********
*/
//

#include "DocumentAnalysis.h"
#include "malloc.h"
#include "MyUsart.h"
#include "sdio.h"
#include "MyGPIO.h"

using namespace stm32f407;
#define FILE_MAX_TYPE_NUM		7	//���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM		4	//���FILE_MAX_SUBT_NUM��С��
char *const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]=
        {
                {"BIN"},			//BIN�ļ�
                {"LRC"},			//LRC�ļ�
                {"NES"},			//NES�ļ�
                {"TXT","C","H"},	//�ı��ļ�
                {"WAV","MP3","APE","FLAC"},//֧�ֵ������ļ�
                {"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
                {"AVI"},			//��Ƶ�ļ�
        };
DocumentAnalysis::DocumentAnalysis() {

}
/***************************************************************
  *  @brief     �������� ��ʼ������
  *  @param     ����   ��ȡSD�ĵ�ַ
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint8_t DocumentAnalysis::DocumentAnalysisInit(char * filepath) {

    this->m_filepath= (char *)mymalloc(SRAMIN,32);
    Array_selection_flag=1;
    m_filepath=filepath;
    my_mem_init(SRAMIN);
    my_mem_init(SRAMCCM);
    fs=( FATFS*)mymalloc(SRAMIN,sizeof(FATFS));
    u8 res=f_mount(fs,"0:",1);
    file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
//    Buf16Bits=(uint8_t *)mymalloc(SRAMIN,I2S_TX_DMA_BUF_SIZE);
    ConstPname = (uint8_t *) mymalloc(SRAMIN, 225 * +1);
    wavfileinfo = (FILINFO*)mymalloc(SRAMIN, sizeof(FILINFO));  /* �����ڴ� */
    wavdir=(DIR*)mymalloc(SRAMIN, sizeof(DIR));

    if(res){
        demomyUsart<<"SD error"<<endl;
        return 1;
    }
    demomyUsart<<"SD ok"<<endl;//this->DocumentAnalysisShowSDInfo();
    DocumentAnalysisGetIndex();//�õ�������
//    DocumentAnalysisNameWithPath();//����
//    DocumentAnalysisASongInit();

    return 0;
}

/***************************************************************
  *  @brief     �������� �õ�һ�׸���ļ���Ϣ�Լ��ж��Ƿ�����Ҫ���ļ�
  *  @param     ���� ��
  *  @note      ��ע �������ڵõ�һ�������ֵ�·��֮��ʹ��
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/
uint8_t DocumentAnalysis::DocumentAnalysisASongInit() {
//    FIL*ftemp;
    u8 *buf;
    UINT  br=0;
    u8 res=0;
    ChunkRIFF *riff;
    ChunkFMT *fmt;
    ChunkFACT *fact;
    ChunkDATA *data;
   // ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
    buf=(u8*)mymalloc(SRAMIN,512);
    if (file&&buf){
        res=f_opendir(wavdir,(const TCHAR*)m_filepath); 	//��Ŀ¼
        if (FR_OK!=res){
            res=4;
            return res;
        }

     demomyUsart<<endl<<"res:"<<( res=f_open(file,(TCHAR*)ConstPname,FA_READ))<<endl;//���ļ�
//        demomyUsart<<"pname"<<(char *)pname<<endl;
//        demomyUsart<<"pnameInt:"<<(int )pname<<endl;
        if (res==FR_OK){

            f_read(file,buf,512,(UINT *)&br);
            riff=(ChunkRIFF*)buf;
            if (riff->Format==0X45564157){
                fmt=(ChunkFMT *)(buf+12);//FMT��
                fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//FACT��
                if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)this->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//����fact/LIST���ʱ��(δ����)
                else this->datastart=12+8+fmt->ChunkSize;//������fact/LIST���ʱ��(δ����)

                data=(ChunkDATA *)(buf+this->datastart);
                if (data->ChunkID==0X61746164){
                   this->audioformat=fmt->AudioFormat;		//��Ƶ��ʽ
                   this->nchannels=fmt->NumOfChannels;		//ͨ����
                   this->samplerate=fmt->SampleRate;		//������
                   this->BitRateSpeed=fmt->ByteRate*8;			//�õ�λ��
                   this->blockalign=fmt->BlockAlign;		//�����
                   this->bps=fmt->BitsPerSample;			//λ��,16/24/32λ
                   this->datasize=data->ChunkSize;			//���ݿ��С
                   this->datastart=this->datastart+8;		//��������ʼ�ĵط�,8��ChunkDATA��� ChunkID+ChunkSize

                   demomyUsart<<"audioformat;  "<<audioformat<<endl;
                   demomyUsart<<"nchannels;    "<<nchannels<<endl;
                   demomyUsart<<"blockalign;   "<<blockalign<<endl;
                   demomyUsart<<"datasize;     "<<(int )datasize<<endl;
                   demomyUsart<<"totsec ;      "<<(int )totsec <<endl;
                   demomyUsart<<"cursec ;      "<<(int )cursec <<endl;
//                   demomyUsart<<"bitrate;      "<<bitrate;<<endl;
                   demomyUsart<<"BitRateSpeed; "<<(int )BitRateSpeed<<endl;
                   demomyUsart<<"samplerate;   "<<(int )samplerate<<endl;
                   demomyUsart<<"bps;          "<<bps<<endl;
                   demomyUsart<<"datastart;    "<<(int )datastart<<endl;

                }else{
                    res=4;
                }
            }else{
                res=3;
            }
        }else{
            res=2;
        }
    }else{
        res = 1;
    }
   // myfree(SRAMIN,ftemp);
    return res;
}
/***************************************************************
  *  @brief     �������� ��䷢������ ����һ�׸�Ľ�����Ϣ�Զ�ѡ����24bit����16bit
  *  @param     ����
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint32_t DocumentAnalysis:: DocumentAnalysisTransmitBufFill() {
//    DocumentAnalysisNameWithPath();
    demomyUsart<<"Fill in once successfully"<<endl;
    demomyUsart<<"DocumentAnalysisTransmitBufFill:pname:"<<(char *)ConstPname<<endl;
    uint8_t * buf;
    if(Array_selection_flag){
        Array_selection_flag=0;
        buf=Buf1;
    }else{
        Array_selection_flag=1;
        buf=Buf0;
    }
    memset(buf,0,I2S_TX_DMA_BUF_SIZE);
    u16 readlen=0;
    u32 bread;
    u16 i;
    u8 *p;
    demomyUsart<<"bps:"<<(int )bps<<endl;
    if (this->bps==24){
        uint8_t tempBuf24Bits[I2S_TX_DMA_BUF_SIZE]={0};
//                (uint8_t *)mymalloc(SRAMIN,I2S_TX_DMA_BUF_SIZE);
        readlen=(I2S_TX_DMA_BUF_SIZE/4)*3;
        f_read(file,tempBuf24Bits,readlen,(UINT *)&bread);
        p=tempBuf24Bits;
        for (int j = 0; j < I2S_TX_DMA_BUF_SIZE;) {
            buf[j++]=p[1];
            buf[j]=p[2];
            j+=2;
            buf[j++]=p[0];
            p+=3;
        }
//        for (int x = 0; x < 512;x++){
//            demomyUsart<<""<<x<<":buf:"<<buf[x]<<endl;
//        }
        bread=(bread*4)/3;
       // myfree(SRAMIN,this->tempBuf24Bits);
//       delete tempBuf24Bits;
//        f_close(file);
//        demomyUsart<<"DocumentAnalysisTransmitBufFillLoopafter:pname:"<<(char *)ConstPname<<endl;
//        return 1;
    }else{
        f_read(file,buf,I2S_TX_DMA_BUF_SIZE,(UINT *)&bread);
        if (bread<I2S_TX_DMA_BUF_SIZE){
            for (int j = bread; j < I2S_TX_DMA_BUF_SIZE - bread; ++j) {
                buf[j] = 0;
            }
        }

    }
    return bread;
}
/***************************************************************
  *  @brief     �������� �õ���ǰ���ŵĵط�
  *  @param     ����
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint32_t DocumentAnalysis::DocumentAnalysisGetCurTime() {
    long long fpos;
    totsec=datasize/(BitRateSpeed/8);	//�����ܳ���(��λ:��)
    fpos=file->fptr-datastart; 					//�õ���ǰ�ļ����ŵ��ĵط�
    cursec=fpos*totsec/datasize;	//��ǰ���ŵ��ڶ�������?
}
/***************************************************************
  *  @brief     �������� չʾSD����Ϣ �Լ��ж��Ƿ�ɹ���ʼ��
  *  @param     ���� ��
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/
void DocumentAnalysis::DocumentAnalysisShowSDInfo() {
    HAL_SD_CardInfoTypeDef SDCardInfo;
        uint64_t Cardcap;
        HAL_SD_CardCIDTypeDef SDCardCID;
        HAL_SD_GetCardCID(&hsd,&SDCardCID);	//��ȡCID
        HAL_SD_GetCardInfo(&hsd,&SDCardInfo);					//��ȡSD����Ϣ
        switch(SDCardInfo.CardType)
        {
            case CARD_SDSC:
            {
                if(SDCardInfo.CardVersion == CARD_V1_X)
                    demomyUsart<<("Card Type:SDSC V1\r\n");
                else if(SDCardInfo.CardVersion == CARD_V2_X)
                    demomyUsart<<("Card Type:SDSC V2\r\n");
            }
                break;
            case CARD_SDHC_SDXC:demomyUsart<<("Card Type:SDHC\r\n");break;
        }
        Cardcap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//����SD������
        demomyUsart.Myprintf("Card ManufacturerID:%d\r\n",SDCardCID.ManufacturerID);					//������ID
        demomyUsart.Myprintf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);								//����Ե�ַ
        demomyUsart.Myprintf("LogBlockNbr:%d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));					//��ʾ�߼�������
        demomyUsart.Myprintf("LogBlockSize:%d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));					//��ʾ�߼����С
        demomyUsart.Myprintf("Card Capacity:%d MB\r\n",(uint32_t)(Cardcap>>20));							//��ʾ����
        demomyUsart.Myprintf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);

    }
/***************************************************************
  *  @brief     �������� �õ��ļ���·�����ļ���
  *  @param     ����   ��
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint8_t* DocumentAnalysis::DocumentAnalysisNameWithPath() {
    uint8_t res;char *tempname= (char*)mymalloc(SRAMIN,32);uint8_t i=0;
    memset(ConstPname,0,128);
//    res= f_opendir(wavdir,m_filepath);
//    if (res==FR_OK){
        dir_sdi(wavdir,wavindextbl[curindex]);
        res= f_readdir(wavdir,wavfileinfo);
        if ((res!=FR_OK)||wavfileinfo->fname[0]==0){
            return nullptr;
        }
        strcpy((char *)ConstPname,"0:/MUSIC/");
        while (wavfileinfo->fname[i]!=0){
            tempname[i]= tolower(wavfileinfo->fname[i]);
            i++;
        }
        strcat((char *)ConstPname,tempname);
//        strcpy(ConstPname,(char *)pname);
        demomyUsart<<"DocumentAnalysisNameWithPath:pname:";
        demomyUsart.print((char *)ConstPname);
        curMusicname=wavfileinfo->fname;
        ShowIndex=curindex+1;
//    }
    f_closedir(wavdir);
    return  ConstPname;

}

/***************************************************************
  *  @brief     �������� �õ����и�����Ŀ
  *  @param     ����
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint16_t DocumentAnalysis::DocumentAnalysisGetAllMusicNum() {
    uint8_t res;
    uint16_t rval=0;
    DIR tDir;
    FILINFO * tFileInfo;
    tFileInfo= (FILINFO *)mymalloc(SRAMIN,sizeof(FILINFO));
    res=f_opendir(&tDir,m_filepath);
    if ((res==FR_OK)&&(tFileInfo)){
        while (true){
            res=f_readdir(&tDir,tFileInfo); //��˳���ȡÿһ���ļ�
            if ((res!=FR_OK)||(tFileInfo->fname[0]==0))break;

            res= DocumentAnalysisJudgeType(tFileInfo->fname);
            if ((res& 0XFF)==0x40){ //ȡ��4λ�ж��ǲ��������ļ�
                rval++;
            }
        }
    }else{
        return 1;
    }
    myfree(SRAMIN,tFileInfo);
    this->totwavnum=rval;
    return 0;
}
/***************************************************************
  *  @brief     ��������  �жϵ�ǰ�ļ��ĸ�ʽ
  *  @param     ����   �ļ�����
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/

uint8_t DocumentAnalysis::DocumentAnalysisJudgeType(char *fname) {

    char tbuf[5]={0};
    u8 *attr;//��׺��
    u8 i=0,j;
    while(i<250)
    {
        i++;
        if(*fname=='\0')break;//ƫ�Ƶ��������.
        fname++;
    }
    if(i==250)return 0XFF;//������ַ���.
    for(i=0;i<5;i++)//�õ���׺��
    {
        fname--;
        if(*fname=='.')
        {
            fname++;
            attr=(u8 *)fname;

            break;
        }
    }
    strcpy(tbuf,(const char*)attr);//copy
//    demomyUsart<<"tbuf:"<<endl;
    for(i=0;i<4;i++){
        tbuf[i]=toupper(tbuf[i]);//ȫ����Ϊ��д
//        demomyUsart<<tbuf[i];
    }
    for(i=0;i<FILE_MAX_TYPE_NUM;i++)	//����Ա�
    {
        for(j=0;j<FILE_MAX_SUBT_NUM;j++)//����Ա�
        {
            if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
            if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
            {
                return (i<<4)|j;
            }
        }
    }
    return 0XFF;//û�ҵ�
}
DocumentAnalysis::~DocumentAnalysis(){
    DocumentAnalysisDeStruct();

//    myfree(SRAMIN,wavfileinfo);

}

/***************************************************************
  *  @brief     �������� �õ����ֵ�ַ������洢��wavindextbl����ʹ��curindex���з���
  *  @param     ����
  *  @note      ��ע ����
  *  @Sample usage:     ������ʹ�÷��� ��ֱ�ӵ���
   * @author    ZEN3
 **************************************************************/
uint8_t DocumentAnalysis::DocumentAnalysisGetIndex() {
    uint8_t res;
    uint32_t temp;
    GPIO LED(PF, 9);
    if (f_opendir(wavdir, m_filepath)) {
        LED.TimingToggle();
    }
    DocumentAnalysisGetAllMusicNum();
    if (!totwavnum) demomyUsart << "no musicFile" << endl;
    wavindextbl = (u16 *) mymalloc(SRAMIN, (2 * totwavnum));


    if (!wavfileinfo || !wavindextbl) {
        LED.TimingToggle();
    }
    /*��¼����*/
    res = f_opendir(wavdir, m_filepath);
    if (res == FR_OK) {

        curindex = 0;
        while (true) {
            temp = wavdir->dptr;

            res = f_readdir(wavdir, wavfileinfo);
            if ((res != FR_OK) || (wavfileinfo->fname[0] == 0))break;//ĩβ���˳�
            res= DocumentAnalysisJudgeType(wavfileinfo->fname);
            if ((res&0xFF)==0x40){ //ȡ��4λ�ж��ǲ��������ļ�
                wavindextbl[curindex++] = temp;
            }

        }

    }
    curindex=0;
    return res;
}

FATFS *DocumentAnalysis::getFs() const {
    return fs;
}

void DocumentAnalysis::setFs(FATFS *fs) {
    DocumentAnalysis::fs = fs;
}

uint16_t DocumentAnalysis::getAudioformat() const {
    return audioformat;
}

void DocumentAnalysis::setAudioformat(uint16_t audioformat) {
    DocumentAnalysis::audioformat = audioformat;
}

uint16_t DocumentAnalysis::getNchannels() const {
    return nchannels;
}

void DocumentAnalysis::setNchannels(uint16_t nchannels) {
    DocumentAnalysis::nchannels = nchannels;
}

uint16_t DocumentAnalysis::getBlockalign() const {
    return blockalign;
}

void DocumentAnalysis::setBlockalign(uint16_t blockalign) {
    DocumentAnalysis::blockalign = blockalign;
}

uint32_t DocumentAnalysis::getDatasize() const {
    return datasize;
}

void DocumentAnalysis::setDatasize(uint32_t datasize) {
    DocumentAnalysis::datasize = datasize;
}

uint32_t DocumentAnalysis::getTotsec() const {
    return totsec;
}

void DocumentAnalysis::setTotsec(uint32_t totsec) {
    DocumentAnalysis::totsec = totsec;
}

uint32_t DocumentAnalysis::getCursec() const {
    return cursec;
}

void DocumentAnalysis::setCursec(uint32_t cursec) {
    DocumentAnalysis::cursec = cursec;
}

uint32_t DocumentAnalysis::getBitRateSpeed() const {
    return BitRateSpeed;
}

void DocumentAnalysis::setBitRateSpeed(uint32_t bitRateSpeed) {
    BitRateSpeed = bitRateSpeed;
}

uint32_t DocumentAnalysis::getSamplerate() const {
    return samplerate;
}

void DocumentAnalysis::setSamplerate(uint32_t samplerate) {
    DocumentAnalysis::samplerate = samplerate;
}

uint16_t DocumentAnalysis::getBps() const {
    return bps;
}

void DocumentAnalysis::setBps(uint16_t bps) {
    DocumentAnalysis::bps = bps;
}

uint32_t DocumentAnalysis::getDatastart() const {
    return datastart;
}

void DocumentAnalysis::setDatastart(uint32_t datastart) {
    DocumentAnalysis::datastart = datastart;
}
 DIR *DocumentAnalysis::getWavdir()  {
    return wavdir;
}

void DocumentAnalysis::setWavdir( DIR *wavdir) {
    DocumentAnalysis::wavdir = wavdir;
}

FILINFO *DocumentAnalysis::getWavfileinfo() const {
    return wavfileinfo;
}

void DocumentAnalysis::setWavfileinfo(FILINFO *wavfileinfo) {
    DocumentAnalysis::wavfileinfo = wavfileinfo;
}

char *DocumentAnalysis::getCurMusicname() const {
    return curMusicname;
}

void DocumentAnalysis::setCurMusicname(char *curMusicname) {
    DocumentAnalysis::curMusicname = curMusicname;
}

u8 *DocumentAnalysis::getFn() const {
    return fn;
}

void DocumentAnalysis::setFn(u8 *fn) {
    DocumentAnalysis::fn = fn;
}

u8 *DocumentAnalysis::getPname() const {
    return ConstPname;
}

void DocumentAnalysis::setPname(u8 *pname) {
    DocumentAnalysis::ConstPname = ConstPname;
}

u16 DocumentAnalysis::getTotwavnum() const {
    return totwavnum;
}

void DocumentAnalysis::setTotwavnum(u16 totwavnum) {
    DocumentAnalysis::totwavnum = totwavnum;
}

u16 DocumentAnalysis::getCurindex() const {
    return curindex;
}

void DocumentAnalysis::setCurindex(u16 curindex) {
    DocumentAnalysis::curindex = curindex;
}

uint16_t DocumentAnalysis::getShowIndex() const {
    return ShowIndex;
}

void DocumentAnalysis::setShowIndex(uint16_t showIndex) {
    ShowIndex = showIndex;
}

u16 *DocumentAnalysis::getWavindextbl() const {
    return wavindextbl;
}

void DocumentAnalysis::setWavindextbl(u16 *wavindextbl) {
    DocumentAnalysis::wavindextbl = wavindextbl;
}

char *DocumentAnalysis::getMFilepath() const {
    return m_filepath;
}

void DocumentAnalysis::setMFilepath(char *mFilepath) {
    m_filepath = mFilepath;
}

void DocumentAnalysis::DocumentAnalysisDeStruct() {
    myfree(SRAMIN, wavfileinfo);    /* �ͷ��ڴ� */
    myfree(SRAMIN, ConstPname);          /* �ͷ��ڴ� */
    myfree(SRAMIN, wavindextbl);   /* �ͷ��ڴ� */
//    myfree(SRAMIN, Buf16Bits);
    myfree(SRAMIN,file);
    myfree(SRAMIN,wavdir);

    demomyUsart<<"deStruct OK"<<endl;
}

FIL *DocumentAnalysis::getFile() const {
    return file;
}

 uint8_t *DocumentAnalysis::getBuf0() {
    return Buf0;
}

 uint8_t *DocumentAnalysis::getBuf1()  {
    return Buf1;
}







