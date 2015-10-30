// TinyShell class body
// 2015/10/24 by kuma35

#include <ctype.h>
#include <HardwareSerial.h>
#include "TinyShell.h"

#define ERR_MSG_UNDERFLOW " Stack underflow."
#define ERR_MSG_OVERFLOW " Stack overflow."
#define ERR_MSG_UNKNOWN " unknow word."
#define CRLF "\r\n"

const int TinyShell::NEWLINE = 0x0D;

int TinyShell::do_help(void) {
  this->_serial->print(F(" ( ... -- ... ) stack effect. ( pre command stack -- post command stack" CRLF \
			 "help  ( -- ) is this command." CRLF		\
			 ".  ( n -- ) print number." CRLF));
  return 1;
}


int TinyShell::do_number_print(void) {
  // .  ( n -- ) print number.
  // ret:1;done, 0;error
  if (!this->_data_stack.popable(1)) {
    this->_serial->println(ERR_MSG_UNDERFLOW);
    return 0;
  }
  this->_serial->print(this->_data_stack.pop());
  return 1;
}

TinyShell::TinyShell()
  : _serial(&Serial) {
  //
  //this->_data_stack = new Stack();
  //this->_line_buffer = new String();
}

// ret:1 is number and number storing to num.
// ret:0 is not number. num no edited.
int TinyShell::isDigits(String *strp, int *nump) {
  int index = 0;
  if ((*strp)[index] != '-' &&
      (*strp)[index] != '+' &&
      !isdigit((*strp)[index])) {
    return 0;
  }
  for (index = 1; index < strp->length(); index++) {
    if (!isdigit((*strp)[index])) {
	return 0;
      }
  }
  *nump = atoi(strp->c_str());
  return 1;
}


// func: get line ( to newline ) and return last char. 
int TinyShell::get_line(void) {
  //String &line_buffer
  char c[2] = {'\0', '\0'};
  //Serial.print("get_line:");
  while (this->_serial->available()) {
    c[0] = (char)this->_serial->read();
    //Serial.print(" ");
    //Serial.print(c[0], HEX);
    this->_line_buffer += c;
    if (c[0] == NEWLINE) {
      this->_serial->println();	// echo newline
      break;
    } else {
      this->_serial->print(c);	// echo
    }
  }
  //Serial.println("");
  return c[0];
}


// ret:0 token end
// ret:1 token got
int TinyShell::get_token(String *tokenp) {
  int index;
  this->_line_buffer.trim();
  if (!this->_line_buffer.equals("")) {
    index = this->_line_buffer.indexOf(" ", 0);
    if (index > -1) {
      *tokenp = this->_line_buffer.substring(0, index);
      this->_line_buffer = this->_line_buffer.substring(index+1);
    } else {
      *tokenp = this->_line_buffer;
      this->_line_buffer = "";
    }
    return 1;
  } else {
    return 0;
  }
}

// ret:0 not number
// ret:1 number
int TinyShell::get_number(String *tokenp) {
  int num;
  if (this->isDigits(tokenp, &num)) {
    if (this->_data_stack.pushable(1)) {
      this->_data_stack.push(num);
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

String TinyShell::buffer(void) {
  return this->_line_buffer;
}

void TinyShell::clear_buffer(void) {
  this->_line_buffer = "";
}

// 適宜オーバーライドされたし。
// ret:>=1; command result code is plus 100
// ret:0; unknown word
int TinyShell::execute(String *tokenp) {
  int status = 100;	// execut return status. command result code is plus 100.
  if (tokenp->equals("help")) {
    status += this->do_help();
  } else if (tokenp->equals(".")) {
    status += this->do_number_print();
  } else if (tokenp->equals("dup")) {
    status += this->_data_stack.dup();
    if (status == 100) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_OVERFLOW);
    } else if (status == 99) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_UNDERFLOW);
    }
  } else if (tokenp->equals("swap")) {
    status += this->_data_stack.swap();
    if (status == 100) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_UNDERFLOW);
    }
  } else if (tokenp->equals("drop")) {
    status += this->_data_stack.drop();
    if (status == 100) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_UNDERFLOW);
    }
  } else if (tokenp->equals("over")) {
    status += this->_data_stack.over();
    if (status == 100) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_OVERFLOW);
    } else if (status == 99) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_UNDERFLOW);
    }
  } else if (tokenp->equals("rot")) {
    status += this->_data_stack.rot();
    if (status == 100) {
      this->_serial->print(tokenp->c_str());
      this->_serial->println(ERR_MSG_UNDERFLOW);
    }
  } else if (tokenp->equals("dump")) {
    this->_data_stack.dump(this->_serial);
  } else {
    // unknown word
    this->_serial->print(tokenp->c_str());
    this->_serial->println(ERR_MSG_UNKNOWN);
    status = 0;
  }
  return status;
}
