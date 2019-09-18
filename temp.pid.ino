#include <PID_v1.h>
#include "max6675.h"

// Max6675 Module Pins
#define ktcSO 8
#define ktcCS 9
#define ktcCLK 10

// MAX6675 Module Instance
MAX6675 ktc(ktcCLK, ktcCS, ktcSO);

// Relay Pins
#define FRelay 6
#define LRelay 7

// Relay Variables
int WindowSize = 4000;
unsigned long windowStartTime;

// PID Variables
double Setpoint, Input, Output;
double Kp = 1000,Ki = 100, Kd = 50;

// PID Instance
PID Temp(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// unsigned long millitime = millis();

/*****************************************************************/

void setup() {
  Serial.begin(9600);

  // Pins Initialization
  pinMode(FRelay, OUTPUT);
  pinMode(LRelay, OUTPUT);
  
  // PID Initialization
  Temp.SetOutputLimits(0, WindowSize);
  Temp.SetMode(AUTOMATIC);
  Setpoint = 40;
  
  // give the MAX a little time to settle
  delay(500);
}

void loop() {
  // Setting Input
  Input = ktc.readCelsius();

  // Computing Output
  Temp.Compute();

  //time to shift the Relay Window
  unsigned long now = millis();
  if (now - windowStartTime > WindowSize)
  {
    windowStartTime += WindowSize;
  }
  
  // Setting Output
  if (Output < now - windowStartTime) 
  {
    digitalWrite(FRelay, HIGH);
    digitalWrite(LRelay, LOW);
  }
  else
  {
    digitalWrite(FRelay, LOW);
    digitalWrite(LRelay, HIGH);
  }

  //if (((millis() - millitime) >= 1000) && ((millis() - millitime) <= 1010))
  //{   
    Serial.print(Input);
    Serial.print(" ");
    Serial.println(Output);
    Serial.print(" ");  
    Serial.println(Setpoint);
  //}
  delay(500);
}
