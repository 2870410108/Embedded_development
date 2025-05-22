#include "snak_game.h"
#include "OLED12864.h"
#include "MPU6050.h"
float * data;
int main(){
//	OLED_Init();
//    MPU6050_Init();
//	OLED_showNumber(0,0,MPU6050_ReadData(0x))
//	//init_game();
//	while(1)
//	{	
//	//gesture_check();
//	//MainLoop();		
//	}   
	OLED_Init();
    MPU6050_Init();
	while(1)
	{	
	data=MPU6050_GetData();
    OLED_showNumber(0,0,data[6],0,1);
    OLED_showNumber(2,0,data[8],0,1);
    OLED_showNumber(4,0,data[9],0,1);		
	}           	
}


