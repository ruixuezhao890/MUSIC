/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : DocumentAnalysis.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/9/14
*********************************************************************
*********
*/
//

//记得如果中断不能正常进入检查关于i2s的回调函数是否指向正确
#ifndef MY_LIBARARY_DOCUMENTANALYSIS_H
#define MY_LIBARARY_DOCUMENTANALYSIS_H
#include "sys.h"
#include "ff.h"
#define I2S_TX_DMA_BUF_SIZE    8192		//定义TX DMA 数组大小(播放192Kbps@24bit的时候,需要设置8192大才不会卡)
#define ROOTPATH "0:/MUSIC"

#define T_BIN		0X00	//bin文件
#define T_LRC		0X10	//lrc文件

#define T_NES		0X20	//nes文件
#define T_SMS		0X21	//sms文件

#define T_TEXT		0X30	//.txt文件
#define T_C			0X31	//.c文件
#define T_H			0X32    //.h文件

#define T_WAV		0X40	//WAV文件
#define T_MP3		0X41	//MP3文件
#define T_APE		0X42	//APE文件
#define T_FLAC		0X43	//FLAC文件

#define T_BMP		0X50	//bmp文件
#define T_JPG		0X51	//jpg文件
#define T_JPEG		0X52	//jpeg文件
#define T_GIF		0X53	//gif文件

#define T_AVI		0X60	//avi文件
typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;这里固定为"RIFF",即0X46464952 */
    uint32_t ChunkSize ;        /* 集合大小;文件总大小-8 */
    uint32_t Format;            /* 格式;WAVE,即0X45564157 */
}ChunkRIFF;     /* RIFF块 */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;这里固定为"fmt ",即0X20746D66 */
    uint32_t ChunkSize ;        /* 子集合大小(不包括ID和Size);这里为:20. */
    uint16_t AudioFormat;       /* 音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM */
    uint16_t NumOfChannels;     /* 通道数量;1,表示单声道;2,表示双声道; */
    uint32_t SampleRate;        /* 采样率;0X1F40,表示8Khz */
    uint32_t ByteRate;          /* /字节速率; */
    uint16_t BlockAlign;        /* 块对齐(字节); */
    uint16_t BitsPerSample;     /* 单个采样数据大小;4位ADPCM,设置为4 */
//    uint16_t ByteExtraData;   /* 附加的数据字节;2个; 线性PCM,没有这个参数 */
}ChunkFMT ;      /* fmt块 */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;这里固定为"fact",即0X74636166; */
    uint32_t ChunkSize ;        /* 子集合大小(不包括ID和Size);这里为:4. */
    uint32_t NumOfSamples;      /* 采样的数量; */
}ChunkFACT;     /* fact块 */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;这里固定为"LIST",即0X74636166; */
    uint32_t ChunkSize ;        /* 子集合大小(不包括ID和Size);这里为:4. */
}ChunkLIST;     /* LIST块 */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;这里固定为"data",即0X5453494C */
    uint32_t ChunkSize ;        /* 子集合大小(不包括ID和Size) */
}ChunkDATA;     /* data块 */

typedef __PACKED_STRUCT
{
    ChunkRIFF riff;             /* riff块 */
    ChunkFMT fmt;               /* fmt块 */
//    ChunkFACT fact;           /* fact块 线性PCM,没有这个结构体 */
    ChunkDATA data;             /* data块 */
}__WaveHeader;  /* wav头 */

class DocumentAnalysis {
public:
    DocumentAnalysis();
    uint8_t DocumentAnalysisInit(char * filepath);
    uint8_t DocumentAnalysisASongInit();
    uint8_t* DocumentAnalysisNameWithPath();
    uint8_t DocumentAnalysisGetIndex();
    uint16_t DocumentAnalysisGetAllMusicNum();
    uint32_t DocumentAnalysisTransmitBufFill();
    uint32_t DocumentAnalysisGetCurTime();
    uint8_t DocumentAnalysisJudgeType(char * fname);
    void DocumentAnalysisDeStruct();
    ~DocumentAnalysis();


private:
    void DocumentAnalysisShowSDInfo();
    FIL *file;
    uint8_t Buf0[I2S_TX_DMA_BUF_SIZE];
public:
     uint8_t *getBuf0();

