#ifndef __UserConfig_H__
#define __UserConfig_H__

#include "stm32f10x.h"
///////////////////////////////////GPIO配置///////////////////////////////////////////////////


#define PeriphClock         		RCC_APB2Periph_GPIOB            //待开启的时钟外设
#define GPIO_Periph        			GPIOB							//待初始化的GPIO寄存器
#define OLED_GPIO_SCL_Pin        	GPIO_Pin_8			    		//待初始化SCL的引脚编号
#define OLED_GPIO_SDA_Pin        	GPIO_Pin_9			    		//待初始化SDA的引脚编号
#define MPU_GPIO_SCL_Pin        	GPIO_Pin_10			    		//待初始化SCL的引脚编号
#define MPU_GPIO_SDA_Pin        	GPIO_Pin_11			    		//待初始化SDA的引脚编号

///////////////////////////////////OLED配置///////////////////////////////////////////////////
#define OLED_ADDRESS 0x78 // OLED I2C 地址  
#define OLED_COMMAND 0x00 // OLED 命令模式  
#define OLED_DATA 0x40    // OLED 数据模式
#endif

