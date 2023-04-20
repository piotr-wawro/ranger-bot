#include <Arduino.h>
#include "Motor.h"

Motor::Motor(uint8_t pinInA, uint8_t pinInB, uint8_t pinPwm) {
  this->pinInA = pinInA;
  this->pinInB = pinInB;
  this->pinPwm = pinPwm;

  pinMode(pinInA, OUTPUT);
  pinMode(pinInB, OUTPUT);
  pinMode(pinPwm, OUTPUT);

  digitalWrite(pinInA, inA);
  digitalWrite(pinInB, inB);
  analogWrite(pinPwm, pwm);
}

void Motor::move(int8_t speed) {
  if (speed == 0) {
    inA = LOW;
    inB = LOW;
    pwm = 0;
  } else if (speed > 0) {
    inA = HIGH;
    inB = LOW;
    pwm = speed / 100.0 * 255;
  } else {
    inA = LOW;
    inB = HIGH;
    pwm = -speed / 100.0 * 255;
  }

  digitalWrite(pinInA, inA);
  digitalWrite(pinInB, inB);
  analogWrite(pinPwm, pwm);
}

bool Motor::isMoving() {
  return pwm != 0;
}