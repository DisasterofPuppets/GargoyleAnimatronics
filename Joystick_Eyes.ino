/*
 * With the servo wiring pointing to the Right
 * 0 degrees - max anti clockwise
 * 90 degrees - neutral aka middle
 * 180 degrees - max clockwise
 * 
 * Joystick held with connectors to the left
 * 
 * Left = 0 
 * Right = 1023
 * Up = 0
 * Down = 1023
 * 
 * SERVO SETUP
 *  !! - Reverse direction of counterpart
 * EYELIDS
 * 0 - Top left Eyelid 
 * 1 - Bottom Left Eyelid 
 * 2 - Top Right Eyelid 
 * 3 - Bottom Right Eyelid
 * 
 * LEFT EYE
 * 4 - Up
 * 5 - Down !!
 * 6 - Left
 * 7 - Right !!
 * 
 * RIGHT EYE
 * 8 - Up
 * 9 - Down !!
 * 10 - Left
 * 11 - Right !!
 *
 * 12
 * 13
 * 14
 * 15
 
 */

// Micro Servo Position confirmation
//Disaster of Puppets 2020
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define joyX A0 // X pin input
#define joyY A1 // X pin input

// used to store Joystick mapping
int xPos;
int yPos;
const int switchPin = 3; // digital pin for switch input
int buttonState = 0;
int righteyeX;
int lefteyeX;
int righteyeY;
int lefteyeY;
int pos = 0;
int monitoring = 1; //1 = off


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  620 // this is the 'maximum' pulse length count (out of 4096)


void setup(){

          Serial.begin(115200);
          pwm.begin();
            
          pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
          
          pinMode(joyX, INPUT);
          pinMode(joyY, INPUT);
          digitalWrite(switchPin, HIGH);
          
          //  set all to home positions

          // Eyelids set to open
          pwm.setPWM(0,0,hometoAngle(0));
          pwm.setPWM(1,0,hometoAngle(0));
          pwm.setPWM(2,0,hometoAngle(0));
          pwm.setPWM(3,0,hometoAngle(0));

          // Left Eye set to middle
          pwm.setPWM(4,0,hometoAngle(90));
          pwm.setPWM(5,0,hometoAngle(90));
          pwm.setPWM(6,0,hometoAngle(90));
          pwm.setPWM(7,0,hometoAngle(90));

          // Right Eye set to middle
          pwm.setPWM(8,0,hometoAngle(90));
          pwm.setPWM(9,0,hometoAngle(90));
          pwm.setPWM(10,0,hometoAngle(90));
          pwm.setPWM(11,0,hometoAngle(90));
          
          
          delay(100);
          
}

void loop(){

//monitoring
if (monitoring== 0){        
        Serial.print("X : ");
        Serial.println(analogRead(joyX));
        Serial.print("Button: ");
        Serial.println(digitalRead(switchPin));
        Serial.println( "================" );  // Carriage Return
     
}       
// end monitoring


if (digitalRead(switchPin)== 0){
  blink();
}
       
// get the joystick position
        
int xPos = analogRead(joyX); // reads X position
int yPos = analogRead(joyY); // reads Y position
        
// Map the joystick position to the servo angle
        
lefteyeX = map(xPos,0,1023,125,620); // Maps the Joystick X reading to servo angle 
lefteyeY = map(yPos,0,1023,125,620); // Maps the Joystick X reading to servo angle 
righteyeX = map(xPos,0,1023,125,620); // Maps the Joystick X reading to servo angle 
righteyeY = map(yPos,0,1023,125,620); // Maps the Joystick X reading to servo angle 


// Left Eye movement
   pwm.setPWM(4,0,leftY(lefteyeY)+90); // Up
   pwm.setPWM(5,0,reverse(lefteyeY)+90); // Down
   pwm.setPWM(6,0,leftX(lefteyeX)+90); // L
   pwm.setPWM(7,0,reverse(lefteyeX)+90); // R

// Right Eye movement
   pwm.setPWM(8,0,leftY(lefteyeY)+90); // Up
   pwm.setPWM(9,0,reverse(lefteyeY)+90); // Down
   pwm.setPWM(10,0,leftX(lefteyeX)+90); // L
   pwm.setPWM(11,0,reverse(lefteyeX)+90); // R
   
   

  if (monitoring == 0){
        
        Serial.print("Right Eye X : ");
        Serial.println(righteyeX);
  }

}

int hometoAngle(int ang){
  int pulse = map(ang,0,180, SERVOMIN,SERVOMAX);
  return pulse;
}

//right eye servo min max mapping left and right
int rightX(int ang2){
  int pulse = map(ang2,0,180,50,150);
  return pulse;
}
//reverseeye servo min max mapping left and right
int reverse(int ang6){
  int pulse = 620 - map(ang6,0,180,50,150);
  return pulse;
}

//lefteye servo min max mapping left and right
int leftX(int ang4){
  int pulse = map(ang4,0,180,50,150);
  return pulse;
}

//lefteye servo min max mapping left and right
int leftY(int ang8){
  int pulse = map(ang8,0,180,50,150);
  return pulse;
}


void blink(){
  //left lids
  pwm.setPWM(0,0,hometoAngle(180));   
  pwm.setPWM(1,0,hometoAngle(180));
  //right lids
  pwm.setPWM(2,0,hometoAngle(180));
  pwm.setPWM(3,0,hometoAngle(180));   
  delay(100);
  pwm.setPWM(0,0,hometoAngle(0));   
  pwm.setPWM(1,0,hometoAngle(0));
  pwm.setPWM(2,0,hometoAngle(0));   
  pwm.setPWM(3,0,hometoAngle(0));
}
