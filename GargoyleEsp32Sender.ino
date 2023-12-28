#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid = "WIFIIDHERE";
const char *password = "PASSHERE";
const char *raspberryPiIP = "192.168.1.80";
const int raspberryPiPort = 8888;

WiFiUDP udp;


#define mod1Pin 27
#define mod2Pin 26
#define mod3Pin 25
#define mod4Pin 35
#define xPin 39
#define yPin 36

const int numSamples = 10;
bool mod1;
bool mod2;
bool mod3;
bool mod4;
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
  

  mod1 = debounce(mod1Pin);
  mod2 = debounce(mod2Pin);
  mod3 = debounce(mod3Pin);
  mod4 = debounce(mod4Pin);
  xpos = multisample(xPin);
  ypos = multisample(yPin);




//map the joystick position to an angle
  xpos = map(xpos,0,4095,0,1023);
  ypos = map(ypos,0,4095,0,1023); 

Serial.print("X Pos : ");
Serial.println(xpos);
Serial.print("Y Pos : ");
Serial.println(ypos);
Serial.print("Button 1 : ");
Serial.println(mod1);
Serial.print("Button 2 : ");
Serial.println(mod2);
Serial.print("Button 3 : ");
Serial.println(mod3);
Serial.print("Button 4 : ");
Serial.println(mod4);


 // My variables
String data = String(xpos)+","+String(ypos)+","+String(mod1)+","+String(mod2)+","+String(mod3)+","+String(mod4);

Serial.println(data);

  udp.beginPacket(raspberryPiIP, raspberryPiPort);
  udp.print(data);
  udp.endPacket();

  delay(100);
}

int multisample(int pin) {
  int total = 0;
  
  for (int i = 0; i < numSamples; i++) {
    total += analogRead(pin);
    delay(1);  // You can adjust this delay if needed
  }

  return total / numSamples;
}

bool debounce(int pin) {
  static int lastButtonState = HIGH;
  int buttonState = digitalRead(pin);

  if (buttonState != lastButtonState) {
    delay(50); // Adjust this delay as needed
    buttonState = digitalRead(pin);
  }

  lastButtonState = buttonState;
  return buttonState == LOW;
}
