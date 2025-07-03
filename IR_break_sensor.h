#ifndef IR_BREAK_SENSOR_H
#define IR_BREAK_SENSOR_H

#include <Arduino.h>
#include "PinConfig.h"

extern bool is_ball_detected;
extern bool previous_ball_detect;

void setup_ball_detect();
void ball_detect();

#endif