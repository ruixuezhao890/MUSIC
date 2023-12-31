/**
*********************************************************************
*********
* @project_name :my_libarary
* @file : outputStream.cpp
* @author : zen3
* @brief : None
* @attention : None
* @date : 2023/8/14 
*********************************************************************
*********
*/
//

#include <cmath>
#include "outputStream.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
size_t outputStream::printNumber(unsigned long n, uint8_t base) {
    char buf[8*sizeof(long )+1];
    char *str=&buf[sizeof(buf)-1];
    *str='\0';

    if (base<2){
        base=10;
    }

    do {
        char c=n%base;
        n/=base;

        *--str=c<10?c+'0':c+'A'-10;
    } while (n);
    return write(str);
}

size_t outputStream::printFloat(double number, uint8_t digits) {
    size_t n=0;
    if (isnan(number))//判断是否为正确的数据类型
        return print("nan");
    if (isinf(number))//判断是否为无穷
        return print("inf");
    if (number > 4294967040.0)
        return print("ovf"); // constant determined empirically
    if (number < -4294967040.0)
        return print("ovf"); // constant determined empirically

    // Handle negative numbers
    if (number < 0.0)
    {
        n += print('-');
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long long int_part = (unsigned long long)number;
    double remainder = number - (double)int_part;
    n += print(int_part);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
        n += print(".");
    }

    // Extract digits from the remainder one at a time
    while (digits--> 0)
    {
        remainder *= 10.0;
        unsigned int toPrint = (unsigned int)(remainder);
        n += print(toPrint);
        remainder -= toPrint;
    }

    return n;
}

size_t outputStream::print(const String & s) {
    return write(s.c_str(),s.length());
}

size_t outputStream::print(const char *str) {
    return write(str);
}

size_t outputStream::print(char c) {
    return write(c);
}

size_t outputStream::print(unsigned char b, int base) {
    return print((unsigned long long)b,base);
}

size_t outputStream::print(int n, int base) {
    return print((long long)n,base);
}

size_t outputStream::print(unsigned int n, int base) {
    return print((unsigned long long)n,base);
}

size_t outputStream::print(long long n, int base) {
    if (base==0){
        return write(n);
    }
    else if (base==10){
        if (n<0){
            int t = print('-');
            n=-n;
            return printNumber(n,10)+t;
        }
        return printNumber(n,10);
    }
    else{
        return printNumber(n,base);
    }
}

size_t outputStream::print(unsigned long long n, int base) {
    if (base==0){
        return write(n);
    }else
    {
        return printNumber(n,base);
    }

}

size_t outputStream::print(double n, int digits) {
    return printFloat(n,digits);
}

size_t outputStream::println(const String &s) {
    size_t n= print(s);
    n+=println();
    return n;
}

size_t outputStream::println(const char * c) {
    size_t n= print(c);
    n+= println();
    return n;
}

size_t outputStream::println(char c) {
    size_t n= print(c);
    n+= println();
    return n;
}

size_t outputStream::println(unsigned char b, int base) {
    size_t n = print(b, base);
    n += println();
    return n;
}

size_t outputStream::println(int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t outputStream::println(unsigned int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t outputStream::println(long long int num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t outputStream::println(unsigned long long num, int base) {
    size_t n = print(num, base);
    n += println();
    return n;
}

size_t outputStream::println(double num, int digits) {
    size_t n = print(num, digits);
    n += println();
    return n;
}

//void outputStream::SetSelectMode(uint16_t Set) {
//    SelectMode=Set;
//}


