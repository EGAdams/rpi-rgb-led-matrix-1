#pragma once
#ifndef Serial_h
#define Serial_h
#include <iostream>

class SerialObject {
 public:
  SerialObject();
  void println(std::string text_to_print);
  void print(std::string text_to_print);
  void println(int text_to_print);
  void print(int text_to_print);

 private:
  int _buffer_size;
};

#endif