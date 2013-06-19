#include <Servo.h>

const int trig = 7;
const int echo = 8;

unsigned long duration, t, start;
int angle;
Servo myservo;

void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  myservo.attach(9);
  myservo.write(angle);
  delay(500);//let servo turn to zero
  start = millis();
  angle = 0;
}


void loop()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH, 600000);
  
  Serial.print(angle);
  Serial.print(',');
  Serial.print(duration);
  Serial.println();
  
  angle = ((millis() - start) % 3600) / 10 ;
  if(angle > 180) angle = 360 - angle;
  myservo.write(angle);
}
