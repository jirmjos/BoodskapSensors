#include <BoodskapConfig.h>
#include <BoodskapMessage.h>
#include <BoodskapNodeMCUCommunicator.h>
#include <ThreePhaseVoltageSensor.h>

#define PIN_1 D5
#define PIN_2 D6
#define PIN_3 D7

void commandReceived(BoodskapMessage* cmd);
void voltageChanged(bool p1, bool p2, bool p3);

BoodskapNodeMCUCommunicator boodskap(&commandReceived);
ThreePhaseVoltageSensor vMeter(PIN_1, PIN_2, PIN_3, &voltageChanged);

uint32_t lastMessage = 0;

void setup() {

  Serial.begin(BAUD_RATE);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(3500);

  DEBUG_PRINTLN("Boodskap 3Phase VSensor Starting....");
  DEBUG_PRINTLN("");

  boodskap.setup();
  vMeter.setup();

  DEBUG_PRINTLN("");
  DEBUG_PRINTLN(BOODSKAP_FIRMWARE_VERSION);
  DEBUG_PRINTLN("Setup complete.");

}

void loop() {

  boodskap.loop();
  vMeter.loop();

  if (boodskap.isConnected() && (millis() - lastMessage >= 25000)) {

    BoodskapMessage message;
    bool p1;
    bool p2;
    bool p3;

    vMeter.voltage(&p1, &p2, &p3);

    boodskap.create(999, &message);

    message.set("alive", (uint32_t)millis());
    message.set("p1", p1);
    message.set("p2", p2);
    message.set("p3", p3);

    boodskap.send(&message);

    lastMessage = millis();
  }

}

void commandReceived(BoodskapMessage* cmd)
{
  DEBUG_PRINTLN("Command received...");
  cmd->get().printTo(Serial);
  DEBUG_PRINTLN("");
}

void voltageChanged(bool p1, bool p2, bool p3)
{

  BoodskapMessage message;
  boodskap.create(999, &message);
  message.set("alive", (uint32_t)millis());
  message.set("p1", p1);
  message.set("p2", p2);
  message.set("p3", p3);

  boodskap.send(&message);

  lastMessage = millis();

}
