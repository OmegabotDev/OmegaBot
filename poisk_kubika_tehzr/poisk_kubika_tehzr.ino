#define M1_DIR     4
#define M1_PWM     5
#define M2_DIR     7
#define M2_PWM     6



#include<Wire.h>
#include <Servo.h>

Servo servBase;
Servo servNeck;

int SLAVE_ADDRESS = 0x04;
int xpos = 0;
int ypos = 0;
byte rd;
int msg = 201;


void setup() {
  // put your setup code here, to run once:
  // Создаем обмен данными между платами Расбери и Ардуино
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(processMessage);
  Wire.onRequest(sendAnalogReading);
  // В коде должны быть объявлены функции с именами processMessage и sendAnalogReading

  servNeck.attach(9);
  servBase.attach(10);
  servBase.write(90); 
  servNeck.write(40); 

  InitMotors();


  

}

void(* resetFunc) (void) = 0; // эта функция будет вызывать перезагрузку, если произойдет сбой при передаче данных

// при перезагрузке платы все переменные примкут стартовые значения

void loop() {
  // put your main code here, to run repeatedly:
  
  
  int x = visionGetLine();

  // Двигаемся только в том случае, если робот видит линию
  if(x>-50 && x<50)
  {
    // Релейное регулирование по пяти позициям
    if(x>-10 && x<10)
    {
      Motors(0,0);
    }
    else
    {
      if(x>10)
      {
        Motors(-140,140);
        
      }
      if(x<-10)
      {
        Motors(140,-140);
      
      }
      
    }
  }
  else
  {
    Motors(0,0);
  }
    

 
  
  if(Wire.available()!=0) // Вызов перезагрузки в случае ошибки
      resetFunc();
  

}

// Моторы
void InitMotors()
{
  pinMode(M1_DIR, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_DIR, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
}



void Motors(int Speed1, int Speed2)
{
  if(Speed1 > 255)  Speed1 = 255;
  if(Speed1 < -255) Speed1 = -255;
  if(Speed2 > 255)  Speed2 = 255;
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


// vision functions

void visionStop() // Принудительная остановка функции компьютерного зрения на модуле Raspberry
{
  msg = 201;
}

int visionGetLine()
{
  msg = 249;
  delay(50);
  xpos = -1;
  xpos=rd;
  if( xpos == -1 || xpos>101)
  {
    return 404;
  }
  else
  {
    return xpos-50;
  }
  
  
}


//data transfering
void processMessage(int n) {
  rd = Wire.read();
  

  
}
void sendAnalogReading() {
  int reading  = msg;

  Wire.write(reading);
}
