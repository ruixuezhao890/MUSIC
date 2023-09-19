/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : inputStream.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#include "inputStream.h"

int inputStream::available() {
    return buff.data_size;
}

int inputStream::read() {
    if(buff.data_size==0){
        return -1;
    }
    buff.data_size--;
    return buff.buf[buff.read_index++];
}

int inputStream::peek() {
    if (buff.data_size==0){
        return -1;
    }
    return buff.buf[buff.read_index];
}

long inputStream::scanNumber() {
    char ch;
    do
    {
        ch = getc();
    } while (ch != '-' && (ch <= '0' || ch >= '9'));

    long res;
    int fu_flag = 0;

    if (ch == '-')
    {
        res = 0;
        fu_flag = 1;
    }
    else
    {
        res = ch - '0';
    }

    ch = getc();
    while (ch >= '0' && ch <= '9')
    {
        res *= 10;
        res += (ch - '0');
        ch = getc();
    }

    if (fu_flag)
        res = -res;
    return res;
}

double inputStream::scanFloat() {
    char ch;
    do
    {
        ch = getc();
    } while (ch != '-' && (ch <= '0' || ch >= '9'));

    double res;
    int fu_flag = 0;

    if (ch == '-')
    {
        res = 0;
        fu_flag = 1;
    }
    else
    {
        res = (double)(ch - '0');
    }

    ch = getc();
    while (ch >= '0' && ch <= '9')
    {
        res *= 10.0;
        res += (double)(ch - '0');
        ch = getc();
    }

    if (ch != '.')
    {
        if (fu_flag)
            res = -res;
        return res;
    }

    double d = 0.1;

    while (1)
    {
        ch = getc();
        if (ch < '0' || ch > '9')
            break;
        res += d * (double)(ch - '0');
        d /= 10.0;
    }
    if (fu_flag)
        res = -res;
    return res;
}

int inputStream::getc() {
//    if (available()<=0){
//        return -1;
//    }
    return read();
}

int inputStream::scan(char *in) {
    char ch;
    do
    {
        ch = getc();
    } while (ch <' '|| ch > '~');

    int pos = 0;
    in[pos++] = ch;
    while (1)
    {
        ch = getc();
        if (ch < ' ' || ch > '~')
        {
            break;
        }
        in[pos++] = ch;
    }
    in[pos] = '\0';
    return pos;
}

int inputStream::scan(String &in) {
    in=" ";
    char ch;
    do
    {
        ch = getc();
    } while (ch < ' ' || ch > '~');
    in += ch;
    while (1)
    {
        ch = getc();
        if (ch < ' ' || ch > '~')
        {
            break;
        }
        in += ch;
    }
    return in.length();
}

int inputStream::scan(char &in) {
    in = getc();
    return in;
}

int inputStream::scan(unsigned char &in) {
    in = scanNumber();
    return in;
}

int inputStream::scan(int &in) {
    in = scanNumber();
    return in;
}
int inputStream::scan(uint16_t &in) {
    in = scanNumber();
    return in;
}
int inputStream::scan(uint32_t &in) {
    in = scanNumber();
    return in;
}
int inputStream::scan(unsigned int &in) {
    in = scanNumber();
    return in;
}

int inputStream::scan(long long int &in) {
    in = scanNumber();
    return in;
}

int inputStream::scan(unsigned long long int &in) {
    in = scanNumber();
    return in;
}

int inputStream::scan(double &in) {
    in = scanFloat();
    return (int)in;
}

int inputStream::scan(float &in) {
    in = scanFloat();
    return (int)in;
}

int inputStream::scan(int & in, int base) {
    CMDFlag=1;
    in=read();
    CMDFlag=0;
    return 1;
}

int inputStream::scan(uint16_t & in, int base) {
    CMDFlag=1;
    in=read();
    CMDFlag=0;
    return 1;
}

int inputStream::scan(uint32_t & in, int base) {
    CMDFlag=1;
    in=read();
    CMDFlag=0;
    return 1;
}

int inputStream::scan(unsigned int & in, int base) {
    CMDFlag=1;
    in=read();
    CMDFlag=0;
    return 1;
}

int inputStream::scan(long long int & in, int base) {
    CMDFlag=1;
    in=read();
    CMDFlag=0;
    return 1;
}

int inputStream::scan(unsigned long long int & in, int base) {
    CMDFlag=1;
    in=read();
    CMDFlag=0;
    return 1;
}

int inputStream::scan(uint8_t &in, int base) {
    CMDFlag=2;
    in=read();
    CMDFlag=0;
    return 1;
}
