#ifndef PINCONFIG_H
#define PINCONFIG_H

// color indication pins
const int colorSwitchInput = 27;
const int servo_switch = 8;

// ball launcher motor pins
const int launch_enA = 11;
const int launch_in1 = 30;
const int launch_in2 = 31;


//revolver servo pin
const int revolverPin = 12;

const int play_game_pin = 41;
const int IR_Break_Pin = A1;

//ball revolver pins
//const int dirPin = 22;
//const int stepPin = 23;
//const int enableRev = 24;
//const int MSI1 = 25;
//const int MSI2 = 26;

//Speaker pins are 18 and 19 (Serial1)

const int IR_BEACON_DETECT = 20;


// Right ultrasonic sensor pins
const int trigside = 50;
const int echoside = 48;

// Analog front distance sensor pin
const int analogPin = A0;

// Motor control pins
const int PWM1 = 9;    // Left motor PWM
const int IN1_1 = 4;
const int IN2_1 = 5;
const int PWM2 = 10;   // Right motor PWM
const int IN1_2 = 7;
const int IN2_2 = 6;

// Encoder pins
const int  ENCA = 2;
const int  ENCB = 3;
const int  ENCC = 18;
const int  ENCD = 19;


#endif