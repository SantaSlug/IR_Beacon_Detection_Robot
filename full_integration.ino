#include "Arduino.h"
#include "color_switch.h"
#include "revolver.h"
#include "launcher.h"
#include "speaker.h"
#include "IR_break_sensor.h"
#include "game_play.h"
#include "beacon_detected.h"
#include "drive_sys.h"
#include <util/atomic.h>

volatile int posi = 0; // Left encoder
volatile int posj = 0; // Right encoder

int turn_count = 1;

// Define states
const int IDLE = 100; // STATE_0
const int SEARCH = 101; // STATE_1
const int LAUNCH = 102; // STATE_2
const int TURN = 103; // STATE_3
const int GO_HOME = 104; // STATE_4

// Define boolean input variables
bool R = 0; // Running
bool USF = 0; // Ultrasonic Front Sensor
bool PL = 1; // Package Loaded
bool BD = 0; // Beacon Detected
bool H = 0; // At IDLE

int leftCount;

// Define state variable and initialize
int state = IDLE; //start in IDLE state

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("IDLE");

  //no setup revolver function
  setup_speaker();
  setup_color_switch();
  setup_revolver();
  setup_launcher();
  setup_customer_dectection();
  setup_drive();
  setup_game_play();

}

unsigned long curTime = 0;
unsigned long shoot_trig = 0;


void loop() {
  FSM();
}


void FSM(){
    //Read inputs
  R = is_playing_game(R);//is_playing_game(R);
  USF = FUS_checking(USF);
  PL = is_ball_detected;
  BD = detect_customer();
  H = (turn_count % 4) == 0;
  curTime = millis();

  unsigned long idle_time = 0;



  

  //switching color can occur in any state based on user input
  color_switch();
  
  // FSM
  switch (state){
    case IDLE: // IDLE state
      //Set outputs (000)
      drive_idle();
      shoot_idle();
      ball_detect();
      run_speaker();

      //Next state logic
      if ((R == 1) && (PL == 1)){
        state = SEARCH;
        Serial.println("SEARCH");
        play_now(5,get_rnd(5));
      }
      else{
        state = IDLE;
        Serial.println("IDLE");
      }
      break;

    case SEARCH: // SEARCH state
      //Set outputs (100)
      drive_forward();
      shoot_idle();
      run_speaker();

      //Next state logic
      if (R == 0){
        state = IDLE;
        Serial.println("IDLE");
      }
      else if ((R == 1) && (USF == 0) && ((BD == 0) || curTime > idle_time + 5000)){
        if (curTime > idle_time  +5000){
          turn_count --;
        }
        state = SEARCH;
        Serial.println("SEARCH");
      }
      else if ((R == 1) && (BD == 1)){
        state = LAUNCH;
        lastRan = curTime+1000;
        shoot_trig = curTime;
        rev_count = 0;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
          posi = 0;
        }

        Serial.println("LAUNCH");
      }
      else if ((R == 1) && (USF == 1) && (BD == 0)){
        state = TURN;
        Serial.println("TURN");
      }
      break;

    case LAUNCH: // LAUNCH state 
        shoot();
        drive_idle();

      //Next state logic
      if (R == 0){
        state = IDLE;
        Serial.println("IDLE");
        play_now(3,get_rnd(3));
      }
      else if ((R == 1) && (PL == 1) && (BD == 0)){
        state = SEARCH;
        Serial.println("SEARCH");
        play_now(3,get_rnd(3));
      }
      else if ((R == 1) && (PL == 1) && (BD == 1)){
        state = LAUNCH;
        Serial.println("LAUNCH");
      }
      else if ((R == 1) && (PL == 0)){
        state = GO_HOME;
        Serial.println("GO_HOME");
        play_now(4,get_rnd(4));
      }
      break;

    case TURN: // TURN state 
      //Set outputs (010)
      drive_turn();
      shoot_idle();
      

      //Next state logic
      if (R == 0){
        state = IDLE;
        Serial.println("IDLE");
        turn_count++;
      }
      else if ((R == 1) && (USF == 0) && (PL == 1)){
        state = SEARCH;
        Serial.println("SEARCH");
        turn_count++;
      }
      else if ((R == 1) && (USF == 1)){
        state = TURN;
        Serial.println("TURN");
      }
      else if ((R == 1) && (USF == 0) && (PL == 0)){
        state = GO_HOME;
        Serial.println("GO_HOME");
        turn_count++;
      }
      break;

    case GO_HOME: // GO_HOME state 
      //Set outputs (100)
      drive_forward();
      shoot_idle();
      run_speaker();

      //Next state logic
      if (R == 0){
        state = IDLE;
        Serial.println("IDLE");
      }
      else if ((R == 1) && (USF == 1) && (PL == 0) && (H == 1)){
        state = IDLE;
        Serial.println("IDLE");
      }
      else if ((R == 1) && (PL == 1)){
        state = SEARCH;
        Serial.println("SEARCH");
      }
      else if ((R == 1) && (USF == 1) && (PL == 0) && (H == 0)){
        state = TURN;
        Serial.println("TURN");
      }
      else if ((R == 1) && (USF == 0) && (PL == 0)){
        state = GO_HOME;
        Serial.println("GO_HOME");
      }
      break;
  }
}