     uint8_t *getBuf1();

private:
    uint8_t Buf1[I2S_TX_DMA_BUF_SIZE];
public:
    FIL *getFile() const;

private:

    //uint8_t* tempBuf24Bits;
//    uint8_t* Buf16Bits;
    uint8_t   Array_selection_flag;
    FATFS* fs;
    uint16_t audioformat;       /* 音频格式;0X01,表示线性PCM;0X11表示IMA ADPCM */
    uint16_t nchannels;         /* 通道数量;1,表示单声道;2,表示双声道; */
    uint16_t blockalign;        /* 块对齐(字节); */
    uint32_t datasize;          /* WAV数据大小 */
    uint32_t totsec ;           /* 整首歌时长,单位:秒 */
    // uint32_t totsec ;           /* 整首歌时长,单位:秒 */
    uint32_t cursec ;           /* 当前播放时长 *
  //  uint32_t bitrate;           /* 比特率(位速) */
    uint32_t BitRateSpeed;     /* 比特率(位速) */
    uint32_t samplerate;        /* 采样率 */
    uint16_t bps;               /* 位数,比如16bit,24bit,32bit */
    uint32_t datastart;         /* 数据帧开始的位置(在文件里面的偏移) */

    DIR* wavdir;	 		//目录
    FILINFO* wavfileinfo;//文件信息
    char *curMusicname; //当前音乐名称
    u8 *fn;   			//长文件名
    u8 *ConstPname;			//带路径的文件名

    u16 totwavnum; 		//音乐文件总数
    u16 curindex;		//图片当前索引
    uint16_t ShowIndex; //显示索引

    u16 *wavindextbl;	//音乐索引表
    char *m_filepath;   //根目录文件名
public:
    FATFS *getFs() const;

    void setFs(FATFS *fs);

    uint16_t getAudioformat() const;

    void setAudioformat(uint16_t audioformat);

    uint16_t getNchannels() const;

    void setNchannels(uint16_t nchannels);

    uint16_t getBlockalign() const;

    void setBlockalign(uint16_t blockalign);

    uint32_t getDatasize() const;

    void setDatasize(uint32_t datasize);

    uint32_t getTotsec() const;

    void setTotsec(uint32_t totsec);

    uint32_t getCursec() const;

    void setCursec(uint32_t cursec);

    uint32_t getBitRateSpeed() const;

    void setBitRateSpeed(uint32_t bitRateSpeed);

    uint32_t getSamplerate() const;

    void setSamplerate(uint32_t samplerate);

    uint16_t getBps() const;

    void setBps(uint16_t bps);

    uint32_t getDatastart() const;

    void setDatastart(uint32_t datastart);

    DIR *getWavdir() ;

    void setWavdir( DIR *wavdir);

    FILINFO *getWavfileinfo() const;

    void setWavfileinfo(FILINFO *wavfileinfo);

    char *getCurMusicname() const;

    void setCurMusicname(char *curMusicname);

    u8 *getFn() const;

    void setFn(u8 *fn);

    u8 *getPname() const;

    void setPname(u8 *pname);

    u16 getTotwavnum() const;

    void setTotwavnum(u16 totwavnum);

    u16 getCurindex() const;

    void setCurindex(u16 curindex);

    uint16_t getShowIndex() const;

    void setShowIndex(uint16_t showIndex);

    u16 *getWavindextbl() const;

    void setWavindextbl(u16 *wavindextbl);

    char *getMFilepath() const;

    void setMFilepath(char *mFilepath);
};


#endif //MY_LIBARARY_DOCUMENTANALYSIS_H
