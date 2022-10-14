/*
    NXTMotor.cpp - Library to control the NXT Motor
*/

#include "Arduino.h"
#include "NXTMotor.h"

NXTMotor::NXTMotor(int pinMotionA, int pinMotionB, int pinSensorInterruptA, int pinSensorInterruptB) {
    _pinMotionA = pinMotionA;
    _pinMotionB = pinMotionB;
    _pinSensorInterruptA = pinSensorInterruptA;
    _pinSensorInterruptB = pinSensorInterruptB;

    _currentDirection = 0;
    _currentPosition = 0;
    _currentRotateToPosition = 0;
    _rotateToAngle = -1;
    _rotateToAngleForward = true;

    // Initialize Motor Pins
    pinMode(_pinMotionA, OUTPUT);
    pinMode(_pinMotionB, OUTPUT);
    digitalWrite(_pinMotionA, LOW);
    digitalWrite(_pinMotionB, LOW);

    // Initialize timer variables for each sensor
    _lastSensorATrigger = millis();
    _deltaToLastSensorATrigger = millis();
    _lastSensorBTrigger = millis();
    _deltaToLastSensorBTrigger = millis();
}


/*
    PUBLIC Functions
*/

int NXTMotor::getSensorAPin() {
    return _pinSensorInterruptA;
}

int NXTMotor::getSensorBPin() {
    return _pinSensorInterruptB;
}

int NXTMotor::getAngle() {
    return _currentPosition;
}

// called by the loop method to give room for any calculation
void NXTMotor::update() {
    // If we are executing the move to angle command, things get a bit complicated with 
    // acceleration and deceleration and so on
    if (_rotateToAngle > 0) {
        if(_rotateToAngleForward) _setForwardSpeed(255);
        else _setBackwardSpeed(255);
    }
}

void NXTMotor::setForwardSpeed(int speed) {
    _rotateToAngle = -1; // stop any pending rotate to command
    _setForwardSpeed(speed);
}

// Speed = pwm
void NXTMotor::_setForwardSpeed(int speed) {
    if (speed > 0) _currentDirection = 1;
    else _currentDirection = 0;
    analogWrite(_pinMotionA, speed);
    digitalWrite(_pinMotionB, LOW);
}

void NXTMotor::setBackwardSpeed(int speed) {
    _rotateToAngle = -1; // stop any pending rotate to command
    _setBackwardSpeed(speed);
}

void NXTMotor::_setBackwardSpeed(int speed) {
    if (speed > 0) _currentDirection = -1;
    else _currentDirection = 0;
    analogWrite(_pinMotionA, LOW);
    digitalWrite(_pinMotionB, -speed);
}

void NXTMotor::stop(bool applyBreaks) {
    _rotateToAngle = -1;
    _currentDirection = 0;
    if (applyBreaks) {
        digitalWrite(_pinMotionA, HIGH);
        digitalWrite(_pinMotionB, HIGH);
    } else {
        digitalWrite(_pinMotionA, LOW);
        digitalWrite(_pinMotionB, LOW);
    }    
}

void NXTMotor::rotateTo(int angle, bool forward) {
    _currentRotateToPosition = 0;
    _rotateToAngle = angle;
    _rotateToAngleForward = forward;
}

void NXTMotor::triggerSensorA_ISR() {
    if (_currentDirection > 0) {
        _currentPosition++;
    } else if (_currentDirection < 0) {
        _currentPosition--;
    }
    // we do not update the position if somebody moves the motor by hand
    // this would require a bit more logic to identify direction through
    // the sensors. I read somewhere how this is possible but can't recall
    // it right now, therefore this is a TODO

    long now = micros();
    _deltaToLastSensorATrigger = _lastSensorATrigger - now;
    _lastSensorATrigger = now;
}

void NXTMotor::triggerSensorB_ISR() {
    if (_currentDirection > 0) {
        _currentPosition++;
    } else if (_currentDirection < 0) {
        _currentPosition--;
    }
    // we do not update the position if somebody moves the motor by hand
    // this would require a bit more logic to identify direction through
    // the sensors. I read somewhere how this is possible but can't recall
    // it right now, therefore this is a TODO

    long now = micros();
    _deltaToLastSensorBTrigger = _lastSensorBTrigger - now;
    _lastSensorBTrigger = now;
}

void NXTMotor::printConfiguration() {
    Serial.println("Initializing NXTMotor: ");
    Serial.print("pinMotionA: ");
    Serial.println(_pinMotionA);
    Serial.print("pinMotionB: ");
    Serial.println(_pinMotionB);
    Serial.print("pinSensorInterruptA: ");
    Serial.println(_pinSensorInterruptA);
    Serial.print("pinSensorInterruptB: ");
    Serial.println(_pinSensorInterruptB);
}

void NXTMotor::printTimer() {
    String outStr = "Time in Microseconds : " + String(_deltaToLastSensorATrigger);
    outStr = outStr + " - " + String(_deltaToLastSensorBTrigger);

    Serial.println(outStr);

    outStr = "Difference : " + String(_lastSensorATrigger - _lastSensorBTrigger);
    Serial.println(outStr);

    Serial.println();

}

/*
    PRIVATE Functions
*/

