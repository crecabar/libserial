/* 
 * File:   main.cpp
 * Author: crecabar
 *
 * Created on April 29, 2015, 10:15 AM
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <Serial.h>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    Serial* arduino;
    char port[] = "/dev/cu.usbmodem1451";
    char* line = new char[100];
    char* buffer = new char[100];
    char data = 'A';
    
    arduino = new Serial(port, 9600, PARITY_8N1, 0);
    int result = arduino->initPort();
    usleep(10000);
    
    cout << "Init returned: " << result << endl;
    
    cout << "Write on terminal, (.) dot for end. " << endl;
    while(data != '.') {
        cin >> data;
        buffer[0] = data;
        arduino->sendData(buffer);
        //arduino->getData(line);
        //cout << line << endl;
    }
    
    arduino->closePort();
    
    return 0;
}

