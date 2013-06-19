#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

// Motor shield class
struct Motor
{
  void attach(int aa, int bb, int vv) 
  { 
    a=aa;
    b=bb;
    v=vv; 
    pinMode(a, OUTPUT); 
    pinMode(b, OUTPUT);
    pinMode(v, OUTPUT);
  }
  
  // Set the speed (using an RPM pin)
  void setSpeed(int x)
  {
    analogWrite(v, x);
  }
  
  // Lock the wheels
  void lock()
  {
    digitalWrite(a, HIGH); 
    digitalWrite(b, HIGH);   
  }
  
  // Reverse
  void rev()
  {
    digitalWrite(a, HIGH); 
    digitalWrite(b, LOW);   
  }
  
  // Forward
  void fwd()
  {
    digitalWrite(a, LOW); 
    digitalWrite(b, HIGH);   
  }
  
  // Free rolling
  void free()
  {
    digitalWrite(a, LOW); 
    digitalWrite(b, LOW);   
  }
  
private:
  int a,b,v;
};

#endif//Motor_h

