github#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Servo Controller 1 ---------------------
int wingLeft = 0;
int wingRight = 1;
int wingMid = 2;
int lEyeX = 3;
int lEyeY = 4;
int lBlinkTop = 5;
int lBlinkBot = 6;
int rEyeX = 7;
int rEyeY = 8;
int rBlinkTop = 9;
int rBlinkBot = 10;
int Neck = 11;
// ----------------------------------------


//timers

uint32_t start=0;
uint32_t timeout=0;
unsigned long previousMillis = 0;
const long interval = 1000;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// these will need to be changed for different servos.
#define SERVOMIN 125
#define SERVOMAX 575


const int numSamples = 10;

// Joystick position storage values
#define joyXPin A0
#define joyYPin A1
#define joySwPin 3
#define modButton1 4
#define modButton2 5
#define modButton3 6
int joyX = 0; 
int joyY = 0;
int joyB = 0;
int mapX;
int mapY;
bool LEDON = false;
bool mod1 = false;
bool mod2 = false;
bool mod3 = false;
bool yHit = false;



//Joy X default home = 507
// Joy Y default home = 508
// Switch Pullup = 1 by default


void setup() {

  Serial.begin(9600);
  
  pwm.begin();
  pwm.setPWMFreq(60);

  pinMode(joySwPin,INPUT_PULLUP);

// you can replace a bunch of these with a for statement, Leave for now so you can configure

// set all WING servos to default positions (0 for now)
    pwm.setPWM(wingLeft,0,angleToPulse(0)); //servo number , board number, angle
    pwm.setPWM(wingRight,0,angleToPulse(0)); //servo number , board number, angle
    pwm.setPWM(wingMid,0,angleToPulse(180)); //servo number , board number, angle (it is reversed)

 // Right Eye
    pwm.setPWM(rEyeX,0,angleToPulse(90)); //servo number , board number, angle
    pwm.setPWM(rEyeY,0,angleToPulse(90)); //servo number , board number, angle
    pwm.setPWM(rBlinkTop,0,angleToPulse(0)); //servo number , board number, angle
    pwm.setPWM(rBlinkBot,0,angleToPulse(0)); //servo number , board number, angle

// Left Eye
    pwm.setPWM(lEyeX,0,angleToPulse(90)); //servo number , board number, angle
    pwm.setPWM(lEyeY,0,angleToPulse(90)); //servo number , board number, angle
    pwm.setPWM(lBlinkTop,0,angleToPulse(0)); //servo number , board number, angle
    pwm.setPWM(lBlinkBot,0,angleToPulse(0)); //servo number , board number, angle

  
  delay(2000);
}

void loop() {

unsigned long currentMillis = millis();
// start timers

// checking the values several times and averaging for smoother results

   //joyX = multisample(joyXPin);
   //joyY = multisample(joyYPin);

   joyX = analogRead(joyXPin);
   joyY = analogRead(joyYPin);
   joyB = digitalRead(joySwPin);
   mod1 = digitalRead(modButton1);
   mod2 = digitalRead(modButton2);
   mod3 = digitalRead(modButton3);

  Serial.print("X = ");
  Serial.println(joyX);
  Serial.print("Y = ");
  Serial.println(joyY);
  //Serial.print("Button = ");
  //Serial.println(joyB);
  //Serial.print("Mod Button = ");
  //Serial.println(mod);
  //Serial.print("LED Button = ");
  //Serial.println(LEDON);
  //Serial.print("xHit: ");
  //Serial.println(xHit);
  //Serial.print("Time difference: ");
  //Serial.println(currentMillis - previousMillis);

//JB();
//WINGX();
//WINGY();
EYES();
}

//|||||||||||||||||||||||||||||||||||||||||||||||||||||JOYSTICK FUNCTIONS|||||||||||||||||||||||||||||||||||||||||||||||

//*************************************************JOYSTICK BUTTON PRESS***************************

void JB(){

  if (joyB == 0 && !LEDON){
    //this will be for some sort of LED function
  
    LEDON = true; //false is set at startup.
      }
  if (joyB == 1 && LEDON){
    //This will stop the LEDS
    
    LEDON = false;
    
}

}

