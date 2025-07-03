#include "launcher.h"

// Define subsystem-specific pins (keep these local to the launcher subsystem)


// Implement setup_launcher()
void setup_launcher(){
    pinMode(launch_enA, OUTPUT);
    pinMode(launch_in1, OUTPUT);
    pinMode(launch_in2, OUTPUT);
}

int measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 3000);
  if (duration == 0) return 999;
  return duration * 0.034 / 2;
}

int launch_speed; 
void launch_ball(){

    int dist = measureDistance(trigside, echoside);
    if (dist < 17){
        launch_speed =60;
    }else if (dist > 30){
        launch_speed =100;
    }else{
        launch_speed =40*(dist-17)+60;
    }
    
    int launch_pwm = map(launch_speed, 0, 100, 0 , 255);
    analogWrite(launch_enA, launch_pwm);
    digitalWrite(launch_in1, LOW);
    digitalWrite(launch_in2, HIGH);
}

void stop_launch_ball(){
    analogWrite(launch_enA, 0);
    digitalWrite(launch_in1, LOW);
    digitalWrite(launch_in2, LOW);
}