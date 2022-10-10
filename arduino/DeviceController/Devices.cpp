/*
    Devices.cpp - Class to handle devices
*/

#include "Arduino.h"
#include "Devices.h"
// Include NXTMotor Controller
#include "NXTMotor.h"
#include "ReplyHandler.h"

Devices::Devices(ReplyHandler* reply) {
    _reply = reply;
}

void Devices::processCommandDeviceControl(char arrBytes[], int byteCount) {
    // DeviceID
    if (arrBytes[1] == 0 && !this->motorA ||
        arrBytes[1] == 1 && !this->motorB ||
        arrBytes[1] == 2 && !this->motorC) {
            int arr[1] = {arrBytes[1]};
            _reply->setErrorReply(31, arr, 1);
    } else if (arrBytes[1] == 0 || arrBytes[1] == 1 || arrBytes[1] == 2) {
        // DeviceIDs 0, 1 & 2 point to NXTMotors
        NXTMotor* mot;
        if (arrBytes[1] == 0)  mot = this->motorA;
        else if (arrBytes[1] == 1)  mot = this->motorB;
        else if (arrBytes[1] == 2)  mot = this->motorC;
        
        switch(arrBytes[2]) {
            case 0: // set speed forward
                mot->setForwardSpeed(arrBytes[3]);
                break;
            case 1: // set speed backward
                mot->setBackwardSpeed(arrBytes[3]);
                break;
            case 2: // set breaks
                mot->setBreaks(arrBytes[3]);
                break;
        }
    }
}

