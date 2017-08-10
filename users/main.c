#include "main.h"
#define DISPLAY LCD_Clear(WHITE); LCD_DisplayString(40,30,24,"Mode Select");LCD_Draw_Line(39,0,39,320);for(i=0;i<9;i++){ LCD_DisplayString(40,60+24*i,24,(u8*)String[i]); }LCD_Draw_Rectangle(40,60+24*step,240,60+24*(step+1));
			
extern u8 mode;
extern int mode_change_flag;
extern unsigned int x_pos,y_pos;
extern float Roll,Pitch;
float roll0,pitch0;
char String[9][20]={{"Stop"},{"Task1:2"},{"Task2:1->5"},{"Task3:1->4->5"},{"Task4:1->9"},{"Task5"},{"Task6"},{"Task7"},{"Calibration"}};
int main(){
	//int i;
	All_Init();
	mode=0;
	//for(i=0;i<100;i++)
	 MPU6050_Data_Offset();
	
	//Delay_ms(200);
	//roll0=Roll;
	//pitch0=Pitch;
	
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
		}
		if(mode!=0&&mode!=8){
			LCD_Clear(WHITE);
			LCD_DisplayString(40,290,24,String[mode]);
			LCD_Draw_Circle(x_pos/2+40,y_pos/2+20,10);
			LCD_Draw_Rectangle(40,20,200,180);
		}
		Delay_ms(1);
	}
}
