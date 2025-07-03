#ifndef BEACON_DECTECT_H
#define BEACON_DECTECT_H

#include <Arduino.h> // Include Arduino core functions
#include "PinConfig.h"

// Declare functions
void setup_customer_dectection();
bool detect_customer();
void risingEdgeISR();

#endif