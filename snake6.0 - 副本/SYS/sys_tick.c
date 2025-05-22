#include "sys_tick.h"
 
 

//微秒延时
void Delay_us(u32 nus)
{
    if (nus == 0) return;  // 避免无效调用
	  SysTick->CTRL &= ~(1 << 2);  // 1. 选择外部时钟源 (HCLK/8)
    SysTick->LOAD = nus * 9 - 1;  // 1. 设定倒计时初值
    SysTick->CTRL &= ~(1 << 1);            // 2. 关闭中断模式
    SysTick->VAL = 0x00;                   // 3. 计数器清零
    SysTick->CTRL |= (1 << 0);             // 4. 启动计数

    while ((SysTick->CTRL & (1 << 16)) == 0);  // 5. 等待倒计时结束
    SysTick->CTRL &= ~0x07;  // 6. 彻底关闭 SysTick
    SysTick->VAL = 0x00;     // 7. 清空计数器
}

//毫秒延时
void Delay_ms(u32 nms)
{
    while (nms--) Delay_us(1000);  
}

//秒延时
void Delay_s(u32 ns)
{
    while (ns--) Delay_ms(1000);  
}

