/*
 * With the servo wiring pointing to the Right
 * 0 degrees - max clockwise
 * 90 degrees - neutral aka middle
 * 180 degrees - max anti clockwise
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
 * EYELIDS (Mg90s servos)
 * 0 - Top left Eyelid 
 * 1 - Bottom Left Eyelid 
 * 2 - Top Right Eyelid 
 * 3 - Bottom Right Eyelid
 * 
 * LEFT EYE (MG996R Servo)
 * 4 - Left , Right
 * 5 - Up, Down
 * 
 * RIGHT EYE (MG996R Servo)
 * 6 - Left , Right
 * 7 - Up, Down 
 
 */

// Micro Servo Position confirmation
//Disaster of Puppets 2020
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define joyX A0 // X pin input
#define joyY A1 // X pin input

// used to store Joystick mapping
int wake = 0;
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

          // Eyelids set to closed
          pwm.setPWM(0,0,hometoAngle(180));
          pwm.setPWM(1,0,hometoAngle(180));
          pwm.setPWM(2,0,hometoAngle(180));
          pwm.setPWM(3,0,hometoAngle(180));

          // Left Eye set to middle
          pwm.setPWM(4,0,hometoAngle(90)); //(Left, Right)
          pwm.setPWM(5,0,hometoAngle(90)); //(Up, Down)
          
          // Right Eye set to middle 
          pwm.setPWM(6,0,hometoAngle(90)); //(Left, Right)
          pwm.setPWM(7,0,hometoAngle(90)); //(Up, Down)
          
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

// Eyelids are closed on initiation, clicking push button once will open them..
//note, make function here to make multiple closed / open here for wake up sequence.
if (digitalRead(switchPin)== 0){
      if (wake == 0){
        wake = 1;
        pwm.setPWM(0,0,hometoAngle(0));
        pwm.setPWM(1,0,hometoAngle(0));
        pwm.setPWM(2,0,hometoAngle(0));
        pwm.setPWM(3,0,hometoAngle(0));
      }
      else if (wake == 1){
        blink();
      }
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
   pwm.setPWM(4,0,leftY(lefteyeX)+90); // Left Right
   pwm.setPWM(5,0,leftX(lefteyeY)+90); // Up Down
   
 // Right Eye movement
   pwm.setPWM(6,0,rightY(righteyeX)+90); // Left Right
   pwm.setPWM(7,0,rightY(righteyeY)+90); // Up Down
 
   
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

//right eye servo min max mapping left and right
int rightY(int ang9){
  int pulse = map(ang9,0,180,50,150);
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
  //open again
  pwm.setPWM(0,0,hometoAngle(0));   
  pwm.setPWM(1,0,hometoAngle(0));
  pwm.setPWM(2,0,hometoAngle(0));   
  pwm.setPWM(3,0,hometoAngle(0));
}
