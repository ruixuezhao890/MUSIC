/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : inputStream.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_INPUTSTREAM_H
#define MY_LIBARARY_INPUTSTREAM_H
#include "main.h"
#include "WString.h"
#define CMD 0
#define BUFFMAX 256
typedef struct
{
     uint8_t buf[BUFFMAX];      //接收缓冲区
     uint8_t write_index;   //写指针
     uint8_t read_index;    //读指针
     uint16_t data_size;    //缓冲区接收到的数据长度
    /* data */
}__rec_buf;
class inputStream{
public:
    uint8_t CMDFlag=0;//考虑到读取指令的情况
    __rec_buf buff;
    virtual int available();
    virtual int read();
    virtual int peek();

    /***阻滞读取**********************/
    long scanNumber();
    double scanFloat();

    int getc();

    int scan(char *); //扫描字符串
    int scan(String &);
    int scan(char &);
    int scan(unsigned char &);      //扫描字符
    int scan(int &);                //扫描数字
    int scan(uint16_t &);                //扫描数字
    int scan(uint32_t &);                //扫描数字
    int scan(unsigned int &);       //扫描数字
    int scan(long long &);          //扫描数字
    int scan(unsigned long long &); //扫描数字
    int scan(double &);             //扫描浮点数
    int scan(float &);              //扫描浮点数


    int scan(int &,int base);                //扫描器件返回指令
    int scan(uint8_t &,int base);                //扫描器件返回指令
    int scan(uint16_t &,int base);           //扫描器件返回指令
    int scan(uint32_t &,int base);           //扫描器件返回指令
    int scan(unsigned int &,int base);       //扫描器件返回指令
    int scan(long long &,int base);          //扫描器件返回指令
    int scan(unsigned long long &,int base); //扫描器件返回指令

    template <typename T>
    inputStream &operator>>(T &out)
    {
        scan(out);
        return *this;
    }
};
#endif //MY_LIBARARY_INPUTSTREAM_H
