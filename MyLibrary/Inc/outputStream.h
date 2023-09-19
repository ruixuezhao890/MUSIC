/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : outputStream.h
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#ifndef MY_LIBARARY_OUTPUTSTREAM_H
#define MY_LIBARARY_OUTPUTSTREAM_H

#include <cstring>
#include "WString.h"

//#ifdef __cplusplus
//extern "C"
//{
//#endif
#include "main.h"
#define endl  "\r\n"
#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define CMD 0

/*______________________类的声明在这下面________________:*/

class outputStream{
private:
    size_t printNumber(unsigned long,uint8_t);
    size_t printFloat(double ,uint8_t);

public:
//    static uint16_t SelectMode;
//    static void SetSelectMode(uint16_t Set);
    virtual size_t write(uint8_t) = 0;
    size_t write(const char *str)
    {
        if (str == NULL)
            return 0;
        return write((const uint8_t *)str, strlen(str));
    }
    virtual size_t write(const uint8_t *, size_t) = 0;
    size_t write(const char *buffer, size_t size)
    {
        return write((const uint8_t *)buffer, size);
    }

    // size_t print(const __FlashStringHelper *);
    size_t print(const String &);
    size_t print(const char[]);
    size_t print(char);

    size_t print(unsigned char, int = DEC);
    size_t print(int, int  = DEC);
    size_t print(unsigned int, int = DEC);
    size_t print(long long, int = DEC);
    size_t print(unsigned long long, int = DEC);
    size_t print(double, int = 2);
    // size_t print(const Printable &);

    // size_t println(const __FlashStringHelper *);
    size_t println(const String &s);
    size_t println(const char[]);
    size_t println(char);
    size_t println(unsigned char, int = DEC);
    size_t println(int, int = DEC);
    size_t println(unsigned int, int = DEC);
    size_t println(long long, int = DEC);
    size_t println(unsigned long long, int = DEC);
    size_t println(double, int = 2);
    // size_t println(const Printable &);
    size_t println(void) { return print("\r\n"); };

    template<typename T>
    outputStream &operator<<(T out)
    {
        print(out);
        return *this;
    }

    outputStream &operator<<(String &str)
    {
        print(str);
        return *this;
    }
};
//uint16_t outputStream::SelectMode=DEC;
//#ifdef __cplusplus
//}
//#endif
#endif //MY_LIBARARY_OUTPUTSTREAM_H
