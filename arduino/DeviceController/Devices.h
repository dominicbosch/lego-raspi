/*
    Devices.h - Class to handle devices
*/

#ifndef Devices_h
#define Devices_h

#include <Arduino.h>
// Include NXTMotor Controller
#include "NXTMotor.h"
#include "ReplyHandler.h"

class Devices {
    public:
        Devices(ReplyHandler* reply);
        // Current limitation of this code: hard wired motors
        NXTMotor* motorA = NULL;
        NXTMotor* motorB = NULL;
        NXTMotor* motorC = NULL;
        void processCommandDeviceControl(char arrBytes[], int byteCount);

    private:
        ReplyHandler* _reply;
};

#endif