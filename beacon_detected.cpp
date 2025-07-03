#include "beacon_detected.h"

// Global volatile variables to be updated in the ISR.
volatile unsigned long lastRiseTime = 0;
volatile unsigned long period = 0;
volatile float frequency = 0;

void setup_customer_dectection() {
  // Initialize serial communication for debugging.


  // Configure the input pin connected to the square wave.
  // Here, we use digital pin 2 (which supports external interrupts on many Arduino boards).
  pinMode(IR_BEACON_DETECT, INPUT);

  // Attach an interrupt on pin 2 to trigger on each rising edge.
  attachInterrupt(digitalPinToInterrupt(IR_BEACON_DETECT), risingEdgeISR, RISING);
  
}
  static float freq1 = 0;
  static float freq2 = 0;
  static bool firstSampleTaken = false;

bool detect_customer() {
  // Use static variables to hold two consecutive frequency readings.

  // Safely read the frequency variable, which may be updated in the ISR.
  noInterrupts();
  float currentReading = frequency;
  interrupts();
  freq1 = currentReading;
  firstSampleTaken = true;
  return (freq1 >= 1400 && freq1 <= 1600);
  
  // Delay between samples to allow the signal to stabilize.
}

// Interrupt Service Routine (ISR) triggered on each rising edge of the square wave.
void risingEdgeISR() {
  // Record current time in microseconds.
  unsigned long currentTime = micros();

  // Calculate the period as the difference between this and the last rising edge.
  period = currentTime - lastRiseTime;
  
  // Update the last rising time for the next measurement.
  lastRiseTime = currentTime;

  // Calculate frequency only if the period is non-zero (in Hz).
  if (period > 0) {
    frequency = 1000000.0 / period;
  }
}
