#include <DigitalStateSensor.h>

void stateChanged(uint8_t state);
DigitalStateSensor dSensor(D4, 50, &stateChanged);

void setup() {

  Serial.begin(115200);
  dSensor.setup();

}

void loop() {
  dSensor.loop();
}

void stateChanged(uint8_t state){
  Serial.println(state == HIGH ? "On" : "Off");
}
