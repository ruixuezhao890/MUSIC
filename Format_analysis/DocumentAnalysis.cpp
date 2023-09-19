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
#define FILE_MAX_TYPE_NUM		7	//最多FILE_MAX_TYPE_NUM个大类
#define FILE_MAX_SUBT_NUM		4	//最多FILE_MAX_SUBT_NUM个小类
char *const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]=
        {
                {"BIN"},			//BIN文件
                {"LRC"},			//LRC文件
                {"NES"},			//NES文件
                {"TXT","C","H"},	//文本文件
                {"WAV","MP3","APE","FLAC"},//支持的音乐文件
                {"BMP","JPG","JPEG","GIF"},//图片文件
                {"AVI"},			//视频文件
        };
DocumentAnalysis::DocumentAnalysis() {

}
/***************************************************************
  *  @brief     函数作用 初始化该类
  *  @param     参数   读取SD的地址
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
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
    wavfileinfo = (FILINFO*)mymalloc(SRAMIN, sizeof(FILINFO));  /* 申请内存 */
    wavdir=(DIR*)mymalloc(SRAMIN, sizeof(DIR));

    if(res){
        demomyUsart<<"SD error"<<endl;
        return 1;
    }
    demomyUsart<<"SD ok"<<endl;//this->DocumentAnalysisShowSDInfo();
    DocumentAnalysisGetIndex();//得到索引表
//    DocumentAnalysisNameWithPath();//测试
//    DocumentAnalysisASongInit();

    return 0;
}

/***************************************************************
  *  @brief     函数作用 得到一首歌的文件信息以及判断是否是需要的文件
  *  @param     参数 无
  *  @note      备注 ：必须在得到一个带名字的路径之后使用
  *  @Sample usage:     函数的使用方法 ：直接调用
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
        res=f_opendir(wavdir,(const TCHAR*)m_filepath); 	//打开目录
        if (FR_OK!=res){
            res=4;
            return res;
        }

     demomyUsart<<endl<<"res:"<<( res=f_open(file,(TCHAR*)ConstPname,FA_READ))<<endl;//打开文件
//        demomyUsart<<"pname"<<(char *)pname<<endl;
//        demomyUsart<<"pnameInt:"<<(int )pname<<endl;
        if (res==FR_OK){

            f_read(file,buf,512,(UINT *)&br);
            riff=(ChunkRIFF*)buf;
            if (riff->Format==0X45564157){
                fmt=(ChunkFMT *)(buf+12);//FMT块
                fact=(ChunkFACT *)(buf+12+8+fmt->ChunkSize);//FACT块
                if(fact->ChunkID==0X74636166||fact->ChunkID==0X5453494C)this->datastart=12+8+fmt->ChunkSize+8+fact->ChunkSize;//具有fact/LIST块的时候(未测试)
                else this->datastart=12+8+fmt->ChunkSize;//不具有fact/LIST块的时候(未测试)

                data=(ChunkDATA *)(buf+this->datastart);
                if (data->ChunkID==0X61746164){
                   this->audioformat=fmt->AudioFormat;		//音频格式
                   this->nchannels=fmt->NumOfChannels;		//通道数
                   this->samplerate=fmt->SampleRate;		//采样率
                   this->BitRateSpeed=fmt->ByteRate*8;			//得到位速
                   this->blockalign=fmt->BlockAlign;		//块对齐
                   this->bps=fmt->BitsPerSample;			//位数,16/24/32位
                   this->datasize=data->ChunkSize;			//数据块大小
                   this->datastart=this->datastart+8;		//数据流开始的地方,8是ChunkDATA块的 ChunkID+ChunkSize

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
  *  @brief     函数作用 填充发送数组 根据一首歌的解析信息自动选择是24bit还是16bit
  *  @param     参数
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
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
  *  @brief     函数作用 得到当前播放的地方
  *  @param     参数
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
   * @author    ZEN3
 **************************************************************/

uint32_t DocumentAnalysis::DocumentAnalysisGetCurTime() {
    long long fpos;
    totsec=datasize/(BitRateSpeed/8);	//歌曲总长度(单位:秒)
    fpos=file->fptr-datastart; 					//得到当前文件播放到的地方
    cursec=fpos*totsec/datasize;	//当前播放到第多少秒了?
}
/***************************************************************
  *  @brief     函数作用 展示SD卡信息 以及判断是否成功初始化
  *  @param     参数 无
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
   * @author    ZEN3
 **************************************************************/
