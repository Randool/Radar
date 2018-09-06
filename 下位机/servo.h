#define TIMES (2048 / direction)

sbit KEY1 = P3^2;	// ȷ�ϼ�
sbit KEY2 = P3^3;	// ������ʱ����ת
sbit KEY3 = P1^7;	// ����˳ʱ����ת

sbit s1 = P4^1;
sbit s2 = P4^2;
sbit s3 = P4^3;
sbit s4 = P4^4;

void step(bit trend) {
	uint16 i, z = TIMES;
	if (trend == anticlockwise) {
		for (i = 0; i < z; ++i) {
			switch (i % 4) {
				case 0: s1 = 1; s2 = 0; s3 = 0; s4 = 0; break;
				case 1: s1 = 0; s2 = 1; s3 = 0; s4 = 0; break;
				case 2: s1 = 0; s2 = 0; s3 = 1; s4 = 0; break;
				case 3: s1 = 0; s2 = 0; s3 = 0; s4 = 1; break;
			}
			delay(1);
		}
	} else {
		for (i = 0; i < z; ++i) {
			switch (i % 4) {
				case 0: s1 = 0; s2 = 0; s3 = 0; s4 = 1; break;
				case 1: s1 = 0; s2 = 0; s3 = 1; s4 = 0; break;
				case 2: s1 = 0; s2 = 1; s3 = 0; s4 = 0; break;
				case 3: s1 = 1; s2 = 0; s3 = 0; s4 = 0; break;
			}
			delay(1);
		}
	}
	P4 = 0;
}

// ���ڲ�������޷��Զ������Ƕȣ�
// ��Ҫ�ֶ�������ʼ�Ƕ�
void adjust() {
	while (1) {
		if (!KEY1) {
			Duang();
			break;
		}
		else if (!KEY2) {
			step(clockwise);
		}
		else if (!KEY3) {
			step(anticlockwise);
		}
	}
}

/*
STEP	A	B	C	D
	1	1	0	0	0
	2	0	1	0	0
	3	0	0	1	0
	4	0	0	0	1
ÿ��һ�������źţ����������ת5.625��
*/