#include "IRSensor.h"

IRSensor::IRSensor(uint8_t s1, uint8_t s2) {
  this->s1 = s1;
  this->s2 = s2;

  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
}

uint8_t IRSensor::readLeft() {
  return _read(s1);
}

uint8_t IRSensor::readRight() {
  return _read(s2);
}

uint8_t IRSensor::_read(uint8_t sensor) {
    return analogRead(sensor);
}
