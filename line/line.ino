#include <Wire.h>

#define M1_DIR 4
#define M1_PWM 5
#define M2_DIR 7
#define M2_PWM 6
int _1_datchikleft = 0 ;
int LineSensorRead(int SensorPin)
{
  if(SensorPin < A0 || SensorPin > A3)
    return -1;

  int Data = analogRead(SensorPin);
  return Data;
}

int _2_datchikright = 0 ;
int _3_speed = 0 ;
int _4_reg = 0 ;
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
int UltrasonikRead()
{
  int err_cnt = 0;
  Wire.requestFrom(0x38, 1);  //Отправляем запрос для получения данных с датчика
  while((Wire.available() == 0) && (err_cnt <= 25600)){
    err_cnt++;
  }
  if (err_cnt >= 25600)
    return (-1);                      //Ошибка работы датчика
  else
  {     
    int Data = Wire.read();              // Получаем дальность с датчика в сантиметрах
    return (Data);
  }
}

void setup()
{
  InitMotors();
  Wire.begin();
  Serial.begin(9600);
  Wire.setClock(8000);
}

void loop()
{
  _1_datchikleft = LineSensorRead(A3) ;
  _2_datchikright = LineSensorRead(A2) ;
  _3_speed = 120;
  int flag = 1;
  //int Data = UltrasonikRead();
  //Serial.println(Data);
  
  /*if(Data < 20){
    flag = 0;
  }
  */
  _4_reg = ( 1 * ( ( _1_datchikleft - 70 ) - _2_datchikright ) ) ;
  Motors(( _3_speed + _4_reg )*flag, ( _3_speed - _4_reg )*flag);
}
