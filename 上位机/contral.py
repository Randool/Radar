import serial
from serial.tools import list_ports


class Contral:
    """ 串口控制\n说明：ack为暂停……"""

    def __init__(self, BAUD=9600):
        self.BAUD = BAUD
        self.ack = b"\x11"
        self.port = None
        self.serial = None

    def connect_STC(self):
        if self.serial is not None:
            print("已连接")
            return
        print("端口扫描中...")
        for port in list_ports.comports():
            port_info = list(port)
            if "CH340" in port_info[1]:
                print(f"STC 在 [{port_info[0]}]")
                self.port = port_info[0]
                break
        if self.port is not None:
            self.serial = serial.Serial(self.port, self.BAUD)
            print("安排上了")
        else:
            print("请检查端口是否连接")

    def close_STC(self):
        try:
            self.serial.write(self.ack)
            self.serial.close()
            self.serial = None
            print("取消安排")
        except Exception as e:
            print(e)

    def get_distance(self) -> float:
        self.serial.flushInput()
        while True:
            a = self.serial.read()
            if a == self.ack:
                continue
            else:
                b = self.serial.read()
                return (ord(a) * 256 + ord(b)) / 1000
