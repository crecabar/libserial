/* 
 * File:   Serial.h
 * Author: crecabar
 *
 * Created on April 29, 2015, 10:16 AM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#include <stdio.h>

#define     PARITY_7E1      1
#define     PARITY_8N1      2

class Serial
{
private:
    char* _dev;
    char* _buffer;
    char* _temp;
    int _baud;
    int _dataBits;
    int _bufferSize;
    int _parity;
    int _bufferIndex;
    int _blocking;
    int _fd;
public:
    //Serial();
    Serial(char* dev, int baud, int parity, int blocking);
    ~Serial();
    int initPort();
    void flushPort();
    int getData(char* data);
    int sendData(char* data);
    char getChar();
    int sendChar(char data);
    void closePort();
    int getBaud() const;
    void setBaud(int baud);
    int getBlocking() const;
    void setBlocking(int blocking);
    char* getDev() const;
    void setDev(char* dev);
    int getParity() const;
    void setParity(int parity);
};

#endif	/* SERIAL_H */

