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

void Devices::processCommandDeviceControl(int arrBytes[], int byteCount) {
    // DeviceID
    if (arrBytes[1] == 0 && !this->motorA ||
        arrBytes[1] == 1 && !this->motorB ||
        arrBytes[1] == 2 && !this->motorC) {
            int arr[1] = {arrBytes[1]};
            _reply->setErrorReply(31, arr, 1);
    } else {
        NXTMotor* mot;
        if (arrBytes[1] == 0)  mot = this->motorA;
        else if (arrBytes[1] == 1)  mot = this->motorB;
        else if (arrBytes[1] == 2)  mot = this->motorC;

        // 0 = set speed forward
        
        if (arrBytes[2] == 0) {
            mot->setSpeed(arrBytes[3]);
        }
    }
}

