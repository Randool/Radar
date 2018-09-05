import os
import sys

from PyQt5 import QtWidgets

from canvas import QtCore, RaderCanvas, directions


class AppWindow(QtWidgets.QMainWindow):

    def __init__(self):
        QtWidgets.QMainWindow.__init__(self)
        self.setWindowTitle("Rader")

        self.set_main_widget()  # 设置主界面
        self.set_menu_bar()  # 设置菜单栏

    def set_menu_bar(self):
        """ 菜单栏设置 """
        self.menu_link = QtWidgets.QMenu("Link", self)
        self.menu_link.addAction("Connect", lambda: self.rc.connect_STC(), QtCore.Qt.CTRL + QtCore.Qt.Key_A)
        self.menu_link.addAction("Disconnect", lambda: self.rc.close_STC(), QtCore.Qt.CTRL + QtCore.Qt.Key_D)
        self.menu_link.addAction("Quit", self.__quit_func, QtCore.Qt.CTRL + QtCore.Qt.Key_Q)
        self.menuBar().addMenu(self.menu_link)
        
        self.menu_action = QtWidgets.QMenu("Action", self)
        self.menu_action.addAction("Start", self.__start_func, QtCore.Qt.CTRL + QtCore.Qt.Key_S)
        self.menu_action.addAction("Pause", self.__pause_func, QtCore.Qt.CTRL + QtCore.Qt.Key_P)
        self.menu_action.addAction("Log", self.__read_log_func, QtCore.Qt.CTRL + QtCore.Qt.Key_R)
        self.menuBar().addMenu(self.menu_action)

        self.menu_help = QtWidgets.QMenu("Help", self)
        self.menu_help.addAction("Help", self.__help_msg, QtCore.Qt.CTRL + QtCore.Qt.Key_H)
        self.menuBar().addMenu(self.menu_help)

    def set_main_widget(self):
        """ 主界面设置 """
        self.main_widget = QtWidgets.QWidget(self)
        dashboard = QtWidgets.QVBoxLayout(self.main_widget)
        self.rc = RaderCanvas(self.main_widget, 6, 5, dpi=100)
        dashboard.addWidget(self.rc)
        self.main_widget.setFocus()
        self.setCentralWidget(self.main_widget)

    def __start_func(self):
        if self.rc.serial is None:
            QtWidgets.QMessageBox.about(self, "Prompt", "未连接！\n请检查端口连接，并按‘Ctrl+A’连接STC。")
            return
        self.rc.start_measure()

    def __pause_func(self):
        if self.rc.serial is None:
            QtWidgets.QMessageBox.about(self, "Prompt", "未连接STC板，无法暂停。")
            return
        self.rc.pause_measure()

    def __read_log_func(self):
        if self.rc.serial is None:
            QtWidgets.QMessageBox.about(self, "Prompt", "未连接STC板，无法读取信息。")
            return
        self.rc.take_out_log()

    def __quit_func(self):
        if self.rc.serial is not None:
            self.rc.close_STC()
        self.close()

    def __help_msg(self):
        QtWidgets.QMessageBox.about(self, "Help", "1.将C代码烧录进STC\n2.Ctrl+a连接STC\n3.Ctrl+s开始测量\n4.Ctrl+p暂停测量\n5.Ctrl+q退出程序")


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    aw = AppWindow()
    aw.show()
    sys.exit(app.exec_())
