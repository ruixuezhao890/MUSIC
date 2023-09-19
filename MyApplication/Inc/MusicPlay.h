#ifndef MUSICPLAY_H
#define MUSICPLAY_H
#include "MyI2s.h"
#include "WString.h"
#include "vector"
#include "MyKey.h"
//#include "WM8978.h"

class MusicPlay{

public:
    MusicPlay();
    MusicPlay(I2S_HandleTypeDef *hi2s);
    void MusicPlayInit();
    void MusicPlayStart();
    void MusicPlayStop();
    void MusicPlayDestruct();
    void MusicPlayASong();
    uint8_t PlayStartAwave();
    void audio_play();
    ~MusicPlay();
private:
    String rootPath;
    MyI2s * MusicI2s;
public:
    uint8_t wavwitchbuf = 0;

    uint8_t wavtransferend;
    MyI2s *getMusicI2S() ;
    uint8_t getPlayStaut();
private:
    uint8_t PlayStaut;
    I2S_HandleTypeDef *m_hi2s;
    MyButton * key;

};
void MusicPlayHandleInit(MusicPlay* temp);
//extern std::vector<MusicPlay *> MusicPlayHandle;

#endif