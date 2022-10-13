// Include the AM2315 library code
#include <Wire.h>

#include "NXTMotor.h"
#include "Devices.h"
#include "ReplyHandler.h"

#define BAUD_RATE 115200            // should be fast enough and still allow serial to work
#define I2C_ADDRESS 0x20            // I2C Address
#define STATIC_MSG_LENGTH 16        // comm protocol: 16 bytes exchanged back and forth
#define PRINTERVAL 5000             // The print interval

ReplyHandler* reply = new ReplyHandler(STATIC_MSG_LENGTH);
Devices* devices = new Devices(reply);
char arrReceivedBytes[STATIC_MSG_LENGTH];

unsigned long previousMillis = millis();

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial) {} // wait for serial port to connect.

    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(dataReceived);
    Wire.onRequest(sendData);

    Serial.println("Application Ready");
}

void loop() {
    report_Timer();
}

void report_Timer() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= PRINTERVAL) {
        previousMillis = currentMillis;
        // motorA->printTimer();
    }
}

// We gladly receive some bytes from the master (dynamic length).
// while he expects us to reply with 16 bytes later on
void dataReceived(int byteCount) {
    int i = 0;
    while (Wire.available()) {
        arrReceivedBytes[i++] = Wire.read();
    }
    processCommand(arrReceivedBytes, byteCount);
}

// We try to make sense of what the master sent us and pipe the command
// into the correct channels (currently either device initialisation or control)
void processCommand(char arrBytes[], int byteCount) {
    if (byteCount < 4) return;

    // Switch on Scenario
    switch(arrBytes[0]) {
        case 0: // Device Initialisation
            if (arrBytes[1] == 0) { // NXTMotor
                initializeNXTMotor(arrBytes, byteCount);
            } else {
                reply->setErrorReply(25);
            }
            break;

        case 1: // Device Control
            devices->processCommandDeviceControl(arrBytes, byteCount);
            break;
    }
}

// Currently there are three available slots hard wired for NXTMotors
void initializeNXTMotor(char arrBytes[], int byteCount) {
    if (arrBytes[2] == 0 || arrBytes[2] == 1 || arrBytes[2] == 2) {
        NXTMotor* mot = new NXTMotor(arrBytes[3], arrBytes[4], arrBytes[5], arrBytes[6]);
        if (arrBytes[2] == 0) {
            devices->motorA = mot;
            attachInterrupt(digitalPinToInterrupt(devices->motorA->getSensorAPin()), triggerMotorASensorA_ISR, RISING);
            attachInterrupt(digitalPinToInterrupt(devices->motorA->getSensorBPin()), triggerMotorASensorB_ISR, RISING);
        }
        else if (arrBytes[2] == 1) {
            devices->motorB = mot;
            attachInterrupt(digitalPinToInterrupt(devices->motorB->getSensorAPin()), triggerMotorBSensorA_ISR, RISING);
            attachInterrupt(digitalPinToInterrupt(devices->motorB->getSensorBPin()), triggerMotorBSensorB_ISR, RISING);
        }
        else if (arrBytes[2] == 2) {
            devices->motorC = mot;
            attachInterrupt(digitalPinToInterrupt(devices->motorC->getSensorAPin()), triggerMotorCSensorA_ISR, RISING);
            attachInterrupt(digitalPinToInterrupt(devices->motorC->getSensorBPin()), triggerMotorCSensorB_ISR, RISING);
        }
        reply->setConfirmReply(arrBytes, byteCount);
    }
    else reply->setErrorReply(30);
}

void sendData() {
    Wire.write(reply->getReply(), STATIC_MSG_LENGTH);
}

/*
    Static functions for the interrupt handling of all motors.
    So called glue functions...
*/
void triggerMotorASensorA_ISR() { devices->motorA->triggerSensorA_ISR(); }
void triggerMotorASensorB_ISR() { devices->motorA->triggerSensorB_ISR(); }
void triggerMotorBSensorA_ISR() { devices->motorB->triggerSensorA_ISR(); }
void triggerMotorBSensorB_ISR() { devices->motorB->triggerSensorB_ISR(); }
void triggerMotorCSensorA_ISR() { devices->motorC->triggerSensorA_ISR(); }
void triggerMotorCSensorB_ISR() { devices->motorC->triggerSensorB_ISR(); }
