// TinyShell class header
// 2015/10/24 by kuma35

#ifndef _TinyShell_h_
#define _TinyShell_h_
//#include <arduino.h>
#include <HardwareSerial.h>
#include "Stack.h"

class TinyShell {
 private:
  String _line_buffer;
  HardwareSerial *_serial;
  Stack _data_stack;
  int do_help(void);
  int do_number_print(void);
  int isDigits(String *tokenp, int *nump);
 public:
  TinyShell();
  const static int NEWLINE;
  int get_line(void);
  int get_token(String *tokenp);
  int get_number(String *tokenp);
  String buffer(void);
  void clear_buffer(void);
  int execute(String *tokenp);
};

#endif
