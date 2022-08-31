#include <Wire.h>
#include <Servo.h>
Servo servo_pin_9;
int _1_goal = 0 ;
int ButtonRead(int ButtonPin)
{
  int Data = digitalRead(ButtonPin);
  if(ButtonPin == 12) Data = !Data;
  return Data;
}

int _2_distance = 0 ;
int _3_angle = 0 ;
int _4_dst = 0 ;
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

void ServoMove(Servo S1, int Angle)
{
  if(Angle > 180) Angle = 180;
  if(Angle < 0) Angle = 0;

  S1.write(Angle);
}


void setup()
{
  InitUltrasonic();
  servo_pin_9.attach(9);
  _1_goal = 90 ;

}

void loop()
{
  if (ButtonRead(8))
  {
    _2_distance = 300 ;
    _1_goal = 90 ;
    for (_3_angle= 180; _3_angle>= ( 1 ); _3_angle-- )
    {
      ServoMove(servo_pin_9,_3_angle);
      _4_dst = UltrasonikRead() ;
      if (( ( ( _4_dst ) <= ( _2_distance ) ) && ( ( 10 ) <= ( _4_dst ) ) ))
      {
        _1_goal = _3_angle ;
        _2_distance = _4_dst ;
      }
    }
  }
  ServoMove(servo_pin_9,_1_goal);
}
