// >> ESP8266, PROGRAMMED FOR M.5 PROJECT

// >> LIBRARIES
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// >> VARIABLE DEFENITIONS
#define BLYNK_TEMPLATE_ID "[TEMPLATE_ID]"
#define BLYNK_DEVICE_NAME "Arduino Project"
#define BLYNK_AUTH_TOKEN "[AUTH_TOKEN]"
#define BLYNK_PRINT Serial

// >> SETTINGS
char ssid[] = "[SSID_NAME]";
char pass[] = "[SSID_PASSWORD]";

// >> SERVER CONNECTED FUNCTIONS
BLYNK_CONNECTED() {
  // >> UI SYNCING & WRITING
  Blynk.syncVirtual(V0, V10, V11);
  Blynk.virtualWrite(V12, "CONNECTING...");
  // >> CONNECTION DELAY
  for (int i = 0; i < 89; i++) {
    delay(105);
  }
}

// >> UI WRITE FUNCTIONS
BLYNK_WRITE(V0) {
  int value = param.asInt();
  String p = "p";
  Serial.println(p + value);
}

BLYNK_WRITE(V10) {
  int value = param.asInt();
  String a = "a";
  Serial.println(a + value);
}

// >> SETUP
void setup() {
  Serial.begin(115200);
  WiFi.forceSleepWake();
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// >> LOOP
void loop() {
  Blynk.run();
}

// >> SERIAL EVENT FUNCTION
void serialEvent() {
  while (Serial.available()) {
    char myChar = (char)Serial.read();

    // >> SERIAL EVENT CONDITIONS
    if (myChar == 'C') {
      Blynk.virtualWrite(V12, "CONNECTED!");
    }
    if (myChar == 'O') {
      int value = Serial.parseInt();
      Blynk.virtualWrite(V11, value);
    }
    if (myChar == 'M') {
      float value = Serial.parseFloat();
      Blynk.virtualWrite(V1, value);
    }
    if (myChar == 'H') {
      float value = Serial.parseFloat();
      Blynk.virtualWrite(V3, value);
    }
    if (myChar == 'T') {
      float value = Serial.parseFloat();
      Blynk.virtualWrite(V2, value);
    }
  }
}