#include "main.h"
#include "ov7670.h"
PID_Type MX={0,0,0,0,0,0,0,0,0,0};
PID_Type MY={0,0,0,0,0,0,0,0,0,0};

extern PID_Type* Motor_X;
extern PID_Type* Motor_Y;
extern PID_Type* Speed_X;
extern PID_Type* Speed_Y;

extern int mode_change_flag;


 void All_Init(void)
{
	Motor_X=&MX;
	Motor_Y=&MY;
	Speed_X=&MX;
	Speed_Y=&MX;	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    
	LED_Configuration();																//LED��ʼ��
	TIM5_Configuration();																//��ʱ����ʼ��
	//TIM6_Configuration();																//TIM6��ʼ��
	TIM3_Configuration();
	Usart1_Init(38400);																//����1��ʼ��
	Usart2_Init(115200);																//����2��ʼ��
	PWM_Configuration();																//PWM��ʼ��
	GPIO_Configuration();																//GPIO��ʼ��
	I2c_Soft_Init();																		//iic��ʼ��
	Delay_ms(100);																			//mpu6050��ʼ��֮ǰ��Ҫ��ʱ
	MPU6050_Init(20); 																	//mpu6050��ʼ������ͨƵ��20Hz
	Delay_ms(100);																			//�����Ƴ�ʼ��ǰ����ʱ
	ak8975_ok = !(ANO_AK8975_Run());										//�����Ƴ�ʼ��
	delay_ms(100);
	LCD_Init();
	LCD_Clear(WHITE);
	//OV7670_Configuration();
	Key_Configuration();
	InnerLoopInit();																		//�ڻ����ڳ�ʼ��
	//TIM6_Start();					//ControlLoop��ʼ
	mode_change_flag=1;
}



