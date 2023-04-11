#include <Arduino.h>
#include <AccelStepper.h> // https://www.airspayce.com/mikem/arduino/AccelStepper/index.html

// Driver accelstepper https://www.ee-diary.com/2021/08/arduino-stepper-motor-speed-control.html
int ANALOG_IN = A2; //  analog input pin for reading the control voltage from PWM D11 NANO to run spindle with stepper driver
const int STEP_PIN = 9;      // Step CLK+
const int DIR_PIN = 10;      // Dir CW+
const unsigned int rpmMin = 0; // edit for max min rpm settings
const unsigned long rpmMax = 100;
const unsigned int stepSize = 16; // set stepsize on your stepper driver 1-2-8-16 steps  
const unsigned int stepMin = ((rpmMin*200/60)*stepSize); //200 steps = 1 rev, check your stepper spec. 
const unsigned long stepMax = ((rpmMax*200/60)*stepSize);
int speed; 
//sampelrate
#define PERIOD 500000 //period in us 1/1000000 seconds
unsigned long last_us = 0L ;

//AccelStepper stepper = AccelStepper(stepper.DRIVER, STEP_PIN, DIR_PIN); 
AccelStepper stepper(stepper.DRIVER, STEP_PIN, DIR_PIN); 


void setup() {
  Serial.begin(115200);
// Driver Setup 
  stepper.setMaxSpeed(stepMax); // steps/s Tune for your setup.  
  stepper.setAcceleration(50); // steps/s^2 Tune for your setup. 
}

// Forward Declarations 
//void pwmToStepper(); // Driver 



// MAIN FUNCTION 
void loop() {
 
 if (micros() - last_us > PERIOD)
 {
  last_us += PERIOD ;
  speed = map(analogRead(ANALOG_IN), 0, 1023, stepMin, stepMax);
  
 }
  //pwmToStepper();
// this works  
 // speed = map(analogRead(ANALOG_IN), 0, 1023, stepMin, stepMax);  //map() uses integers no float 
  //stepper.move(1); // was 100 
  stepper.setSpeed(speed);
   stepper.runSpeed();
 
// // https://hackaday.io/project/183279-accelstepper-the-missing-manual/details
// void pwmToStepper(){ 

 }


