// Stack class header
// 2013/07/04 by kuma35
// for push:
// if (this->pushable()) push(), etc.
// for pop
// if (this->popable()) pop(), etc.

#ifndef _Stack_h_
#define _Stack_h_
//#include <arduino.h>
#include <HardwareSerial.h>

#define STACK_SIZE_DEFAULT_INIT 16

class Stack {
 private:
  static const int _stack_size_default = STACK_SIZE_DEFAULT_INIT;
  static const int _stack_empty = -1;
  HardwareSerial *serial;
  int _stack_size;
  int _stack_buffer[STACK_SIZE_DEFAULT_INIT];
  int _stack_pointer;
 public:
  Stack();
  int popable(int depth = 1);
  int pop(void);
  int pushable(int depth = 1);
  void push(int value);
  void clear(void);
  void dump(HardwareSerial &serial);
  void dup(void);
};

#endif
