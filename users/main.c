#include "main.h"

extern enum PendulumMode NS;			
extern char mode;

int main(){
	All_Init();
	mode=0;
	while(1){
		mode=KEY_Scan();
	}
	
}

