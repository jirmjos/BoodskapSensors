#include "DigitalStateSensor.h"

DigitalStateSensor::DigitalStateSensor(uint8_t pin, uint32_t procDelay, void (*callback)(uint8_t on) ):
  _pin(pin), _procDelay(procDelay), _state(false), _lastProcessed(0), _callback(callback)
{

}

void DigitalStateSensor::setup()
{
  pinMode(_pin, INPUT);
}

void DigitalStateSensor::loop()
{
  if((millis() - _lastProcessed) < _procDelay) return;

  uint8_t state = digitalRead(_pin);

  if(state != _state){
    _callback(state);
  }

  _state = state;
  _lastProcessed = millis();
}
