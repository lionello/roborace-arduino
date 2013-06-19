#ifndef UltrasonicSensor_h
#define UltrasonicSensor_h

#include "Arduino.h"

struct UltrasonicSensor
{ 
  UltrasonicSensor()
  {
    _start = _duration = 0;
  }
  
  void attach(int echopin, int trigpin)
  {
    _echo = echopin;
    _trig = trigpin;
    pinMode(_echo, INPUT);
    pinMode(_trig, OUTPUT);
    digitalWrite(_trig, LOW);
  }
  
  bool useIRQ()
  {
    if (_echo == 2) {
      irq0sensor = this;
      attachInterrupt(0, irq0, FALLING);
      return true;
    }
    else if (_echo == 3) {
      irq1sensor = this;
      attachInterrupt(1, irq1, FALLING);
      return true;
    }
    return false;
  }
  
  void trigger()
  {
    pulse(_trig, &_start);
  }
 
  int sample()
  { 
    trigger();
    int us = pulseIn(_echo, HIGH, MaxDuration);
    if (us == 0)
      us = MaxDuration;
    return addSample(us);
  }

  // convert the time into a distance
  int distanceCM() { return microsecondsToCentimeters(_duration); }
  int distanceInch() { return microsecondsToInches(_duration); }

  static int microsecondsToInches(int microseconds)
  {
    // According to Parallax's datasheet for the PING))), there are
    // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
    // second).  This gives the distance travelled by the ping, outbound
    // and return, so we divide by 2 to get the distance of the obstacle.
    // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
    return microseconds / 74 / 2;
  }
  
  static int microsecondsToCentimeters(int microseconds)
  {
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    return microseconds / 29 / 2;
  }
  
private:
  static UltrasonicSensor *irq0sensor, *irq1sensor;
  static const int MaxDuration = 20000;    // microseconds/us

  int _echo, _trig;
  volatile int _duration;
  volatile long _start;
  
  int addSample(int value)
  {
    if (value > 0)
    // TODO: use window
      _duration = value;
    return _duration;
  }
  
  void irq(long time)
  {
    addSample(time - _start - TimeOffset);
    _start = 0;
  }
  
  static void irq0()
  {
    long now = micros();
    irq0sensor->irq(now);
  }
  
  static void irq1()
  {
    long now = micros();
    irq1sensor->irq(now);
  }
  
  static const int TimeOffset = -10; // 10us too much TBD
  
  static void pulse(int pin, volatile long* timer)
  {
    *timer = micros();
    digitalWrite(pin, LOW);      // needed? TBD
    delayMicroseconds(2);
    digitalWrite(pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pin, LOW);
  }
};

#endif//UltrasonicSensor_h

