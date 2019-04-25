/* California High Speed Rail Delivery System
 * by: Gineton Alencar
 * date: 04/16/2019
 * license: TBD
 * 
 * 
 */

#include <Servo.h>

Servo gate;

int pos = 0;
const int closed = 0;
const int opened = 165;
const int ballDeliveryTime = 700;

// Directions for wheels.
#define FORWARD 1
#define REVERSE 0

#define MOTOR_R 5
#define MOTOR_L 4

#define GATE 9

// photoresistor uses 10k resistor on ground to work
// OSOYOO light dark sensor comes with pot for sensitivity
#define SENSOR 2

bool sensorValue;

bool ballDelivered;

bool resetTriggered;

// value for white is LOW
// value for black is HIGH


void setup() {
  Serial.begin(9600);
  
  // Setup motors to output
  pinMode(MOTOR_R, OUTPUT);
  pinMode(MOTOR_L, OUTPUT);

  // Light-dark sensor
  pinMode(SENSOR, INPUT);
  
  // micro-servo
  gate.attach(GATE);

  ballDelivered = false;
  resetTriggered = true;
}

void loop() {
  //digitalWrite(MOTOR_R, HIGH);
  //analogWrite(MOTOR_R, HIGH);
  
  sensorValue = digitalRead(SENSOR);
  //gate.write(closed);
  Serial.println(sensorValue);  
  // Black detected == HIGH
  if (sensorValue == HIGH) {
    if(!ballDelivered && resetTriggered) {
      deliverBall();
    }
    //resetTriggered = false;
  }
  if (sensorValue == LOW) {
    if (ballDelivered) {
      resetTriggered = true;
    }
    if (resetTriggered) {
      ballDelivered = false;
    }
  }
  // if ballDelivered once
  //  keep gate.write(closed)
  //  until sensorValue detects "white" (low)
  //  then reset
}

void deliverBall() {
    gate.write(opened);
    delay(ballDeliveryTime);
    gate.write(closed);
    ballDelivered = true;
}

void setupMotors() {
}
