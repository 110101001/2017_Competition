#include "main.h"
#define DISPLAY LCD_Clear(WHITE); LCD_DisplayString(40,30,24,"Mode Select");LCD_Draw_Line(39,0,39,320);for(i=0;i<9;i++){ LCD_DisplayString(40,60+24*i,24,(u8*)String[i]); }LCD_Draw_Rectangle(40,60+24*step,240,60+24*(step+1));sprintf(str,"P:%1.1f,R:%1.1f",Pitch,Roll);LCD_DisplayString(80,300,16,str);
			
extern u8 mode;
extern int mode_change_flag;
extern unsigned int x_pos,y_pos;
extern float Roll,Pitch;
extern PID_Type* Motor_X;
extern PID_Type* Motor_Y;
extern PID_Type* Speed_X;
extern PID_Type* Speed_Y;
char String[9][20]={{"Stop"},{"Task1:2"},{"Task2:1->5"},{"Task3:1->4->5"},{"Task4:1->9"},{"Task5"},{"Task6"},{"Task7"},{"Calibration"}};
int main(){
//	All_Init();
//	mode=0;
// 	interface();
		PWM_Configuration();																//PWM初始化
	GPIO_Configuration();																//GPIO初始化
  Set_Motor(600,600);
}

/*
按键0为mode0（回到水平状态） 按某键为Stop
*/
void interface(){
	char str[20];
	u8 key=0;
	u8 step_pid=0;
	u8 col_pid=0;
	u8 step=0;
	u8 i=0;
	u8 temp_mode;
	float temp=0;
	float skip=1;
	PID_Type* Point;
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
					step=(step==8?8:step+1);
					while(key=='8') key=Get_KeyValue();
					DISPLAY
				}
			}
			while(key=='5') key=Get_KeyValue();
			if(mode!=step) mode_change_flag=1;
			mode=step;
		}
		if(mode!=0){
			LCD_Clear(WHITE);
			LCD_DisplayString(40,290,24,String[mode]);
			LCD_Draw_Circle(x_pos/2+5,y_pos/2+5,5);
			LCD_Draw_Rectangle(5,5,165,165);
			sprintf(str,"P:%1.1f",Pitch);
			LCD_DisplayString(170,10,16,str);
			sprintf(str,"R:%1.1f",Roll);
			
			LCD_DisplayString(170,30,16,str);
			LCD_DisplayString(0,180,16,"MOTX:");
			sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Motor_X->kp,Motor_X->ki,Motor_X->kd);
			LCD_DisplayString(40,180,16,str);
			LCD_DisplayString(0,200,16,"MOTY:");
			sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Motor_Y->kp,Motor_Y->ki,Motor_Y->kd);
			LCD_DisplayString(40,200,16,str);
			LCD_DisplayString(0,220,16,"SPDX:");
			sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Speed_X->kp,Speed_X->ki,Speed_X->kd);
			LCD_DisplayString(40,220,16,str);
			LCD_DisplayString(0,240,16,"SPDY:");
			sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Speed_Y->kp,Speed_Y->ki,Speed_Y->kd);
			LCD_DisplayString(40,240,16,str);
			LCD_Draw_Circle(50+col_pid*55,178+step_pid*20,2);
			sprintf(str,"Skip:%f",skip);
			LCD_DisplayString(100,260,16,str);
			if(key=='8'){
				step_pid=(step_pid==3?3:step_pid+1);
				while(key=='8') key=Get_KeyValue();
			}
			if(key=='2'){
				step_pid=(step_pid==0?0:step_pid-1);
				while(key=='2') key=Get_KeyValue();
			}
			if(key=='4'){
				col_pid=(col_pid==0?0:col_pid-1);
				while(key=='4') key=Get_KeyValue();
			}
			if(key=='6'){
				col_pid=(col_pid==2?2:col_pid+1);
				while(key=='6') key=Get_KeyValue();
			}
			if(key=='5'){
				temp_mode=mode;
				mode=0;
				Set_Motor(0,0);
				while(key=='5'){
					key=Get_KeyValue();
					if(key!='5'){
						delay_ms(10);
						key=Get_KeyValue();
					}
				}
				while(key!='5'){
					key=Get_KeyValue();
					
					if(key=='8'){
						temp-=skip;
						while(key=='8') key=Get_KeyValue();
					}
					if(key=='2'){
						temp+=skip;
						while(key=='2') key=Get_KeyValue();
					}
					if(key=='4'){
						skip/=10;
						while(key=='4') key=Get_KeyValue();
					}
					if(key=='6'){
						skip*=10;
						while(key=='6') key=Get_KeyValue();
					}
			
					switch(step_pid){
						case 0:Point=Motor_X;break;
						case 1:Point=Motor_Y;break;
						case 2:Point=Speed_X;break;
						case 3:Point=Speed_Y;break;
					}
					switch(col_pid){
						case 0:Point->kp+=temp;break;
						case 1:Point->ki+=temp;break;
						case 2:Point->kd+=temp;break;
					}
					temp=0;
					LCD_Clear(WHITE);
					LCD_DisplayString(40,290,24,String[mode]);
					LCD_Draw_Circle(x_pos/2+5,y_pos/2+5,5);
					LCD_Draw_Rectangle(5,5,165,165);
					sprintf(str,"P:%1.1f",Pitch);
					LCD_DisplayString(170,10,16,str);
					sprintf(str,"R:%1.1f",Roll);
					LCD_DisplayString(170,30,16,str);
					
					LCD_DisplayString(0,180,16,"MOTX:");
					sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Motor_X->kp,Motor_X->ki,Motor_X->kd);
					LCD_DisplayString(40,180,16,str);
					LCD_DisplayString(0,200,16,"MOTY:");
					sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Motor_Y->kp,Motor_Y->ki,Motor_Y->kd);
					LCD_DisplayString(40,200,16,str);
					LCD_DisplayString(0,220,16,"SPDX:");
					sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Speed_X->kp,Speed_X->ki,Speed_X->kd);
					LCD_DisplayString(40,220,16,str);
					LCD_DisplayString(0,240,16,"SPDY:");
					sprintf(str,"P:%1.f  I:%1.3f  D:%1.f",Speed_Y->kp,Speed_Y->ki,Speed_Y->kd);
					LCD_DisplayString(40,240,16,str);
					LCD_Draw_Circle(50+col_pid*55,178+step_pid*20,2);
					sprintf(str,"Skip:%f",skip);
					LCD_DisplayString(100,260,16,str);
				}
			while(key=='5') key=Get_KeyValue();
			mode=temp_mode;
			}
			delay_ms(20);
		}
		Delay_ms(10);
	}
}
