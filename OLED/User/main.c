#include "OLED12864.h"

const uchar  food[8]={0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF};
int main(){
	
  OLED_Init();
  OLED_showNumber(0,0,45.35,0,0);    //显示浮点数
	OLED_showNumber(2,0,66,1,0);			 //显示整数
  OLED_showString(3,0,"dsfsaf",1);	 //显示字符串
	OLED_showImage(6,0,8,8,food);      //显示8x8像素图片
	OLED_ColorTurn(0);								 //反色显示开启
  //OLED_Scroll_horizontally(1);       //水平滚动开启
	while(1)
	{	
		
	}                         
}


