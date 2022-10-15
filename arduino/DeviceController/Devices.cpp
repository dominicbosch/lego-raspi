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

void Devices::processCommandDeviceControl(unsigned char arrBytes[], int byteCount) {
    // check DeviceID
    if (arrBytes[1] == 0 && this->motorA == NULL ||
        arrBytes[1] == 1 && this->motorB == NULL ||
        arrBytes[1] == 2 && this->motorC == NULL) {
            // If device is selected but not initialized, we throw an error
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
            case 2: // stop with or without breaks
                mot->stop(arrBytes[3]);
                break;
            case 3: // rotate to angle
                if (byteCount < 6) _reply->setErrorReply(35);
                else {
                    // combine 4th and 5th byte into one angle integer
                    // we need more than 255 to designate the rotate to angle
                    // since circles have 360 degrees, really
                    int angle = (arrBytes[3] << 8) | arrBytes[4];
                    mot->rotateTo(angle, arrBytes[5]);
                }
                break;
            default:
                _reply->setErrorReply(0);
        }
        _reply->setConfirmReply(arrBytes, byteCount);
    }
}

