#pragma once
#include <Arduino.h>

class IRSensor {
  private:
    uint8_t s1;
    uint8_t s2;

    uint8_t _read(uint8_t sensor);

  public:
    IRSensor(uint8_t s1, uint8_t s2);
    uint8_t readLeft();
    uint8_t readRight();
};
