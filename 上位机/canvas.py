from matplotlib.backends.backend_qt5agg import \
    FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt5 import QtCore, QtWidgets

from contral import Control, directions, np


step = 2 * np.pi / directions


class RaderCanvas(Control, FigureCanvas):
    """ 用于展示雷达图 """

    def __init__(self, parent=None, width=5, height=4, dpi=100):
        super().__init__()

        # 设置图像属性
        self.__fig__ = Figure(figsize=(width, height), dpi=dpi)
        self.__axes__ = self.__fig__.add_subplot(111, projection="polar")

        FigureCanvas.__init__(self, self.__fig__)
        self.setParent(parent)
        FigureCanvas.setSizePolicy(
            self, QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding
        )
        FigureCanvas.updateGeometry(self)

        # 数据部分
        self.__R__ = np.zeros(directions)
        self.__theta__ = np.linspace(0, np.pi*2, directions)
        self.__Index__ = 0  # 下标
        self.__trend__ = 1  # 方向

        # 初始化图像
        self.__axes__.set_rmax(4.3)
        self.__axes__.set_rticks([1, 2, 3, 4])
        self.__axes__.set_rlabel_position(0)
        
        # 计时器
        self.__timer__ = QtCore.QTimer(self)

    def start_measure(self, freq=60):
        """ 连接STC并启动定时器 """
        self.serial.write(self.ALIVE)
        self.__timer__.timeout.connect(self.update_figure)
        self.__timer__.start(freq)

    def pause_measure(self):
        """ 暂停定时器 """
        self.serial.write(self.ACK)
        self.__timer__.stop()

    def update_figure(self):
        """ 更新作图 """
        r = self.get_distance()
        print(f"{self.__Index__}\tr = {r} m")
        
        self.__R__[self.__Index__] = r
        self.__Index__ += self.__trend__
        
        if self.__Index__ == -1 or self.__Index__ == directions:
            self.__trend__ = -self.__trend__
            self.__Index__ += self.__trend__

        self.__axes__.cla()
        self.__axes__.plot(self.__theta__, self.__R__)
        self.__axes__.set_rticks([1, 2, 3, 4])
        self.__axes__.set_rmax(4.3)
        self.__axes__.set_rlabel_position(0)
        self.draw()
