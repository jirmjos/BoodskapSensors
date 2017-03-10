#ifndef _BOODSKAP_CONFIG_H
#define _BOODSKAP_CONFIG_H

//
// ============================== Basic Configuration ===========================================================
//
#define WIFI_SSID "boodskap" // Change this to suit your wifi environment
#define WIFI_PASSWORD "b00dskap" // -- do --
#define BOODSKAP_HOST "192.168.1.159" //35.160.137.8
#define BOODSKAP_PORT 5555
#define BOODSKAP_DOMAIN_KEY "myremothgs" // You will get this once your register in the platform
#define BOODSKAP_API_KEY "FFet6ZnVU5th" // -- do --
#define BOODSKAP_ASSET_ID "mything" // a unique name
#define BOODSKAP_DEVICE_MODEL "Nodemcu 1.0" // Change this to your choice of name
#define BOODSKAP_FIRMWARE_VERSION "1.0.0" // Please follow Semantic Versioning Specification (SemVer) http://semver.org/
#define HEARTBEAT_INTERVAL 15000 // How frequently ping packets should be sent (milliseconds)
#define PACKET_BUFFER_LEN 512 //Max JSON data size, try to limit your data size within 512 bytes, split into multiple messages if you need so
//
//===================================================================================================================
//

#define BAUD_RATE 115200
#define SSID_LEN 40
#define SSID_PSWD_LEN 30
#define SSID_JOIN_TIMEOUT 60000
#define BOODSKAP_HOST_LEN 30
#define DOMAIN_KEY_LEN 15
#define API_KEY_LEN 15
#define ASSET_ID_LEN 20
#define DEVICE_MODEL_LEN 20
#define FIRMWARE_VER_LEN 15
#define UDP_LOCAL_PORT 1025
#define PLATFORM_CONFIG_FILE "/boodskap.json"
#define DEVICE_CONFIG_FILE "/config.json"

// Sensor Specific Configurations

#if defined(_THREE_PHASE_VOLTAGE_SENSOR_H)
#define VOLTAGE_CUTOFF 3.5
#define VRESISTANCE1 1000.0
#define VRESISTANCE2 500.0
#define PROCESS_DELAY 1500
#endif

#endif //_BOODSKAP_CONFIG_H
