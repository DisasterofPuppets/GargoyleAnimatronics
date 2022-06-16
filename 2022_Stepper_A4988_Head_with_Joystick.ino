//Hold the joystick with the wiring to the left

#define xaxis A0
#define yaxis A1
int xpos;
int ypos;
int jstate;
const int joybutton = 4;
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;
const int stepperSpeed = 5000;
 
void setup()
{
  // Declare pins as Outputs 
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode (joybutton, INPUT_PULLUP);
  Serial.begin(115200);
}
void loop()
{

xpos = analogRead(xaxis);
ypos = analogRead(yaxis);
jstate = digitalRead(joybutton);


/*/ Testing
Serial.print("X : ");
Serial.println(analogRead(xaxis));
Serial.print("Y : ");
Serial.println(analogRead(yaxis));
Serial.print("Button : ");
Serial.println(digitalRead(joybutton));
delay(1000);

Serial.print("X : ");
Serial.println(xpos);
Serial.print("Y : ");
Serial.println(ypos);
Serial.print("B : ");
Serial.println(jstate);
delay(1000);
*/
// Did I mention you need to hold joystick with wiring on the left?
//move stepper Anticlockwise
while (xpos <= 480){
  digitalWrite(dirPin, HIGH);
  anti();
}

while (xpos >= 520){
  digitalWrite(dirPin, LOW);
  clockwise();
}


if ((xpos >=481) and (xpos <= 520) && (ypos >= 481) and (ypos <= 520)){
  //stop stepper
  digitalWrite(stepPin,LOW);
  }

/* 
  // Set motor direction clockwise
  digitalWrite(dirPin, HIGH);
 
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution /4; x++) //quater of a turn
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(5000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(5000);
  }
 delay(5000); // Wait a second

  // Set motor direction anti-clockwise
  digitalWrite(dirPin, LOW);
 
  // Spin motor slowly
  for(int x = 0; x < stepsPerRevolution /4; x++) //quater of a turn
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(5000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(5000);
  }
 delay(5000); // Wait a second
  
*/
}
void anti(){
    
    xpos = analogRead(xaxis);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperSpeed);
}

void clockwise(){
    
    xpos = analogRead(xaxis);
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperSpeed);
}
