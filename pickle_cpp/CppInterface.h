#ifndef CppInterface_H
#define CppInterface_H

#include <napi.h>
#include <iostream>
#include "../GameObject/GameObject.h"
#include "../PinInterface/PinInterface.h"
#include "../PinState/PinState.h"
#include "../TranslateConstant/TranslateConstant.h"

class CppInterface : public Napi::ObjectWrap<CppInterface> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  CppInterface(const Napi::CallbackInfo& info);

 private:
  Napi::Value GetValue(const Napi::CallbackInfo& info);
  Napi::Value PlusOne(const Napi::CallbackInfo& info);
  Napi::Value Multiply(const Napi::CallbackInfo& info);
  Napi::Value digitalWrite(const Napi::CallbackInfo& info);
  Napi::Value digitalRead(const Napi::CallbackInfo& info);
  Napi::Value analogRead(const Napi::CallbackInfo& info);
  Napi::Value gameLoop(const Napi::CallbackInfo& info);
  Napi::Value getPinMap(const Napi::CallbackInfo& info);
  double value_;
  TranslateConstant _translator;
  GameObject* _gameObject;
  GameObject gameObject;
  PinInterface* _pinInterface;
  PinState* _pinState;
  std::map<std::string, int> _cpp_interface_pin_map;
};

#endif
