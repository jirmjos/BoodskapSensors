#include <BoodskapConfig.h>
#include <BoodskapMessage.h>
#include <BoodskapNodeMCUCommunicator.h>

void commandReceived(BoodskapMessage* cmd);

BoodskapNodeMCUCommunicator boodskap(&commandReceived);

uint32_t lastMessage = 0;

void setup() {

  Serial.begin(BAUD_RATE);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(3500);

  DEBUG_PRINTLN("Boodskap Communicator Starting....");
  DEBUG_PRINTLN("");

  boodskap.setup();

  DEBUG_PRINTLN(BOODSKAP_FIRMWARE_VERSION);
  DEBUG_PRINTLN("Setup complete.");
}

void loop() {

  boodskap.loop();

  /**
  if (boodskap.isConnected() && (millis() - lastMessage >= 25000)) {
    BoodskapMessage message;
    boodskap.create(100, &message);
    message.set("temperature", 29.45);
    message.set("humidity", 32.11);
    boodskap.send(&message);
    lastMessage = millis();
  }
  */
}

void commandReceived(BoodskapMessage* cmd)
{
  DEBUG_PRINTLN("Command received...");
  cmd->get().printTo(Serial);
  DEBUG_PRINTLN("");
}
