
from PyQt5 import QtGui, QtCore
import pyqtgraph as pg
import pyqtgraph.opengl as gl
from OpenGL.GL import glViewport
import sys
import argparse
import numpy
import os
import math
import numpy as np
import stl_parser as stl


class Window(QtGui.QWidget):

    def __init__(self):
        super(Window, self).__init__()
        width = QtGui.QDesktopWidget().screenGeometry().width()
        height = QtGui.QDesktopWidget().screenGeometry().height()
        self.resize(width, height)
        self.setWindowTitle("Team DUSTER Ground Station")
        self.setStyleSheet("background-color: white")

    def generateGridLayout(self):
        layout = QtGui.QGridLayout(self)
        self.setLayout(layout)
        return(layout)

        # Starting putting the widgets

    def setLogo(self, layout):
        logo = QtGui.QPixmap('logo.png')
        lb1 = QtGui.QLabel(self)
        lb1.setPixmap(logo)
        layout.addWidget(lb1, 0, 0)
        layout.setVerticalSpacing(200)

    def setTitle(self, layout):
        l2 = QtGui.QLabel()
        l2.setText("RORO I GROUND STATION . . . . .  . . . .  . . . . ")
        l2.setStyleSheet('font-size:30pt;font-family:RALE WAY; color: #02E148')
        layout.addWidget(l2, 0, 1, 1, 1)

    def setExitButton(self, layout):
        btn = QtGui.QPushButton("Quit", self)
        btn.clicked.connect(QtCore.QCoreApplication.instance().quit)
        layout.addWidget(btn, 3, 10, 1, 1)
        btn.setStyleSheet('font-size: 16pt; font-family: RALE WAY;  color: #02E148; border: 0.5px solid #02E148; border-radius: 5px;s')

    def setRocketAnimation(self, layout):
        w = gl.GLViewWidget()
        layout.addWidget(w, 1, 0, 3, 1)
        w.pan(0, 0, 0)
        w.setCameraPosition(distance=30)
        ax = gl.GLAxisItem()
        ax.setSize(5, 2, 2)
        ax.translate(0, 0, 0)
        w.addItem(ax)
        g = gl.GLGridItem()
        g.scale(1, 1, 1)
        g.translate(0, 0, 0)
        w.addItem(g)
        return w



def main():
    parser = argparse.ArgumentParser(description='Plot 3D Orientation',
                    epilog='reading from stdin, format: q0,q1,q2,q3[,x,y,z]')
    parser.add_argument('--title', default='3D Orientation\n',
                       help='set window title')
    parser.add_argument('--stl', default=None)
    args = parser.parse_args()


    app = QtGui.QApplication([])
    GUI = Window()
    layout = GUI.generateGridLayout()

    GUI.setLogo(layout)
    GUI.setExitButton(layout)
    rocketAnimation = GUI.setRocketAnimation(layout)

    GUI.show()

    if args.stl == None:
        mesh = stl.cubeMeshItem()
    else:
        stlmodel = stl.loader()
        stlmodel.load_stl(args.stl)
        # stlmodel.draw()
        mesh = stlmodel.mesh()
        mesh.setColor(pg.mkColor('w'))

    mesh.translate(0,0,1)
    rocketAnimation.addItem(mesh)
    print(QtGui.QPaintDevice.physicalDpiX(rocketAnimation))


    thread = stl.InputThread(mesh, args)
    thread.finished.connect(app.exit)
    thread.start()
    app.exec_()

if __name__ == '__main__':
    main()
