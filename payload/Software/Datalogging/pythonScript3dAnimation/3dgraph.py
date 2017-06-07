
from PyQt5 import QtGui, QtCore
from PyQt5.QtCore import QObject, pyqtSignal
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
        self.setStyleSheet("background-color: white;")

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

    # def setTitle(self, layout):
    #     l2 = QtGui.QLabel()
    #     l2.setText("Ground Station . . . . .  . . . .  . . . . ")
    #     l2.setStyleSheet('font-size:20pt;font-family:RALE WAY; color: #02E148')
    #     layout.addWidget(l2, 0, 0)

    def setExitButton(self, layout):
        btn = QtGui.QPushButton("Quit", self)
        btn.clicked.connect(QtCore.QCoreApplication.instance().quit)
        layout.addWidget(btn, 3, 5, 1, 1)
        btn.setStyleSheet('font-size: 16pt; font-family: RALE WAY;  color: #02E148; border: 0.5px solid #02E148; border-radius: 5px;s')

    def setRocketAnimation(self, layout):
        w = gl.GLViewWidget()
        layout.addWidget(w, 2, 0, 2, 5)
        w.pan(0, 0, 1)
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

    def setRawDataBox(self, layout):
        self.textbox = QtGui.QTextEdit()
        self.textbox.setAlignment(QtCore.Qt.AlignBaseline)
        self.textbox.resize(100, 200)
        layout.addWidget(self.textbox, 2, 5, 2, 5)

    def updateRawDataBox(self, value):
        self.textbox.setText(value)

    def setAltitudePlot(self, layout):
        win = pg.GraphicsWindow(title="Altitude", border=True)
        win.setWindowTitle('pyqtgraph example: Plotting')
        pg.setConfigOptions(antialias=True)
        p1 = win.addPlot(title="Attitude", y=np.random.normal(size=100))
        layout.addWidget(win, 0, 4, 2,2)

def rad2deg(rad):
    return 360*rad/2/math.pi

class InputThread(QtCore.QThread):
    origin = pyqtSignal()
    def __init__(self, mesh_item, args, GUI, layout):
        self.mesh_item = mesh_item
        self.GUI = GUI
        self.layout = layout
        super(InputThread, self).__init__()

    def run(self):
        while True:
            line = sys.stdin.readline()
            self.origin.connect(self.done)
            self.origin.emit()
            i = line.split(',')
            if len(i) >= 4:
                q = [float(x) for x in i[:4]]
                # print(q)
                if (len(i) == 4+3):
                    t = [float(x) for x in i[4:]]
                    self.updateMeshTransform(q, t)
                else:
                    self.updateMeshTransform(q)



    def updateMeshTransform(self, q, t=[0,0,1]):
        v = numpy.array(q[1:])
        vnorm = numpy.linalg.norm(v)
        v = v * 1/vnorm
        angle = 2*math.atan2(vnorm, q[0])
        # print('3d', angle, v)
        tr = pg.Transform3D()
        tr.translate(*t)
        tr.rotate(rad2deg(angle), *v)

        tr.rotate(90, 1,0,0)
        # tr.translate(0, 0, -0.5)
        tr.scale(0.015, 0.015, 0.015)

        self.mesh_item.setTransform(tr)

    def done(self):
        print("hello")


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
    #GUI.setRawDataBox(layout)
    GUI.setRawDataBox(layout)
    # GUI.setTitle(layout)

    rocketAnimation = GUI.setRocketAnimation(layout)
    # GUI.setAltitudePlot(layout)
    GUI.show()

    if args.stl == None:
        mesh = stl.cubeMeshItem()
    else:
        stlmodel = stl.loader()
        stlmodel.load_stl(args.stl)
        stlmodel.draw()
        mesh = stlmodel.mesh()
        mesh.setColor(pg.mkColor('w'))

    mesh.translate(0, 0, 1)
    rocketAnimation.addItem(mesh)


    thread = InputThread(mesh, args, GUI, layout)
    thread.finished.connect(app.exit)
    thread.start()
    app.exec_()

if __name__ == '__main__':
    main()
