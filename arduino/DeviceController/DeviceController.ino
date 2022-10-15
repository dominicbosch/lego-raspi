// Include the AM2315 library code
#include <Wire.h>

#include "NXTMotor.h"
#include "Devices.h"
#include "ReplyHandler.h"
#include "config.h"

ReplyHandler* reply = new ReplyHandler(STATIC_MSG_LENGTH);
Devices* devices = new Devices(reply);
unsigned char arrReceivedBytes[STATIC_MSG_LENGTH]; // comm protocol expects all 0 - 255 bytes

unsigned long previousDeviceMillis = millis();
unsigned long previousPrintMillis = millis();

void setup() {
    Serial.begin(BAUD_RATE);
    while (!Serial) {} // wait for serial port to connect.

    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(dataReceived);
    Wire.onRequest(sendData);

    Serial.println("Application Ready");
}

void loop() {
    updateDevices();
    printReport();
}

void updateDevices() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousDeviceMillis >= INTERVAL_DEVICE_UPDATE) {
        previousDeviceMillis = currentMillis;
        if(devices->motorA != NULL) devices->motorA->update();
        if(devices->motorB != NULL) devices->motorB->update();
        if(devices->motorC != NULL) devices->motorC->update();
    }
}

void printReport() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousPrintMillis >= INTERVAL_PRINT) {
        previousPrintMillis = currentMillis;
        if(devices->motorA != NULL) {
            Serial.println("MotorA configured");
            Serial.print("MotorA angle:");
            Serial.println(devices->motorA->getAngle());
            Serial.print("MotorA RPM:");
            Serial.println(devices->motorA->getCurrentRPM());
        } else {
            Serial.println("MotorA not configured");
        }
        if(devices->motorB != NULL) {
            Serial.println("MotorB configured");
        } else {
            Serial.println("MotorB not configured");
        }
        if(devices->motorC != NULL) {
            Serial.println("MotorC configured");
        } else {
            Serial.println("MotorC not configured");
        }
        Serial.println("\n----------------\n");
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
void processCommand(unsigned char arrBytes[], int byteCount) {
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
        
        default:
            reply->setErrorReply(0);
    }
}

// Currently there are three available slots hard wired for NXTMotors
void initializeNXTMotor(unsigned char arrBytes[], int byteCount) {
    if (arrBytes[2] == 0 || arrBytes[2] == 1 || arrBytes[2] == 2) {
        if (byteCount < 6) {
            reply->setErrorReply(1);
            return;
        }
        NXTMotor* mot = new NXTMotor(arrBytes[3], arrBytes[4], arrBytes[5], arrBytes[6]);
        if (arrBytes[2] == 0) {
            devices->motorA = mot;
            attachInterrupt(digitalPinToInterrupt(devices->motorA->getSensorAPin()), triggerMotorASensorA_ISR, CHANGE);
            attachInterrupt(digitalPinToInterrupt(devices->motorA->getSensorBPin()), triggerMotorASensorB_ISR, CHANGE);
        }
        else if (arrBytes[2] == 1) {
            devices->motorB = mot;
            attachInterrupt(digitalPinToInterrupt(devices->motorB->getSensorAPin()), triggerMotorBSensorA_ISR, CHANGE);
            attachInterrupt(digitalPinToInterrupt(devices->motorB->getSensorBPin()), triggerMotorBSensorB_ISR, CHANGE);
        }
        else if (arrBytes[2] == 2) {
            devices->motorC = mot;
            attachInterrupt(digitalPinToInterrupt(devices->motorC->getSensorAPin()), triggerMotorCSensorA_ISR, CHANGE);
            attachInterrupt(digitalPinToInterrupt(devices->motorC->getSensorBPin()), triggerMotorCSensorB_ISR, CHANGE);
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
