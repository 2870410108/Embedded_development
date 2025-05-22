#ifndef __OLED12864_h__
#define __OLED12864_h__

#define uchar unsigned char
	
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_ColorTurn(uchar i);
void OLED_Scroll_horizontally(uchar set_R_or_L);
void OLED_showChar(uchar Y, uchar X,uchar C,uchar dis_pattern);
void OLED_showString(uchar y, uchar x,uchar * str,uchar dis_pattern);
void OLED_showNumber(uchar y, uchar x, float number, uchar display_type, uchar dis_pattern);
void OLED_showImage(uchar y, uchar x,uchar width ,uchar height, const uchar * image );

#endif
