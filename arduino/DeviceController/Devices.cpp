/*
    Devices.cpp - Class to handle devices
*/

#include "Arduino.h"
#include "Devices.h"
// Include NXTMotor Controller
#include "NXTMotor.h"

Devices::Devices() {}

void Devices::processCommandDeviceControl(int byteCount, int arrBytes[]) {
    // DeviceID
    if (arrBytes[1] == 0) {
        // 0 = set speed
        if (arrBytes[2] == 0) {
            motorA->setSpeed(arrBytes[3]);
        }
    }
}

