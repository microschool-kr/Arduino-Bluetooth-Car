// 블루투스
// RX => D3
// Tx => D2
// 모터 드라이버
// A1 => 5
// A2 => 6
// B1 => 10
// B2 => 11
// 초음파 센서
// echo => 12
// trig => 13

#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3);

int motor_AA = 5;
int motor_AB = 6;
int motor_BA = 10;
int motor_BB = 11;

int echo_pin = 12;
int trig_pin = 13;

long distance = 0;
char dir = 'S';
char ch = 'S';

int speed = 110;

unsigned long last_time = 0;
unsigned long now_time = 0;
unsigned long bt_time = 0;

void setup()
{
  Serial.begin(115200);
  BT.begin(9600);

  pinMode(motor_AA, OUTPUT);
  pinMode(motor_AB, OUTPUT);
  pinMode(motor_BA, OUTPUT);
  pinMode(motor_BB, OUTPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  drive(ch);
}

void drive(char c)
{
  if (c == 'F')
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, speed);
    analogWrite(motor_BA, speed);
    analogWrite(motor_BB, 0);
    dir = 'F';
  }
  else if (c == 'L')
  {
    analogWrite(motor_AA, speed);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, speed);
    analogWrite(motor_BB, 0);
    dir = 'L';
  }
  else if (c == 'R')
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, speed);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, speed);
    dir = 'R';
  }
  else if (c == 'B')
  {
    analogWrite(motor_AA, speed);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, speed);
    dir = 'B';
  }
  else if (c == 'G')
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, speed / 2);
    analogWrite(motor_BA, speed);
    analogWrite(motor_BB, 0);
    dir = 'G';
  }
  else if (c == 'I')
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, speed);
    analogWrite(motor_BA, speed / 2);
    analogWrite(motor_BB, 0);
    dir = 'I';
  }
  else if (c == 'H')
  {
    analogWrite(motor_AA, speed / 2);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, speed);
    dir = 'H';
  }
  else if (c == 'J')
  {
    analogWrite(motor_AA, speed);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, speed / 2);
    dir = 'J';
  }
  else if (c == 'S' || c == 'D')
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, 0);
    dir = 'S';
  }
  else
  {
    drive(dir);
  }
}

void getDistance()
{
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  distance = pulseIn(echo_pin, HIGH) * 17 / 1000;
}

void loop()
{
  now_time = millis();
  if (now_time - last_time >= 10)
  {
    getDistance();
    if (distance < 10)
    {
      drive('S');
    }
    last_time = now_time;
  }
  if (BT.available())
  {
    ch = BT.read();
    bt_time = now_time;
    if (distance < 10 && ch == 'F' || ch == 'G' || ch == 'I')
    {
      drive('S');
    }
    drive(ch);
  }
  else if (now_time - bt_time >= 1000)
  {
    ch = 'S';
    drive(ch);
  }
}
