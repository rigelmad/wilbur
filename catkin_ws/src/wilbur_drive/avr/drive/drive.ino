#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Bool.h>

#include "motor.h" // For motors

/**** ROS SETUP ****/
ros::NodeHandle nh;
void lwheel_cb(const std_msgs::Float32& msg);
ros::Subscriber<std_msgs::Float32> lwheel_vtarget_sub("lwheel_vtarget", lwheel_cb);
void rwheel_cb(const std_msgs::Float32& msg);
ros::Subscriber<std_msgs::Float32> rwheel_vtarget_sub("rwheel_vtarget", rwheel_cb);
void stop_cb(const std_msgs::Bool& msg);
ros::Subscriber<std_msgs::Bool> stop_msg_sub("/tofs/tof_stop", stop_cb);
/******************/

// Populated from the Motor I/O table here:
// https://wiki.dfrobot.com/Quad_Motor_Driver_Shield_for_Arduino_SKU_DRI0039
// motor.h:6 -> Motor(int dir_pin, bool fwd_dir, int speed_pin);
Motor m1(4, HIGH, 3); // Left Motor
Motor m2(12, LOW, 11); // Left Motor
Motor m3(8, LOW, 5); // Right Motor
Motor m4(7, HIGH, 6); // Right Motor

bool forward_stop = false;

void setup() {
  nh.initNode();
  nh.subscribe(lwheel_vtarget_sub);
  nh.subscribe(rwheel_vtarget_sub);
  nh.subscribe(stop_msg_sub);
  stopAll();
}

void loop() {
  nh.spinOnce();
}

/**** Ros Callbacks ****/

void stop_cb(const std_msgs::Bool& msg) {
  if (msg.data == true) {
    stopAll();
  }
  forward_stop = msg.data;
}

// Handle left wheel speeds
void lwheel_cb(const std_msgs::Float32& msg) {
  int vel_left = float_map(msg.data, -1.0, 1.0, -255.0, 255.0); // Casts to int
  uint8_t speed_left = abs(vel_left);

  if (forward_stop && vel_left > 0) return;

  if (vel_left < 0) { // If left velocity is negative
    m1.rev(speed_left);
    m2.rev(speed_left);
  } else { // Left velocity is positive
    m1.fwd(speed_left);
    m2.fwd(speed_left);
  }
}

// Handle right wheel speeds
void rwheel_cb(const std_msgs::Float32& msg) {
  int vel_right = float_map(msg.data, -1.0, 1.0, -255.0, 255.0); // Casts to int
  uint8_t speed_right = abs(vel_right);

  if (forward_stop && vel_right > 0) return;

  if (vel_right < 0) { // If right velocity is negative
    m3.rev(speed_right);
    m4.rev(speed_right);
  } else { // right velocity is positive
    m3.fwd(speed_right);
    m4.fwd(speed_right);
  }
}
/***********************/

/**** Custom functions ****/
void stopAll() {
  m1.stop();
  m2.stop();
  m3.stop();
  m4.stop();
}

float float_map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
/**************************/
