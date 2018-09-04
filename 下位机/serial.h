#define READ 0x22

static bit busy = 1;

void send_byte(uint8 byte) {
	SBUF = byte;
	while (busy);
	busy = 1;
}

/**
 * ���;�����߶�ȡ�洢
 */
void send_data(uint8 ack, uint16 Data) {
	// ��λ�������ȡ�洢
	if ( SBUF == READ ) {
		uint8 p = 0;
		while (p < direction) {
			send_byte(read_addr(p++));
			send_byte(read_addr(p++));
		}
	}
	
	do {  // �ȴ�~ack
		send_byte(ack);
		delay( wait_ack );
	} while ( (SBUF^(~ack)) );
	
	send_byte(Data >> 8);
	send_byte(Data & 0xff);
}
