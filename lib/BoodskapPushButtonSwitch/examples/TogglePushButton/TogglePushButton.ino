#include <PushButtonSwitch.h>

void buttonPressed(bool on);
PushButtonSwitch kSwitch(D4, &buttonPressed);

void setup() {

  Serial.begin(115200);
  kSwitch.setup();

}

void loop() {
  kSwitch.loop();
}

void buttonPressed(bool on)
{
  Serial.println(on ? "On" : "Off");
}
