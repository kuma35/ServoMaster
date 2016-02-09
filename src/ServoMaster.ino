// ServoMaster.ino
// I2C master
// ServoMaster control my pwm port and remote pwm port by I2C communication.
// 2015.10.07 by kuma35
// prototyping
// master is THIS, slave is pro5v328 * 2,
// slave address 1, 2.
// servo id is, master 0,1, slave1 0,1, slave2 0,1

#include <EEPROM.h>
#include <HardwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include <string.h>

#include "TinyShell.h"

#define SERVO_VOLUME 2
#define CR '\x0d'
#define LF '\x0a'
#define ERR_STACK_OVERFLOW "Stack over flow"
#define EEPROM_I2C_ADDR	0
#define EEPROM_SERVO_0 1 
#define EEPROM_SERVO_1 2
#define RECEIVE_BUF_LEN 130
#define EEPROM_SERVO_0_MIN 3	// big endian 2 byte int. 0x1234 to 0x12,0x34
#define EEPROM_SERVO_0_MAX 5	// big endian 2 byte int. 0x1234 to 0x12,0x34
#define EEPROM_SERVO_1_MIN 7	// big endian 2 byte int. 0x1234 to 0x12,0x34
#define EEPROM_SERVO_1_MAX 9	// big endian 2 byte int. 0x1234 to 0x12,0x34

Servo Servos[SERVO_VOLUME];
int Servo_min[SERVO_VOLUME];
int Servo_max[SERVO_VOLUME];

TinyShell Shell;
int I2c_addr;

void get_receive(int bytes) {
  int target_servo = -1;
  int pulse = 0;
  if (bytes < 1) {
    Serial.println(F("no receive data"));
    return;
  }
  while (bytes) {
    unsigned char data = Wire.read();
    bytes--;
    switch (data) {
      // command
      // 'S',<<servo_number>>,
      // 'W', <<num-high>>, <<num-low>>	writeMicroseconds()
      // 'C' center
    case 'S':
      if (bytes < 1) {
	Serial.println(F("no target number"));
	bytes = 0;
	break;
      }
      target_servo = (int)Wire.read();
      bytes--;
      break;
    case 'W':
      if (target_servo == -1) {
	Serial.print(F("no target servo:"));
	bytes = 0;
	break;
      }
      if (bytes < 2) {
	Serial.println(F("no pulse value"));
	bytes = 0;
	break;
      }
      pulse = (Wire.read() << 8)|(Wire.read());
      Servos[target_servo].writeMicroseconds(pulse);
      bytes -= 2;
      break;
    case 'C':
      if (target_servo == -1) {
	Serial.print(F("no target servo:"));
	bytes = 0;
	break;
      }
      Servos[target_servo].writeMicroseconds((Servo_min[target_servo] + Servo_max[target_servo]) / 2);
      bytes--;
      break;
    default:
      Serial.print(F("unknown command:"));
      Serial.println(data, HEX);
      bytes = 0;
      break;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.print(F("I am I2C "));
  I2c_addr = EEPROM.read(EEPROM_I2C_ADDR);
  if (I2c_addr) {
    Wire.begin(I2c_addr);	// I2C slave
    Wire.onReceive(get_receive);
    Serial.print(F("slave;"));
    Serial.println(I2c_addr);
  } else {
    Wire.begin();	// I2C master
    Serial.println(F("master;0"));
  }
  int servo_pin = EEPROM.read(EEPROM_SERVO_0);
  Serial.print(F("Servo 0 to D"));
  Serial.print(servo_pin);
  Servos[0].attach(servo_pin);
  Servo_min[0] = ((int)EEPROM.read(EEPROM_SERVO_0_MIN) << 8) |
    (EEPROM.read(EEPROM_SERVO_0_MIN+1));
  Servo_max[0] = ((int)EEPROM.read(EEPROM_SERVO_0_MAX) << 8) |
    (EEPROM.read(EEPROM_SERVO_0_MAX+1));
  Serial.print(F(" "));
  Serial.print(Servo_min[0]);
  Serial.print(F(" - "));
  Serial.println(Servo_max[0]);
  
  servo_pin = EEPROM.read(EEPROM_SERVO_1);
  Serial.print(F("Servo 1 to D"));
  Serial.print(servo_pin);
  Servos[1].attach(servo_pin);
  Servo_min[1] = ((int)EEPROM.read(EEPROM_SERVO_1_MIN) << 8) |
    (EEPROM.read(EEPROM_SERVO_1_MIN+1));
  Servo_max[1] = ((int)EEPROM.read(EEPROM_SERVO_1_MAX) << 8) |
    (EEPROM.read(EEPROM_SERVO_1_MAX+1));
  Serial.print(F(" "));
  Serial.print(Servo_min[1]);
  Serial.print(F(" - "));
  Serial.println(Servo_max[1]);

  Serial.print(">");
}

String buffer;
String token;

void loop()
{
  if (Serial.available() >0) {
    delay(10);
    int last_char = Shell.get_line();
    //Serial.print(F("last_char:"));
    //Serial.print(last_char, HEX);
    //Serial.print(F("TinyShell::NEWLINE"));
    //Serial.print(TinyShell::NEWLINE, HEX);
    if (last_char == CR) {
      //Serial.print(F("in CR"));
      while (Shell.get_token(&token)) {
	//Serial.print(F("in While"));
	if (Shell.get_number(&token)) {
	  //Serial.print(F("number"));
	} else if (Shell.execute(&token) <= 100) {
	  //Serial.print(F("executed error"));
	  // command execution status:
	  // 0;unkown word
	  // 100<=status;command execute done. and error
	  // 101>=status;command execute done. and success continue next word execute...
	  Serial.print(F("stop. not yet exec words: "));
	  Serial.println(Shell.buffer().c_str());
	  Shell.clear_buffer();
	  break;	// stop interpreting
	}
      }
      Serial.print(F(">"));
    } else {
      //Serial.print(F("TinyShell::NEWLINE passed"));
    }
  }
 }
