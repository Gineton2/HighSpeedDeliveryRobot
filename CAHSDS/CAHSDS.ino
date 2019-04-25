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
int closed = 0;
int opened = 180;

// Directions for wheels.
#define FORWARD 1
#define REVERSE 0

#define MOTOR_R 5
#define MOTOR_L 4

#define GATE 9

// photoresistor uses 10k resistor on ground to work
int photoResistor = A2;

int sensorValue;

// value for white cardboard is ~790
// value for dark cardboard is ~650


void setup() {
  Serial.begin(9600);
  
  // Setup motors to output
  pinMode(MOTOR_R, OUTPUT);
  pinMode(MOTOR_L, OUTPUT);

  //photoresistor
  //pinMode(photoResistor, INPUT);
  
  // micro-servo
  gate.attach(GATE);
}

void loop() {
  //digitalWrite(MOTOR_R, HIGH);
  //analogWrite(MOTOR_R, HIGH);
  
  sensorValue = analogRead(photoResistor);
  
  Serial.println(sensorValue);  
  
  if ( sensorValue > 700) {
    gate.write(closed);
  } else {
    for (pos = closed; pos <= opened; pos += 1) {
      gate.write(pos);
      delay(13);
    }
  }
  delay(200);
  

}

void setupMotors() {
}
