#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include <SSD1306.h> // alias for `#include "SSD1306Wire.h"`
//#include <SH1106.h> // alias for `#include "SSD1306Wire.h"`
#include <OLEDDisplayUi.h>

#include <BoodskapConfig.h>
#include <BoodskapMessage.h>
#include <BoodskapNodeMCUCommunicator.h>

void commandReceived(BoodskapMessage* cmd);

BoodskapNodeMCUCommunicator boodskap(&commandReceived);

SSD1306  display(0x3c, D3, D5);
//SH1106 display(0x3c, D3, D5);
uint32_t lastMessage = 0;

void setup() {

  //Wire.begin(D3, D5);

  Serial.begin(BAUD_RATE);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(3500);

  DEBUG_PRINTLN("Boodskap Communicator Starting....");
  DEBUG_PRINTLN("");

  boodskap.setup();

  DEBUG_PRINTLN(BOODSKAP_FIRMWARE_VERSION);
  DEBUG_PRINTLN("Setup complete.");

  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "128X32 OLED Device");
  display.display();
  delay(10);
}

void loop() {

  boodskap.loop();

  if (boodskap.isConnected() && (millis() - lastMessage >= 25000)) {
    BoodskapMessage message;
    boodskap.create(1000, &message);
    message.set("alive", (uint32_t)millis());
    boodskap.send(&message);
    lastMessage = millis();
  }

}

void commandReceived(BoodskapMessage* cmd)
{
  DEBUG_PRINTLN("Command received...");
  cmd->get().printTo(Serial);
  DEBUG_PRINTLN("");

  switch(cmd->data().get<uint16_t>("font")){
    case 0:
    display.setFont(ArialMT_Plain_10);
    break;
    case 1:
    display.setFont(ArialMT_Plain_16);
    break;
    case 2:
    display.setFont(ArialMT_Plain_24);
    break;
  }

  display.clear();
  display.setColor((OLEDDISPLAY_COLOR)cmd->data().get<uint16_t>("color"));
  display.setTextAlignment((OLEDDISPLAY_TEXT_ALIGNMENT)cmd->data().get<uint16_t>("align"));

  int16_t x = cmd->data().get<int16_t>("x");
  int16_t y = cmd->data().get<int16_t>("y");

  display.drawString(x, y, cmd->data().get<String>("msg"));
  display.display();

  delay(10);

}
