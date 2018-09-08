#ifndef SERIAL_H
#define SERIAL_H

#define ACK   0x11
#define READ  0x22
#define ALIVE 0xee	// �ɸ���

static bit busy = 1;

void send_byte(uint8 byte) {
	SBUF = byte;
	while (busy);
	busy = 1;
}

/**
 * ���;�����߶�ȡ�洢
 */
uint8 data_buff[direction*2];

void send_data(uint16 Data) {
	if ( SBUF == READ ) {	// ��λ�������ȡ�洢
		uint8 i = 0;
		EA = 0;
		for (i = 0; i < direction; ++i) {
			data_buff[i<<1] = read_addr(i<<1);			delay_ms(1);
			data_buff[(i<<1)+1] = read_addr((i<<1)+1);	delay_ms(1);
		}
		EA = 1;
		delay_ms(1);
		for (i = 0; i < direction; ++i) {
			send_byte( data_buff[i<<1] );
			send_byte( data_buff[(i<<1)+1] );
		}
	}
	
	do {  // �ȴ���λ����֤
		send_byte(ACK);
		delay_ms( wait_ack );
	} while ( SBUF^ALIVE );	// SBUF != ALIVE
	
	send_byte(Data >> 8);
	send_byte(Data & 0xff);
}

#endif
