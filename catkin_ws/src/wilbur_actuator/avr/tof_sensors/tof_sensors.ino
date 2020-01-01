// ROS includes
#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>

#include <SparkFun_RFD77402_Arduino_Library.h> //Use Library Manager or download here: https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library
#include <movingAvg.h>

#define MUX_ADDR 0x70 //7-bit unshifted default I2C Address
#define TOF1_PORT 3 // The Mux port that the 1st TOF is on
#define TOF2_PORT 4 // The Mux port that the 2nd TOF is on

#define ACTUATOR_PIN 5

RFD77402 myDistance; //Hook object to the library

// ROS setup
ros::NodeHandle nh;

std_msgs::Int16 dist1;
std_msgs::Int16 dist2;
std_msgs::Bool stop_msg;
ros::Publisher dist_pub1("/tofs/tof1", &dist1);
ros::Publisher dist_pub2("/tofs/tof2", &dist2);
ros::Publisher wheel_stop_pub("/tofs/tof_stop", &stop_msg);

void stop_act_cb(const std_msgs::Empty& msg);

/*** Other constants ***/

const int FRONT_THRESHOLD = 150;
const int HARDSTOP_THRESHOLD = 80;
const int TIME_TO_PASS_OVER_OBSTACLE = 3000;
const int MOVE_UP_HEIGHT = 30;
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
  //ROS Setup
  nh.initNode();

  // Register the publisher object
  nh.advertise(dist_pub1);
  nh.advertise(dist_pub2);
  nh.advertise(wheel_stop_pub);

  dist1avg.begin();
  dist2avg.begin();

  // TOF Setup
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

  delay(2000);
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

  // Populate the distance message with the proper data
  dist1.data = distanceX;
  dist2.data = distanceY;

  // Queue the dist1 message for publishing
  dist_pub1.publish(&dist1);
  dist_pub2.publish(&dist2);

  // Propagate out the published message
  nh.spinOnce();

  if (distanceX < HARDSTOP_THRESHOLD) {
    pos -= MOVE_UP_HEIGHT;
    lastUpTime = millis();
    if (!sentHardStop) {
      stop_msg.data = true;
      wheel_stop_pub.publish(&stop_msg);
      sentHardStop = true;
    }
  } else if (millis() - lastMoveTime > MOVE_THRESHOLD){
    if (sentHardStop) {
      stop_msg.data = false;
      wheel_stop_pub.publish(&stop_msg);
      sentHardStop = false;
    }
    lastMoveTime = millis();
    if (distanceX < FRONT_THRESHOLD) {
      pos -= MOVE_UP_HEIGHT;
      lastUpTime = millis();
    } else if (distanceY > Y_MIN\
      && confidenceY < 2000\
      && (millis() - lastUpTime) > TIME_TO_PASS_OVER_OBSTACLE) { // 2000 is a magic number that, over which, the sensor would return a higher number than it should've
      pos += 5;
    } else if (distanceY < Y_MAX && confidenceY < 2000) {
      pos -= 5;
    }
  }

  if (pos < 0) pos = 0;
  if (pos > 255) pos = 255;


  analogWrite(ACTUATOR_PIN, pos);

  delay(10);

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
