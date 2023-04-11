#include "FastAccelStepper.h" //https://github.com/gin66/FastAccelStepper/blob/master/src/FastAccelStepper.h

//for fastaccelstepper
#define stepPinStepper 9// for avr: either use pin 9 or 10 aka OC1A or OC1B
#define dirPinStepper 10 //#define enablePinStepper 26
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;
// for making PWM in 

int ANALOG_IN = A2; //  analog input pin for reading the control voltage from PWM D11 NANO to run spindle with stepper driver


int speed; 

//sampelrate
#define PERIOD 500000 //period in us 1/1000000 seconds
unsigned long last_us = 0L ;

void setup() {
   Serial.begin(115200);
   //pinMode(ANALOG_IN, INPUT);
   //digitalWrite(ANALOG_IN, LOW);
   engine.init();
   stepper = engine.stepperConnectToPin(stepPinStepper);
}
//thingas to do , set threshold for pot to make rpm stable and stop is not 0 rpm at moment 
// stepper in separate void ?? 
// snygga till samplearte
void loop() {
  if (micros() - last_us > PERIOD) //samplerate on PWM in
 {
  last_us += PERIOD ;
  speed = map(analogRead(ANALOG_IN),0, 1023, 0, 4000); // 0 speed to 4000Hz 
  
 }
  if(stepper){
      stepper->setDirectionPin(dirPinStepper);
      stepper->setAutoEnable(true);
      stepper->setSpeedInHz(speed); // 4000Hz 150acceleration and 20v 2.6A no micro stepping gives 1200 rpm nema23
      stepper->setAcceleration(150); // was 150 3
      stepper->runForward();
      Serial.println((stepper->getCurrentSpeedInMilliHz()/3000)); //rpm= 1000uS/200steps*60sec

  }


}