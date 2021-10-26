//Makros for TetraBot.cpp
#define Debug           // comment out to deactivate
#ifdef Debug
  #define PRINT(x) Serial.print(x);
#else
  #define PRINT(x)
#endif

#define LEGPWM(i,joint) legArray[i].joint.pin,0,map(max(min(legArray[i].joint.position,legArray[i].joint.constrain[MAX])\
,legArray[i].joint.constrain[MIN]),0,180,legArray[i].joint.pwm[MIN],legArray[i].joint.pwm[MAX])
