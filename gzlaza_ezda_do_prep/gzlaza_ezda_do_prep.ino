#include <Servo.h>
#include <Wire.h>
Servo servo_pin_9;                
#define M1_DIR 4
#define M1_PWM 5
#define M2_DIR 7
#define M2_PWM 6
void ServoMove(Servo S1, int Angle)
{
  if(Angle > 180) Angle = 180;
  if(Angle < 0) Angle = 0;

  S1.write(Angle);
}

void InitUltrasonic()
{
  // Открываем соединение с датчиком по I2C
  Wire.begin();
  Wire.setClock(8000);
}

int UltrasonikRead()
{
  // Запрашиваем данные с датчика
  Wire.requestFrom(56, 1);
  // Ждем ответа с датчика
  while(Wire.available() == 0);
  // Получаем дальность с датчика расстояния в сантиметрах
  int S = Wire.read();
  delay(50);

  return (S);
}

void InitMotors()
{
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
}

void Motors(int Speed1, int Speed2)
{
  if(Speed1 > 255) Speed1 = 255;
  if(Speed1 < -255) Speed1 = -255;
  if(Speed2 > 255) Speed2 = 255;
  if(Speed2 < -255) Speed2 = -255;

  if(Speed1 > 0)
  {
    digitalWrite(M1_DIR, 1);
    analogWrite(M1_PWM, Speed1);
  }
  else
  {
    digitalWrite(M1_DIR, 0);
    analogWrite(M1_PWM, -Speed1);
  }

  if(Speed2 > 0)
  {
    digitalWrite(M2_DIR, 1);
    analogWrite(M2_PWM, Speed2);
  }
  else
  {
    digitalWrite(M2_DIR, 0);
    analogWrite(M2_PWM, -Speed2);
  }
}


void setup()
{
  servo_pin_9.attach(9);
  InitUltrasonic();
  InitMotors();
}

void loop()
{
  ServoMove(servo_pin_9,90);
  if (( ( UltrasonikRead() ) > ( 30 ) ))
  {
    Motors(100, 100);
  }
  else
  {
    if (( ( 20 ) > ( UltrasonikRead() ) ))
    {
      Motors(-120, -120);
    }
    else
    {
      Motors(0, 0);
    }
  }
}
