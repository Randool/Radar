#include "STC15F2K60S2.H"
#include <intrins.h>

/**
 * �ı���Щ���ÿ��������ٶ�
 */
#define Duang_time	8	// �ƹ�Ч�����ʱ��
#define direction	32	// ȡ������������ҪΪ2�ı���
#define wait_ack	10	// ����ack�ļ��ʱ��

#define clockwise 1
#define anticlockwise 0

typedef unsigned int uint16;
typedef unsigned char uint8;

uint8 code weixuan[] = {0, 1, 2, 3, 4, 5, 6, 7};

uint8 code duanxuan[10] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66,
    0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

void delay_4us() {
	;;
}

void delay(uint16 ms) {
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
		delay(Duang_time);
		P0 = which_led;
		which_led >>= 1;
	}
}