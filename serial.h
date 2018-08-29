static bit busy = 1;

void send_byte(uint8 byte) {
	SBUF = byte;
	while (busy);
	busy = 1;
}

void send_data(uint8 ack, uint16 Data) {
	do {
		send_byte(ack);
		delay( wait_ack );
	} while ( (SBUF^(~ack)) );
	
	send_byte(Data >> 8);
	send_byte(Data & 0xff);
}

// void recv_data() {
// 	
// }
