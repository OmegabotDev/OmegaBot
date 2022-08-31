#include <Servo.h>


Servo srvUp;
Servo srvClaw;
int isBtn =0;
void setup() {
  // put your setup code here, to run once:

  srvUp.attach(9);
  srvClaw.attach(10);
  pinMode(8, INPUT);
  srvUp.write(90);
  srvClaw.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  isBtn = digitalRead(8);
  if(isBtn)
  {
    srvUp.write(10);
    delay(500);
    srvUp.write(90);
    delay(500);
    for(int i = 0;i<3;i++)
    {
      srvClaw.write(10);
      delay(300);
      srvClaw.write(90);
      delay(300);
    }
    srvUp.write(10);
    
  }
  
  
}
