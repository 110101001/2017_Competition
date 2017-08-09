#include "main.h"

void X_Forward(int pwm)
{   	
	DUTY1=pwm;
	X_IN1=1,X_IN2=0;
}

void Y_Forward(int pwm)
{   	
	DUTY2=pwm;
	Y_IN1=1,Y_IN2=0;
}

void X_Backward(int pwm)
{   	
	DUTY1=pwm;
	X_IN1=0,X_IN2=1;
}

void Y_Backward(int pwm)
{   	
	DUTY2=pwm;
	Y_IN1=0,Y_IN2=1;
}


void Set_Motor(float pwm1,float pwm2)//x方向，y方向
{
	if(pwm1 >= 0)
	{
     X_Forward((int)pwm1);
	}
	else if(pwm1 < 0)
	{
     X_Backward((int)(-pwm1));
	}

	if(pwm2 >= 0)
	{
     Y_Forward((int)pwm2);
	}
	else if(pwm2 < 0)
	{
     Y_Backward((int)(-pwm2));
	} 	
}
