// Include the AM2315 library code
// #include <Wire.h>

#define MOT_A1_PIN 5
#define MOT_A2_PIN 6

const int motor1Pin = 2; // Pulse 1 from motor
const long interval = 1000;           // output interval

String outStr = "";

unsigned long previousMillis = 0;        // will store last time count was printed
unsigned long previousPulseCount1 = 0;
unsigned long actualPulseCount1 = 0;
unsigned long previousPulseCount2 = 0;
unsigned long actualPulseCount2 = 0;
unsigned long currentMillis = millis();

volatile long pulseCounter1 = 0;
volatile long pulseCounter2 = 0;

void setup() {
  // Initialize Motor Pin
  pinMode(MOT_A1_PIN, OUTPUT);
  pinMode(MOT_A2_PIN, OUTPUT);

  digitalWrite(MOT_A1_PIN, LOW);
  digitalWrite(MOT_A2_PIN, LOW);

  // Attach an interrupt to the ISR vector
  attachInterrupt(digitalPinToInterrupt(2), pin2_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(3), pin3_ISR, RISING);
  Serial.begin(9600);
  Serial.println("Application Start");
}

void loop() {

  // Ramp speed up
  for (int i = 0; i< 11; i++){
    spin_and_wait(25*i, 500);
    report_PulseCount();
  }

  // Full speed forward
  spin_and_wait(255, 2000);
  report_PulseCount();

  // Ramp speed into full reverse
  for (int i = 0; i < 21; i++){
      spin_and_wait(255 - 25 * i, 500);
      report_PulseCount();
  }

  // Full speed reverse
  spin_and_wait(-255 , 2000);
  report_PulseCount();

  // Stop
  spin_and_wait(0, 2000);
  report_PulseCount();
}

void pin2_ISR(){
  pulseCounter1 = pulseCounter1 +1;
}

void pin3_ISR(){
  pulseCounter2 = pulseCounter2 +1;
}

void report_PulseCount(){

//  if (currentMillis - previousMillis >= interval) {
//    previousMillis = currentMillis;
    
    actualPulseCount1 = pulseCounter1-previousPulseCount1;
    previousPulseCount1 = pulseCounter1;

    actualPulseCount2 = pulseCounter2-previousPulseCount2;
    previousPulseCount2 = pulseCounter2;

    outStr = String(actualPulseCount1);
    outStr = outStr + " - " + String(actualPulseCount2);
    
    Serial.println(outStr);
//  }
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
  delay(duration);
}
