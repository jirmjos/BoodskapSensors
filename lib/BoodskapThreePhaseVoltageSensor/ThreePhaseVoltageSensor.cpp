#include "ThreePhaseVoltageSensor.h"

ThreePhaseVoltageSensor::ThreePhaseVoltageSensor(uint8_t p1, uint8_t p2, uint8_t p3, void (*callback)(bool p1, bool p2, bool p3)):
   _callback(callback), _oP1(false), _oP2(false), _oP3(false), _p1(p1), _p2(p2), _p3(p3), _lastProcessed(0)
{

}

void ThreePhaseVoltageSensor::setup() {
  pinMode(_p1, INPUT);
  pinMode(_p2, INPUT);
  pinMode(_p3, INPUT);
}

void ThreePhaseVoltageSensor::loop() {

  if((millis() - _lastProcessed) < PROCESS_DELAY) return;

  bool p1 = readVoltage(_p1);
  bool p2 = readVoltage(_p2);
  bool p3 = readVoltage(_p3);
  
  if (p1 != _oP1 || p2 != _oP2 || p3 != _oP3) {
    _callback(p1, p2, p3);
  }

  _oP1 = p1;
  _oP2 = p2;
  _oP3 = p3;

  _lastProcessed = millis();
}

void ThreePhaseVoltageSensor::voltage(bool* p1, bool* p2, bool* p3){
  *p1 = _oP1;
  *p2 = _oP2;
  *p3 = _oP3;
}

bool ThreePhaseVoltageSensor::readVoltage(uint8_t pin)
{

  int value = analogRead(pin);
  float vout = (value * 5.0) / 1024.0; // see text
  float vin = vout / (VRESISTANCE2 / (VRESISTANCE1 + VRESISTANCE2));
  return vin >= VOLTAGE_CUTOFF ? true : false;
}
