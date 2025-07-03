#ifndef REVOLVER_H
#define REVOLVER_H

#include <Arduino.h> // Include Arduino core functions
#include <Servo.h>
#include "PinConfig.h"
#define MICROSTEPS 8
#define ROTATION 40

extern unsigned long lastRan;
extern int rev_count;

// Declare functions
void setup_revolver();
void revolver_rotate(bool override = false, int pos = ROTATION*MICROSTEPS);
  
#endif