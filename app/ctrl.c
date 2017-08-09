#include "main.h"

int mode;
int mode_change_flag;
extern float Roll,Pitch,Yaw;    				//???

extern PID_Type* Motor_X;
extern PID_Type* Motor_Y;

void mode1(void)
{
	if(mode_change_flag==1)
	{
		
		
		mode_change_flag=0;
	}
}
void mode2(void);
void mode3(void);
void mode4(void);
void mode5(void);
void mode6(void);

extern uint32_t time_count_begin;

