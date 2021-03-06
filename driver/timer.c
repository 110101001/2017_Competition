#include "main.h"

extern double speed_x,speed_y;
static u32 Time_Ms=0;
static u32 Time_Last=0;
extern u8 mode;
extern float roll0,pitch0;
float pitch_pre,roll_pre;

/*----TIM2---TIM6-----*/
void TIM5_Configuration(void)//TIM2作为系统时钟，CNT寄存器中的为计数开始到现在的微秒数
{
   TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    tim.TIM_Period = 0xFFFFFFFF;
    tim.TIM_Prescaler = 84 - 1;	 //1M 的时钟  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_ARRPreloadConfig(TIM5, ENABLE);	
    TIM_TimeBaseInit(TIM5, &tim);
    TIM_Cmd(TIM5,ENABLE);	
}

void TIM5_IRQHandler(void)										
{
	  if (TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET) 
		{
			  TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
        TIM_ClearFlag(TIM5, TIM_FLAG_Update);
		}
} 

void Delay_us(uint32_t us)										//用TIM2的计数值来做到精确延时
{
    uint32_t now = Get_Time_Micros();
    while (Get_Time_Micros() - now < us);
		return;
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
        Delay_us(1000);
}


void TIM6_Configuration(void)							
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 84-1;        //84M internal clock
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 1000 - 1;  //1ms,1000Hz
    TIM_TimeBaseInit(TIM6,&tim);
}

void TIM6_Start(void)
{
    TIM_Cmd(TIM6, ENABLE);	 
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}
void TIM6_DAC_IRQHandler(void)									//TIM6的回调函数1ms调用一次，用于精确进入ControlLoop
{
	 if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	  {
			TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update);
			//ControlLoop();
    }

}
void InnerLoopInit(void)
{
	int i=0;
	for (i=0;i<INERLOOPLENGTH;i++)
	{
		GetInnerLoop(i);
	}
}	
uint32_t GetInnerLoop(int loop)								//用于获得精确的函数调用的周期
{
	static uint32_t Time[2][20]={0};//Time[0] is the last time, Time[1] is the new time;
	Time[0][loop] = Time[1][loop];
	Time[1][loop] = Get_Time_Micros();
	return Time[1][loop]-Time[0][loop];
}

void TIM3_Configuration(void)//1000-1,84-1,1ms进入一次中断
{
	  NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    tim.TIM_Period = 999;
    tim.TIM_Prescaler = 83;	 //1M 的时钟  
	  tim.TIM_ClockDivision=TIM_CKD_DIV1; 	
	  TIM_TimeBaseInit(TIM3,&tim);
	  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许更新中断
	  TIM_Cmd(TIM3,ENABLE); 
	
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM5中断
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
		NVIC_Init(&NVIC_InitStructure);  

    TIM_Cmd(TIM3,ENABLE);	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 
	{
		Time_Ms++;
		if (Time_Ms%2==0){
			pitch_pre=Pitch,roll_pre=Roll;
		MPU6050_Read();
	  MPU6050_Data_Prepare((TIM5->CNT-Time_Last)/1000000.0f);
  	IMUupdate(0.5f *((TIM5->CNT-Time_Last)/1000000.0f),mpu6050.Gyro_deg.x, mpu6050.Gyro_deg.y, mpu6050.Gyro_deg.z, //??IMU
						mpu6050.Acc.x, mpu6050.Acc.y, mpu6050.Acc.z,&Roll,&Pitch,&Yaw);			
			if(fabs(Roll)>180||fabs(Pitch)>180)
				Roll=roll_pre,Pitch=pitch_pre;
			Time_Last=TIM5->CNT;
					if (Time_Ms%30==0)
				{
					ANO_AK8975_Read();	
				//	Motor_X->now=Roll,Motor_Y->now=Pitch;
						switch(mode)
						{	
							case CALIBRATION: calibration();break;
							case 0: mode0(); break;
							case 1: mode1(); break;
							case 2: mode2(); break;
							case 3: mode3(); break;
							case 4: mode4(); break;
							//case 7: mode7(); break;
							//case Task4: mode4(); break;
							//case Task5: mode5(); break;
							default:break;
						}	
				}	
		}
		DataTransferTask(Time_Ms);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
}


