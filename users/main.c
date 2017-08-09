#include "main.h"
extern enum PendulumMode NS;			

int main(){
	
	All_Init();
	NS=Stop;
	while(1) Get_KeyValue();
}


