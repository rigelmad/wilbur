#include "QEI.h"
#include "mbed.h"
#include <ros.h>
#include <std_msgs/Int16.h>

/*** ROS Setup ***/
ros::NodeHandle nh;
std_msgs::Int16 lwheel_msg;
ros::Publisher lwheel_pub("lwheel", &lwheel_msg);
std_msgs::Int16 rwheel_msg;
ros::Publisher rwheel_pub("rwheel", &rwheel_msg);

// Physical constants for Wilbur
const float WHEEL_DIAMETER_METERS = 0.065; // 6.5cm
const int COUNTS_PER_ENCODER_REV = 12; // Encoder is 12 ppr
const int GEAR_RATIO = 48; // 48:1 gear ratio, 48 turns of the motor turns the wheel once

// Serial pc(USBTX, USBRX);
QEI left_enc (D3, D4, NC,
  (COUNTS_PER_ENCODER_REV * GEAR_RATIO),
  WHEEL_DIAMETER_METERS, QEI::X2_ENCODING);
QEI right_enc (D6, D5, NC,
  (COUNTS_PER_ENCODER_REV * GEAR_RATIO),
  WHEEL_DIAMETER_METERS, QEI::X2_ENCODING);

int main() {
    // ROS Init
    nh.initNode();
    nh.advertise(lwheel_pub);
    nh.advertise(rwheel_pub);

    while(1) {
      float vel[2];

      int16_t lpulse = left_enc.getPulses();
      int16_t rpulse = right_enc.getPulses();

      lwheel_msg.data = lpulse;
      rwheel_msg.data = rpulse;

      lwheel_pub.publish(&lwheel_msg);
      rwheel_pub.publish(&rwheel_msg);

      nh.spinOnce();
      wait(0.01);
    }
}
