#include "PushButtonSwitch.h"

PushButtonSwitch::PushButtonSwitch(uint8_t pin, uint32_t pressDelay, void (*callback)(bool on) ):
  _pin(pin), _pressDelay(pressDelay), _state(false), _lastPressed(0), _callback(callback)
{

}

void PushButtonSwitch::setup()
{
  pinMode(_pin, INPUT);
}

void PushButtonSwitch::loop()
{
  if((millis() - _lastPressed) < _pressDelay) return;

  bool pressed = (digitalRead(_pin) == LOW);

  if(pressed){
    _state = !_state;
    _callback(_state);
  }

  _lastPressed = millis();
}
