#pragma once
#include "Serial.h"

SerialObject::SerialObject() {
  _buffer_size = 0;
}

void SerialObject::println(std::string text_to_print) {
  std::cout << text_to_print << std::endl;
}

void SerialObject::print(std::string text_to_print) {
  std::cout << text_to_print;
}

void SerialObject::println(int text_to_print) {
  std::cout << text_to_print << std::endl;
}

void SerialObject::print(int text_to_print) {
  std::cout << text_to_print;
}
