/*
    Devices.h - Class to handle devices
*/

#ifndef Devices_h
#define Devices_h

#include <Arduino.h>
// Include NXTMotor Controller
#include "NXTMotor.h"

class Devices {
    public:
        Devices();
        // Current limitation of this code: hard wired motors
        NXTMotor* motorA;
        NXTMotor* motorB;
        NXTMotor* motorC;
        void processCommandDeviceControl(int arrBytes[]);

    private:
};

#endif