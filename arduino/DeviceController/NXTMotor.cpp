/*
    NXTMotor.cpp - Library to control the NXT Motor
*/

#include "Arduino.h"
#include "NXTMotor.h"

NXTMotor::NXTMotor(int pinMotionA, int pinMotionB, int pinSensorInterruptA, int pinSensorInterruptB)
{
    _pinMotionA = pinMotionA;
    _pinMotionB = pinMotionB;
    _pinSensorInterruptA = pinSensorInterruptA;
    _pinSensorInterruptB = pinSensorInterruptB;

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

int NXTMotor::getSensorAPin()
{
    return _pinSensorInterruptA;
}

int NXTMotor::getSensorBPin()
{
    return _pinSensorInterruptB;
}

// Speed = pwm
void NXTMotor::setSpeed(int speed)
{
    if(speed < 0) { // reverse speeds
        analogWrite(_pinMotionA, -speed);
        digitalWrite(_pinMotionB, LOW);
    } else {  // stop or forward
        digitalWrite(_pinMotionA, LOW);
        analogWrite(_pinMotionB, speed);
    }
}

void NXTMotor::triggerSensorA_ISR()
{
    long now = micros();
    _deltaToLastSensorATrigger = _lastSensorATrigger - now;
    _lastSensorATrigger = now;
}

void NXTMotor::triggerSensorB_ISR()
{
    long now = micros();
    _deltaToLastSensorBTrigger = _lastSensorBTrigger - now;
    _lastSensorBTrigger = now;
}

void NXTMotor::printConfiguration()
{
    Serial.println("Initializing NXTMotor: ");
    Serial.print("pinMotionA: ");
    Serial.println(_pinMotionA);
    Serial.print("pinMotionB: ");
    Serial.println(_pinMotionB);
    Serial.print("pinSensorA: ");
    Serial.println(_pinSensorInterruptA);
    Serial.print("pinSensorB: ");
    Serial.println(_pinSensorInterruptB);
}
void NXTMotor::printTimer()
{
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

