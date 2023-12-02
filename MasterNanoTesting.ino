// Smoothing obtained from XRobots https://github.com/XRobots/ServoSmoothing
// https://youtu.be/jsXolwJskKM#include 

//if you have trouble uploading, disconnect the servo board from external power

#include <Adafruit_PWMServoDriver.h>

// set to flase to stop serial output
// make sure this is off so everything responds in realtime
const bool debug = false;

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
#define SERVOMAX 620


const int numSamples = 10;

// Joystick position storage values
#define joyXPin A0
#define joyYPin A1
#define joySwPin 3
#define modButton1 4
#define modButton2 5
#define modButton3 6
int joyX; 
int joyY;
int mapX;
int mapY;
float xSmoothed;
float ySmoothed;
float xPrev;
float yPrev;
bool joyB = false;
bool wake = false;
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
    pwm.setPWM(rBlinkTop,0,angleToPulse(180)); //servo number , board number, angle
    pwm.setPWM(rBlinkBot,0,angleToPulse(180)); //servo number , board number, angle

// Left Eye
    pwm.setPWM(lEyeX,0,angleToPulse(90)); //servo number , board number, angle
    pwm.setPWM(lEyeY,0,angleToPulse(90)); //servo number , board number, angle
    pwm.setPWM(lBlinkTop,0,angleToPulse(180)); //servo number , board number, angle
    pwm.setPWM(lBlinkBot,0,angleToPulse(180)); //servo number , board number, angle

  
  delay(2000);
}

void loop() {

unsigned long currentMillis = millis();
// start timers

// Eyelids are closed on initiation, clicking push button once will open them..
// get the joystick button State

/*
if (digitalRead(joySwPin)){ // on first press of thejoystick button
  if (wake = false){
    wake = !wake; //set wake to true /opposite of existing status
    RISEANDSHINE(); //run the wake up function.
  }
  else if (wake){    // the above statement has already run, Gargoyle will be awake, so randomly run the Blink function.
    BLINK();
  }
}
*/
// checking the values several times and averaging for smoother results

   //joyX = multisample(joyXPin);
   //joyY = multisample(joyYPin);

   //joyX = analogRead(joyXPin);
   //joyY = analogRead(joyYPin);
   //joyB = digitalRead(joySwPin);
   //mod1 = digitalRead(modButton1);
  // mod2 = digitalRead(modButton2);
   //mod3 = digitalRead(modButton3);

  //Serial.print("X = ");
  //Serial.println(joyX);
  //Serial.print("Y = ");
  //Serial.println(joyY);
  //Serial.print("Button = ");
  //Serial.println(joyB);
  //Serial.print("Mod Button 1 = ");
  //Serial.println(mod1);
  //Serial.print("Mod Button 2 = ");
  //Serial.println(mod2);
  //Serial.print("Mod Button 3 = ");
  //Serial.println(mod3);
  //Serial.print("LED Button = ");
  //Serial.println(LEDON);
  //Serial.print("xHit: ");
  //Serial.println(xHit);
  //Serial.print("Time difference: ");
  //Serial.println(currentMillis - previousMillis);


///////////////////////////////////////////////////// T E S T // C O D E ///////////////////////////////

EYES();

//////////////////////////////////////////// E N D // T E S T // C O D E ///////////////////////////////

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
  
  joyX = multisample(joyXPin);

// mapping the values of the Joystick to the min / max of the Servo
  mapX = map(joyX,0,1023,SERVOMIN,SERVOMAX);
  

  xSmoothed = (mapX *0.05) + (xPrev * 0.95);
  xPrev = xSmoothed;

  if (debug){ // only shows text if debugging is on
    Serial.print("mapX = ");
    Serial.println(mapX);
    delay(500);
  }

// this value needs to be just below the neutral position

  if (joyX <= 490){ // if the joystick goes left..
    pwm.setPWM(wingMid,0,angleToPulse(xSmoothed));
    if (debug){ //only prints if debug is true
      Serial.println("moving wings");
    }
 }

}
//*************************************************************************************************************************************

