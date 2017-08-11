#include "main.h"

//第一环，当前位置到目标位置的差值控制球的速度
PID_Type* Speed_X;
PID_Type* Speed_Y;

//第二环，速度变成电机PWM占空比
PID_Type* Motor_X;
PID_Type* Motor_Y;

void set_pid(PID_Type* PID,float kp,float ki,float kd)
{
	PID->kp=kp;
	PID->ki=ki;
	PID->kd=kd;
	PID->errNow=PID->errOld1=0;
	PID->output=0;
}

void pid_cal(PID_Type* PID)
{
  float dErrP, dErrD;
	PID->errNow=PID->ref-PID->now;  
  dErrP = PID->errNow;
  PID->I += PID->errNow;
  dErrD = PID->errNow - PID->errOld1;
  PID->output = (PID->kp * dErrP + PID->ki * PID->I + PID->kd * dErrD);
  PID->errOld1 = PID->errNow;  
	if((PID->I)>PID->limitI) PID->I=PID->limitI;
	if((PID->I)<-PID->limitI) PID->I=-PID->limitI;
	if((PID->output)>DUTY_MAX) PID->output=DUTY_MAX;
	if((PID->output)<-DUTY_MAX) PID->output=-DUTY_MAX;
}


