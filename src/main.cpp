//for display
#include <Wire.h>
#include <LiquidCrystal_I2C.h> //http://wiki.sunfounder.cc/index.php?title=I2C_LCD2004#Step_2:Add_library
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//for Menu & Buttons https://www.youtube.com/watch?v=qrziemoqjlM
enum pageType (root_menu, run_as_lathe, run_as_step); // all menu page options 
enum pageType currPage = root_menu; // holds current page
// constant port address
const int btn_accept = D8 ; // D8 is K on shield
const int btn_up = D2 ; // D2 is A on shiled 
const int btn_dwn = D4 ; // D4 is C on shield 
const int btn_cancel = D7 ; // D7 is E on shield 



//for fastaccelstepper
#include "FastAccelStepper.h" //https://github.com/gin66/FastAccelStepper/blob/master/src/FastAccelStepper.h
#define stepPinStepper 9// for avr: either use pin 9 or 10 aka OC1A or OC1B
#define dirPinStepper 10 //#define enablePinStepper 26
FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;
// for making PWM in 
int ANALOG_IN = A2; //  analog input pin for reading the control voltage from PWM D11 NANO to run spindle with stepper driver
int speed; // map analog to digital 

//sampelrate for PWM
#define PERIOD 500000 //period in us 1/1000000 seconds
unsigned long last_us = 0L ;

void setup() {
   Wire.begin();
   Serial.begin(115200);
   //pinMode(ANALOG_IN, INPUT);
   //digitalWrite(ANALOG_IN, LOW);
   
   //for stepper driver
   engine.init();
   stepper = engine.stepperConnectToPin(stepPinStepper);
   //for display
   lcd.init();  //initialize the lcd
   lcd.backlight();  //open the backlight 
 

}

// Forward Declarations 
void pwmToStepper(); // Driver 
void LCD();  // you guessed it ! 
void menu(); // menu and buttons

//thingas to do , set threshold for pot to make rpm stable and stop is not 0 rpm at moment 
// stepper in separate void ?? 
//LCD in separate void 
// snygga till samplearte
void loop() {
  if (micros() - last_us > PERIOD) //samplerate on PWM in
 {
  last_us += PERIOD ;
  speed = map(analogRead(ANALOG_IN),0, 1023, 0, 4000); // 0 speed to 4000Hz 
  LCD(); // update LCD 
 }
 pwmToStepper();
 menu();

}

void pwmToStepper(){ // Driver 
if(stepper){
      stepper->setDirectionPin(dirPinStepper);
      stepper->setAutoEnable(true);
      stepper->setSpeedInHz(speed); // 4000Hz 150acceleration and 20v 2.6A no micro stepping gives 1200 rpm nema23
      stepper->setAcceleration(150); // was 150 3
      stepper->runForward();
      Serial.println((stepper->getCurrentSpeedInMilliHz()/3000)); //rpm= 1000uS/200steps*60sec
  }
}

void LCD(){
  lcd.setCursor ( 10, 0 );            // go to the top left corner
  lcd.print(    (stepper->getCurrentSpeedInMilliHz()/3000)); // write this string on the top row
  lcd.setCursor ( 0, 1 );            // go to the 2nd row
  lcd.print("   some text  "); // pad string with spaces for centering
  lcd.setCursor ( 0, 2 );            // go to the third row
  lcd.print("  even more text   "); // pad with spaces for centering
  lcd.setCursor ( 0, 3 );            // go to the fourth row
  lcd.print(" even more more text ");

}

void menu(){
  switch (currPage){
    case root_menu: page_root_menu(); break; //welcome splash 
    case run_as_lathe: page_run_as_lathe(); break; // X , Z axis in stepping mode and C axis rotating as spindle. 
    case run_as_step: page_run_as_step(); break; // X , Z , C in stepping increment mode.
  }
}