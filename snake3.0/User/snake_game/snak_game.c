#include "OLED12864.h"
#include "MPU6050.h"
#include "snake_display_data.h"
#include "sys_tick.h"
#include "Timer.h"
#include "stdlib.h"

int8_t snake_x[30], snake_y[30];
//食物的坐标位置
int8_t food_x, food_y;
//蛇的长度
uint8_t snake_lenght;
//蛇的方向
uint8_t dir;
//游戏分数
uint8_t score;

float *data_array;
float roll, pitch;

#define up          pitch<-6
#define down    	pitch>7
#define left        roll>6
#define right       roll<-7

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
    init_timer_3();
	dir = 4;//初始化方向
    snake_lenght = 3;//初始化蛇的长度
	food_x = 5;
    food_y = 6;
    snake_x[0] = 5, snake_y[0] = 3;//初始化蛇头位置
    snake_x[1] = 4, snake_y[1] = 3;//初始化第二节点
    snake_x[2] = 3, snake_y[2] = 3;//初始化第三节点
    OLED_showImage(0, 0, 128, 64, image1_128X64);//欢迎界面
    Delay_s(3);
    OLED_Clear();//清屏
    print_OLED_SnakeBody();//画出蛇身
		srand(TIM_GetCounter(TIM3));//初始化食物
    OLED_showImage(food_y, food_x * 8, 8, 8, food);//打印食物
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
        Delay_ms(50);
        if (up)
            if (dir != 2) {//检测当前方向是否与预计修改的方向冲突
                dir = 1;
                return;
            }
    }
    if (down) {
        Delay_ms(50);
        if (down)
            if (dir != 1) {
                dir = 2;
                return;
            }
    }
    if (left) {
        Delay_ms(50);
        if (left)
            if (dir != 4) {
                dir = 3;
                return;
            }
    }
    if (right) {
        Delay_ms(50);
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
//判断蛇是否死亡
uint8_t isDeath()
{
    int8_t i;
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
    if (get_ms() > 10) {
        dir_check();
        if (isDeath()) {
			TIM_Cmd(TIM3,DISABLE);
            OLED_Clear();//清屏
            OLED_showString(0, 13, "GAME OVER !!!", 1);
            OLED_showString(2, 20, "SCORE --> ", 1);
            OLED_showNumber(2,80,score,1,1);
        } else {
            Food_check();
			//OLED_Clear();
            move();
        }
        set_ms(0);
    }
}
