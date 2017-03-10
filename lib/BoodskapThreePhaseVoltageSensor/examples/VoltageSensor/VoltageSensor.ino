#include <Arduino.h>
#include "ThreePhaseVoltageSensor.h"

#define PIN_1 D5
#define PIN_2 D6
#define PIN_3 D7

void voltageChanged(bool p1, bool p2, bool p3);

ThreePhaseVoltageSensor vMeter(PIN_1, PIN_2, PIN_3, &voltageChanged);

uint32_t elapsed = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  vMeter.setup();
  Serial.println();
  Serial.println();
  Serial.println();
}

void loop() {

  vMeter.loop();

  if ((millis() - elapsed) >= 15000) {
    elapsed = millis();
    Serial.println("Heartbeat");
  }
}

void voltageChanged(bool p1, bool p2, bool p3)
{
  Serial.print("P1 : ");
  Serial.print(p1);
  Serial.print(" P2 : ");
  Serial.print(p2);
  Serial.print(" P3 : ");
  Serial.println(p3);
}
