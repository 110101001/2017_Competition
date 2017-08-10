#ifndef _PWM_H_
#define _PWM_H_

#define DUTY_MAX 999

#define DUTY1 (TIM4->CCR1)//B8
#define DUTY2 (TIM4->CCR2)//B96hn 
#define DUTY3 (TIM4->CCR3)
#define DUTY4 (TIM4->CCR4)


void PWM_Configuration(void);

#define SetPWMOut(CH1,CH2,CH3,CH4) \
											TIM4->CCR1=CH1;\
											TIM4->CCR2=CH2;\
											TIM4->CCR3=CH3;\
											TIM4->CCR4=CH4;

#endif
