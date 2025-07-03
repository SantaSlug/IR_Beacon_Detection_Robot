#include "color_switch.h"
#include "speaker.h"

Servo colorSwitchServo;

int prev_switch = LOW;
int flip_switch = HIGH;

void setup_color_switch() {
    pinMode(colorSwitchInput, INPUT_PULLUP);
    prev_switch = !digitalRead(colorSwitchInput);
    flip_switch = digitalRead(colorSwitchInput);

    colorSwitchServo.attach(servo_switch);
    
      if (flip_switch == HIGH){
        play_now(1,2);
        for (int i = 0; i<3000; i++){
          colorSwitchServo.write(0);
        }
      }else{
        play_now(1,1);
        for (int i = 0; i<3000; i++){
          colorSwitchServo.write(180);
        }
      }
    
    delay(10);
    colorSwitchServo.detach();  
}

void color_switch(){
  
    prev_switch = flip_switch;
    flip_switch = digitalRead(colorSwitchInput);
    
    if (flip_switch != prev_switch){
      colorSwitchServo.attach(servo_switch);
      
      if (flip_switch == HIGH){
        play_now(1,2);
        for (int i = 0; i<3000; i++){
          colorSwitchServo.write(0);
        }
      }else{
        play_now(1,1);
        for (int i = 0; i<3000; i++){
          colorSwitchServo.write(180);
        }
      }
      delay(10);
      colorSwitchServo.detach();
    }
  }