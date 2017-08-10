#include "main.h"

u8 mode;
int mode_change_flag;
int time;
extern float Roll,Pitch,Yaw;
uint32_t time_count_begin;
int static_time_begin;

extern PID_Type* Speed_X;
extern PID_Type* Speed_Y;

extern PID_Type* Motor_X;
extern PID_Type* Motor_Y;

unsigned int x_pre,y_pre;
unsigned int x_pos,y_pos;//��usart.c�н��и���
double x_speed,y_speed;

/*====================================================
Mode0���ܣ�ƽ�������ˮƽ
====================================================*/
void mode0(void)
{
		if(mode_change_flag==1)
	{
		set_pid(Motor_X,100,0,0);
	  set_pid(Motor_Y,0,0,0);
		time_count_begin=TIM5->CNT;		
		Motor_X->ref=Motor_Y->ref=0;
		mode_change_flag=0;
	}
	Motor_X->now=Roll,Motor_Y->now=Pitch;
  pid_cal(Motor_X),pid_cal(Motor_Y);
	Set_Motor(Motor_X->output,Motor_Y->output);
}

/*====================================================
Mode1���ܣ���С�����λ��2��ͣ��������5s
====================================================*/
void mode1(void)
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
	time=(TIM5->CNT - time_count_begin);
	time_count_begin=TIM5->CNT;		
	x_speed=(double)(x_pos-x_pre)/(double)time,y_speed=(double)(y_pos-y_pre)/(double)time;
	Speed_X->ref=C2X,Speed_X->now=x_pos,Speed_Y->ref=C2Y,Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
	Set_Motor(Motor_X->output,Motor_Y->output);
	x_pre=x_pos,y_pre=y_pos;

}

/*====================================================
Mode2���ܣ�15s�ڴ�1����5����5��ͣ��������2s
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
	//����һ��ֱ���˶���Ŀ��λ��
  time=(TIM5->CNT - time_count_begin);
	time_count_begin=TIM5->CNT;		
	x_speed=(double)(x_pos-x_pre)/(double)time,y_speed=(double)(y_pos-y_pre)/(double)time;
	Speed_X->ref=C5X,Speed_X->now=x_pos,Speed_Y->ref=C5Y,Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
		x_pre=x_pos,y_pre=y_pos;
}

/*====================================================
Mode3���ܣ���1����4��ͣ��������2s��
           �ٽ���5��ͣ��������2s
           ��������������ʱ�䲻����20s
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
  time=(TIM5->CNT - time_count_begin);
	time_count_begin=TIM5->CNT;		
	x_speed=(double)(x_pos-x_pre)/(double)time,y_speed=(double)(y_pos-y_pre)/(double)time;
	if((TIM5->CNT-static_time_begin)/1000000.0f<=9)//9���������
	{
	Speed_X->ref=C4X,Speed_X->now=x_pos,Speed_Y->ref=C4Y,Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
	}
	else
	{	
	Speed_X->ref=C5X,Speed_X->now=x_pos,Speed_Y->ref=C5Y,Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
	}
	x_pre=x_pos,y_pre=y_pos;
}

/*====================================================
Mode4���ܣ�30s�ڴ�1����9��ͣ��������2s
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
	//����һ��ֱ���˶���Ŀ��λ��
  time=(TIM5->CNT - time_count_begin);
	time_count_begin=TIM5->CNT;		
	x_speed=(double)(x_pos-x_pre)/(double)time,y_speed=(double)(y_pos-y_pre)/(double)time;
	x_pre=x_pos,y_pre=y_pos;
	Speed_X->ref=C9X,Speed_X->now=x_pos,Speed_Y->ref=C9Y,Speed_Y->now=y_pos;
  pid_cal(Speed_X),pid_cal(Speed_Y);
	Motor_X->ref=Speed_X->output,Motor_Y->ref=Speed_Y->output;
	Motor_X->now=x_speed,Motor_Y->now=y_speed;
	pid_cal(Motor_X),pid_cal(Motor_Y);
  Set_Motor(Motor_X->output,Motor_Y->output);	
}

//=====================���Ӳ���======================
void mode5(void);
void mode6(void);

extern uint32_t time_count_begin;

