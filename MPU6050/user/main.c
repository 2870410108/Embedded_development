#include "stm32f10x.h"
#include "sys_tick.h"
#include "MPU6050.h"
#include <stdio.h>

float *data;

// UART1 初始化
void Init_UART1(void) {
    USART_InitTypeDef usartInitTypeDef;
    GPIO_InitTypeDef gpioInitTypeDef;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 GPIO
    gpioInitTypeDef.GPIO_Pin = GPIO_Pin_9;  // TX
    gpioInitTypeDef.GPIO_Mode = GPIO_Mode_AF_PP;
    gpioInitTypeDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpioInitTypeDef);

    gpioInitTypeDef.GPIO_Pin = GPIO_Pin_10;  // RX
    gpioInitTypeDef.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpioInitTypeDef);

    // 配置 USART1
    usartInitTypeDef.USART_WordLength = USART_WordLength_8b;
    usartInitTypeDef.USART_StopBits = USART_StopBits_1;
    usartInitTypeDef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usartInitTypeDef.USART_BaudRate = 115200;
    usartInitTypeDef.USART_Parity = USART_Parity_No;
    usartInitTypeDef.USART_Mode = USART_Mode_Tx;  // 只用发送
    USART_Init(USART1, &usartInitTypeDef);

    USART_Cmd(USART1, ENABLE);
}

// UART 发送字符串
void UART_SendString(char *str) {
    while (*str) {
        while (!(USART1->SR & USART_SR_TXE));  // 等待发送寄存器空
        USART1->DR = *str++;
    }
}

// TIM2 初始化（100Hz，10ms 周期）
void TIM2_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Prescaler = 7200 - 1;  // 72MHz / 7200 = 10kHz
    TIM_InitStructure.TIM_Period = 100 - 1;      // 10kHz / 100 = 100Hz (10ms)
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
    TIM_Cmd(TIM2, ENABLE);
}

int main(void) {
	  char buffer[50];
    float gyro_x_offset = -4.816;  // 陀螺仪 X 轴零点偏移（需校准）
	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 中断优先级分组
    Init_UART1();
	  UART_SendString("UART1 Init Done\n");  // 调试：UART1 初始化完成
	  MPU6050_Init();
	  UART_SendString("MPU6050 Init Done\n");  // 调试：MPU6050 初始化完成
	  TIM2_Init();  // 初始化定时器
	  UART_SendString("TIM2 Init Done\n");  // 调试：TIM2 初始化完成
    

    


    // 校准陀螺仪偏移（运行一次后注释掉）
//    float sum_gx = 0.0;
//    int i, count = 1000;
//    UART_SendString("Calibration Start\n");  // 调试：校正开始
//    for (i = 0; i < count; i++) {
//        UART_SendString("2");  // 调试：每次循环
//        data = MPU6050_GetData();
//        sum_gx += data[3];  // 陀螺仪 ω_x
//        while (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != SET);  // 等待 10ms
//        TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//    }
//    gyro_x_offset = sum_gx / count;
//    sprintf(buffer, "Gyro X Offset: %.3f\n", gyro_x_offset);
//    UART_SendString(buffer);  // 直接用 UART_SendString 输出

//    UART_SendString("Calibration Done\n");  // 调试：校正完成

    while (1) {
        if (TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) == SET) {  // 每 10ms 执行一次
            TIM_ClearFlag(TIM2, TIM_FLAG_Update);

            // 采集 MPU6050 数据
            data = MPU6050_GetData();

            // 获取加速度计和陀螺仪数据
            float ax = data[0];         // 加速度计 X（g）
            float ay = data[1];         // 加速度计 Y
            float az = data[2];         // 加速度计 Z
            float gx = data[3] - gyro_x_offset;  // 陀螺仪 X（°/s，横滚轴）
            float roll_fused = data[8];  // STM32 融合的横滚角（alpha=0.95238）

            // 串口发送数据（格式：ax,ay,az,gx,roll_fused\n）
            sprintf(buffer, "%.3f,%.3f,%.3f,%.3f,%.3f\n", ax, ay, az, gx, roll_fused);
            UART_SendString(buffer);
        }
    }
}