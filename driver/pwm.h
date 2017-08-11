#ifndef _PWM_H_
#define _PWM_H_

#define DUTY_MAX 999

#define DUTY1 (TIM4->CCR1)//D12
#define DUTY2 (TIM4->CCR2)//D13

void PWM_Configuration(void);

#endif
