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

#include "TinyShell.h"

#define SERVO_VOLUME 2
#define LF '\x0aa'
#define ERR_STACK_OVERFLOW "Stack over flow"
#define EEPROM_I2C_ADDR	0
#define EEPROM_SERVO_0 1 
#define EEPROM_SERVO_1 2

Servo Servos[SERVO_VOLUME];

TinyShell Shell;
int I2c_addr;

void setup()
{
  Serial.begin(9600);
  Serial.print(F("I am I2C "));
  I2c_addr = EEPROM.read(EEPROM_I2C_ADDR);
  if (I2c_addr) {
    Wire.begin(I2c_addr);	// I2C slave
    Serial.print(F("slave;"));
    Serial.println(I2c_addr);
  } else {
    Wire.begin();	// I2C master
    Serial.println(F("master;0"));
  }
  int servo_pin = EEPROM.read(EEPROM_SERVO_0);
  Serial.print(F("Servo 0 to D"));
  Serial.println(servo_pin);
  Servos[0].attach(servo_pin);
  servo_pin = EEPROM.read(EEPROM_SERVO_1);
  Serial.print(F("Servo 1 to D"));
  Serial.println(servo_pin);
  Servos[1].attach(servo_pin);

  Serial.print(">");
}

int i = 0;
int j = 1500;

String token;

void loop()
{
  if (Shell.get_line() == Shell.NEWLINE) {
    while (Shell.get_token(&token)) {
      if (Shell.get_number(&token)) {
      } else if (Shell.execute(&token) <= 100) {
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
  }
  if (i < 1500) {
    Servos[0].writeMicroseconds(i);
  } else {
    i = 0;
  }
  if (j > 0) {
    Servos[1].writeMicroseconds(j);
  } else {
    j = 1500;
  }
  delay(100);
  i += 100;
  j -= 100;
}
