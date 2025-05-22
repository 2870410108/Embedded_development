#include "snak_game.h"
/*
第二版：主要目标：1.美化开场UI
				   2.优化死亡边界检测以及咬到自己死亡检测
				   3.优化蛇的身体以及，食物
				   4.优化吃到食物分数显示
*/

int main(){
	
	init_game();
	
	while(1)
	{	
		MainLoop();
		gesture_check();
	}                         
}

