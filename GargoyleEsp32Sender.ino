//DOIT ESP32 DEVKIT V1
#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP udp;

char packetBuffer[255];
unsigned int localPort = 9999;
char *serverip = "192.168.1.46";
unsigned int serverport = 8888;

// CHANGE ME
const char *ssid = "********";
const char *password = "*************";

#define xPin 13
#define yPin 14
#define jbPin 27
#define mod1Pin 25
#define mod2Pin 33
#define mod3Pin 32

int m1lastState = HIGH; // the previous state from the mod1 input pin
int m1currentState;     // the current reading from the mod1 input pin
int m2lastState = HIGH; // the previous state from the mod2 input pin
int m2currentState;     // the current reading from the mod2 input pin
int m3lastState = HIGH; // the previous state from the mod3 input pin
int m3currentState;     // the current reading from the mod3 input pin

int xPos;
int yPos;
bool jButton;
bool mod1B;
bool mod2B;
bool mod3B;

void setup() {
  Serial.begin(9600);
  pinMode(jbPin,INPUT_PULLUP);
  pinMode(mod1Pin,INPUT_PULLUP);
  pinMode(mod2Pin,INPUT_PULLUP);
  pinMode(mod3Pin,INPUT_PULLUP);

// Connect to Wifi network.
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(F("."));
  }
  udp.begin(localPort);
  Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);
}

void loop() {
 
  xPos = analogRead(xPin);
  yPos = analogRead(yPin);
  jButton = digitalRead(jbPin);
  mod1B = digitalRead(mod1Pin);
  mod2B = digitalRead(mod2Pin);
  mod3B = digitalRead(mod3Pin);

//map the joystick position to an angle
  xPos = map(xPos,0,4095,0,180);
  yPos = map(yPos,0,4095,0,180);  

 int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print(" Received packet from : "); Serial.println(udp.remoteIP());
    int len = udp.read(packetBuffer, 255);
    Serial.printf("Data : %s\n", packetBuffer);
    Serial.println();
  }
  delay(500);

// My variables
String dataPacket = String(xPos)+","+String(yPos)+","+String(jButton)+","+String(mod1B)+","+String(mod2B)+","+String(mod3B);

  Serial.print("[Client Connected] "); Serial.println(WiFi.localIP());
  udp.beginPacket(serverip, serverport);
  udp.print(dataPacket);
  udp.endPacket();

/*
  Serial.print("X Position : ");
  Serial.println(xPos);
  Serial.print("Y Position : ");
  Serial.println(yPos);
  Serial.print("Joystick Button Pressed? : ");
  Serial.println(jButton);

  Serial.print("Mod 1 Button Pressed? : ");
  Serial.println(mod1B);
  Serial.print("Mod 2 Button Pressed? : ");
  Serial.println(mod2B);
  Serial.print("Mod 3 Button Pressed? : ");
  Serial.println(mod3B);
*/
}
