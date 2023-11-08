// >> UNO R3, PROGRAMMED FOR M.5 PROJECT

// >> LIBRARIES
#include <dht.h>
#include <SimpleTimer.h>

// >> VARIABLE DECLARATIONS
#define PUMP_PIN 6
#define DHT11_PIN 7
#define MOISTURESENSOR_PIN A0
dht DHT;
SimpleTimer timer;

const int In[1] = { MOISTURESENSOR_PIN };
const int Out[2] = { PUMP_PIN, DHT11_PIN };
bool isWatering = false;
int isAutoOn = 0, isPumpOn = 0, ReadDHT;
float Moisture, MoisturePercentage, Temperature, Humidity;

// >> SETTINGS
const float WetPercentage = 60;
const int AutoWateringDelay, TolerancePercentage = 4.25;

// >> DATA SEND FUNCTIONS
void unoDataSend(float value, String code) {
  Serial.println(code + value);
}
void checkConnected() {
  Serial.println("C");
}
void checkSwitch() {
  unoDataSend(isWatering, "O");
}
void checkMoisture() {
  unoDataSend(MoisturePercentage, "M");  // Calculates moisture as a percent
}
void checkTemperature() {
  unoDataSend(Temperature, "T");
}
void checkHumidity() {
  unoDataSend(Humidity, "H");
}

// >> SETUP
void setup() {
  Serial.begin(115200);

  // >> CONNECTION DELAY
  for (int i = 0; i < 89; i++) {
    delay(105);
  }
  // >> INPUT PINMODE
  for (int i = 0; i < sizeof(In); i++) {
    pinMode(In[i], INPUT);
  }
  // >> OUTPUT PINMODE
  for (int i = 0; i < sizeof(Out); i++) {
    pinMode(Out[i], OUTPUT);
  }

  // >> DATA SEND INITIATIONS
  timer.setInterval(95L, checkConnected);
  timer.setInterval(100L, checkSwitch);
  timer.setInterval(150L, checkMoisture);
  timer.setInterval(1000L, checkHumidity);
  timer.setInterval(1050L, checkTemperature);
}

// >> LOOP
void loop() {
  // >> TIMER RUN
  timer.run();

  // >> VALUE ASSIGNATIONS
  ReadDHT = DHT.read11(DHT11_PIN);
  Moisture = analogRead(MOISTURESENSOR_PIN);
  Temperature = DHT.temperature;
  Humidity = DHT.humidity;

  // >> CONDITION INSPECTIONS & WRITES
  if (!isWatering) {
    MoisturePercentage = 100 - (Moisture / 1023) * 100;
  } else {
    MoisturePercentage = 100 - (Moisture / 1023) * 100 + TolerancePercentage;
  }

  if (isPumpOn) {
    isWatering = true;
    digitalWrite(PUMP_PIN, 1);
  } else if (isAutoOn && MoisturePercentage <= WetPercentage) {
    isWatering = true;
    digitalWrite(PUMP_PIN, 1);
    delay(5000);
    isWatering = false;
    digitalWrite(PUMP_PIN, 0);
  } else {
    isWatering = false;
    digitalWrite(PUMP_PIN, 0);
  }
}

// >> SERIAL EVENT FUNCTION
void serialEvent() {
  while (Serial.available()) {
    char myChar = (char)Serial.read();

    // >> SERIAL EVENT CONDITIONS
    if (myChar == 'a') {
      int value = Serial.parseInt();
      isAutoOn = value;
    }
    if (myChar == 'p') {
      int value = Serial.parseInt();
      isPumpOn = value;
    }
  }
}