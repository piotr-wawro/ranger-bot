#include "Ranger.h"

void Ranger::init() {
  gyro.begin();
  buzzer.setpin(45);
  rgbLed.setpin(44);
}

// Makes the Ranger move forward or backward for a given amount of time.
// Gyroscope is used to keep the Ranger on the same bearing.
void Ranger::move(int8_t speed, int16_t milliseconds) {
  gyro.update();
  int16_t startAngle = gyro.getAngleZ();
  int16_t endAngle = startAngle;
  int16_t endTime = millis() + milliseconds;

  while(millis() < endTime) {
    if(speed >= 0 && !isBlocked()) {
      _move(speed, endAngle);
    }
    else if(speed < 0) {
      _move(speed, endAngle);
    }
  }

  // Make sure the Ranger is heading in the right direction
  _turn(speed, endAngle);
  stop();
}

// Makes the Ranger move at a given bearing. It doesn't stop!
void Ranger::moveAtBearing(uint8_t speed, int16_t endAngle) {
  _move(speed, endAngle);
}

// Turn a given amount of degrees. Positive angle is to the right.
void Ranger::turn(uint8_t speed, int16_t angle) {
  gyro.update();
  double startAngle = gyro.getAngleZ();
  double endAngle = _normalizeAngle(startAngle + angle);
  _turn(speed, endAngle);
}

// Turn to a given bearing.
void Ranger::turnAtBearing(uint8_t speed, int16_t endAngle) {
  _turn(speed, endAngle);
}

void Ranger::follow(uint8_t minDistance, uint8_t maxDistance) {
  uint8_t halfDistance = minDistance + (maxDistance - minDistance)/2;
  int16_t distance = ultrasonicSensor.distanceCm();

  if(distance < minDistance) {
    move(-80, 200);
  }
  else if (distance >= minDistance && distance < halfDistance) {
    stop();
  }
  else if (distance >= halfDistance && distance < maxDistance) {
    move(80, 200);
  }
  else {
    stop();
  }
}

void Ranger::oneInside(uint8_t speed) {
  if(irSensor.readLeft() > 100 && irSensor.readRight() <= 100) {
    firstLeft = true;
  }
  else if(irSensor.readLeft() <= 100 && irSensor.readRight() > 100) {
    firstLeft = false;
  }

  if(irSensor.readLeft() <= 100 || irSensor.readRight() <= 100) {
    leftMotor.move(speed);
    rightMotor.move(speed);
  }
  else if(firstLeft) {
    leftMotor.move(speed);
    rightMotor.move(-speed);
  }
  else {
    leftMotor.move(-speed);
    rightMotor.move(speed);
  }
}

void Ranger::twoInside(uint8_t speed) {
  if(irSensor.readLeft() <= 100 && irSensor.readRight() <= 100) {
    leftMotor.move(speed);
    rightMotor.move(speed);
  }
  else if(irSensor.readLeft() > 100 && irSensor.readRight() <= 100) {
    leftMotor.move(speed);
    rightMotor.move(-speed);
  }
  else if(irSensor.readLeft() <= 100 && irSensor.readRight() > 100) {
    leftMotor.move(-speed);
    rightMotor.move(speed);
  }
}

void Ranger::twoInsideAvoidCollision(uint8_t speed) {
  if(!isBlocked()) {
    if(irSensor.readLeft() <= 100 && irSensor.readRight() <= 100) {
      leftMotor.move(speed);
      rightMotor.move(speed);
    }
    else if(irSensor.readLeft() > 100 && irSensor.readRight() <= 100) {
      leftMotor.move(speed);
      rightMotor.move(-speed);
    }
    else if(irSensor.readLeft() <= 100 && irSensor.readRight() > 100) {
      leftMotor.move(-speed);
      rightMotor.move(speed);
    }
  }
  else {
    stop();
    turn(speed, -90);
    move(speed, 1500);
    turn(speed, 90);
    move(speed, 1500);
    turn(speed, 90);
    
    while (irSensor.readLeft() > 100 || irSensor.readRight() > 100) {
      leftMotor.move(speed);
      rightMotor.move(speed);
    }
    stop();

    turn(speed, -50);
  }
}

