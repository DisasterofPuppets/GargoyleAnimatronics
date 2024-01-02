#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP udp;
#include <FastLED.h>
#define NUM_SAMPLES 10
#define JOY_X_PIN 39
#define JOY_Y_PIN 36

int buttonPins[] = {27,26,25,33};
bool buttonState[] = {false, false, false, false};
bool lastButtonState[] = {HIGH, HIGH, HIGH, HIGH};
unsigned int localPort = 9999;
char *serverip = "192.168.1.80";
unsigned int serverport = 8888;

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

const char *ssid = "WIFI";
const char *password = "PASS";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(F("."));
  }
  udp.begin(localPort);
  Serial.printf("UDP Client : %s:%i \n", WiFi.localIP().toString().c_str(), localPort);

  pinMode(JOY_X_PIN, INPUT);
  pinMode(JOY_Y_PIN, INPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  int x = readJoystick(JOY_X_PIN);
  int y = readJoystick(JOY_Y_PIN);

  // Update xpos and ypos with joystick readings
  xpos = x;
  ypos = y;

  for (int i = 0; i < 4; i++) {
    bool currentButtonState = digitalRead(buttonPins[i]);
    if (currentButtonState != lastButtonState[i] && currentButtonState == LOW) {
      buttonState[i] = !buttonState[i];
      delay(200); // debounce
    }
    lastButtonState[i] = currentButtonState;
  }

  // Update the mod variables based on the button states
  mod1Toggle = buttonState[0];
  mod2Toggle = buttonState[1];
  mod3Toggle = buttonState[2];
  mod4Toggle = buttonState[3];

  String data = String(xpos)+","+String(ypos)+","+String(mod1Toggle ? "ON" : "OFF")+","+String(mod2Toggle ? "ON" : "OFF")+","+String(mod3Toggle ? "ON" : "OFF")+","+String(mod4Toggle ? "ON" : "OFF");

  Serial.println(data);

  Serial.print("[Client Connected] "); Serial.println(WiFi.localIP());
  udp.beginPacket(serverip, serverport);
  udp.printf(data.c_str());
  udp.endPacket();

  if (udp.endPacket() == 0) {
    Serial.println("Error sending UDP packet");
  }

  Serial.println(strerror(118));
  delay(500);
}


int readJoystick(int pin) {
  long sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
    delay(1); // wait 1ms
  }
  int avg = sum / NUM_SAMPLES;
  return map(avg, 0, 4095, 0, 1023);
}
