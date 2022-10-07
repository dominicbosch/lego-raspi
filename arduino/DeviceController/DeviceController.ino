// Include the AM2315 library code
#include <Wire.h>

// Include NXTMotor Controller
#include "NXTMotor.h"

#define address 0x05          //I2c Address

long inVal = 0;               // Value to be captured from the I2c bus

const long interval = 5000;
unsigned long previousMillis = millis();

// Current limitation of this code
// Define Motor A with motion pins 5 & 6 and sensor pins 2 & 3
NXTMotor* motorA;
NXTMotor* motorB;
NXTMotor* motorC;
//(5, 6, 2, 3);

void setup() {
    Serial.begin(9600);

    Wire.begin(address);
    Wire.onReceive(dataReceived);
    Wire.onRequest(sendData);

    Serial.println("Application Ready");

    motorA = new NXTMotor(5, 6, 2, 3);
    motorA->printConfiguration();
    // Initialize Sensor Interrupts. Attach an interrupt to the ISR vector
    attachInterrupt(digitalPinToInterrupt(motorA->getSensorAPin()), triggerMotorASensorA_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(motorA->getSensorBPin()), triggerMotorASensorB_ISR, RISING);
    }

void loop() {
    report_Timer();
}

void report_Timer()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        motorA->printTimer();
    }
}


void dataReceived(int byteCount) {
    Serial.print("Receiving Bytes: ");
    Serial.println(byteCount);
    while (Wire.available()) {
        int byte = Wire.read();
        Serial.print("byte captured: ");
        Serial.println(byte);
    }

}

void sendData() {
    Wire.write(inVal);
}

/*
    Static functions for the interrupt handling of all motors
*/
void triggerMotorASensorA_ISR() { motorA->triggerSensorA_ISR(); }
void triggerMotorASensorB_ISR() { motorA->triggerSensorB_ISR(); }
void triggerMotorBSensorA_ISR() { motorB->triggerSensorA_ISR(); }
void triggerMotorBSensorB_ISR() { motorB->triggerSensorB_ISR(); }
void triggerMotorCSensorA_ISR() { motorC->triggerSensorA_ISR(); }
void triggerMotorCSensorB_ISR() { motorC->triggerSensorB_ISR(); }
