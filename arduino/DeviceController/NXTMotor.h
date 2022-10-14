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
        int getAngle();

        void update();
        void setForwardSpeed(int speed);
        void setBackwardSpeed(int speed);
        void stop(bool applyBreaks);
        void rotateTo(int angle, bool forward);

        void triggerSensorA_ISR();
        void triggerSensorB_ISR();

        void printConfiguration();
        void printTimer();
        
    private:
        int _pinMotionA;
        int _pinMotionB;
        int _pinSensorInterruptA;
        int _pinSensorInterruptB;
        int _currentDirection;
        int _currentPosition;
        int _currentRotateToPosition;
        int _rotateToAngle;
        bool _rotateToAngleForward;

        volatile long _lastSensorATrigger;
        volatile long _lastSensorBTrigger;
        volatile long _deltaToLastSensorATrigger;
        volatile long _deltaToLastSensorBTrigger;

        void _setForwardSpeed(int speed);
        void _setBackwardSpeed(int speed);
};

#endif