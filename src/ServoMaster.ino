// ServoMaster.ino
// I2C master
// ServoMaster control my pwm port and remote pwm port by I2C communication.
// 2015.10.07 by kuma35
// prototyping
// master is THIS, slave is pro5v328 * 2,
// slave address 1, 2.
// servo id is, master 0,1, slave1 0,1, slave2 0,1

#include <HardwareSerial.h>
#include <Servo.h>
#include <Wire.h>

#include "TinyShell.h"

#define SERVO_VOLUME 2
#define LF '\x0aa'
#define ERR_STACK_OVERFLOW "Stack over flow"

Servo Servos[SERVO_VOLUME];

TinyShell Shell;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  Servos[0].attach(3);
  Servos[1].attach(5);
}

int i = 0;
int j = 1500;

String token;

void loop()
{
  if (Shell.get_line() == Shell.NEWLINE) {
    while (Shell.get_token(&token)) {
      if (Shell.get_number(&token)) {
      } else if (Shell.execute(&token)) {
      }
    }
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
