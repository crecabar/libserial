/* 
 * File:   Serial.cpp
 * Author: crecabar
 *
 * Created on April 29, 2015, 10:15 AM
 */

#include <Serial.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <paths.h>
#include <termios.h>
#include <sysexits.h>
#include <sys/param.h>
#include <sys/select.h>
#include <sys/time.h>
#include <time.h>
#include <iostream>

using namespace std;

/**
 * 
 */
//Serial::Serial()
//{
//    //_dev = "/dev/tty.usbmodem1451";
//    //_baud = 9600;
//    _dataBits = 8;
//    _bufferSize = 1000;
//    _buffer = new char[_bufferSize];
//    _temp = new char [_bufferSize];
//    //_parity = PARITY_8N1;
//    _bufferIndex = 0;
//    //_blocking = 0;
//}

/**
 * 
 * @param dev
 * @param baud
 * @param parity
 * @param blocking
 */
Serial::Serial(char* dev, int baud, int parity, int blocking)
{
    _dev = dev;
    _baud = baud;
    _parity = parity;
    _blocking = blocking;
    _dataBits = 8;
    _bufferSize = 1000;
    _buffer = new char[_bufferSize];
    _temp = new char [_bufferSize];
    _bufferIndex = 0;
}

/**
 * 
 */
Serial::~Serial()
{
    closePort();
}

/**
 * 
 * @return 
 */
int Serial::initPort()
{
    cout << "SERIAL: Opening " << _dev << " at " << _baud << " baud..." << endl;
    _fd = open(_dev, O_RDWR | O_NOCTTY | O_NDELAY);
    
    if (_fd != -1)
    {
        cout << "OK" << endl;
        
        if (_blocking == 1)
            cout << "SERIAL: Blocking enabled" << endl;
        else
            cout << "SERIAL: Blocking disabled" << endl;
        
        fcntl(_fd, F_SETFL, FNDELAY);
        struct termios options;
        tcgetattr (_fd, &options);
        
        switch (_baud)
        {
            case 4800:
                cfsetispeed(&options, B4800);
                cfsetospeed(&options, B4800);
                break;
            case 9600:
                cfsetispeed(&options, B9600);
                cfsetospeed(&options, B9600);
                break;
            case 38400:
                cfsetispeed(&options, B38400);
                cfsetospeed(&options, B38400);
                break;
            case 57600:
                cfsetispeed(&options, B57600);
                cfsetospeed(&options, B57600);
                break;
            case 115200:
                cfsetispeed(&options, B115200);
                cfsetospeed(&options, B115200);
                break;
            default:
                cfsetispeed(&options, B9600);
                cfsetospeed(&options, B9600);
                break;
        }
        
        options.c_cflag |= (CLOCAL | CREAD);
        
        switch (_parity)
        {
            case PARITY_7E1:
                options.c_cflag |= PARENB;
                options.c_cflag &= ~PARODD;
                cout << "SERIAL: Parity set to 7E1" << endl;
                break;
            case PARITY_8N1:
                options.c_cflag |= CS8;
                cout << "SERIAL: Parity set to 8N1" << endl;
                break;
            default:
                options.c_cflag |= CS8;
                cout << "SERIAL: No parity set" << endl;
                break;
        }
        
        options.c_cflag &= ~CRTSCTS;
        options.c_lflag &= ~( ICANON | ECHO | ECHOE | ISIG);
        
        tcsetattr(_fd, TCSANOW, &options);
    }
    else
    {
        cout << "FAIL" << endl;
        cerr << "Error opening port: ";
        closePort();
    }
    
    return _fd;
}

/**
 * 
 */
void Serial::flushPort()
{
    if (_fd != -1) ioctl(_fd, TCIOFLUSH, 2);
}

/**
 * 
 * @param data
 * @return 
 */
int Serial::getData(char* data)
{
    char *tmp;
    char c;
    int i = 0;
    
    tmp = (char*)malloc(sizeof(char)); //Allocating memory for at least 2 bytes.
    while ((c = this->getChar()) != '\0') {
        tmp[i++] = c; //Assign char read to i position in array and then increment i by one.
        tmp = (char*)realloc(tmp, (i+1)*sizeof(char));
    }
    tmp[i] = '\0'; //Required to make data a string.
    memcpy(data, tmp, i); //transfer the content of tmp to data.
    return i;
    
//    if (_fd != -1)
//    {
//        //cout << "getData received " << sizeof(data) << " bytes to work" << endl;
//        int a = read(_fd, data, 100);
//        cout << "Reading " << a << " bytes from " << this->_dev << endl;
//        return a;
//    }
//    else
//        return -1;
}

/**
 * 
 * @param data
 * @return 
 */
int Serial::sendData(char* data)
{
    int result;
    for (int i = 0; i < strlen(data); i++) {
        result = this->sendChar(data[i]);
    }
    return result;
//    if (_fd != -1)
//    {
//        cout << "Writing " << strlen(data) << " bytes to " << this->_dev << endl;
//        return write(_fd, data, strlen(data));
//    }
//    else
//        return -1;
}

/**
 * 
 * @return 
 */
char Serial::getChar()
{
    //int delay = 0;
    char c = -1;
    int n = -1;
    
    if (_fd != -1)
    {
        n = read (_fd, &c, 1);
        if (c >= 32 && c <= 126)
            return c;
        else
            return -1;
//        do
//        {
//            n = read(_fd, &c, 1);
//            if (n < 1)
//            {
//                usleep(10);
//                delay++;
//            }
//        } while (n < 1 || delay < 5);
    }
    return c;
}

/**
 * 
 * @param data
 * @return 
 */
int Serial::sendChar(char data)
{
    if (_fd != -1) {
        write(_fd, &data, 1);
        usleep(2500);
        return 1;
    } else return -1;
}

/**
 * 
 */
void Serial::closePort()
{
    if (_fd != -1)
    {
        close(_fd);
        cout << "SERIAL: Device " << _dev << " is now closed." << endl;
    }
}

int Serial::getBaud() const {
    return _baud;
}

void Serial::setBaud(int baud) {
    this->_baud = baud;
}

int Serial::getBlocking() const {
    return _blocking;
}

void Serial::setBlocking(int blocking) {
    this->_blocking = blocking;
}

char* Serial::getDev() const {
    return _dev;
}

void Serial::setDev(char* dev) {
    this->_dev = dev;
}

int Serial::getParity() const {
    return _parity;
}

void Serial::setParity(int parity) {
    this->_parity = parity;
}
