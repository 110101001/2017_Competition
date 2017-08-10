#include "main.h"

//��һ������ǰλ�õ�Ŀ��λ�õĲ�ֵ��������ٶ�
PID_Type* Speed_X;
PID_Type* Speed_Y;

//�ڶ������ٶȱ�ɵ��PWMռ�ձ�
PID_Type* Motor_X;
PID_Type* Motor_Y;

void set_pid(PID_Type* PID,float kp,float ki,float kd)
{
	PID->kp=kp;
	PID->ki=ki;
	PID->kd=kd;
	PID->errNow=PID->errOld1=PID->errOld2=0;
	PID->output=0;
}

void pid_cal(PID_Type* PID)
{
  float dErrP, dErrI, dErrD;
	PID->errNow=PID->ref-PID->now;  
  dErrP = (PID->errNow - PID->errOld1);
  dErrI = PID->errNow;
  dErrD = (PID->errNow - 2.0f * PID->errOld1 + PID->errOld2);
  PID->doutput = (PID->kp * dErrP + PID->ki * dErrI + PID->kd * dErrD);
  PID->output += PID->doutput;//ռ�ձ�
	PID->errOld2 = PID->errOld1; 
  PID->errOld1 = PID->errNow;  
	if((PID->output)>DUTY_MAX) PID->output=DUTY_MAX;
	if((PID->output)<-DUTY_MAX) PID->output=-DUTY_MAX;
}


