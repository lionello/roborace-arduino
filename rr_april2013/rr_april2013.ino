#include <Servo.h>

#include "Motor.h"
#include "Window.h"
#include "UltrasonicSensor.h"

#include "PID_v1.h"
double Setpoint = 30, Input=0, Output;
PID myPID(&Input, &Output, &Setpoint, 2, 0.1, 0.1, DIRECT);

Servo steering;
Motor motor;
UltrasonicSensor front, right;

// Pin declaration
const int Motor1A = 9;
const int Motor1B = 10;
const int Motor1V = 11;
const int SensorFrontTrig = 4;  // transmission pin
const int SensorFrontEcho = 2; // receiving pin
const int SensorRightTrig = 5;
const int SensorRightEcho = 3;
const int Servo1 = 6;


const int SteeringAmount = 30;
const int SteeringExtreme = 40;
const int SteeringNeutral = 70;
const int MaxSpeed = 150;
const int MinSpeed = 100;

void setup() { 
  // initialize serial communication:
  Serial.begin(115200);
  
  pinMode(SensorFrontTrig, OUTPUT);
  pinMode(SensorFrontEcho, INPUT);
  pinMode(SensorRightTrig, OUTPUT);
  pinMode(SensorRightEcho, INPUT);

  motor.attach(Motor1A, Motor1B, Motor1V);
  
  steering.attach(Servo1);  // attaches the servo on pin 9 to the servo object 
  
  right.attach(SensorRightEcho, SensorRightTrig);
  front.attach(SensorFrontEcho, SensorFrontTrig);

  //turn the PID on
  myPID.SetOutputLimits(-SteeringAmount, SteeringAmount);
  myPID.SetMode(AUTOMATIC);

  motor.setSpeed(MinSpeed);
  motor.fwd();
}

Window rightw, frontw;

void loop() 
{
  if (front.sample() > 0)
    frontw.add(front.distanceCM());
    
  if (right.sample() > 0)
    rightw.add(right.distanceCM());

  int distF = front.distanceCM();
  int distR = right.distanceCM();
  //int distF = frontw.avg();
  //int distR = rightw.avg();
  Serial.println(distF);
  Serial.println(distR);
  
  if (distF < 20) {
    Serial.println("backing up...");
    steering.write(SteeringNeutral + SteeringExtreme);
    motor.lock();//fullstop
    delay(100);
    motor.setSpeed(MinSpeed);
    motor.rev();
  }
  else if (distF < 40)
  {
    motor.fwd();
    Serial.println("easy now...");
    motor.setSpeed(MinSpeed);
    steering.write(SteeringNeutral - SteeringExtreme);
  }
  else
  {
    motor.fwd();
    motor.setSpeed(MaxSpeed);
    Input = distR;
    myPID.Compute();
    Serial.println(Output);
    steering.write(Output + SteeringNeutral);
  }

  delay(50);
}