void DocumentAnalysis::DocumentAnalysisShowSDInfo() {
    HAL_SD_CardInfoTypeDef SDCardInfo;
        uint64_t Cardcap;
        HAL_SD_CardCIDTypeDef SDCardCID;
        HAL_SD_GetCardCID(&hsd,&SDCardCID);	//获取CID
        HAL_SD_GetCardInfo(&hsd,&SDCardInfo);					//获取SD卡信息
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
        Cardcap=(uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);	//计算SD卡容量
        demomyUsart.Myprintf("Card ManufacturerID:%d\r\n",SDCardCID.ManufacturerID);					//制造商ID
        demomyUsart.Myprintf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);								//卡相对地址
        demomyUsart.Myprintf("LogBlockNbr:%d \r\n",(uint32_t)(SDCardInfo.LogBlockNbr));					//显示逻辑块数量
        demomyUsart.Myprintf("LogBlockSize:%d \r\n",(uint32_t)(SDCardInfo.LogBlockSize));					//显示逻辑块大小
        demomyUsart.Myprintf("Card Capacity:%d MB\r\n",(uint32_t)(Cardcap>>20));							//显示容量
        demomyUsart.Myprintf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);

    }
/***************************************************************
  *  @brief     函数作用 得到文件带路径的文件名
  *  @param     参数   无
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
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
  *  @brief     函数作用 得到所有歌曲数目
  *  @param     参数
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
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
            res=f_readdir(&tDir,tFileInfo); //按顺序读取每一个文件
            if ((res!=FR_OK)||(tFileInfo->fname[0]==0))break;

            res= DocumentAnalysisJudgeType(tFileInfo->fname);
            if ((res& 0XFF)==0x40){ //取高4位判断是不是音乐文件
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
  *  @brief     函数作用  判断当前文件的格式
  *  @param     参数   文件名字
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
   * @author    ZEN3
 **************************************************************/

uint8_t DocumentAnalysis::DocumentAnalysisJudgeType(char *fname) {

    char tbuf[5]={0};
    u8 *attr;//后缀名
    u8 i=0,j;
    while(i<250)
    {
        i++;
        if(*fname=='\0')break;//偏移到了最后了.
        fname++;
    }
    if(i==250)return 0XFF;//错误的字符串.
    for(i=0;i<5;i++)//得到后缀名
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
        tbuf[i]=toupper(tbuf[i]);//全部变为大写
//        demomyUsart<<tbuf[i];
    }
    for(i=0;i<FILE_MAX_TYPE_NUM;i++)	//大类对比
    {
        for(j=0;j<FILE_MAX_SUBT_NUM;j++)//子类对比
        {
            if(*FILE_TYPE_TBL[i][j]==0)break;//此组已经没有可对比的成员了.
            if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//找到了
            {
                return (i<<4)|j;
            }
        }
    }
    return 0XFF;//没找到
}
DocumentAnalysis::~DocumentAnalysis(){
    DocumentAnalysisDeStruct();

//    myfree(SRAMIN,wavfileinfo);

}

/***************************************************************
  *  @brief     函数作用 得到音乐地址索引表存储在wavindextbl当中使用curindex进行访问
  *  @param     参数
  *  @note      备注 ：无
  *  @Sample usage:     函数的使用方法 ：直接调用
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
    /*记录索引*/
    res = f_opendir(wavdir, m_filepath);
    if (res == FR_OK) {

        curindex = 0;
        while (true) {
            temp = wavdir->dptr;

            res = f_readdir(wavdir, wavfileinfo);
            if ((res != FR_OK) || (wavfileinfo->fname[0] == 0))break;//末尾了退出
            res= DocumentAnalysisJudgeType(wavfileinfo->fname);
            if ((res&0xFF)==0x40){ //取高4位判断是不是音乐文件
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
    myfree(SRAMIN, wavfileinfo);    /* 释放内存 */
    myfree(SRAMIN, ConstPname);          /* 释放内存 */
    myfree(SRAMIN, wavindextbl);   /* 释放内存 */
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







