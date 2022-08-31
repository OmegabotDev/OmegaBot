#include <Servo.h>
Servo servo_pin_9;
Servo servo_pin_10;
#define M1_DIR 4
#define M1_PWM 5
#define M2_DIR 7
#define M2_PWM 6
int ButtonRead(int ButtonPin)
{
  int Data = digitalRead(ButtonPin);
  if (ButtonPin == 12) Data = !Data;
  return Data;
}

void ServoMove(Servo S1, int Angle)
{
  if (Angle > 180) Angle = 180;
  if (Angle < 0) Angle = 0;

  S1.write(Angle);
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
  if (Speed1 > 255) Speed1 = 255;
  if (Speed1 < -255) Speed1 = -255;
  if (Speed2 > 255) Speed2 = 255;
  if (Speed2 < -255) Speed2 = -255;

  if (Speed1 > 0)
  {
    digitalWrite(M1_DIR, 1);
    analogWrite(M1_PWM, Speed1);
  }
  else
  {
    digitalWrite(M1_DIR, 0);
    analogWrite(M1_PWM, -Speed1);
  }

  if (Speed2 > 0)
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

//Ехать вперед
void MoveForward(int Speed)
{
  Motors(Speed, Speed);
}

int IKSensorRead(int SensorPin)
{
  if (SensorPin < A0 || SensorPin > A3)
    return -1;

  int Data = analogRead(SensorPin);
  return Data;
}

void Stop()
{
  Motors(0, 0);
}

void MoveLeft(int Speed)
{
  Motors(Speed, -Speed);
}

void MoveLeftByDelay(int Speed, int Time)
{
  MoveLeft(Speed);
  delay(Time);
  Stop();
}

void MoveForwardByDelay(int Speed, int Time)
{
  MoveForward(Speed);
  delay(Time);
  Stop();
}


void setup()
{
  pinMode(12, INPUT_PULLUP);
  servo_pin_9.attach(9);
  servo_pin_10.attach(10);
  InitMotors();
}

void loop()
{
  while ( ButtonRead(12) )
  {
    do
    {
      ServoMove(servo_pin_9, 170);
      ServoMove(servo_pin_10, 0);
      MoveForward(90);
    }
    while (( ( 500 ) > ( IKSensorRead(A0) ) ));
    Stop();
    delay( 1000 );
    ServoMove(servo_pin_9, 120);
    delay( 1000 );
    ServoMove(servo_pin_9, 0);
    delay( 1000 );
    ServoMove(servo_pin_10, 45);
    delay( 1000 );
    MoveLeftByDelay(100, 1000);
    delay( 1000 );
    MoveForwardByDelay(150, 1500 );
    delay( 1000 );
    ServoMove(servo_pin_10, 0);
    delay( 1000 );
    ServoMove(servo_pin_9, 100);
  }

  Stop();
}
