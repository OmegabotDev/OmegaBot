
#include "botControl.h"

struct cmd
{
  int cmd_id;
  unsigned int delaySec;
  int val;

};

int k = 1;
int mspeed = 0;
cmd commands[100];
int iter = 0;
int numComs = 0;
char bytes[8];
char left_motor_dir;
char right_motor_dir;
int left_motor_power;
int right_motor_power;

void setup() {
  // put your setup code here, to run once:
  InitMotors();
  Serial.begin(9600);
  Serial.println("Programm started:");

}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {//если пришли данные
    // read the incoming byte:
    Serial.readBytes(bytes, 8);
    Serial.println("Here comes the byte:");
    Serial.print(" Byte 01 = ");
    Serial.println(bytes[0], DEC);
    Serial.print(" Byte 02 = ");
    Serial.println(bytes[1], DEC);
    Serial.print(" Byte 03 = ");
    Serial.println(bytes[2], DEC);
    Serial.print(" Byte 04 = ");
    Serial.println(bytes[3], DEC);
    Serial.print(" Byte 05 = ");
    Serial.println(bytes[4], DEC);
    Serial.print(" Byte 06 = ");
    Serial.println(bytes[5], DEC);
    Serial.print(" Byte 07 = ");
    Serial.println(bytes[6], DEC);
    Serial.print(" Byte 08 = ");
    Serial.println(bytes[7], DEC);
    Serial.println("----------------");

    if (int(bytes[0]) == 0)
    {
      Serial.println("Run");
      Serial.println(runBot());
    }
    if (int(bytes[0]) == 2)
    {
      Serial.println("Stop");
      Motors(0, 0);
      numComs = 0;
    }
    if (int(bytes[0]) == 3)
    {
      Serial.println("joystick controll");
      // joystick controll
      left_motor_dir = bitRead(bytes[1], 7);
      right_motor_dir = bitRead(bytes[2], 7);
      left_motor_power = bytes[1] & 0b01111111;
      right_motor_power = bytes[2] & 0b01111111;
      Serial.println(left_motor_power);
      Serial.println(right_motor_power);
      left_motor_power = left_motor_power * 255 / 100;
      right_motor_power = right_motor_power * 255 / 100;
      left_motor_power = changeDir(left_motor_dir, left_motor_power);
      right_motor_power = changeDir(right_motor_dir, right_motor_power);
      Serial.println(left_motor_power);
      Serial.println(right_motor_power);
      Motors(left_motor_power, right_motor_power);

      while (Serial.available() > 8)
      {
        Serial.println(Serial.available());
        Serial.println(Serial.readBytes(bytes, 8));
      }

      // legacy mode
      /*
        int mLspeed = int(int(bytes[1])*255/100);
        int mRspeed = int(int(bytes[2])*255/100);
        Motors(int(bytes[1]*k),int(bytes[2])*k);
      */
    }
    if (int(bytes[0]) == 4)
    {
      int n = bytes[3];
      Serial.print("Record comands: ");
      Serial.println(n);
      numComs = 0;
      for (int i = 0; i < n; i++)
      {
        if (Serial.available() > 0)
        {
          Serial.readBytes(bytes, 8);
          Serial.print("comand: ");
          
          commands[i].cmd_id = bytes[0];
          Serial.println(commands[i].cmd_id);
          Serial.println("--------");
          Serial.println(bytes[1],BIN);
          Serial.println("--------");
          Serial.println(bytes[2],BIN);
          Serial.println("--------");
          commands[i].delaySec = (((int( bytes[2]&0b11111111 )) << 8) | int(bytes[1]&0b11111111));
          Serial.println(commands[i].delaySec,BIN);
          Serial.println(commands[i].delaySec);
          commands[i].val = bytes[3];
          
          numComs++;
        }

      }

    }

  }
  else
  {
    Motors(0, 0);

  }

}



int runBot()
{
  for (int i = 0; i < numComs; i++)
  {
    switch (commands[i].cmd_id)
    {
      case 1:
        Serial.print(" W: ");
        Serial.println(commands[i].val);
        mspeed = int(int(commands[i].val) * 255 / 100);
        Serial.println(mspeed);
        mspeed = 180;
        Motors(mspeed, mspeed);
        delay(commands[i].delaySec);
        break;
      case 2:
        Serial.print(" s: ");
        Serial.println(commands[i].val);

        mspeed = int(int(commands[i].val) * 255 / 100);
        Serial.println(mspeed);
        mspeed = 180;
        Motors(-mspeed, -mspeed);
        delay(commands[i].delaySec);
        break;
      case 3:
        Serial.print(" a: ");
        Serial.println(commands[i].val);
        mspeed = int(int(commands[i].val) * 255 / 100);
        Serial.println(mspeed);
        mspeed = 180;
        Motors(-mspeed, mspeed);
        delay(commands[i].delaySec);
        break;
      case 4:
        Serial.print(" d: ");
        Serial.print(commands[i].val);
        mspeed = int(int(commands[i].val) * 255 / 100);
        Serial.println(mspeed);
        mspeed = 180;
        Motors(mspeed, -mspeed);
        delay(commands[i].delaySec);
        break;
      default:
        return -1;
    }

  }
  Motors(0, 0);
  return 1;

}
