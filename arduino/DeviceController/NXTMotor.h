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
        int getCurrentRPM();

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
        int _currentMeasuredSpeed;
        int _lastSpeedCountPosition;
        int _currentRotateToPosition;
        int _rotateToAngle;
        bool _rotateToAngleForward;

        // we would need to attachInterrupts both for RISING & FALLING in order
        // to find out the direction of the movement. For now we just use what
        // we got as command and set the pins for to keep things simple
        // volatile long _lastSensorATrigger;
        // volatile long _lastSensorBTrigger;
        // volatile long _deltaToLastSensorATrigger;
        // volatile long _deltaToLastSensorBTrigger;

        void _setForwardSpeed(int speed);
        void _setBackwardSpeed(int speed);
};

#endif