#include "STC15F2K60S2.H"
#include <intrins.h>

#define Duang_time	10	// �ƹ�Ч�����ʱ��
#define direction	32	// ȡ��������
#define wait_ack	50	// ����ack�ļ��ʱ��

typedef unsigned int uint16;
typedef unsigned char uint8;

unsigned char code weixuan[] = {0, 1, 2, 3, 4, 5, 6, 7};

unsigned char code duanxuan[10] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,
    0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

void delay(unsigned int ms) {
	unsigned char i, j;
	while (ms--) {
		i = 11, j = 190;
		while (i--)
			while (j--);
	}
}

void Delay25us() {
	unsigned char i;
	_nop_();
	_nop_();
	i = 72;
	while (--i);
}

void Duang() {
	uint8 i = 9, which_led = 0x80;
	while (i--) {
		delay(Duang_time);
		P0 = which_led;
		which_led >>= 1;
	}
}