void shoot(){
  revolver_rotate();
  launch_ball();
}

void shoot_idle(){
  //digitalWrite(enableRev,HIGH);
  stop_launch_ball();
}

unsigned long turnStartTime = 0;
const unsigned long turnDuration = 1500;  // how long to turn in ms
unsigned long currentTime;
float frontDist;

/*
bool FUS_checking(bool isTurning){
  float frontDist = frontSensorDistance();
  currentTime = millis();
  
  if (!isTurning && frontDist <= 22 && frontDist > 1) {
    isTurning = true;
    turnStartTime = currentTime;
  }
  if (isTurning && currentTime> turnStartTime + turnDuration) {
    drive_idle();
    isTurning = false;
  }
  return isTurning;
}
*/
bool FUS_checking(bool isTurning){
  float frontDist = frontSensorDistance();
  currentTime = millis();

  
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    leftCount = posi;
  }
  Serial.println(leftCount);

    if (!isTurning && frontDist <= 22 && frontDist > 1) {
    isTurning = true;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
      posi = 0;
    }
    setMotor(0, 130, PWM1, IN1_1, IN2_1); // Left motor backward
    setMotor(1, 130, PWM2, IN1_2, IN2_2); // Right motor forward
  }
  if (isTurning && leftCount <= -330) {
    drive_idle();
    isTurning = false;
  }
  return isTurning;
}



// Constants for analog-to-distance conversion (front sensor)
const float minVoltage = 0.4;
const float maxVoltage = 3.1;
const int minDistance = 10;
const int maxDistance = 80;

// Wall-following motor control
void setMotor(int dir, int pwmVal, int pwm, int in1, int in2) {
  analogWrite(pwm, pwmVal);
  if (dir == 1) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
}

// Convert analog front sensor voltage to distance
float convertVoltageToDistance(float voltage) {
  if (voltage <= minVoltage) return maxDistance;
  if (voltage >= maxVoltage) return maxDistance;
  return 27.86 / (voltage - 0.42);
}

// Get distance from front sensor
float frontSensorDistance() {
  int sensorValue = analogRead(analogPin);
  float voltage = sensorValue * (5.0 / 1023.0);
  return convertVoltageToDistance(voltage);
}

// Perform wall-following
void drive_forward() {
  int rightDistance = measureDistance(trigside, echoside);
  int baseSpeed = 110;
  int offset = 20;

  int leftSpeed = baseSpeed;
  int rightSpeed = baseSpeed + 35;
  const int high_thres = 4;
  const int low_thres = 1;
  const int low_off = 5;
  const int high_off = 40;
  const int ideal = 17;

  if (rightDistance-ideal>high_thres){
    leftSpeed = baseSpeed + high_off;
    rightSpeed = baseSpeed - high_off;
  }else if (rightDistance-ideal<-high_thres){
    leftSpeed = baseSpeed - high_off;
    rightSpeed = baseSpeed + high_off;
  }else if (rightDistance-ideal>low_thres){
    leftSpeed = baseSpeed + (rightDistance-ideal)/(high_thres-low_thres)*(high_off-low_off);
    rightSpeed = baseSpeed - (rightDistance-ideal)/(high_thres-low_thres)*(high_off-low_off);
  }else if (rightDistance-ideal<-low_thres){
    leftSpeed = baseSpeed + (rightDistance-ideal)/(high_thres-low_thres)*(high_off-low_off);
    rightSpeed = baseSpeed - (rightDistance-ideal)/(high_thres-low_thres)*(high_off-low_off);
  }else{
    leftSpeed = baseSpeed;
    rightSpeed = baseSpeed;
  }

  setMotor(1, leftSpeed, PWM1, IN1_1, IN2_1);
  setMotor(1, rightSpeed, PWM2, IN1_2, IN2_2);
}

void drive_idle() {
  analogWrite(PWM1, 0);
  analogWrite(PWM2, 0);
  digitalWrite(IN1_1, LOW);
  digitalWrite(IN2_1, LOW);
  digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_2, LOW);
}

void setup_drive() {

  pinMode(PWM1, OUTPUT);
  pinMode(IN1_1, OUTPUT);
  pinMode(IN2_1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(IN1_2, OUTPUT);
  pinMode(IN2_2, OUTPUT);

  // Sensor pin setup
  pinMode(trigside, OUTPUT);
  pinMode(echoside, INPUT);
  pinMode(analogPin, INPUT);
  // Encoder pin setup
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  pinMode(ENCC, INPUT);
  pinMode(ENCD, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoderA, RISING);
  attachInterrupt(digitalPinToInterrupt(ENCC), readEncoderB, RISING);

  // Reset encoders
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
    posi = 0;
    posj = 0;
  }

}

void drive_turn(){
    setMotor(0, 125, PWM1, IN1_1, IN2_1); // Left motor backward
    setMotor(1, 145, PWM2, IN1_2, IN2_2); // Right motor forward
}



// Encoder ISR for left wheel
void readEncoderA() {
  int b = digitalRead(ENCB);
  if (b > 0) {
    posi++;
  } else {
    posi--;
  }
}

// Encoder ISR for right wheel
void readEncoderB() {
  int d = digitalRead(ENCD);
  if (d > 0) {
    posj--;
  } else {
    posj++;
  }
}







