#include <SparkFun_RFD77402_Arduino_Library.h> //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library

#define MUX_ADDR 0x70 //7-bit unshifted default I2C Address
#define TOF1_PORT 3 // The Mux port that the 1st TOF is on
#define TOF2_PORT 4

#define ACTUATOR_PIN 5

RFD77402 myDistance; //Hook object to the library

unsigned int pos = 100; // Start actuator at the middle

void setup() {
  Serial.begin(9600);
  // TOF setup

  analogWrite(ACTUATOR_PIN, pos);

  delay(1000);
}

void loop() {

  analogWrite(ACTUATOR_PIN, 200);
  delay(5000);
  analogWrite(ACTUATOR_PIN, 25);
  delay(5000);

//  if (distanceX < 200) {
//    pos -= 5;
//  } else {
//    pos += 5;
//  }
//
//  if (pos < 0) pos = 0;
//  if (pos > 255) pos = 255;
//
  analogWrite(ACTUATOR_PIN, pos);



  delay(30);
}