void Ranger::exploreMaze(uint8_t speed) {
  if(irSensor.readLeft() <= 100 && irSensor.readRight() > 100) {
    leftMotor.move(speed);
    rightMotor.move(speed);
  }
  else if(irSensor.readLeft() <= 100 && irSensor.readRight() <= 100) {
    leftMotor.move(speed);
    rightMotor.move(-speed);
  }
  else if(irSensor.readLeft() > 100 && irSensor.readRight() > 100) {
    leftMotor.move(-speed);
    rightMotor.move(speed);
  }
}

void Ranger::stop() {
  leftMotor.move(0);
  rightMotor.move(0);
}

bool Ranger::isMoving() {
  return leftMotor.isMoving() || rightMotor.isMoving();
}

bool Ranger::isBlocked() {
  return ultrasonicSensor.distanceCm() < 6;
}

void Ranger::_turn(uint8_t speed, int16_t endAngle) {
  gyro.update();
  double deltaAngle = _normalizeAngle(endAngle - gyro.getAngleZ());

  while(abs(deltaAngle) > anglePrecision) {
    while(abs(deltaAngle) > anglePrecision) {
      int8_t scaledSpeed = _scaleTurnSpeed(speed, deltaAngle);

      if(deltaAngle < 0) {
        leftMotor.move(-scaledSpeed);
        rightMotor.move(scaledSpeed);
      }
      else {
        leftMotor.move(scaledSpeed);
        rightMotor.move(-scaledSpeed);
      }

      gyro.update();
      deltaAngle = _normalizeAngle(endAngle - gyro.getAngleZ());
    }

    leftMotor.move(0);
    rightMotor.move(0);
    delay(stabilizationTime);

    gyro.update();
    deltaAngle = _normalizeAngle(endAngle - gyro.getAngleZ());
  }
}

void Ranger::_move(int8_t speed, int16_t endAngle) {
    gyro.update();
    int16_t deltaAngle = _normalizeAngle(endAngle - gyro.getAngleZ());

    if(speed > 0 && isBlocked()) {
      stop();
      return;
    }

    if(deltaAngle <= 0 && speed > 0) {
      leftMotor.move(_scaleMoveSpeed(speed, deltaAngle));
      rightMotor.move(speed);
    }
    else if(deltaAngle > 0 && speed > 0) {
      leftMotor.move(speed);
      rightMotor.move(_scaleMoveSpeed(speed, deltaAngle));
    }
    else if(deltaAngle <= 0 && speed < 0) {
      leftMotor.move(speed);
      rightMotor.move(-_scaleMoveSpeed(-speed, deltaAngle));
    }
    else if(deltaAngle > 0 && speed < 0) {
      leftMotor.move(-_scaleMoveSpeed(-speed, deltaAngle));
      rightMotor.move(speed);
    }
}

// Normalizes angle to be between -180 and 180 degrees.
int16_t Ranger::_normalizeAngle(int16_t angle) {
  return angle - floor((angle + 180) / 360.0) * 360;
}

// Piecewise function that scales the speed of the Ranger.
// Reduces speed linearly from speed to minimalSpeed when
// the Ranger approaches the set bearing.
uint8_t Ranger::_scaleTurnSpeed(uint8_t speed, int16_t deltaAngle) {
  double breakpoint = 45.0;

  if(abs(deltaAngle) > breakpoint) {
    return speed;
  }
  else {
    return (speed-minimalSpeed)/breakpoint * abs(deltaAngle) + minimalSpeed;
  }
}

// Piecewise function that scales the speed of the Ranger.
// Reduces speed linearly from speed to minimalSpeed when
// the Ranger moves away from the set bearing.
uint8_t Ranger::_scaleMoveSpeed(uint8_t speed, int16_t deltaAngle) {
  double breakpoint = 10.0;

  if(abs(deltaAngle) > breakpoint) {
    return minimalSpeed;
  }
  else {
    return (minimalSpeed-speed)/breakpoint * abs(deltaAngle) + speed;
  }
}