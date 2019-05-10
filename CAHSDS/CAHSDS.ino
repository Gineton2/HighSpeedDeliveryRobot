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
const int closed = 3;
const int opened = 75;

// Set wait times for ball drop
const int gateOpenTime = 700;
const int ballDeliveryTime = 1000;
const int resetWaitTime = 100;

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

// Switch for turning vehicle on and off
#define POWER 7

// bools for sanity checks
bool sensorValue;
bool ballDelivered;
bool resetTriggered;
bool powerOn;
bool motorDirection;


void setup() {
  Serial.begin(9600);
  
  // Light-dark sensor
  // value for white is LOW
  // value for black is HIGH
  pinMode(SENSOR, INPUT);

  // Set Power and Direction switches to PULLUP.
  // This makes readings opposite (on is off, off is on).
  // PULLUP is used to avoid needing external resistors,
  // taking advantage of the Arduino board's internal resistors.
  pinMode(POWER, INPUT_PULLUP);
  pinMode(DIRECTION, INPUT_PULLUP);
  
  // micro-servo
  gate.attach(GATE);

  // Motorshield
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  motorOne->setSpeed(motorSpeedFull);
  motorTwo->setSpeed(motorSpeedFull);
  
  ballDelivered = false;
  resetTriggered = true;

  gate.write(closed);
}

void loop() {

  sensorValue = digitalRead(SENSOR);
  powerOn = !digitalRead(POWER);
  motorDirection = digitalRead(DIRECTION);
  

  Serial.println("Sensor: " + sensorValue); 
  Serial.println("Power: " + powerOn);
  Serial.println("Direction: " + motorDirection);

  if(!powerOn) {
    stopMotors();
    //gate.write(closed);
  } else {
  
    // Black detected == HIGH
    // If black detected, ball not delivered and reset not triggered
    // then stop motos, deliver ball,
    // wait for ball drop,
    // and run motors again.
    if (sensorValue == HIGH) {
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
    if (sensorValue == LOW) {
      delay(resetWaitTime);
      if (sensorValue == LOW) {
        if (ballDelivered) {
          resetTriggered = true;
        }
        if (resetTriggered) {
          ballDelivered = false;
        }
        runMotors();
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
  if(motorDirection) {
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
