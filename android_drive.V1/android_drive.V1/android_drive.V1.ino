#define M1_DIR     4
#define M1_PWM     5
#define M2_DIR     7
#define M2_PWM     6
char bytes[6];
char left_motor_dir;
char right_motor_dir;
int left_motor_power;
int right_motor_power;

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

//Ехать вперед
void MoveForward(int Speed)
{
  Motors(Speed, Speed);
}

int changeDir(char dir, int power){
  if(dir == 0) {
    power *= -1;
    return power;
  }
  return power;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Hello World!"); 
}

void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available() > 0) {//если пришли данные
      // read the incoming byte:
      Serial.readBytes(bytes, 6);
      left_motor_dir = bitRead(bytes[3], 7);
      right_motor_dir = bitRead(bytes[4], 7);
      left_motor_power = bytes[3] & 0b01111111; 
      right_motor_power = bytes[4] & 0b01111111;
      left_motor_power = left_motor_power * 255 / 100;
      right_motor_power = right_motor_power * 255 / 100;
      left_motor_power = changeDir(left_motor_dir, left_motor_power);
      right_motor_power = changeDir(right_motor_dir, right_motor_power);

      Motors(left_motor_power, right_motor_power);
      
      Serial.print(bytes[3], DEC);
      Serial.print("   ");
      Serial.print(bytes[4], DEC);
      Serial.print("   ");
      Serial.print(left_motor_power, DEC);
      Serial.print("   ");
      Serial.print(right_motor_power, DEC);
      Serial.println();
   }
}
