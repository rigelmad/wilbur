#include <SparkFun_RFD77402_Arduino_Library.h> //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library
#include <movingAvg.h>

#define MUX_ADDR 0x70 //7-bit unshifted default I2C Address
#define TOF1_PORT 3 // The Mux port that the 1st TOF is on
#define TOF2_PORT 4

#define ACTUATOR_PIN 5


RFD77402 myDistance; //Hook object to the library


const int FRONT_THRESHOLD = 150;
const int HARDSTOP_THRESHOLD = 80;
const int TIME_TO_PASS_OVER_OBSTACLE = 2000;
const int Y_MIN = 40;
const int Y_MAX = 30;
unsigned long lastMoveTime = 0;
unsigned long lastUpTime = 0;
int pos = 100; // Start actuator at the middle
const int MOVE_THRESHOLD = 50;
bool sentHardStop = false;
movingAvg dist1avg(10);
movingAvg dist2avg(10);
bool isActuatorStopped = false;

void setup() {
  Serial.begin(57600);
  dist1avg.begin();
  dist2avg.begin();
  // TOF setup
  enableMuxPort(TOF1_PORT);
  if (myDistance.begin() == false) { //Initializes the sensor. Tells the user if initialization has failed.
    Serial.println("Error initializing Port 1");
    //while (1); //Freeze!
  }
  disableMuxPort(TOF1_PORT);


  enableMuxPort(TOF2_PORT);
  if (myDistance.begin() == false) { //Initializes the sensor. Tells the user if initialization has failed.
    Serial.println("Error initializing Port 2");
    //while (1); //Freeze!
  }
  disableMuxPort(TOF2_PORT);

  analogWrite(ACTUATOR_PIN, pos);

  delay(1000);
}

void loop() {
  enableMuxPort(TOF1_PORT);
  myDistance.takeMeasurement(); //Tell sensor to take measurement and populate distance variable with measurement value

  unsigned int distanceX = dist1avg.reading(myDistance.getDistance()); //Retrieve the distance value
  unsigned int confidenceX = myDistance.getConfidenceValue();
  disableMuxPort(TOF1_PORT);

  enableMuxPort(TOF2_PORT);
  myDistance.takeMeasurement(); //Tell sensor to take measurement and populate distance variable with measurement value

  unsigned int distanceY = dist2avg.reading(myDistance.getDistance()); //Retrieve the distance value
  unsigned int confidenceY = myDistance.getConfidenceValue();
  disableMuxPort(TOF2_PORT);

  if (distanceX < HARDSTOP_THRESHOLD) {
    pos -= 5;
    lastUpTime = millis();
//    if (!sentHardStop) {
//      stop_msg.data = true;
//      wheel_stop_pub.publish(&stop_msg);
//      sentHardStop = true;
//    }
  } else if (millis() - lastMoveTime > MOVE_THRESHOLD){
//    if (sentHardStop) {
//      stop_msg.data = false;
//      wheel_stop_pub.publish(&stop_msg);
//      sentHardStop = false;
//    }
    lastMoveTime = millis();
    if (distanceX < FRONT_THRESHOLD) {
      pos -= 5;
      lastUpTime = millis();
    } else if (distanceY > Y_MIN \ 
      && confidenceY < 2000 \ 
      && (millis() - lastUpTime) > TIME_TO_PASS_OVER_OBSTACLE) { // 2000 is a magic number that, over which, the sensor would return a higher number than it should've
      pos += 5;
    } else if (distanceY < Y_MAX && confidenceY < 2000) {
      pos -= 5;
    }
  }

  if (pos < 0) pos = 0;
  if (pos > 255) pos = 255;

  analogWrite(ACTUATOR_PIN, pos);


  Serial.println();
  Serial.print("Dist 1: ");
  Serial.print(distanceX);
  Serial.print(", ");
  Serial.print(confidenceX);
  Serial.print("     ");
  Serial.print("Dist 2: ");
  Serial.print(distanceY);
  Serial.print(", ");
  Serial.print(confidenceY);
  Serial.print("     ");
  Serial.print("Pos: ");
  Serial.print(pos);
  Serial.println();
  delay(30);

}

/*
  Basic control and commands for the PCA9548A/TCA9548A I2C multiplexer
*/

//Enables a specific port number
void enableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Set the wanted bit to enable the port
  settings |= (1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
}

//Disables a specific port number
void disableMuxPort(byte portNumber)
{
  if (portNumber > 7) portNumber = 7;

  Wire.beginTransmission(MUX_ADDR);
  //Read the current mux settings
  Wire.requestFrom(MUX_ADDR, 1);
  if (!Wire.available()) return; //Error
  byte settings = Wire.read();

  //Clear the wanted bit to disable the port
  settings &= ~(1 << portNumber);

  Wire.write(settings);
  Wire.endTransmission();
}

long runningAverage(int M) {
  #define LM_SIZE 10
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;

  return sum / count;
}
