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

#define STOP 5
#define FOWARD 1
#define LEFT 2
#define RIGHT 3
#define BACK 4

SoftwareSerial BT(2, 3);

int motor_AA = 5;
int motor_AB = 6;
int motor_BA = 10;
int motor_BB = 11;

int echo_pin = 12;
int trig_pin = 13;

long distance = 0;
uint8_t dir = STOP;

unsigned long last_time = 0;
unsigned long now_time = 0;

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
  drive();
}

void drive()
{
  if (dir == FOWARD)
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, 128);
    analogWrite(motor_BA, 128);
    analogWrite(motor_BB, 0);
  }
  else if (dir == LEFT)
  {
    analogWrite(motor_AA, 128);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 128);
    analogWrite(motor_BB, 0);
  }
  else if (dir == RIGHT)
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, 128);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, 128);
  }
  else if (dir == BACK)
  {
    analogWrite(motor_AA, 128);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, 128);
  }
  else
  {
    analogWrite(motor_AA, 0);
    analogWrite(motor_AB, 0);
    analogWrite(motor_BA, 0);
    analogWrite(motor_BB, 0);
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
//  getDistance();
  if (BT.available())
  {
    now_time = millis();
    if (now_time - last_time >= 100)
    {
      getDistance();
      last_time = now_time;
    }
    dir = BT.read();
    if (distance < 10 && dir == FOWARD)
    {
      dir = STOP;
    }
    drive();
  }
  else if (millis() - now_time >= 1000)
  {
    dir = STOP;
    drive();
  }
}
