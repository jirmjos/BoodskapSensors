#include "BoodskapNodeMCUCommunicator.h"

BoodskapNodeMCUCommunicator::BoodskapNodeMCUCommunicator(void (*callback)(BoodskapMessage* command))
  : BoodskapCommunicator(callback)
{

}

void BoodskapNodeMCUCommunicator::begin()
{

  SPIFFS.begin();

  FSInfo fsInfo;
  SPIFFS.info(fsInfo);
  DEBUG_PRINT("Size : ");
  DEBUG_PRINT(fsInfo.totalBytes);
  DEBUG_PRINT(", Used : ");
  DEBUG_PRINT(fsInfo.usedBytes);
  DEBUG_PRINT(", Block Size : ");
  DEBUG_PRINT(fsInfo.blockSize);
  DEBUG_PRINT(", Page Size : ");
  DEBUG_PRINT(fsInfo.pageSize);
  DEBUG_PRINT(", Max OFiles : ");
  DEBUG_PRINT(fsInfo.maxOpenFiles);
  DEBUG_PRINT(", Max Path Len : ");
  DEBUG_PRINTLN(fsInfo.maxOpenFiles);
  DEBUG_PRINTLN("");

  loadConfig();

  joinAP(SSID_JOIN_TIMEOUT);
}

bool BoodskapNodeMCUCommunicator::loadConfig(){

  bool loaded = false;

  File configFile = SPIFFS.open(PLATFORM_CONFIG_FILE, "r");

  if (!configFile) {
    DEBUG_PRINTLN("Failed to open config file for reading");

    wifiSSID(WIFI_SSID);
    wifiPassword(WIFI_PASSWORD);
    host(BOODSKAP_HOST);
    port(BOODSKAP_PORT);
    domainKey(BOODSKAP_DOMAIN_KEY);
    apiKey(BOODSKAP_API_KEY);
    assetId(BOODSKAP_ASSET_ID);
    deviceModel(BOODSKAP_DEVICE_MODEL);
    firmwareVersion(BOODSKAP_FIRMWARE_VERSION);

  }else{

    size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
    std::unique_ptr<char[]> buf(new char[size]);

    // We don't use String here because ArduinoJson library requires the input
    // buffer to be mutable. If you don't use ArduinoJson, you may as well
    // use configFile.readString instead.
    configFile.readBytes(buf.get(), size);

    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject(buf.get());

    if (!json.success()) {
      DEBUG_PRINTLN("Failed to parse config file");
      loaded = false;
    }else{

      DEBUG_PRINTLN("Loaded config file");
      json.printTo(Serial);

      wifiSSID(json["ssid"]);
      wifiPassword(json["pswd"]);
      domainKey(json["key"]);
      apiKey(json["api"]);
      assetId(json["aid"]);
      host(json["host"]);
      port(json["port"]);
      deviceModel(BOODSKAP_DEVICE_MODEL);
      firmwareVersion(BOODSKAP_FIRMWARE_VERSION);

      loaded = true;
    }

    configFile.close();
  }

  return loaded;
}

bool BoodskapNodeMCUCommunicator::saveConfig(const char* path, JsonObject& json){

        bool saved = false;

        File configFile = SPIFFS.open(path, "w+");
        if (!configFile) {
          DEBUG_PRINT("Failed to open config file ");
          DEBUG_PRINT(path);
          DEBUG_PRINTLN(" for writing");
        }else{

          DEBUG_PRINT("Writing config ");
          DEBUG_PRINTLN(path);

          json.printTo(Serial);
          json.printTo(configFile);

          configFile.flush();
          configFile.close();

          saved = true;
        }

        return saved;

}

bool BoodskapNodeMCUCommunicator::saveSystemConfig(JsonObject& json){
  return saveConfig(PLATFORM_CONFIG_FILE, json);
}

bool BoodskapNodeMCUCommunicator::saveDeviceConfig(JsonObject& json){
  return saveConfig(DEVICE_CONFIG_FILE, json);
}

