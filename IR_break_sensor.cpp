#include "IR_break_sensor.h"
bool previous_ball_detect = false;  // Define the extern variable
bool is_ball_detected = false;  // Define the extern variable
int ball_check_thres = 5;      // Detection threshold

void setup_ball_detect() {
    is_ball_detected = false;
    pinMode(IR_Break_Pin, INPUT);
}

void ball_detect() {
    previous_ball_detect = is_ball_detected;
    Serial.println(analogRead(IR_Break_Pin));
    is_ball_detected = (analogRead(IR_Break_Pin) < ball_check_thres);
}