#include <INTRINS.H>

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 123; j++) { // 大约1ms的延时
            _nop_();
        }
    }
}
void delay_us(unsigned int us) {
    unsigned int i;
    for (i = 0; i < us; i++) {
            _nop_();
    }
}