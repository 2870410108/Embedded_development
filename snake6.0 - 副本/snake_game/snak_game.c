#include "OLED12864.h"
#include "MPU6050.h"
#include "snake_display_data.h"
#include "sys_tick.h"
#include "Timer.h"
#include "stdlib.h"
//蛇坐标位置
int8_t snake_x[30], snake_y[30];
//食物的坐标位置
int8_t food_x, food_y;
//蛇的长度
uint8_t snake_lenght;
//蛇的方向
uint8_t dir;
//游戏分数
uint8_t score;
//游戏是否开始
uint8_t isStart=0;
//菜单指示标志
uint8_t flag=1;
//死亡方式标志
uint8_t flag2;

float *data_array;
float roll, pitch;

#define up          pitch<-7
#define down    	pitch>7
#define left        roll>10
#define right       roll<-10


void print_game_menu(){
	OLED_showString(0,30,"Game menu",1);
	OLED_showString(3,0,"->",0);
	OLED_showString(3,20,"How To Play ",0);
	OLED_showString(5,20,"Start Game ",0);	
}
void menu_control(){

	if (up && flag!=4 && flag!=3 ) {//当已经进入子菜单，上级菜单就不在显示
        Delay_ms(30);                             
        if (up){
			OLED_showString(3,0,"->",0);//在上一个位置画一个箭头，然后消除之前的箭头
			OLED_showString(5,0,"  ",0);
			flag=1;
		}
	}
    if (down && flag!=4 && flag!=3 ) {//当已经进入子菜单，上级菜单就不在显示
       Delay_ms(30);
        if (down){
			OLED_showString(5,0,"->",0);//在下一个位置画一个箭头，然后消除之前的箭头
			OLED_showString(3,0,"  ",0);
			flag=2;
		}
	}
    if (left ) {//左倾就退出子菜单，就回到上机菜单
        Delay_ms(30);
        if (left && flag==3){//当进入游戏说明菜单时左倾退出子菜单
			OLED_Clear();
			print_game_menu();
			flag=1;
		}
		if (left && !isStart && flag==4){//当游戏结束返回菜单
			OLED_Clear();
			print_game_menu();
			flag=1;
		}
				
    }
    if (right) {//右倾就进入子菜单
        Delay_ms(30);
        if (right && flag==1){//当菜单标志等于1 && 右倾就进入游戏说明子菜单
			OLED_Clear();
			OLED_showImage(0, 0, 128, 64, image2_128X64);//游戏说明
			Delay_s(3);
			OLED_showImage(0, 0, 128, 64, image3_128X64);//游戏说明
			Delay_ms(3);
			flag=3;
		}
        if (right && flag==2){//当菜单标志等于2 && 右倾就开始游戏
			init_timer_3();
			dir = 4;//初始化方向
			snake_lenght = 3;//初始化蛇的长度
			food_x = 5;
			food_y = 6;
			snake_x[0] = 5, snake_y[0] = 3;//初始化蛇头位置
			snake_x[1] = 4, snake_y[1] = 3;//初始化第二节点
			snake_x[2] = 3, snake_y[2] = 3;//初始化第三节点
			OLED_Clear();			
			isStart=1;
			flag=4;
		}		
    }
}
void print_OLED_SnakeBody() {
    uint8_t i;
    for (i = 0; i < snake_lenght; i++) {
        OLED_showImage(snake_y[i], snake_x[i] * 8, 8, 8, body);
    }
}
          

//初始化游戏
void init_game() {
    OLED_Init();
    MPU6050_Init();
		OLED_showImage(0, 0, 128, 64, image1_128X64);//兰州石化职业技术大学
    Delay_s(2);
		OLED_Scroll_horizontally(1);
		Delay_ms(2900);
		OLED_Scroll_horizontally_OFF();
		Delay_s(1);
    OLED_Clear();//清屏
		print_game_menu();
		OLED_ColorTurn(0);//反白显示	 
}

//姿态检测
void gesture_check() {
    data_array = MPU6050_GetData();
    roll = data_array[8];
    pitch = data_array[9];
}

