#include "main.h"


void EXTI0_IRQHandler(void)
{		 
	char temp = Get_KeyValue();
	EXTI_ClearITPendingBit(EXTI_Line0);
	if (temp=='D'){
		LCD_Clear(BLACK);
		BRUSH_COLOR=RED;
		LCD_DisplayString(40,30,16,"Control Stoped");
		LCD_DisplayString(40,60,16,"Press A To Reset");
		LCD_DisplayString(40,90,16,"Or Select Mode");
		BRUSH_COLOR=BLACK;
		Delay_ms(1);
		Mode_Task();
	}else if(temp=='A'){
		__set_FAULTMASK(1);
		NVIC_SystemReset();
	}
	//清除LINE2上的中断标志位 
}


