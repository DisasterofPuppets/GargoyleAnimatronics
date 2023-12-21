#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "SLOWKEVIN";
const char *password = "FUKevin07";
const char *raspberryPiIP = "192.168.1.80";
const int raspberryPiPort = 8888;

WiFiUDP udp;

#define xPin 13
#define yPin 14
#define jbPin 27
#define mod1Pin 25
#define mod2Pin 33
#define mod3Pin 32
#define mod4Pin 11

int xpos;
int ypos;
int mod1b;
int mod2b;
int mod3b;
int mod4b;

void setup() {

  
  Serial.begin(115200);



  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {

 // xpos = analogRead(xPin);
  //ypos = analogRead(yPin);
//  mod1b = digitalRead(mod1Pin);
//  mod2b = digitalRead(mod2Pin);
//  mod3b = digitalRead(mod3Pin);
//  mod4b = digitalRead(mod4Pin);

//map the joystick position to an angle
  //xpos = map(xpos,0,4095,0,180);
  //ypos = map(ypos,0,4095,0,180); 

 // My variables
String data = String(xpos)+","+String(ypos)+","+String(mod1b)+","+String(mod2b)+","+String(mod3b)+","+String(mod4b);


  udp.beginPacket(raspberryPiIP, raspberryPiPort);
  udp.print(data);
  udp.endPacket();

  delay(1000);
}
