// Include the AM2315 library code
#include <Wire.h>

// Include NXTMotor Controller
#include "NXTMotor.h"
// Include CommandProcessor
#include "Devices.h"

#define BAUD_RATE 115200            // should be fast enough and still allow serial to work
#define I2C_ADDRESS 0x05            // I2C Address
#define STATIC_REPLY_LENGTH 16      // comm protocol: slave always sends 16 bytes after each command received
#define PRINTERVAL 5000             // The print interval

// initialize the reply array that needs to be filled before the master requests the answer
char arrReplyToMaster[STATIC_REPLY_LENGTH];
unsigned long previousMillis = millis();

Devices* devices = new Devices();

void setup() {
    Serial.begin(BAUD_RATE);

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
    int arrBytes[byteCount];
    while (Wire.available()) {
        arrBytes[i++] = Wire.read();
    }
    processCommand(byteCount, arrBytes);    
}

// We try to make sense of what the master sent us and pipe the command
// into the correct channels (currently either device initialisation or control)
void processCommand(int byteCount, int arrBytes[]) {
    // Switch on Scenario
    switch(arrBytes[0]) {
        case 0: // Device Initialisation
            if (arrBytes[1] == 0) { // NXTMotor
                initializeNXTMotor(byteCount, arrBytes);
            } else setErrorReply(25);
            break;

        case 1: // Device Control
            devices->processCommandDeviceControl(byteCount, arrBytes);
            break;
    }
}

// Currently there are three available slots hard wired for NXTMotors
void initializeNXTMotor(int byteCount, int arrBytes[]) {
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
        setConfirmReply(byteCount, arrBytes);
    }
    else setErrorReply(30);
}

void setConfirmReply(int byteCount, int arrBytes[]) {
    // Clear the reply array
    // memset(arrReplyToMaster, 0, STATIC_REPLY_LENGTH);
    arrReplyToMaster[0] = arrBytes[0];
    arrReplyToMaster[1] = arrBytes[1];
    arrReplyToMaster[2] = arrBytes[2];
    arrReplyToMaster[3] = arrBytes[3];
    arrReplyToMaster[4] = arrBytes[4];
    arrReplyToMaster[5] = arrBytes[5];
    // for(int i = 0; i < byteCount; i++) {
    //     arrReplyToMaster[i] = arrBytes[i];
    // }
}

void setErrorReply(int errCode) {
    arrReplyToMaster[0] = 255;
    arrReplyToMaster[1] = errCode;
}

void sendData() {
    Wire.write(arrReplyToMaster);
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
