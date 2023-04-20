#pragma once
#include <Arduino.h>

class Motor {
  private:
    uint8_t pinInA;
    uint8_t pinInB;
    uint8_t pinPwm;

    bool inA = LOW;
    bool inB = LOW;
    uint8_t pwm = 0;

  public:
    Motor(uint8_t pinInA, uint8_t pinInB, uint8_t pinPwm);
    void move(int8_t speed);
    bool isMoving();
};
