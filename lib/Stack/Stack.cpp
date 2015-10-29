// Stack class body
// 2013/07/04 by kuma35

#include <stdlib.h>
#include "Stack.h"


Stack::Stack() {
  this->_stack_size = this->_stack_size_default;
  this->_stack_pointer = this->_stack_empty;
}

void Stack::clear(void) {
  this->_stack_pointer = this->_stack_empty;
}


int Stack::popable(int depth) {
  return ((this->_stack_pointer - depth) >= this->_stack_empty);
}


int Stack::pushable(int depth) {
  return (this->_stack_pointer + depth <= this->_stack_size);
}


void Stack::push(int value) {
  ++(this->_stack_pointer);
  this->_stack_buffer[this->_stack_pointer] = value;
}

int Stack::pop(void) {
  int value;
  value = this->_stack_buffer[this->_stack_pointer];
  (this->_stack_pointer)--;
  return value;
}


void Stack::dump(HardwareSerial &serial) {
  serial.print("data-stack:");
  if (this->popable()) {
    for (int i=0;i<=_stack_pointer;i++) {
      serial.print(_stack_buffer[i]);
      serial.print(",");
    }
    serial.println("");
  } else {
    serial.println("stack empty.");
  }
}

/*func: ( n -- n n)
 *ret:1;OK
 */
void Stack::dup(void) {
  ++this->_stack_pointer;
  this->_stack_buffer[this->_stack_pointer] = this->_stack_buffer[this->_stack_pointer - 1];
}
