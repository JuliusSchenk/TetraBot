#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <TetraBot.h>

const uint8_t PCAADR = 0x40;
const uint8_t GYADR = +1;
const uint8_t MIN = 0;
const uint8_t MAX = 1;

uint16_t tempcounter=0;
int tempdir=1;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(PCAADR);

struct servo
{
  uint16_t pwm[2];      // MIN (0°)- MAX(180°)clockwise MIN(180°)-MAX(0°)
  uint8_t constrain[2]; // 0-180 Degree
  uint16_t position;    // currentPos
  uint8_t pin;
};
struct leg
{
  servo joint0; //joint0 -> joint2; closes from body -> furthest 
  servo joint1;
  servo joint2;
};

leg legArray[4]={         //Front Right Clockwise
  {
  .joint0={.pwm={160,620} ,.constrain={45,135} ,.position=90 ,.pin=0},
  .joint1={.pwm={160,600} ,.constrain={0,120} ,.position=90 ,.pin=1},
  .joint2={.pwm={130,610} ,.constrain={45,180} ,.position=90 ,.pin=2},
  },
  {
  .joint0={.pwm={110,560} ,.constrain={45,135} ,.position=90 ,.pin=0+4},
  .joint1={.pwm={610,90} ,.constrain={0,120} ,.position=90 ,.pin=1+4},
  .joint2={.pwm={660,180} ,.constrain={45,180} ,.position=90 ,.pin=2+4},
  },
  {
  .joint0={.pwm={110,600} ,.constrain={45,135} ,.position=90 ,.pin=0+8},
  .joint1={.pwm={130,580} ,.constrain={0,120} ,.position=90 ,.pin=1+8},
  .joint2={.pwm={120,630} ,.constrain={45,180} ,.position=90 ,.pin=2+8},
  },
  {
  .joint0={.pwm={130,630} ,.constrain={45,135} ,.position=90 ,.pin=0+12},
  .joint1={.pwm={640,130} ,.constrain={0,120} ,.position=90 ,.pin=1+12},
  .joint2={.pwm={630,190} ,.constrain={45,180} ,.position=90 ,.pin=2+12},
  }
};
// GY-521 x,y,z

void setLegPos(uint8_t legNr, uint16_t pos0,uint16_t pos1,uint16_t pos2){
  legArray[legNr].joint0.position = pos0;
  legArray[legNr].joint1.position = pos1;
  legArray[legNr].joint2.position = pos2;
}

void setLegsPos( uint16_t pos0,uint16_t pos1,uint16_t pos2){
  setLegPos(0,pos0,pos1,pos2);
  setLegPos(1,pos0,pos1,pos2);
  setLegPos(2,pos0,pos1,pos2);
  setLegPos(3,pos0,pos1,pos2);
}

void setLegPWM(){
  for(int i = 0; i < 4; i++){
    pwm.setPWM(LEGPWM(i,joint0));
    pwm.setPWM(LEGPWM(i,joint1));
    pwm.setPWM(LEGPWM(i,joint2));
    PRINT(i)
    PRINT(": pos0: ")
    PRINT(legArray[i].joint0.position)
    PRINT(", pwm: ")
    PRINT(
      map(max(min(legArray[i].joint0.position,legArray[i].joint0.constrain[MAX])
      ,legArray[i].joint0.constrain[MIN]),0,180,legArray[i].joint0.pwm[MIN]
      ,legArray[i].joint0.pwm[MAX]))
    PRINT("; pos1: ")
    PRINT(legArray[i].joint1.position)
    PRINT(", pwm: ")
    PRINT(
      map(max(min(legArray[i].joint1.position,legArray[i].joint1.constrain[MAX])
      ,legArray[i].joint1.constrain[MIN]),0,180,legArray[i].joint1.pwm[MIN]
      ,legArray[i].joint1.pwm[MAX]))
    PRINT("; pos2: ")
    PRINT(legArray[i].joint2.position)
    PRINT(", pwm: ")
    PRINT(
      map(max(min(legArray[i].joint2.position,legArray[i].joint2.constrain[MAX])
      ,legArray[i].joint2.constrain[MIN]),0,180,legArray[i].joint2.pwm[MIN]
      ,legArray[i].joint2.pwm[MAX]))
    PRINT("\n")
  }
}

void setup() {
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);
  setLegsPos(90,90,90);
  delay(100);
  
  Serial.print("Setup finished\n");
}

void loop() {
  if (tempcounter>= 180)
  {
    tempdir =-1;
  }else if (tempcounter<=0)
  {
    tempdir =1;
  }
  setLegsPos(90,tempcounter,180-tempcounter);
  tempcounter+=tempdir;
  
  setLegPWM();
  delay(100);
}