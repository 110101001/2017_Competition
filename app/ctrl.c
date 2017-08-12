#include "main.h"
extern unsigned int area[9][2];
u8 mode;
int mode_change_flag;
int time;
extern float Roll,Pitch,Yaw;
float Roll_ref,Pitch_ref;
uint32_t time_count_begin;
int static_time_begin;

extern PID_Type* Speed_X;
extern PID_Type* Speed_Y;

extern PID_Type* Motor_X;
extern PID_Type* Motor_Y;

unsigned int x_pre,y_pre;
unsigned int x_pos,y_pos;//在usart.c中进行更新
int x_speed,y_speed;

void mode0(void)//停
{
	//Set_Motor(0,0);
}
/*====================================================
calibration功能：平板调节至水平
====================================================*/
void calibration(void)
{
		if(mode_change_flag==1)
	{
		//set_pid(Motor_X,-100,-0.7,-2000); Y: -600,-0.5,-400
		set_pid(Motor_X,-2000,0,0);
	  set_pid(Motor_Y,-2000,0,0);
		Motor_X->ref=Pitch_ref,Motor_Y->ref=Roll_ref;
		mode_change_flag=0;
	}
	Motor_X->now=Pitch,Motor_Y->now=Roll>0?180-Roll:-180-Roll;
  pid_cal(Motor_X);
	pid_cal(Motor_Y);
	Set_Motor(Motor_X->output,Motor_Y->output);
}

/*====================================================
Mode1功能：将小球放在位置2，停留不少于5s
====================================================*/
void mode1(void)
{
	if(mode_change_flag==1)
	{
		set_pid(Speed_X,0,0,0);
	  set_pid(Speed_Y,0,0,0);		
		set_pid(Motor_X,-1800,0,0);
	  set_pid(Motor_Y,-1800,0,0);
		//time_count_begin=TIM5->CNT;		
		mode_change_flag=0;
		//x_pre=x_pos,y_pre=y_pos;
	}
	//time=(TIM5->CNT - time_count_begin);
	//time_count_begin=TIM5->CNT;		
	//x_speed=(x_pos-x_pre)/(float)time,y_speed=(y_pos-y_pre)/(float)time;
	Speed_X->ref=160,Speed_X->now=x_pos,Speed_Y->ref=160,Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=Pitch-Pitch_ref,Motor_Y->now=(Roll>0?180-Roll:-180-Roll)-Roll_ref;
	pid_cal(Motor_X);
	pid_cal(Motor_Y);
	Set_Motor(Motor_X->output,Motor_Y->output);
}

/*====================================================
Mode2功能：15s内从1进入5，在5内停留不少于2s
====================================================*/
void mode2(void)
{
		if(mode_change_flag==1)
	{
		set_pid(Speed_X,0,0,0);
	  set_pid(Speed_Y,0,0,0);		
		set_pid(Motor_X,0,0,0);
	  set_pid(Motor_Y,0,0,0);
		time_count_begin=TIM5->CNT;		
		mode_change_flag=0;
		x_pre=x_pos,y_pre=y_pos;
	}
	//方案一：直接运动到目标位置
	Speed_X->ref=area[4][0],Speed_X->now=x_pos,Speed_Y->ref=area[4][1],Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=Pitch-Pitch_ref,Motor_Y->now=(Roll>0?180-Roll:-180-Roll)-Roll_ref;
	pid_cal(Motor_X);
	pid_cal(Motor_Y);
	Set_Motor(Motor_X->output,Motor_Y->output);
}

/*====================================================
Mode3功能：从1进入4，停留不少于2s，
           再进入5，停留不少于2s
           以上两个动作总时间不超过20s
====================================================*/
void mode3(void)
{
	if(mode_change_flag==1)
	{
		set_pid(Speed_X,0,0,0);
	  set_pid(Speed_Y,0,0,0);		
		set_pid(Motor_X,0,0,0);
	  set_pid(Motor_Y,0,0,0);
		static_time_begin=time_count_begin=TIM5->CNT;		
		mode_change_flag=0;
		x_pre=x_pos,y_pre=y_pos;
	}
	if((TIM5->CNT-static_time_begin)/1000000.0f<=9)//9秒以内完成
	{
	Speed_X->ref=area[3][0],Speed_X->now=x_pos,Speed_Y->ref=area[3][1],Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
	}
	else
	{	
	Speed_X->ref=area[4][0],Speed_X->now=x_pos,Speed_Y->ref=area[4][1],Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
	}
	x_pre=x_pos,y_pre=y_pos;
}

/*====================================================
Mode4功能：30s内从1进入9，停留不少于2s
====================================================*/
void mode4(void)
{
			if(mode_change_flag==1)
	{
		set_pid(Speed_X,0,0,0);
	  set_pid(Speed_Y,0,0,0);		
		set_pid(Motor_X,0,0,0);
	  set_pid(Motor_Y,0,0,0);
		time_count_begin=TIM5->CNT;		
		mode_change_flag=0;
		x_pre=x_pos,y_pre=y_pos;
	}
	//方案一：直接运动到目标位置
  time=(TIM5->CNT - time_count_begin);
	time_count_begin=TIM5->CNT;		
	x_speed=(double)(x_pos-x_pre)/(double)time,y_speed=(double)(y_pos-y_pre)/(double)time;
	x_pre=x_pos,y_pre=y_pos;
	Speed_X->ref=area[8][0],Speed_X->now=x_pos,Speed_Y->ref=area[8][1],Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
}

//=====================发挥部分======================
void mode5(void);
void mode6(void);
void mode7(void)
{
	Roll_ref=Roll>0?180-Roll:-180-Roll,Pitch_ref=Pitch;
}
