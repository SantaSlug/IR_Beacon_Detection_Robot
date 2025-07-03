#include "revolver.h"
#include "speaker.h"
#include "IR_break_sensor.h"

const int rev_delay = 2000;
unsigned long lastRan = 0;
bool canCheck = true;
unsigned long time = 0;
const int speed =-7;
const long overall = 3500;
int rev_count = 0;

Servo revolverServo;

void setup_revolver(){
}

void revolver_rotate(bool override = false, int pos = ROTATION*MICROSTEPS){
  revolverServo.attach(revolverPin);
  // override - should it override the time delay and not care about it 9should only check his which switching from drive to shoot state
  time = millis();
  bool delay_check = time > (lastRan + rev_delay);
  if (override || delay_check){
      revolverServo.write(90+speed);
      delay(250);
      revolverServo.write(93);
      delay(10);
      revolverServo.detach();
      rev_count++;

      lastRan = time;

      canCheck = true;
      //revolverServo.detach();
  }
  if ((time>lastRan+1000) && canCheck){
    ball_detect();
    canCheck = false;
  }
}