//方向检查
void dir_check() {
    if (up) {
        Delay_ms(30);
        if (up)
            if (dir != 2) {//检测当前方向是否与预计修改的方向冲突
                dir = 1;
                return;
            }
    }
    if (down) {
       Delay_ms(30);
        if (down)
            if (dir != 1) {
                dir = 2;
                return;
            }
    }
    if (left) {
        Delay_ms(30);
        if (left)
            if (dir != 4) {
                dir = 3;
                return;
            }
    }
    if (right) {
        Delay_ms(30);
        if (right)
            if (dir != 3) {
                dir = 4;
                return;
            }
    }
}

//食物检测
// 辅助函数：检查位置是否与蛇重叠
uint8_t isPositionOnSnake(int8_t x, int8_t y) {
	uint8_t i;
    for (i = 0; i < snake_lenght; i++) {
        if (snake_x[i] == x && snake_y[i] == y) {
            return 1;  // 位置与蛇身体重叠
        }
    }
    return 0;  // 位置不与蛇身体重叠
}
// 食物检测和生成函数的修改
void Food_check() {
    // 检查蛇头是否吃到食物
    if ((dir == 1 && snake_x[0] == food_x && snake_y[0] - 1 == food_y) ||
        (dir == 2 && snake_x[0] == food_x && snake_y[0] + 1 == food_y) ||
        (dir == 3 && snake_x[0] - 1 == food_x && snake_y[0] == food_y) ||
        (dir == 4 && snake_x[0] + 1 == food_x && snake_y[0] == food_y)) {
        
        // 清除当前食物显示
        OLED_showImage(food_y, food_x * 8, 8, 8, blank);
        
        // 生成新的食物位置，并确保它不与蛇重叠
        do {
            food_x = rand() % 16;  // 生成0到15的随机数，对应x轴
            food_y = rand() % 8;   // 生成0到7的随机数，对应y轴
        } while (isPositionOnSnake(food_x, food_y));
        
        snake_lenght++;
        score++;
        // 显示新食物
        OLED_showImage(food_y, food_x * 8, 8, 8, food);
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
void move() {
    uint8_t i;
	if(!isStart) return;
    // 清除蛇尾的前一个位置
    OLED_showImage(snake_y[snake_lenght - 1], snake_x[snake_lenght - 1] * 8, 8, 8, blank);

    // 更新蛇的每一节的位置
    for (i = snake_lenght - 1; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // 更新蛇头的位置
    switch (dir) {
        case 1:
            snake_y[0]--;
            break;
        case 2:
            snake_y[0]++;
            break;
        case 3:
            snake_x[0]--;
            break;
        case 4:
            snake_x[0]++;
            break;
    }
    print_OLED_SnakeBody(); // 重新绘制整条蛇
	OLED_showImage(food_y, food_x * 8, 8, 8, food);
}
void isDeath()
{
    int8_t i;
    if (snake_x[0] ==-1 || snake_y[0] ==-1 || snake_x[0] == 16 || snake_y[0] == 8){// 蛇头碰到墙壁
		TIM_Cmd(TIM3,DISABLE);//游戏结束，结束定时	
		isStart=0;
		flag2=1;
	}
			        
    for (i = 1; i < snake_lenght; ++i)
        if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]){// 蛇头与身体重合
			TIM_Cmd(TIM3,DISABLE);//游戏结束，结束定时
			isStart=0;
			flag2=2;
		}
}


void MainLoop() {
    
        if (isStart) {//游戏开始
			if (get_ms() > 10) {
			dir_check();      // 先获取新方向
				Food_check();     // 检测食物
        move();           // 执行移动
        isDeath();        // 检测移动后的碰撞    
        set_ms(0);
			}
        } else {
			if(flag2==1){
				Delay_s(1);//展示死亡画面
				OLED_Clear();//清屏
				OLED_showString(0, 20, "Game over !!", 1);
				OLED_showString(2, 0, "The snake's head hit the wall.", 1);
				OLED_showString(6, 0, "Your score :", 1);
				OLED_show_Number(6,100,score,1,1);
				Delay_s(3);//展示死亡画面
				flag2=0;//展示玩清除死亡标志，避免重复显示
				score=0;
			}
			if(flag2==2){
				Delay_s(1);//展示死亡画面
				OLED_Clear();//清屏
				OLED_showString(0, 20, "Game over !!", 1);
				OLED_showString(2, 0, "The snake's headtouched his body.", 1);
				OLED_showString(6, 0, "Your score :", 1);
				OLED_show_Number(6,100,score,1,1);
				Delay_s(3);//展示死亡画面
				flag2=0;
				score=0;
			}
			menu_control();
        }    
}
