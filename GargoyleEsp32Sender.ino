#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "SLOWKEVIN";
const char *password = "FUKevin07";
const char *raspberryPiIP = "192.168.1.80";
const int raspberryPiPort = 8888;

WiFiUDP udp;


#define mod1Pin 26
#define mod2Pin 25
#define mod3Pin 35
#define mod4Pin 34
#define xPin 39
#define yPin 36


int mod1b;
int mod2b;
int mod3b;
int mod4b;
int xpos;
int ypos;


void setup() {
  
  pinMode(mod1Pin,INPUT_PULLUP);
  pinMode(mod2Pin,INPUT_PULLUP);
  pinMode(mod3Pin,INPUT_PULLUP);
  pinMode(mod4Pin,INPUT_PULLUP);
  
  Serial.begin(9600);



  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  

  mod1b = digitalRead(mod1Pin);
  mod2b = digitalRead(mod2Pin);
  mod3b = digitalRead(mod3Pin);
  mod4b = digitalRead(mod4Pin);
  xpos = analogRead(xPin);
  ypos = analogRead(yPin);

//map the joystick position to an angle
  xpos = map(xpos,0,4095,0,180);
  ypos = map(ypos,0,4095,0,180); 

Serial.print("X Pos : ");
Serial.println(xpos);
Serial.print("Y Pos : ");
Serial.println(ypos);
Serial.print("Button 1 : ");
Serial.println(mod1b);
Serial.print("Button 2 : ");
Serial.println(mod2b);
Serial.print("Button 3 : ");
Serial.println(mod3b);
Serial.print("Button 4 : ");
Serial.println(mod4b);


 // My variables
//String data = String(xpos)+","+String(ypos)+","+String(mod1b)+","+String(mod2b)+","+String(mod3b)+","+String(mod4b);
String data = String(xpos)+","+String(ypos)+","+String(mod1b)+","+String(mod2b)+","+String(mod3b)+","+String(mod4b);

  udp.beginPacket(raspberryPiIP, raspberryPiPort);
  udp.print(data);
  udp.endPacket();

  delay(100);
}
