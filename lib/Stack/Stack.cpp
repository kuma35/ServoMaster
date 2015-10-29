// Stack class body
// 2013/07/04 by kuma35

#include <stdlib.h>
#include "Stack.h"


Stack::Stack() {
  this->_pointer = this->_empty;
}

void Stack::clear(void) {
  this->_pointer = this->_empty;
}


bool Stack::popable(int depth) {
  return ((this->_pointer - depth) >= this->_empty);
}


bool Stack::pushable(int depth) {
  return (this->_pointer + depth <= this->_size);
}


void Stack::push(int value) {
  ++(this->_pointer);
  this->_stack[this->_pointer] = value;
}

int Stack::pop(void) {
  int value;
  value = this->_stack[this->_pointer];
  (this->_pointer)--;
  return value;
}


void Stack::dump(HardwareSerial &serial) {
  serial.print("data-stack:");
  if (this->popable()) {
    for (int i=0;i<=this->_pointer;i++) {
      serial.print(this->_stack[i]);
      serial.print(",");
    }
    serial.println("");
  } else {
    serial.println("stack empty.");
  }
}

/*func: get stack depth
 *ret:depth
 */
int Stack::do_depth(void) {
  return this->_pointer + 1;
}

/*func: ( n -- n n)
 *ret:1;OK
 *ret:0;Stack Overflow
 */
int Stack::dup(void) {
  if (this->pushable(1)) {
    ++this->_pointer;
    this->_stack[this->_pointer] = this->_stack[this->_pointer - 1];
    return 1;
  } else {
    return 0;
  }
}

/*func: ( m n -- n m)
 *ret:1;OK
 *ret:0;Stack Overflow
 */
int Stack::swap(void) {
  if (this->do_depth() >= 2) {
    int value = this->_stack[this->_pointer];
    this->_stack[this->_pointer] = this->_stack[this->_pointer - 1];
    this->_stack[this->_pointer - 1] = value;
    return 1;
  } else {
    return 0;
  }
}

/*func (n -- )
 *ret:1;OK
 *ret:0;underflow
 */
int Stack::drop(void) {
  if (this->popable(1)) {
    this->_pointer--;
  } else {
    return 0;
  }
}

/*func:( n m -- n m n)
 *ret:1;OK
 *ret:0;overflow
 *ret:-1;underflow
 */
int Stack::over(void) {
  if (this->popable(2)) {
    if (this->pushable(1)) {
      ++(this->_pointer);
      this->_stack[this->_pointer] = this->_stack[this->_pointer - 2];
      return 1;
    } else {
      return 0;
    }
  } else {
    return -1;
  }
}

/*func (n1 n2 n3 -- n2 n3 n1)
 *ret:1;OK
 *ret:0;underflow
 */
int Stack::rot(void) {
  if (this->popable(3)) {
    int value = this->_stack[this->_pointer - 2];
    this->_stack[this->_pointer - 2] = this->_stack[this->_pointer - 1];
    this->_stack[this->_pointer - 1] = this->_stack[this->_pointer];
    this->_stack[this->_pointer] = value;
    return 1;
  } else {
    return 0;
  }
}
