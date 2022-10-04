#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x05
#define PAYLOAD_SIZE 2

String inString =  "                ";
//                  123456789012345
String outString = "                ";

void setup() {
  Serial.begin(9600);
  Wire.begin(I2C_SLAVE_ADDRESS);

  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);

  Serial.println("Ready Slave 1");              
}

void loop() {}

void requestEvents()
{
  Serial.println("---> recieved request from Raspi");
  Serial.println("Sending : " + outString);
  int strLen = outString.length();
//  Serial.println(strLen);
  for (int i = 1; i <= strLen; i++){
    Wire.write(outString[i]);
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvents(int howMany)
{
  int i = 0;
  while(Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
//    i++;
    inString[i++] = c;
//    Serial.println(c);         // print the character
  }
  Serial.println(inString);
  for (int i = 1; i<= inString.length(); i++){
    outString[i] = inString[i];
  }
//  Serial.println("Number : " + String(howMany));

}

