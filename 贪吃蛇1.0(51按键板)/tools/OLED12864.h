#ifndef __oled12864_h__
#define __oled12864_h__
#define uchar unsigned char
void OLED_Init();
void OLED_Clear();
void OLED_showChar(uchar Y, uchar X,uchar C,bit dis_pattern);
void OLED_showString(uchar y, uchar x,uchar * str,bit dis_pattern);
void OLED_showImage(uchar y, uchar x,uchar width ,uchar height,uchar * image);
void OLED_showImage_64X64();
void OLED_showImage_128X64();
#endif