//******************************************************* X AXIS LEFT - WING HORIZONTAL MOVEMENT *************************************
void WINGX(){

//Pressing left on the joystick moves the wings to the side, with granular control


  mapX = map(joyX,0,1023,SERVOMIN,SERVOMAX);
  // mapping the values of the Joystick to the min / max of the Servo
  
//############################you may need to find a way to prevent this going past 90 degrees for hardware safety

  if (mapX <= 330){ // if the joystick goes left..
    pwm.setPWM(wingMid,0,angleToPulse(mapX));
    Serial.println("moving wings");
 }

}
//*************************************************************************************************************************************

//******************************************************* X AXIS Right - WING UP / DOWN ***********************************************
void WINGY(){

// Pressing right on the joystick opens the wings, moving right a second time lowers them.

  static bool wingsOpen = false;
  static bool joystickRight = false;

  const unsigned long triggerThreshold = 355;
  const unsigned long clearThreshold = 350;

  const unsigned long mapX = map(joyX,0,1023,SERVOMIN,SERVOMAX);

  const bool activate = !joystickRight && mapX >= triggerThreshold;

  if (activate){
    joystickRight = true;

    if (!wingsOpen){
      pwm.setPWM(wingLeft,0,angleToPulse(180));
      pwm.setPWM(wingRight,0,angleToPulse(180));
    }
    else {
      pwm.setPWM(wingLeft,0,angleToPulse(0));
      pwm.setPWM(wingRight,0,angleToPulse(0));
    }

   wingsOpen = !wingsOpen;
  
  }

  else if(joystickRight && mapX <= clearThreshold){
      joystickRight = false;
  }
}



//************************************************************** EYES **************************************************************
// This is controlled by the joysticks ntural state. No mod buttons pressed
// left is 0 - mid 508 - right 1023
void EYES(){

   joyX = multisample(joyXPin);
   joyY = multisample(joyYPin);
   joyB = digitalRead(joySwPin);
   // manually mapping angles here seems to fix issue / doesn't pickup servomin/max???
   mapX = map(joyX,0,1023,0,120);
   mapY = map(joyY,0,1023,SERVOMIN,SERVOMAX); 

//HORIZONTAL MOVEMENT
// centered (default position)
if (mapX >=500 && mapX <= 510){
  pwm.setPWM(rEyeX,0,angleToPulse(90));
  pwm.setPWM(lEyeX,0,angleToPulse(90));
}
else {
  // move with Joystick X
  pwm.setPWM(rEyeX,0,angleToPulse(mapX));
  pwm.setPWM(lEyeX,0,angleToPulse(mapX));
}

// VERTICAL MOVEMENT
// centered (default position)
if (mapY >= 500 && mapY <= 510){
  pwm.setPWM(rEyeY,0,angleToPulse(90));
  pwm.setPWM(lEyeY,0,angleToPulse(90));
}
else {
  // move with Joystick Y
  pwm.setPWM(rEyeY,0,angleToPulse(mapY));
  pwm.setPWM(lEyeY,0,angleToPulse(mapY));
}

// Manual Blink
while (joyB == 0){
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  delay(200);
  
  pwm.setPWM(rBlinkTop,0,angleToPulse(0));
  pwm.setPWM(lBlinkBot,0,angleToPulse(0));
  pwm.setPWM(rBlinkTop,0,angleToPulse(0));
  pwm.setPWM(lBlinkBot,0,angleToPulse(0));
  delay(100); 
}

}
//********************************************************* BLINK ***********************************************************************
void blink() {
  if(millis() - start >= timeout) {
    start = millis();
 timeout = random(5000,15000); // between 5 - 15 seconds;
  
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  delay(200);
  
  pwm.setPWM(rBlinkTop,0,angleToPulse(0));
  pwm.setPWM(lBlinkBot,0,angleToPulse(0));
  pwm.setPWM(rBlinkTop,0,angleToPulse(0));
  pwm.setPWM(lBlinkBot,0,angleToPulse(0));
  delay(100); 
  }
}


  
int multisample(int pin) {
  int total = 0;
  
  for (int i = 0; i < numSamples; i++) {
    total += analogRead(pin);
    delay(1);  // You can adjust this delay if needed
  }

  return total / numSamples;
}

int angleToPulse(int ang){
   int pulse = map(ang,0, 180, SERVOMIN,SERVOMAX);// map angle of 0 to 180 to Servo min and Servo max 
  // Serial.print("Angle: ");Serial.print(ang);
//   Serial.print(" pulse: ");Serial.println(pulse);
   return pulse;
}
