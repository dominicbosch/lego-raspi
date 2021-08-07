// Include the AM2315 library code
#include <Wire.h>

#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

#define SENSOR1_Interrupt 2
#define SENSOR2_Interrupt 3

#define address 0x05          //I2c Address

String outStr = "";

long inVal = 0;               // Value to be captured from the I2c bus

const long interval = 5000;
unsigned long previousMillis = millis();

// volatile long readMillis = 0;
volatile long readMicros1 = 0;
volatile long readMicros2 = 0;

volatile long previousMicros1 = millis();
volatile long actualMicros1 = millis();

volatile long previousMicros2 = millis();
volatile long actualMicros2 = millis();

void setup() {
  // Initialize Motor Pin
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);

  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);

  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(SENSOR1_Interrupt), Interrupt1_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR2_Interrupt), Interrupt2_ISR, RISING);
  Serial.begin(9600);

  Wire.begin(address);

  Wire.onReceive(dataReceived);
  Wire.onRequest(sendData);
  
  Serial.println("Application Ready");
  
  spin_and_wait(0, 0);
  
}

void loop() {

  report_Timer();

}

void Interrupt1_ISR(){
  readMicros1 = micros();
  actualMicros1 = previousMicros1 - readMicros1;
  previousMicros1 = readMicros1;
}

void Interrupt2_ISR(){
  readMicros2 = micros();
  actualMicros2 = previousMicros2 - readMicros2;
  previousMicros2 = readMicros2;
}

void report_Timer(){

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
    
      outStr = "Time in Microseconds : " + String(actualMicros1);
      outStr = outStr + " - " + String(actualMicros2);

      Serial.println(outStr);

      outStr = "Difference : " + String(readMicros1 - readMicros2);
      Serial.println(outStr);

      Serial.println();
      
    }

}

void set_motor_pwm(int pwm, int IN1_PIN, int IN2_PIN){
  if(pwm < 0) { // reverse speeds
    analogWrite(IN1_PIN, -pwm);
    digitalWrite(IN2_PIN, LOW);
  }
  else {  // stop or forward
    digitalWrite(IN1_PIN, LOW);
    analogWrite(IN2_PIN, pwm);
  }
}

void set_motor_currents(int pwm_A){
  set_motor_pwm(pwm_A, MOT_A1_PIN, MOT_A2_PIN);

  Serial.print("Set motor A PWM = ");
  Serial.println(pwm_A);
}

void spin_and_wait(int pwm_A, int duration){
  set_motor_currents(pwm_A);
//  delay(duration);
}

void dataReceived(int byteCount) {
  while (Wire.available()) {
    inVal = Wire.read();
    Serial.print("Value captured: ");
    Serial.println(inVal);
  }
  spin_and_wait(inVal, 0);
  
}

void sendData() {
  Wire.write(inVal);
}
