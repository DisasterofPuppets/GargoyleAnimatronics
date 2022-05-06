//Joystick in the X direction moves Stepper 1 clockwise or counter
//Joystick in the Y direction moves Stepper2 clockwise or counter

// Include the Arduino Stepper Library
#include <Stepper.h>

// Number of steps per output rotation
const int stepsPerRevolution = 200;

//Joystick

int xPin = A0;                                                                                   
int yPin = A1;

// Create Instance of Stepper library
Stepper xStep(stepsPerRevolution, 8, 9, 10, 11);
Stepper yStep(stepsPerRevolution, 4, 5, 6, 7);

void setup()
{
  
    // set the speed at 100 rpm:
  xStep.setSpeed(100);
  yStep.setSpeed(100);
  // initialize the serial port for testing / logging:
  Serial.begin(115200);
}


//If the stepper motor is not hit (ie 1 all good, else 0 )

void loop() 
{

int x_pos = analogRead(xPin);
int y_pos = analogRead(yPin);

  //print out the position of the Joystick
  Serial.print("X : ");
  Serial.println(x_pos);
  Serial.print("Y : ");
  Serial.println(y_pos);

while (x_pos > 600){
   x_pos = analogRead(xPin);
  // Serial.println("clockwise");
  xStep.step(stepsPerRevolution);
}

while (x_pos < 450){
    x_pos = analogRead(xPin);
    //  Serial.println("Anticlockwise");
    xStep.step(-stepsPerRevolution);
}

                                     
while (y_pos > 600){ 
  y_pos = analogRead(yPin);                             
  // Serial.println("clockwise");
  yStep.step(stepsPerRevolution);
}  

while (y_pos < 450){
  y_pos = analogRead(yPin);
  //  Serial.println("Anticlockwise");
  yStep.step(-stepsPerRevolution);
}

}
