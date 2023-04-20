#pragma once
#include <MeBuzzer.h>
#include <MeGyro.h>
#include <MeUltrasonicSensor.h>
#include <MeRGBLed.h>
#include <MeLightSensor.h>
#include <Wire.h>
#include "Motor.h"
#include "IRSensor.h"

class Ranger {
  private:
    uint8_t minimalSpeed = 40;
    uint8_t anglePrecision = 1;
    unsigned long stabilizationTime = 300;
    bool firstLeft = true;

    Motor leftMotor = Motor(47, 46, 10);
    Motor rightMotor = Motor(48, 49, 11);
    MeGyro gyro = MeGyro(0, 0x69);
    IRSensor irSensor = IRSensor(A7, A12);
    MeUltrasonicSensor ultrasonicSensor = MeUltrasonicSensor(10);

    void _turn(uint8_t speed, int16_t endAngle);
    void _move(int8_t speed, int16_t endAngle);
    int16_t _normalizeAngle(int16_t angle);

    uint8_t _scaleTurnSpeed(uint8_t speed, int16_t deltaAngle);
    uint8_t _scaleMoveSpeed(uint8_t speed, int16_t deltaAngle);

  public:
    MeBuzzer buzzer;
    MeRGBLed rgbLed = MeRGBLed(0, 12);
    MeLightSensor lightSensor = MeLightSensor(12);

    void init();

    void move(int8_t speed, int16_t milliseconds);
    void moveAtBearing(uint8_t speed, int16_t endAngle);
    void turn(uint8_t speed, int16_t angle);
    void turnAtBearing(uint8_t speed, int16_t endAngle);
    void follow(uint8_t minDistance, uint8_t maxDistance);
    void oneInside(uint8_t speed);
    void twoInside(uint8_t speed);
    void twoInsideAvoidCollision(uint8_t speed);
    void exploreMaze(uint8_t speed);
    void stop();
    bool isMoving();
    bool isBlocked();
};
