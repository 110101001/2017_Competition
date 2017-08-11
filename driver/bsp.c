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
	LED_Configuration();																//LED初始化
	TIM5_Configuration();																//定时器初始化
	//TIM6_Configuration();																//TIM6初始化
	TIM3_Configuration();
	Usart1_Init(38400);																//串口1初始化
	Usart2_Init(115200);																//串口2初始化
	PWM_Configuration();																//PWM初始化
	GPIO_Configuration();																//GPIO初始化
	I2c_Soft_Init();																		//iic初始化
	Delay_ms(100);																			//mpu6050初始化之前必要延时
	MPU6050_Init(20); 																	//mpu6050初始化，低通频率20Hz
	Delay_ms(100);																			//磁力计初始化前的延时
	ak8975_ok = !(ANO_AK8975_Run());										//磁力计初始化
	delay_ms(100);
	LCD_Init();
	LCD_Clear(WHITE);
	//OV7670_Configuration();
	Key_Configuration();
	InnerLoopInit();																		//内环周期初始化
	//TIM6_Start();					//ControlLoop开始
	mode_change_flag=1;
}



