#include <WiFi.h>
#include <WiFiUdp.h>
#include <FastLED.h>

const char *ssid = "SLOWKEVIN";
const char *password = "FUKevin07";
const char *raspberryPiIP = "192.168.1.80";
const int raspberryPiPort = 8888;

WiFiUDP udp;

#define mod1Pin 27
#define mod2Pin 26
#define mod3Pin 25
#define mod4Pin 33
#define LEDPin 32
#define xPin 39
#define yPin 36
#define COLOR_ORDER GRB
#define CHIPSET     WS2812
#define NUM_LEDS 4
bool gReverseDirection = false;
CRGB leds[NUM_LEDS];

const int numSamples = 10;
bool mod1;
bool mod2;
bool mod3;
bool mod4;
int xpos;
int ypos;
bool mod1Toggle;
bool mod2Toggle;
bool mod3Toggle;
bool mod4Toggle;



void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  FastLED.addLeds<CHIPSET, LEDPin, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  randomSeed(analogRead(0)); // Initialize random seed
  
  pinMode(mod1Pin,INPUT_PULLUP);
  pinMode(mod2Pin,INPUT_PULLUP);
  pinMode(mod3Pin,INPUT_PULLUP);
  pinMode(mod4Pin,INPUT_PULLUP);

}



void loop() {

  mod1 = digitalRead(mod1Pin);
  mod2 = digitalRead(mod2Pin);
  mod3 = digitalRead(mod3Pin);
  mod4 = digitalRead(mod4Pin);
  xpos = multisample(xPin);
  ypos = multisample(yPin);


//map the joystick position to an angle
  xpos = map(xpos,0,4095,0,1023);
  ypos = map(ypos,0,4095,0,1023); 

// set the buton toggles off or on.
// MAKE THIS A FUNCTION  

  if (mod1 == false and mod1Toggle == false){ // for some reason my Pin buttons are reversed...
      Serial.println("Button state changed to ON");
      mod1Toggle = true;
  }
  else if (mod1Toggle == true){
      Serial.println("Button state changed to OFF");
      mod1Toggle = false;
  }
  else {
   // do nothing.
  }
  
 if (mod2 == false and mod2Toggle == false){ // for some reason my Pin buttons are reversed...
      Serial.println("Button state changed to ON");
      mod2Toggle = true;
  }
  else if (mod1Toggle == true){
      Serial.println("Button state changed to OFF");
      mod2Toggle = false;
  }
  else {
   // do nothing.
  }

   if (mod3 == false and mod3Toggle == false){ // for some reason my Pin buttons are reversed...
      Serial.println("Button state changed to ON");
      mod3Toggle = true;
  }
  else if (mod3Toggle == true){
      Serial.println("Button state changed to OFF");
      mod3Toggle = false;
  }
  else {
   // do nothing.
  }

 if (mod4 == false and mod4Toggle == false){ // for some reason my Pin buttons are reversed...
      Serial.println("Button state changed to ON");
      mod4Toggle = true;
  }
  else if (mod4Toggle == true){
      Serial.println("Button state changed to OFF");
      mod4Toggle = false;
  }
  else {
   // do nothing.
  }
 
Serial.print("X Pos : ");
Serial.println(xpos);
Serial.print("Y Pos : ");
Serial.println(ypos);
Serial.print("Button 1 Toggle / STATE: ");
Serial.println(mod1Toggle);
Serial.print("Button 1 Current read: ");
Serial.println(mod1);
Serial.print("Button 2 : ");
Serial.println(mod2Toggle);
Serial.print("Button 3 : ");
Serial.println(mod3Toggle);
Serial.print("Button 4 : ");
Serial.println(mod4Toggle);


 // My variables
String data = String(xpos)+","+String(ypos)+","+String(mod1Toggle ? "ON" : "OFF")+","+String(mod2Toggle ? "ON" : "OFF")+","+String(mod3Toggle ? "ON" : "OFF")+","+String(mod4Toggle ? "ON" : "OFF");

Serial.println(data);

  udp.beginPacket(raspberryPiIP, raspberryPiPort);
  udp.print(data);
  udp.endPacket();

if (udp.endPacket() == 0) {
  Serial.println("Error sending UDP packet");
}

Serial.println(strerror(118));
  delay(500);
}

int multisample(int pin) {
  int total = 0;
  
  for (int i = 0; i < numSamples; i++) {
    total += analogRead(pin);
    delay(1);  // You can adjust this delay if needed
  }

  return total / numSamples;
}