//******************************************************* JOYSTICK X AXIS RIGHT - TOGGLES WING UP/DOWN ********************************
void WINGY(){
  
  static bool wingsOpen = false;
  static bool joystickRight = false;

  const unsigned long triggerThreshold = 510;
  const unsigned long clearThreshold = 490;

  joyX = multisample(joyXPin);

  const unsigned long mapX = map(joyX,0,1023,SERVOMIN,SERVOMAX);
  const bool activate = !joystickRight && mapX >= triggerThreshold;

if (debug){ // only shows text if debugging is on
  Serial.print("mapX = ");
  Serial.println(mapX);
  Serial.print("wings Open = ");
  Serial.println(wingsOpen);
  Serial.print("Activate = ");
  Serial.println(activate);
  Serial.print("joystickRight = ");
  Serial.println(joystickRight);
  delay(500);
}
    

  if (activate){
    joystickRight = true;

    if (!wingsOpen){
      pwm.setPWM(wingLeft,0,angleToPulse(180));
      pwm.setPWM(wingRight,0,angleToPulse(180));
      if (debug){ // only shows text if debugging is on
        Serial.println("Wings Opened");
      }
    }
    else {
      pwm.setPWM(wingLeft,0,angleToPulse(0));
      pwm.setPWM(wingRight,0,angleToPulse(0));
      if (debug){ // only shows text if debugging is on
        Serial.println("Wings Closeded");
      }
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

if (debug){ // only shows text if debugging is on
  Serial.print("mapX = ");
  Serial.println(mapX);
  Serial.print("mapY = ");
  Serial.println(mapY);
  Serial.print("joyB = ");
  Serial.println(joyB);
  delay(500);
}
   
   
   mapX = map(joyX,0,1023,SERVOMIN,SERVOMAX);
   mapY = map(joyY,0,1023,SERVOMIN,SERVOMAX); 

   xSmoothed = (mapX *0.05) + (xPrev * 0.95);
   ySmoothed = (mapY *0.05) + (yPrev * 0.95);

   xPrev = xSmoothed;
   yPrev = ySmoothed;

// Left Eye movement
   pwm.setPWM(lEyeX,0,angleToPulse(xSmoothed)+90); // Left Right
   pwm.setPWM(lEyeY,0,angleToPulse(ySmoothed)+90); // Up Down
   
 // Right Eye movement
   pwm.setPWM(rEyeX,0,angleToPulse(xSmoothed)+90); // Left Right
   pwm.setPWM(rEyeY,0,angleToPulse(ySmoothed)+90); // Up Down

// individual eye movement
   if (modButton1){ // if modeButton1 is held aka true
      pwm.setPWM(lEyeX,0,angleToPulse(xSmoothed)+90); //Left and right
      pwm.setPWM(lEyeY,0,angleToPulse(xSmoothed)+90); //Left and right
   }

// Manual Blink
while ((joyB) && (wake)){ // while both wake and joybutton state is true
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  delay(200);
  
  pwm.setPWM(rBlinkTop,0,angleToPulse(135));
  pwm.setPWM(lBlinkBot,0,angleToPulse(145));
  pwm.setPWM(rBlinkTop,0,angleToPulse(135));
  pwm.setPWM(lBlinkBot,0,angleToPulse(145));
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

void RISEANDSHINE(){
  
  //open
  pwm.setPWM(lBlinkTop,0,angleToPulse(170));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(170));
  pwm.setPWM(rBlinkTop,0,angleToPulse(170));
  pwm.setPWM(rBlinkBot,0,angleToPulse(170));   
  delay(200);
    //open
  pwm.setPWM(lBlinkTop,0,angleToPulse(160));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(160));
  pwm.setPWM(rBlinkTop,0,angleToPulse(160));
  pwm.setPWM(rBlinkBot,0,angleToPulse(160));   
  delay(300);
  //close
  pwm.setPWM(lBlinkTop,0,angleToPulse(180));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(rBlinkBot,0,angleToPulse(180));   
  delay(900);
  //open
  pwm.setPWM(lBlinkTop,0,angleToPulse(160));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(160));
  pwm.setPWM(rBlinkTop,0,angleToPulse(160));
  pwm.setPWM(rBlinkBot,0,angleToPulse(160));    
  delay(200);
  //open
  pwm.setPWM(lBlinkTop,0,angleToPulse(150));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(150));
  pwm.setPWM(rBlinkTop,0,angleToPulse(150));
  pwm.setPWM(rBlinkBot,0,angleToPulse(150));    
  delay(300);
  //close
  pwm.setPWM(lBlinkTop,0,angleToPulse(180));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(rBlinkBot,0,angleToPulse(180));   
  delay(700);
  //open
  pwm.setPWM(lBlinkTop,0,angleToPulse(150));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(150));
  pwm.setPWM(rBlinkTop,0,angleToPulse(150));
  pwm.setPWM(rBlinkBot,0,angleToPulse(150));   
  delay(600);
   //open
  pwm.setPWM(lBlinkTop,0,angleToPulse(155));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(155));
  pwm.setPWM(rBlinkTop,0,angleToPulse(155));
  pwm.setPWM(rBlinkBot,0,angleToPulse(155));   
  delay(50);
    //open wider
  pwm.setPWM(lBlinkTop,0,angleToPulse(150));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(150));
  pwm.setPWM(rBlinkTop,0,angleToPulse(150));
  pwm.setPWM(rBlinkBot,0,angleToPulse(150));   
  delay(300);
   //close
  pwm.setPWM(lBlinkTop,0,angleToPulse(180));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(rBlinkBot,0,angleToPulse(180));    
  delay(100);
    //open wider
  pwm.setPWM(lBlinkTop,0,angleToPulse(135));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(145));
  pwm.setPWM(rBlinkTop,0,angleToPulse(135));
  pwm.setPWM(rBlinkBot,0,angleToPulse(145));   
  delay(300);
}

void BLINK(){
  //left lids
  pwm.setPWM(lBlinkTop,0,angleToPulse(180));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(180));
  //right lids
  pwm.setPWM(rBlinkTop,0,angleToPulse(180));
  pwm.setPWM(rBlinkBot,0,angleToPulse(180));   
  delay(50);
  //open again
  pwm.setPWM(lBlinkTop,0,angleToPulse(135));   
  pwm.setPWM(lBlinkBot,0,angleToPulse(145));
  pwm.setPWM(rBlinkTop,0,angleToPulse(135));   
  pwm.setPWM(rBlinkBot,0,angleToPulse(145));
}
