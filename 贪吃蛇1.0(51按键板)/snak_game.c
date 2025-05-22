#include <REGX52.H>
#include "tools/oled12864.h"
#include "tools/delay_ms.h"
#include "tools/init_timer.h"
#define u8 unsigned char
#define u32 unsigned int


//按键定义
sbit  up   =P1^2;
sbit  down =P1^4;
sbit  left =P1^5;
sbit  right=P1^3;
//打印蛇一个节点所需要的像素点8*8
const u8 code food[8]={0xFF,0xFF,0xC3,0xC3,0xC3,0xC3,0xFF,0xFF};
const u8 code blank[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
const u8 code body[8]={0x00,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x00};
//用于储存蛇节点的位置
u8 snake_x[30],snake_y[30];
//食物的坐标位置
u8 food_x,food_y;
//蛇的长度
u8 snake_lenght;
//蛇的方向
u8 dir;
//定时器重装初始值;
u32 num;
//定时器计数
u8 count;
//游戏分数
u8 score;
//打印蛇
void print_OLED_SnakeBody()
{
   u8 i;
   for(i=0;i<snake_lenght;i++)
   {
      OLED_showImage(snake_y[i], snake_x[i]*8,8,8,body);
   }
 }

//初始化游戏
void init_game()
{
	
	
	dir=1;//初始化方向
	snake_lenght=3;//初始化蛇的长度
	food_x=6;food_y=6;//初始化食物
  snake_x[0]=5,snake_y[0]=3;//初始化蛇头位置
	snake_x[1]=4,snake_y[1]=3;//初始化第二节点
	snake_x[2]=3,snake_y[2]=3;//初始化第三节点
	OLED_Init();//初始化OLED
	OLED_showImage_128X64();//欢迎界面
	delay_ms(1000);
	OLED_Clear();//清屏
	print_OLED_SnakeBody();//画出蛇身
	OLED_showImage(food_y, food_x*8,8,8,food);//打印食物
	num=init_timer(0,1,50000,12,1);//初始化定时器0，方式一
}
//按键检测
void key_check()
{ 
	if(!up||!down||!left||!right)
		{
			if(!up){
				delay_ms(10);
				if(!up)
					while(!up);
				if(dir!=2){//检测当前方向是否与预计修改的方向冲突
						dir=1;return;
					}
			}
			if(!down){
				delay_ms(10);
				if(!down)
					while(!down);
					if(dir!=1){
						dir=2;return;
					}
			}
			if(!left){
				delay_ms(10);
				if(!left)
					while(!left);
					if(dir!=4){
						dir=3;return;
					}
			}
			if(!right){
				delay_ms(20);
				if(!right)
					while(!right);
					if(dir!=3){
						dir=4;return;
					}
			}
		}
}
//食物检测
void Food_check()
{
    if ((dir == 1 && snake_x[0] == food_x && snake_y[0] - 1 == food_y) ||
        (dir == 2 && snake_x[0] == food_x && snake_y[0] + 1 == food_y) ||
        (dir == 3 && snake_x[0] - 1 == food_x && snake_y[0] == food_y) ||
        (dir == 4 && snake_x[0] + 1 == food_x && snake_y[0] == food_y))
    {		
				// 清除食物
        OLED_showImage(food_y,food_x* 8, 8, 8, blank);
        food_x = rand() % 8;
        food_y = rand() % 16;
        snake_lenght++;
        score++;
			// 生成新食物
        OLED_showImage(food_y,food_x* 8, 8, 8, food);
    }
}
//蛇的移动核心思路分析
/*
使用数组存储蛇的节点坐标，蛇头在数组第一个元素
移动时只需要改变蛇头坐标即可，然后蛇尾每一节一次等于前一个蛇节点位置即可
还需要考虑此时蛇的方向，根据方向改变相应坐标
加ru此刻在向上移动，那就需要更新Y坐标
//4 5 6-->3 4 5   Y
//5 5 5-->5 5 5   X
*/
void move(){
    u8 i;

    // 清除蛇尾的前一个位置
    OLED_showImage(snake_y[snake_lenght-1], snake_x[snake_lenght-1] * 8, 8, 8, blank);

    // 更新蛇的每一节的位置
    for(i = snake_lenght - 1; i > 0; i--){
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // 更新蛇头的位置
    switch(dir){
        case 1: snake_y[0]--; break;
        case 2: snake_y[0]++; break;
        case 3: snake_x[0]--; break;
        case 4: snake_x[0]++; break;
    }

    print_OLED_SnakeBody(); // 重新绘制整条蛇
}
//判断蛇是否死亡
bit isDeath()
{
    u8 i;
    if (snake_x[0] == -1 || snake_y[0] == -1 || snake_x[0] == 16 || snake_y[0] == 8)
			// 蛇头在边界或墙壁上
        return 1;
    for (i = 1; i < snake_lenght; ++i)
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
					// 蛇头与身体重合
            return 1;
    return 0;
}
void MainLoop() {
    key_check();
    if (count >= 10) {
        count = 0;
        if (isDeath()) {
            
					OLED_Clear();//清屏
					OLED_showString(0, 13,"GAME OVER !!!",1);
					OLED_showString(2, 13,"SCORE --> ",1);
					OLED_showString(2, 13,"SCORE --> ",1);
        } else {
            Food_check();
            move();
        }
    }
}
void T0_interrupt () interrupt 1{
	TH0=num/256;
	TL0=num%256;
	if(count>=10)
		count=0;
	count++;
}


