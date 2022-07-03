/*
 * SERVO SETUP
 * 
 * EYEBALLLS
 * 0 - Top left Eyelid 
 * 1 - Bottm Left Eyelid
 * 2 - Left Eyeball
 * 3 - Top right Eyelid 
 * 
 * EYELIDS
 * 4 - Bottom right Eyelid 
 * 5 - Right Eyeball
 * 
 */


// Micro Servo Position confirmation
//Disaster of Puppets 2020
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define joyX A0 // X pin input

// used to store Joystick mapping
int xPos;
const int switchPin = 3; // digital pin for switch input
int buttonState = 0;
int righteyeX;
int lefteyeX;
int pos = 0;
int monitoring =1; //off


// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  620 // this is the 'maximum' pulse length count (out of 4096)


void setup(){
          pwm.begin();
            
          pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
          
          pinMode(joyX, INPUT);
          digitalWrite(switchPin, HIGH);
          
          //  set all to home (90)
                   
          pwm.setPWM(0,0,hometoAngle(90));
          pwm.setPWM(1,0,hometoAngle(90));
          pwm.setPWM(2,0,hometoAngle(90));
          pwm.setPWM(3,0,hometoAngle(90));
          // Eyelids set to open not closed
          pwm.setPWM(4,0,hometoAngle(130));
          pwm.setPWM(5,0,hometoAngle(150));
          //pwm.setPWM(6,0,hometoAngle(150)); not used
          //pwm.setPWM(7,0,hometoAngle(130)); not used
          delay(100);
          Serial.begin(115200);
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
        
// Map the joystick position to the servo angle
        
righteyeX = map(xPos,0,1023,125,620); // Maps the Joystick X reading to servo angle REVERSED
lefteyeX = map(xPos,0,1023,620,125); // Maps the Joystick X reading to servo angle 

//Right Eye movement
   pwm.setPWM(5,0,rightX(righteyeX)+90); // R L&R

// Left Eye movement
   pwm.setPWM(2,0,leftX(lefteyeX)+90); // L L&R

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

//lefteye servo min max mapping left and right
int leftX(int ang4){
  int pulse = map(ang4,0,180,50,150);
  return pulse;
}

void blink(){
  //left lids
  pwm.setPWM(0,0,hometoAngle(90));   
  pwm.setPWM(1,0,hometoAngle(90));
  //right lids
  pwm.setPWM(3,0,hometoAngle(90));
  pwm.setPWM(4,0,hometoAngle(90));   
  delay(100);
  pwm.setPWM(0,0,hometoAngle(130));   
  pwm.setPWM(1,0,hometoAngle(150));
  pwm.setPWM(3,0,hometoAngle(150));   
  pwm.setPWM(4,0,hometoAngle(130));
}
