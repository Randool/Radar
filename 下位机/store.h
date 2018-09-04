// ����ʧ�Դ洢

sbit SDA = P4 ^ 0;	// I2C����������
sbit SCL = P5 ^ 0;	// I2C����ʱ����

uint8 pointer;		// ��ַָ��
uint8 write_data;	// д����
uint8 read_data;	// ������

void store_init() {
	// I2C���߳�ʼ��
	SCL = 1;
	delay_4us();
	SDA = 1;
	delay_4us();
	
	pointer = 0x00;
}

void start() {		// ���������ź�
 	SDA = 1;
	delay_4us();
	SCL = 1;
	delay_4us();
	SDA = 0;
	delay_4us(); 	
}

void stop()	{		// ֹͣ�ź�
 	SDA = 0;
	delay_4us();
	SCL = 1;
	delay_4us();
	SDA = 1;
	delay_4us();
}

void response() {	// �ӻ�Ӧ���ź�
	uint8 i = 0;
	SCL = 1;
	delay_4us();
	/*��ʾ����һ��ʱ����û���յ���������Ӧ��
	  ��������Ĭ�ϴ��ڼ��Ѿ��յ����ݶ����ٵȴ�Ӧ���źš�*/
	while(SDA == 1 && (i < 255))
		i++;
	SCL = 0;
	delay_4us();
}

void writebyte(uint8 date) {	// дһ���ֽ�����
	uint8 i, temp;
	temp = date;
	for(i = 0; i < 8; ++i) {
		temp = temp << 1;
		SCL = 0;
		delay_4us();
		SDA = CY;
		delay_4us();
		SCL = 1;
		delay_4us();
	}
	SCL = 0;
	delay_4us();
	SDA = 1;
	delay_4us();
}

uint8 readbyte() {	// ��һ���ֽ�����
	uint8 i,k;
	SCL = 0;
	delay_4us();
	SDA = 1;
	delay_4us();
	for(i = 0; i < 8; i++) {
		SCL = 1;
		delay_4us();
		k = (k<<1) | SDA;
		delay_4us();
		SCL = 0;
		delay_4us();
	}
	delay_4us();
	return k;	
}

void write_addr(uint8 addr, uint8 date) {	// ��dataд��addr��
	start();
	writebyte(0xa0);	// ѡ��24C16
	response();
	writebyte(addr);	// ѡ���ַ
	response();
	writebyte(date);	// д����
	response();
	stop();
}

uint8 read_addr(uint8 addr) { // ��addr��ȡdata
	uint8 date;
	start();
	writebyte(0xa0);	// ѡ���0��24C16
	response();
	writebyte(addr);	// ѡ���ַ
	response();	
	start();
	writebyte(0xa1);	// д����
	response();
	date = readbyte();
	stop();
	return date;
}
