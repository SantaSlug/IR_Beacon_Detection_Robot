#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <Arduino.h> // Include Arduino core functions
#include "PinConfig.h"

// Declare functions
void setup_launcher();
void launch_ball();
void stop_launch_ball();
int measureDistance(int trigPin, int echoPin);

#endif