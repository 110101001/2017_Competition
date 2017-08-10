#include "main.h"
#define DISPLAY LCD_Clear(WHITE); LCD_DisplayString(40,30,24,"Mode Select");LCD_Draw_Line(39,0,39,320);for(i=0;i<7;i++){ LCD_DisplayString(40,60+24*i,24,(u8*)String[i]); }LCD_Draw_Rectangle(40,60+24*step,240,60+24*(step+1));
			
extern enum PendulumMode NS;			
extern u8 mode;
char String[7][20]={{"Stop:Stay"},{"Task1:2"},{"Task2:1->5"},{"Task3:1->4->5"},{"Task4:1->9"},{"Task5"},{"Task6"}};
int main(){
	int i;
	//All_Init();
	//mode=0;
	//interface();
	GPIO_Configuration();
  X_IN1=X_IN2=Y_IN1=Y_IN2=1;
	while(1);
}

void interface(){
	u8 key=0;
	u8 step=0;
	u8 i=0;
	while(1){
		key=Get_KeyValue();
		if(key=='D'){
			mode=0;
		}
		if(mode==0){
			DISPLAY
			while(key!='5'){
				key=Get_KeyValue();
				if(key=='2'){
					step=(step==0?0:step-1);
					while(key=='2') key=Get_KeyValue();
					DISPLAY
				}
				else if(key=='8'){
					step=(step==6?6:step+1);
					while(key=='8') key=Get_KeyValue();
					DISPLAY
				}
			}
			mode=step;
			LCD_Clear(BLACK);
		}
	}
}
