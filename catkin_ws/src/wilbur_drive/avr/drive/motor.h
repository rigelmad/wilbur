#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"

class Motor {
  public:
    Motor(int dir_pin, bool fwd_dir, int speed_pin);
    void fwd(uint8_t speed);
    void rev(uint8_t speed);
    void stop();
  private:
    int _dir_pin, _fwd_dir, _speed_pin;
};

#endif