bool BoodskapNodeMCUCommunicator::isConnected(){
  return (WiFi.status() == WL_CONNECTED) ? true : false;
}

void BoodskapNodeMCUCommunicator::process()
{

  if(!isConnected()){
    joinAP(SSID_JOIN_TIMEOUT);
  }

  if (isConnected() && udp.parsePacket()) {

    udp.read(_packetBuffer, PACKET_BUFFER_LEN);

    BoodskapMessage msg;

    if (!BoodskapMessage::parse(msg, _packetBuffer)) {
      DEBUG_PRINTLN("Invalid data received");
      DEBUG_PRINTLN(_packetBuffer);
    }else{
      messageReceived(&msg);
    }

  }

  if(isConnected() && (millis() - _lastHeartbeat > _heartbeat)){
    ping();
  }

}

void BoodskapNodeMCUCommunicator::send(BoodskapMessage* message)
{
  DEBUG_PRINTLN("\n\nSENDING");
  message->get().printTo(Serial);
  DEBUG_PRINTLN("\n\n");


  if (!isConnected()) {
    DEBUG_PRINTLN("WiFi not connected, not sending");
  } else {

    udp.beginPacket(_host, _port);
    message->get().printTo(udp);
    udp.endPacket();

    _lastHeartbeat = millis();

  }
}

bool BoodskapNodeMCUCommunicator::joinAP(uint32_t timeout)
{

  if(_lastHeartbeat != 0) udp.stop();

  uint32_t started = millis();

  while(!isConnected()){

    DEBUG_PRINTLN("Attempting to connect to SSID: ");
    DEBUG_PRINTLN(_ssid);
    // Connect to WPA/WPA2 network
    WiFi.begin(_ssid, _password);
    delay(10000);

    if(timeout > 0 && (millis() - started >= timeout)){
      break;
    }
  }

  if (isConnected()) {

    udp.begin(_localPort);

    if(!MDNS.begin(_assetId)){
      DEBUG_PRINTLN("Unable to setup MDNS!!!");
    }else{
      DEBUG_PRINT("You can reach me @ http://");
      DEBUG_PRINT(_assetId);
      DEBUG_PRINTLN(".local");

      MDNS.addService("esp", "tcp", 80); // Announce esp tcp service on port 80

      DEBUG_PRINTLN("Sending mDNS query");
      int n = MDNS.queryService("esp", "tcp"); // Send out query for esp tcp services
      DEBUG_PRINTLN("mDNS query done");
      if (n == 0) {
        DEBUG_PRINTLN("no services found");
      }
      else {
        DEBUG_PRINT(n);
        DEBUG_PRINTLN(" service(s) found");
        for (int i = 0; i < n; ++i) {
          // Print details for each service found
          DEBUG_PRINT(i + 1);
          DEBUG_PRINT(": ");
          DEBUG_PRINT(MDNS.hostname(i));
          DEBUG_PRINT(" (");
          DEBUG_PRINT(MDNS.IP(i));
          DEBUG_PRINT(":");
          DEBUG_PRINT(MDNS.port(i));
          DEBUG_PRINTLN(")");
        }
      }
    }
  }

  return isConnected();
}

void BoodskapNodeMCUCommunicator::doOTA(BoodskapMessage* command){

  ESPhttpUpdate.rebootOnUpdate(true);
  t_httpUpdate_return ret = ESPhttpUpdate.update(command->data().get<const char*>("url"), BOODSKAP_FIRMWARE_VERSION);

  switch (ret) {
    case HTTP_UPDATE_FAILED:
      Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("HTTP_UPDATE_NO_UPDATES");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("HTTP_UPDATE_OK");
      break;
  }
}

void BoodskapNodeMCUCommunicator::commandReceived(BoodskapMessage* command){

  switch(command->id()){
    case 99: //OTA Command
      doOTA(command);
    break;
    default:
      DEBUG_PRINTLN("Unknown command received : " + command->id());
  }
}
