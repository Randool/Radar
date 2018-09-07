sbit SDA = P4^0;		  	//I2C���ߵ�������
sbit SCL = P5^5;		  	//I2C���ߵ�ʱ����

void delay() { 		//��ʱ4us
	;;
}

void IIC_init() {	//I2C���߳�ʼ��
	SCL=1;
	delay();
	SDA=1;
	delay();
}

void start() {	 	//���������ź�
 	SDA = 1;
	delay();
	SCL = 1;
	delay();
	SDA = 0;
	delay();
}

void stop()	{	 	//ֹͣ�ź�
 	SDA=0;
	delay();
	SCL=1;
	delay();
	SDA=1;
	delay();
}

void respons() {	//�ӻ�Ӧ���ź�
	uint8 i=0;
	SCL=1;
	delay();
	while(SDA==1 && (i<255))  //��ʾ����һ��ʱ����û���յ���������Ӧ����
		i++;				//������Ĭ�ϴ��ڼ��Ѿ��յ����ݶ����ٵȴ�Ӧ���źš� 
	SCL=0;
	delay();
}

void writebyte(uint8 date) { //��24C16дһ���ֽ�����
	uint8 i, temp;
	temp=date;
	for(i=0;i<8;i++) {
		temp = temp << 1;
		SCL = 0;
		delay();
		SDA = CY;
		delay();
		SCL = 1;
		delay();
	}
	SCL=0;
	delay();
	SDA=1;
	delay();	
}

uint8 readbyte() {	//��24C16��һ���ֽ�����
	uint8 i,k;
	SCL=0;
	delay();
	SDA=1;
	delay();
	for(i=0;i<8;i++)
	{
		SCL=1;
		delay();
		k=(k<<1)|SDA;
		delay();
		SCL=0;
		delay();
	}
	delay();
	return k;	
}

void write_addr(uint8 addr,uint8 date) {  //��24C16�ĵ�ַaddr��д��һ������date
	start();
	writebyte(0xa0);							
	respons();
	writebyte(addr);
	respons();
	writebyte(date);
	respons();
	stop();							
}

uint8 read_addr(uint8 addr) { //��24C16��addr��ַ����һ���ֽ�����
	uint8 date;
	start();
	writebyte(0xa0);
	respons();
	writebyte(addr);
	respons();
	
	start();
	writebyte(0xa1);
	respons();
	date = readbyte();
	stop();
	
	return date;
}
