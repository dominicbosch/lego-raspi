/*
    NXTMotor.h - Library to control the NXT Motor
*/

#ifndef NXTMotor_h
#define NXTMotor_h

#include <Arduino.h>

class NXTMotor {
    public:
        NXTMotor(int pinMotionA, int pinMotionB, int pinSensorA, int pinSensorB);

        int getSensorAPin();
        int getSensorBPin();

        void setForwardSpeed(int speed);
        void setBackwardSpeed(int speed);
        void setBreaks(int yes);

        void triggerSensorA_ISR();
        void triggerSensorB_ISR();

        void printConfiguration();
        void printTimer();
        
    private:
        int _pinMotionA;
        int _pinMotionB;
        int _pinSensorInterruptA;
        int _pinSensorInterruptB;

        volatile long _lastSensorATrigger;
        volatile long _lastSensorBTrigger;
        volatile long _deltaToLastSensorATrigger;
        volatile long _deltaToLastSensorBTrigger;
};

#endif