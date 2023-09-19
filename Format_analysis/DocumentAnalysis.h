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

//�ǵ�����жϲ����������������i2s�Ļص������Ƿ�ָ����ȷ
#ifndef MY_LIBARARY_DOCUMENTANALYSIS_H
#define MY_LIBARARY_DOCUMENTANALYSIS_H
#include "sys.h"
#include "ff.h"
#define I2S_TX_DMA_BUF_SIZE    8192		//����TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����8192��Ų��Ῠ)
#define ROOTPATH "0:/MUSIC"

#define T_BIN		0X00	//bin�ļ�
#define T_LRC		0X10	//lrc�ļ�

#define T_NES		0X20	//nes�ļ�
#define T_SMS		0X21	//sms�ļ�

#define T_TEXT		0X30	//.txt�ļ�
#define T_C			0X31	//.c�ļ�
#define T_H			0X32    //.h�ļ�

#define T_WAV		0X40	//WAV�ļ�
#define T_MP3		0X41	//MP3�ļ�
#define T_APE		0X42	//APE�ļ�
#define T_FLAC		0X43	//FLAC�ļ�

#define T_BMP		0X50	//bmp�ļ�
#define T_JPG		0X51	//jpg�ļ�
#define T_JPEG		0X52	//jpeg�ļ�
#define T_GIF		0X53	//gif�ļ�

#define T_AVI		0X60	//avi�ļ�
typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"RIFF",��0X46464952 */
    uint32_t ChunkSize ;        /* ���ϴ�С;�ļ��ܴ�С-8 */
    uint32_t Format;            /* ��ʽ;WAVE,��0X45564157 */
}ChunkRIFF;     /* RIFF�� */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"fmt ",��0X20746D66 */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size);����Ϊ:20. */
    uint16_t AudioFormat;       /* ��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM */
    uint16_t NumOfChannels;     /* ͨ������;1,��ʾ������;2,��ʾ˫����; */
    uint32_t SampleRate;        /* ������;0X1F40,��ʾ8Khz */
    uint32_t ByteRate;          /* /�ֽ�����; */
    uint16_t BlockAlign;        /* �����(�ֽ�); */
    uint16_t BitsPerSample;     /* �����������ݴ�С;4λADPCM,����Ϊ4 */
//    uint16_t ByteExtraData;   /* ���ӵ������ֽ�;2��; ����PCM,û��������� */
}ChunkFMT ;      /* fmt�� */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"fact",��0X74636166; */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size);����Ϊ:4. */
    uint32_t NumOfSamples;      /* ����������; */
}ChunkFACT;     /* fact�� */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"LIST",��0X74636166; */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size);����Ϊ:4. */
}ChunkLIST;     /* LIST�� */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"data",��0X5453494C */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size) */
}ChunkDATA;     /* data�� */

typedef __PACKED_STRUCT
{
    ChunkRIFF riff;             /* riff�� */
    ChunkFMT fmt;               /* fmt�� */
//    ChunkFACT fact;           /* fact�� ����PCM,û������ṹ�� */
    ChunkDATA data;             /* data�� */
}__WaveHeader;  /* wavͷ */

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
    uint16_t audioformat;       /* ��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM */
    uint16_t nchannels;         /* ͨ������;1,��ʾ������;2,��ʾ˫����; */
    uint16_t blockalign;        /* �����(�ֽ�); */
    uint32_t datasize;          /* WAV���ݴ�С */
    uint32_t totsec ;           /* ���׸�ʱ��,��λ:�� */
    // uint32_t totsec ;           /* ���׸�ʱ��,��λ:�� */
    uint32_t cursec ;           /* ��ǰ����ʱ�� *
  //  uint32_t bitrate;           /* ������(λ��) */
    uint32_t BitRateSpeed;     /* ������(λ��) */
    uint32_t samplerate;        /* ������ */
    uint16_t bps;               /* λ��,����16bit,24bit,32bit */
    uint32_t datastart;         /* ����֡��ʼ��λ��(���ļ������ƫ��) */

    DIR* wavdir;	 		//Ŀ¼
    FILINFO* wavfileinfo;//�ļ���Ϣ
    char *curMusicname; //��ǰ��������
    u8 *fn;   			//���ļ���
    u8 *ConstPname;			//��·�����ļ���

    u16 totwavnum; 		//�����ļ�����
    u16 curindex;		//ͼƬ��ǰ����
    uint16_t ShowIndex; //��ʾ����

    u16 *wavindextbl;	//����������
    char *m_filepath;   //��Ŀ¼�ļ���
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
