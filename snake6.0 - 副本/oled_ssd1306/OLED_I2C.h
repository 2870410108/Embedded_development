#ifndef __OLED_I2C_H__
#define __OLED_I2C_H__

#define uchar unsigned char
	


void Init_OLED_I2C(void);
void OLED_WriteCommand(uchar Command);
void OLED_WriteData(uchar Data);








#endif
