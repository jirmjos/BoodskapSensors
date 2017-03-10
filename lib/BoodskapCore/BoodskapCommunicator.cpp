#include "BoodskapCommunicator.h"


BoodskapCommunicator::BoodskapCommunicator(void (*callback)(BoodskapMessage* command))
  : _callback(callback), _heartbeat(HEARTBEAT_INTERVAL), _lastHeartbeat(0), _localPort(UDP_LOCAL_PORT)
{

}

void BoodskapCommunicator::wifiSSID(const char* ssid) {
  strcpy(_ssid, ssid);
}

void BoodskapCommunicator::wifiPassword(const char* password) {
  strcpy(_password, password);
}

void BoodskapCommunicator::host(const char* host) {
  strcpy(_host, host);
}

void BoodskapCommunicator::port(uint16_t port) {
  _port = port;
}

void BoodskapCommunicator::localPort(uint16_t localPort) {
  _localPort = localPort;
}

void BoodskapCommunicator::domainKey(const char* domainKey) {
  strcpy(_domainKey, domainKey);
}

void BoodskapCommunicator::apiKey(const char* apiKey) {
  strcpy(_apiKey, apiKey);
}

void BoodskapCommunicator::assetId(const char* assetId) {
  strcpy(_assetId, assetId);
}

void BoodskapCommunicator::deviceModel(const char* deviceModel) {
  strcpy(_deviceModel, deviceModel);
}

void BoodskapCommunicator::firmwareVersion(const char* firmwareVersion) {
  strcpy(_firmwareVersion, firmwareVersion);
}

void BoodskapCommunicator::heartbeat(uint32_t heartbeat) {
  _heartbeat = heartbeat;
}


void BoodskapCommunicator::setup() {
  begin();
}

void BoodskapCommunicator::loop() {

  if ((millis() - _lastHeartbeat) >= _heartbeat) {
    ping();
  }

  process();
}

void BoodskapCommunicator::create(int messageId, BoodskapMessage* msg) {
  //BoodskapMessage message(_buffer, messageId, _domainKey, _assetId, _apiKey, _firmwareVersion, _deviceModel);
  msg->withApiKey(_apiKey)->withAssetId(_assetId)->withDeviceModel(_deviceModel);
  msg->withDomainKey(_domainKey)->withFirmwareVersion(_firmwareVersion)->withMessageId(messageId);
  //return message;
}

void BoodskapCommunicator::ping() {
  BoodskapMessage msg ;
  create(1, &msg);
  msg.data()["stamp"] = millis();
  send(&msg);
}

void BoodskapCommunicator::sendAck(uint32_t cmdId, uint32_t corrId, bool ack){

  BoodskapMessage msg ;
  create(2, &msg); //ACK packet
  msg.header()["cmdid"] = cmdId;
  msg.header()["corrid"] = corrId;
  msg.data()["alive"] = millis();
  msg.data()["ack"] = ack;

  send(&msg); //Send back an ACK
}

void BoodskapCommunicator::messageReceived(BoodskapMessage* command) {

  _lastHeartbeat = millis();

  uint32_t stime = 0;
  bool ack = false;

  DEBUG_PRINTLN("\n\nCOMMAND RECEIVED");
  command->get().printTo(Serial);
  DEBUG_PRINTLN("\n\n");

  ack = command->header().get<String>("key").equals(_domainKey);
  ack = ack && command->header().get<String>("api").equals(_apiKey);
  ack = ack && command->header().get<String>("aid").equals(_assetId);

  if(!ack){
    DEBUG_PRINTLN("Illegal command received, ignored");
  }else{

    if(command->id() == -1 || command->id() >= 100){
      _callback(command);
    }else{

        switch (command->id()) {
          case 1: //ping message
            DEBUG_PRINT("Ping received, server time : ");
            DEBUG_PRINTLN(command->data().get<uint32_t>("time"));
            break;
          case 10: //System config
            saveSystemConfig(command->data());
            DEBUG_PRINTLN("Rebooting...");
            ESP.restart();
            break;
          case 11: //Device config
            saveDeviceConfig(command->data());
            DEBUG_PRINTLN("Rebooting...");
            ESP.restart();
            break;
          default:
              commandReceived(command);
        }
    }

  }

  sendAck(command->command(), command->correlationId(), ack);

}
