// Connections to TMC2208
const int dirPin = 2; // Direction
const int stepPin = 3; // Step
int motSpeed = 200;  // lower the number faster it goes....i think*
const int leftPin = 6;
const int rightPin = 7;
int sudoPos = 0;
int runOnce = 1;
// Motor steps per rotation
//const int STEPS_PER_REV = 1600;

void setup() {

Serial.begin(115200);

// Setup the pins as Outputs
pinMode(leftPin,INPUT_PULLUP);
pinMode(rightPin,INPUT_PULLUP);
}
void loop() {
int Res = digitalRead (rightPin);
int Les = digitalRead (leftPin);

/* Testing default switch state (1 is open confirmed..ie HIGH)
if (Les == 0){
  digitalWrite(LED_BUILTIN, HIGH);
}
else if (Res == 0){
  digitalWrite(LED_BUILTIN, HIGH);
}
else {
digitalWrite(LED_BUILTIN, LOW);
}

*/

///////////////////HOMING PROCESS//////////////////

// position stepper in the middle
if (runOnce == 1){

  while (Les == 1 && Res == 1 or Les == 0 && Res == 1){
    //run clockwise till you hit the endstop
    digitalWrite(dirPin,HIGH);
    
      for(int x = 0; x < 1000 ; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(motSpeed);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(motSpeed);
        }
   }

  while (Res == 0 && Les == 1){ 
  //it is either already there or the above has run
  //run anticlockwise till you hit the endstop
    digitalWrite(dirPin,LOW);
      
      for(int x = 0; x < 1000; x++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(motSpeed);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(motSpeed);
        int(sudoPos++); //count steps between endstops
    }

  }
runOnce == 0;
}

// then go back clockwise sudoPos / 2

///////////////////HOMING PROCESS END/////////////

/*

// Spin motor until endstop hit

  while (Res == 0){
    
  }
    // Set motor direction clockwise
    digitalWrite(dirPin,HIGH);
    for(int x = 0; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(motSpeed);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(motSpeed);
    delay(4000);
    }
  }
  
  else if (Res == 1);
  // Set motor direction Counterclockwise
  digitalWrite(dirPin,LOW);

    for(int x = 0; x < STEPS_PER_REV; x++) {
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(motSpeed);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(motSpeed);
}
 */
}
