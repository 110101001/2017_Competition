#ifndef __TB6612FNG_H
#define __TB6612FNG_H

void X_Forward(int pwm);
void Y_Forward(int pwm);
void X_Backward(int pwm);
void Y_Backward(int pwm);

void Set_Motor(float pwm1,float pwm2);

#endif
