import numpy as np
from matplotlib.backends.backend_qt5agg import \
    FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtWidgets

from contral import Contral, directions


step = 2 * np.pi / directions


class RaderCanvas(Contral, FigureCanvas):
    """ 用于展示雷达图 """

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        super().__init__()

        # 设置图像属性
        self.fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = self.fig.add_subplot(111, projection="polar")

        FigureCanvas.__init__(self, self.fig)
        self.setParent(parent)
        FigureCanvas.setSizePolicy(
            self, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding
        )
        FigureCanvas.updateGeometry(self)

        # 数据部分
        self.rQueue = []
        self.thetaQueue = []
        self.buff = 0

        # 初始化图像
        self.axes.set_rmax(4.3)
        self.axes.set_rticks([1, 2, 3, 4])
        self.axes.set_rlabel_position(0)

        # 计时器
        self.timer = QtCore.QTimer(self)

    def start_measure(self, freq=150):
        """ 连接STC并启动定时器 """
        self.serial.write(b"\xee")
        self.timer.timeout.connect(self.update_figure)
        self.timer.start(freq)

    def pause_measure(self):
        """ 暂停定时器 """
        self.serial.write(self.ack)
        self.timer.stop()

    def update_figure(self):
        """ 更新作图 """
        r = self.get_distance()
        print(f"\rr = {r} m")
        if len(self.rQueue) == directions:
            self.rQueue.pop(0)
            self.thetaQueue.pop(0)
        self.rQueue.append(r)
        self.buff += step
        self.thetaQueue.append(self.buff)

        self.axes.cla()
        self.axes.plot(self.thetaQueue, self.rQueue)
        self.axes.set_rticks([1, 2, 3, 4])
        self.axes.set_rmax(4.3)
        self.axes.set_rlabel_position(0)
        self.draw()
