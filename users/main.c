#include "main.h"
#define DISPLAY LCD_Clear(WHITE); LCD_DisplayString(40,30,24,"Mode Select");LCD_Draw_Line(39,0,39,320);for(i=0;i<9;i++){ LCD_DisplayString(40,60+24*i,24,(u8*)String[i]); }LCD_Draw_Rectangle(40,60+24*step,240,60+24*(step+1));
			
extern u8 mode;
extern int mode_change_flag;

char String[9][20]={{"Calibration"},{"Task1:2"},{"Task2:1->5"},{"Task3:1->4->5"},{"Task4:1->9"},{"Task5"},{"Task6"},{"Task7"},{"Stop"}};
int main(){
	int i;
	All_Init();
	mode=0;
	for(i=0;i<100;i++)
	 MPU6050_Data_Offset();
	interface();
}

/*
按键0为mode0（回到水平状态） 按某键为Stop
*/
void interface(){
	u8 key=0;
	u8 step=0;
	u8 i=0;
	while(1){
		key=Get_KeyValue();
		if(key=='D'){
			mode=0;
		}
		if(mode==0||mode==CALIBRATION){
			DISPLAY
			while(key!='5'){
				key=Get_KeyValue();
				if(key=='2'){
					step=(step==0?0:step-1);
					while(key=='2') key=Get_KeyValue();
					DISPLAY
				}
				else if(key=='8'){
					step=(step==8?8:step+1);
					while(key=='8') key=Get_KeyValue();
					DISPLAY
				}
			}
			if(mode!=step) mode_change_flag=1;
			mode=step;
			LCD_Clear(BLACK);
		}
	}
}
