#include "Arduino.h"
#include "motor.h"
/*
 * @params:
 *  dir_pin:  The pin indicating direction
 *  fwd_dir:  The mode the dir_pin should be written to move forward
 *  speed_pin:The pin indicating speed
 */
Motor::Motor(int dir_pin, bool fwd_dir, int speed_pin) {
  _dir_pin = dir_pin;
  _fwd_dir = fwd_dir;
  _speed_pin = speed_pin;

  pinMode(_dir_pin, OUTPUT); // Set the direction pin as output
  pinMode(_speed_pin, OUTPUT); // Set the speed pin as output
}

void Motor::fwd(uint8_t speed) {
  digitalWrite(_dir_pin, _fwd_dir); // Make this motor move forward
  analogWrite(_speed_pin, speed);
}

void Motor::rev(uint8_t speed) {
  digitalWrite(_dir_pin, !_fwd_dir); // Make this motor move the opposite of forward
  analogWrite(_speed_pin, speed);
}

void Motor::stop() {
  analogWrite(_speed_pin, 0);
}
