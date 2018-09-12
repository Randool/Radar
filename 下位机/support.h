#ifndef SUPPORT_H
#define SUPPORT_H

/**
 * 配置项
 */
#define Duang_time	8	// 光效持续时间
#define direction	32	// 取样方向
#define wait_ack	5	// 串口通信等待时间

#define clockwise     1
#define anticlockwise 0

typedef unsigned int uint16;
typedef unsigned char uint8;

sbit sLED = P2^3;		// LED阳极引脚

uint8 code weixuan[] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8 code duanxuan[10] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,
    0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

void delay_ms(uint8 ms) {
	uint8 i, j;
	while (ms--) {
		i = 11, j = 190;
		while (i--)
			while (j--);
	}
}
void Delay25us() {
	uint8 i;
	_nop_();
	_nop_();
	i = 72;
	while (--i);
}

void Duang() {
	uint8 i = 9, which_led = 0x80;
	while (i--) {
		delay_ms(Duang_time);
		P0 = which_led;
		which_led >>= 1;
	}
}

#endif
