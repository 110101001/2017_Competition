#ifndef __PID_H
#define __PID_H
#include "pwm.h"

//位置式PID算法
typedef struct 
{
 float kp;     
 float ki;     
 float kd;
 float ref;
 float now;
 float errNow; //当前的误差
 float errOld1;
 float I;
 float limitI;
 float output;//控制输出
}PID_Type;

void set_pid(PID_Type* PID,float kp,float ki,float kd);
void pid_cal(PID_Type* PID);

#endif
