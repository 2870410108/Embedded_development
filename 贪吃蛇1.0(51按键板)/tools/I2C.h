#ifndef __I2C_H__
#define __I2C_H__


#define uchar unsigned char

void start ();
void stop ();
void send_byte(uchar byte);
//uchar accept_byte();
//void send_response();
bit  accept_response();







#endif
