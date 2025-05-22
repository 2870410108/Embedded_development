#ifndef __MPU6050_H__
#define __MPU6050_H__
#include "stm32f10x.h"										  	  
 




void MPU6050_WriteCommand(uint8_t Command,uint8_t data);
uint8_t MPU6050_ReadData(uint8_t Command);
void  MPU6050_Init(void);
float * MPU6050_GetData(void);

#endif



