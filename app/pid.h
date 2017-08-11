#ifndef __PID_H
#define __PID_H
#include "pwm.h"

//λ��ʽPID�㷨
typedef struct 
{
 float kp;     
 float ki;     
 float kd;
 float ref;
 float now;
 float errNow; //��ǰ�����
 float errOld1;
 float I;
 float limitI;
 float output;//�������
}PID_Type;

void set_pid(PID_Type* PID,float kp,float ki,float kd);
void pid_cal(PID_Type* PID);

#endif
