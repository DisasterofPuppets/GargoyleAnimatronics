// Connections to TMC2208
const int dirPin = 2; // Direction
const int stepPin = 3; // Step
int motSpeed = 200;  // lower the number faster it goes....i think*
const int leftPin = 7;
const int rightPin = 8;
int sudoPos = 0;
int initial = 1;
int rHit = 1;
// Motor steps per rotation
//const int STEPS_PER_REV = 1600;

void setup() {

Serial.begin(115200);

// Setup the pins as Outputs
pinMode(leftPin,INPUT_PULLUP);
pinMode(rightPin,INPUT_PULLUP);
}
void loop() {

int Res = digitalRead(rightPin);
int Les = digitalRead(leftPin);
//Serial.print("Initial set : ");
//Serial.println(initial);
//Serial.print("Left : ");
//Serial.println(Les);
//Serial.print("Right : ");
//Serial.println(Res);

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

if (initial == 1){
    Serial.println("Moving to Right Endstop");
    digitalWrite(dirPin,HIGH);



    // run until right Endstop is hit (0)
      for(int x = 0; Res == 1; x++) {
        Res = digitalRead(rightPin);
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(motSpeed);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(motSpeed);

Serial.print("Right : ");
Serial.println(Res);
        
        }

     // run until left Endstop is hit and count the steps (0)        
        Serial.println("Moving to Left Endstop");
        digitalWrite(dirPin,LOW);

        for(int x = 0; Les == 1; x++) {
          Les = digitalRead(leftPin);
          Serial.print("Left : ");
          Serial.println(Les);
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(motSpeed);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(motSpeed);
          int(sudoPos++); //count steps between endstops
          Serial.print("sudoPos : ");
          Serial.println(sudoPos);
         }
       
       for(int x = 0; x < sudoPos; x++) {
       // Set Stepper to middle
          Serial.print("Setting to Middle : ");
          Serial.println(sudoPos);
          digitalWrite(dirPin,HIGH);
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(motSpeed);
          digitalWrite(stepPin,LOW);
          delayMicroseconds(motSpeed);
          }
  }
}   
