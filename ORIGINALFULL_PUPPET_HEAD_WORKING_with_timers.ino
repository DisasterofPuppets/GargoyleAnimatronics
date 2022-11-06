// SOME DODGY PUPPET LED CONTROL CODE BY Venomouse

#include "FastLED.h"
#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 3
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];  

// Arduino Input PIN numbers
const int Switch_Pin = 2; // digital pin connected to switch output
int X_Pin = 0; // analog pin connected to X output
int Y_Pin = 1; // analog pin connected to Y output
int buttonState = 0;


//Outputs


const int Pump_Pin = 3; // pin for smoke  uses digital pins
int Smoke_LED = 4; // For Smoke LED
const int Jaw_Pin = 6; // pin for Y direction
const int Eye_Pin = 7; // 
const int Head_Pin = 9; // pin for Y direction up (0)

    short nFrameRate1 = 9000;        // frame rate 1 in FPS
    short nFrameRate2 = 1;       // frame rate 2 in FPS
    short nFrameRate3 = 20000;       // frame rate 3 in FPS

    static uint32_t ulNextTime2 = 10;
    static uint32_t ulNextTime1 = 4000;
    static uint32_t ulNextTime3 = 15000;



void setup() 
{
  // put your setup code here, to run once:

  FastLED.addLeds<WS2812, Head_Pin,GRB >(leds[0], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, Eye_Pin, GRB>(leds[1], NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, Jaw_Pin, GRB>(leds[2], NUM_LEDS_PER_STRIP);


  digitalWrite(Switch_Pin, HIGH);
  digitalWrite(Pump_Pin, LOW);
//  digitalWrite(Smoke_LED, LOW);
  digitalWrite(Head_Pin, LOW);
  digitalWrite(Jaw_Pin, LOW);
  Serial.begin(115200);


}

void loop() 
{
FastLED.clear(true);  
FastLED.setBrightness( 255 );
buttonState = digitalRead(Switch_Pin);

//Monitor 
Serial.println("=====================INPUTS=====================");
Serial.println("Switch_Pin :");
Serial.print(digitalRead(Switch_Pin));
Serial.print("X Value :");
Serial.print(analogRead(X_Pin));
Serial.println("Y Value :");
Serial.print(analogRead(Y_Pin));
Serial.println("buttonState :");
Serial.print(buttonState);

Serial.println("=====================OUTPUTS=====================");
Serial.println("Pump_Pin :");
Serial.print(analogRead(Pump_Pin));
Serial.println("Head_Pin :");
Serial.print(analogRead(Head_Pin));
Serial.println("Eye_Pin :");
Serial.print(analogRead(Eye_Pin));
Serial.println("Smoke_LED :");
Serial.print(analogRead(Smoke_LED));
Serial.println("Jaw_Pin :");
Serial.print(analogRead(Jaw_Pin));




////////////////////Monitor Joystick / switches here

//SMOKE PIN PRESSED (Array 0)

if (buttonState == 0) //on

  {
    Smoke();
  }


else

  {
    analogWrite(Pump_Pin, 0);
    analogWrite(Smoke_LED,0);
  }


  //HEAD PIN PRESSED (Array 0)

  while  (analogRead(Y_Pin)< 10 ) 

  {
    Head();
      FastLED.show();


  }


  //EYES PIN PRESSED (Array 1)

  if (analogRead(X_Pin) < 10)
  {
    Eyes();
      FastLED.show();


  }
  else
  {
  FastLED.clear();
  }





  //JAW PIN PRESSED (Array 3)

  while (analogRead(Y_Pin) > 1000)
  {
    Jaw();
      FastLED.show();


  }


  //ALL ON
 while (analogRead(X_Pin) > 1000)
  {
    All();
      FastLED.show();
  }

}


//******************************
//**    F U N C T I O N S     **
//******************************


////////////
// SMOKE //
//////////



void Smoke()  
{
    analogWrite(Pump_Pin, 255);
    analogWrite(Smoke_LED, 255);

}


//////END SMOKE


/////////////////////////////////////
// HORNS - HEAD, Array 0 - Pin: 9 //
///////////////////////////////////

void Head() // 0 in the Array
{
  int8_t H = random(0,25); //HUE
  int8_t S = random(250,256); //SAT
  int8_t V = random(250,256); //BRIGHT

  leds[0][0] = CHSV(H,S,V);
  leds[0][1] = CHSV(H,S,V);



 }

//////END HORNS HEAD



////////////////////////////
// EYES, Array 1 - Pin: 7 //
///////////////////////////

void Eyes() // 
{

    for(int eyebrightness=25; eyebrightness<255; eyebrightness++)
    { 
      //Increment 'eyebrightness' from 50 to 255
      leds[1][0] = CHSV (0,255,255);
      leds[1][1] = CHSV (0,255,255);
      FastLED.setBrightness( eyebrightness );

    }

    for(int eyebrightness=255; eyebrightness>=25; eyebrightness--)
    { 
      //Decrement 'brightness' from 255 to 50
      leds[1][0] = CHSV (0,255,255);
      leds[1][1] = CHSV (0,255,255);
      FastLED.setBrightness( eyebrightness );

    }

}

//////END EYES


////////////////////////////////////
// HORNS - JAW, Array 2 - PIN: 6 //
//////////////////////////////////

void Jaw() // 2 in the array
{
  int8_t H = random(0,20); //HUE
  int8_t S = random(250,256); //SAT
  int8_t V = random(250,256); //BRIGHT

  //middle horn

  leds[2][1] = CHSV(H,S,V);


  // below for edge horns to be same

  H = random(0,50); //RED
  S = random(200,256); //GREEN
  V = random(200,256); //BLUE

  leds[2][0] = CHSV(H,S,V);
  leds[2][2] = CHSV(H,S,V);


}
//////END HORNS - JAW



//////////
// ALL //
////////


void All ( void)
{

    if (millis() >= ulNextTime1) {
        ulNextTime1 = millis() + (1000/nFrameRate1);
        Head();
    }

    if (millis() >= ulNextTime2) {
        ulNextTime2 = millis() + (1000/nFrameRate2);
        Eyes();
    }

    if (millis() >= ulNextTime3) {
        ulNextTime3 = millis() + (1000/nFrameRate3);
        Jaw();
    }

    Smoke();
}


////////END ALL
