/* California High Speed Rail Delivery System
 * by: Gineton Alencar
 * date: 2019/04/16
 * last updated: 2019/05/07
 * license: TBD
 */

// load libraries
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1 and M2.
Adafruit_DCMotor *motorOne = AFMS.getMotor(1);
Adafruit_DCMotor *motorTwo = AFMS.getMotor(2);

// Name servo "gate."
Servo gate;

// Servo gate open and closed values;
const int closed = 0;
const int opened = 165;

// Set wait times for ball drop
const int gateOpenTime = 700;
const int ballDeliveryTime = 1000;
const int resetWaitTime = 700;

// set motor speed value
const int motorSpeedFull = 255;

// servo gate
#define GATE 10

// OSOYOO light dark sensor
// comes with pot for sensitivity
#define SENSOR 2

// Direction forward/backward trigger
// if on go forward, if off go backward
#define DIRECTION 8

#define POWER 7

// bools for sanity checks
bool sensorValue;
bool ballDelivered;
bool resetTriggered;


void setup() {
  Serial.begin(9600);
  
  // Light-dark sensor
  // value for white is LOW
  // value for black is HIGH
  pinMode(SENSOR, INPUT);
  
  // micro-servo
  gate.attach(GATE);

  // Motorshield
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  motorOne->setSpeed(motorSpeedFull);
  motorTwo->setSpeed(motorSpeedFull);
  
  ballDelivered = false;
  resetTriggered = true;
}

void loop() {

  sensorValue = digitalRead(SENSOR);
  powerOn = digitalRead(POWER);

  Serial.println(sensorValue); 
   
  // Black detected == HIGH
  // If black detected, ball not delivered and reset not triggered
  // then stop motos, deliver ball,
  // wait for ball drop,
  // and run motors again.
  if (sensorValue == HIGH && powerOn) {
    if(!ballDelivered && resetTriggered) {
      stopMotors();
      deliverBall();
      delay(ballDeliveryTime);
      runMotors();
    }
  
  }
  
  // if ballDelivered 
  // keep gate closed
  // until sensorValue detects "white" (low)
  // then reset
  if (sensorValue == LOW && powerOn) {
    delay(resetWaitTime);
    if (sensorValue == LOW) {
      if (ballDelivered) {
        resetTriggered = true;
      }
      if (resetTriggered) {
        ballDelivered = false;
      }
    }
  }
}

// 
void deliverBall() {
    gate.write(opened);
    delay(gateOpenTime);
    gate.write(closed);
    ballDelivered = true;
}

void runMotors() {
  // turn on motors
  // if DIRECTION trigger on go forward
  // else go backwards
  if (digitalRead(DIRECTION)) {
    motorOne->run(FORWARD);
    motorTwo->run(FORWARD);
  } else {
      motorOne->run(BACKWARD);
      motorTwo->run(BACKWARD);
  }
}

void stopMotors() {
  motorOne->run(RELEASE);
  motorTwo->run(RELEASE);
}
