#include <MeAuriga.h>
#include "Ranger.h"

Ranger ranger = Ranger();
int16_t bearing = random(-180, 180);

void squareShape() {
  for(int i = 0; i < 4; i++) {
    ranger.move(80, 1000);
    ranger.turn(80, 90);
  }
}

void eightShape() {
  unsigned long startTime = millis();
  unsigned long timeElapsed = 0;
  double timeMovement = 8 * 1000;

  for(int8_t i = -1; i < 2; i+=2) {
    while (timeElapsed < timeMovement) {
      int16_t bearing = (i*360.0/timeMovement) * timeElapsed;
      ranger.moveAtBearing(80, bearing);
      timeElapsed = millis() - startTime;
    }
    startTime = millis();
    timeElapsed = 0;
  }

  ranger.stop();
}

void rgbLedAndBuzzer() {
  unsigned long startTime = millis();
  unsigned long timeElapsed = 0;
  double timeMovement = 8 * 1000;

  ranger.rgbLed.fillPixelsBak(0, 0, 0);
  while (timeElapsed < timeMovement) {
    ranger.rgbLed.setColor(random(1, 13), random(0, 11), random(0, 11), random(0, 11));
    ranger.rgbLed.show();
    ranger.buzzer.tone(random(50, 5000), random(5, 100));
    timeElapsed = millis() - startTime;
  }
  ranger.rgbLed.setColor(0, 0, 0, 0);
  ranger.rgbLed.show();
}

void speedDependentOnLightSensorV1() {
  if(ranger.lightSensor.read() > 5) {
    ranger.move(80, 100);
  }
  else {
    ranger.stop();
  }
}

void speedDependentOnLightSensorV2() {
  ranger.move(ranger.lightSensor.read()/1100.0*100, 100);
}

void buzzerDependentOnLightSensor() {
  ranger.buzzer.tone(ranger.lightSensor.read()*10, 100);
}

void randomRoamingV1() {
  int16_t bearing = random(-180, 180);
  ranger.turnAtBearing(80, bearing);
  while(!ranger.isBlocked()) {
    ranger.moveAtBearing(80, bearing);
  }
}

void randomRoamingV2() {
  ranger.turnAtBearing(80, bearing);
  while(!ranger.isBlocked()) {
    ranger.moveAtBearing(80, bearing);
  }
  ranger.move(-80, 500);

  int r = random();
  if(r % 2 == 0) {
    bearing -= 90;
  }
  else {
    bearing += 90;
  }
}

void obstacleAvoidance() {
  ranger.turnAtBearing(80, bearing);
  while(!ranger.isBlocked()) {
    ranger.moveAtBearing(80, bearing);
  }
  ranger.move(-80, 500);
  ranger.turn(80, -90);
  ranger.move(80, 1000);
  ranger.turn(80, 90);
}

void objectFollowing() {
  ranger.follow(10, 50);
}

void lineFollowingOneInside() {
  ranger.oneInside(50);
}

void lineFollowingTwoInside() {
  ranger.twoInside(50);
}

void lineFollowingWithObstacleAvoidance() {
  ranger.twoInsideAvoidCollision(50);
}

void mazeExplorer() {
  ranger.exploreMaze(50);
}

void setup() {
  Serial.begin(9600);
  ranger.init();
  randomSeed(analogRead(0));

  // squareShape();
  // eightShape();
  // rgbLedAndBuzzer();
}

void loop() {
  // speedDependentOnLightSensorV1();
  // speedDependentOnLightSensorV2();
  // buzzerDependentOnLightSensor();
  // randomRoamingV1();
  // randomRoamingV2();
  // obstacleAvoidance();
  // objectFollowing();
  // lineFollowingOneInside();
  // lineFollowingTwoInside();
  // lineFollowingWithObstacleAvoidance();
  // mazeExplorer();
}
