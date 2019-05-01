/* California High Speed Rail Delivery System
 * by: Gineton Alencar
 * date: 04/16/2019
 * license: TBD
 * 
 * 
 */

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1 and M2
Adafruit_DCMotor *motorOne = AFMS.getMotor(1);
Adafruit_DCMotor *motorTwo = AFMS.getMotor(2);

Servo gate;

int pos = 0;
const int closed = 0;
const int opened = 165;
const int ballDeliveryTime = 700;

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
  
  // Light-dark sensor
  pinMode(SENSOR, INPUT);
  
  // micro-servo
  gate.attach(GATE);

  // Motorshield
  AFMS.begin();  // create with the default frequency 1.6KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  
  // motorOne
  motorOne->setSpeed(150);
  motorOne->run(FORWARD);
  // turn on motor
  motorOne->run(RELEASE);

  // motorTwo
  motorTwo->setSpeed(150);
  motorTwo->run(FORWARD);
  // turn on motor
  motorTwo->run(RELEASE);
  
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
      motorOne->setSpeed(0);
      motorTwo->setSpeed(0);
      deliverBall();
    }
  
  motorOne->setSpeed(255);
  motorTwo->setSpeed(255);
  
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
