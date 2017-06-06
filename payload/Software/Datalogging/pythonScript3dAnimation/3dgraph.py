from pyqtgraph.Qt import QtCore, QtGui
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






def main():
    parser = argparse.ArgumentParser(description='Plot 3D Orientation',
                    epilog='reading from stdin, format: q0,q1,q2,q3[,x,y,z]')
    parser.add_argument('--title', default='3D Orientation\n',
                       help='set window title')
    parser.add_argument('--stl', default=None)
    args = parser.parse_args()




    app = QtGui.QApplication([])
    canvas =  QtGui.QWidget()
    width = screenShape = QtGui.QDesktopWidget().screenGeometry().width()
    height = screenShape = QtGui.QDesktopWidget().screenGeometry().height()


    print (width,height)
    canvas.resize(width,height)



    layout = QtGui.QGridLayout()
    canvas.setLayout(layout)
    canvas.setStyleSheet("background-color: white");

    w = gl.GLViewWidget()
    layout.addWidget(w, 0, 0, 3, 3)



    #image = pg.image('logo.png')
    #layout.addWidget(image, 0, 0, 1, 1)

    win = pg.GraphicsWindow(title="Altitude[m]")
    #pg.setConfigOptions(antialias=True)
    layout.addWidget(win, 4, 4, 1, 1)

    p1 = win.addPlot(title="Basic array plotting", y=np.random.normal(size=100))


    # p6 = win.addPlot(canvas)
    # curve = p6.plot(pen='y')
    # data = np.random.normal(size=(10,1000))
    # layout.addWidget(p6, 1, 4, 1, 1)

    # ptr = 0
    # def update():
    #     global curve, data, ptr, p6
    #     curve.setData(data[ptr%10])
    #     if ptr == 0:
    #         p6.enableAutoRange('xy', False)  ## stop auto-scaling after the first data set is plotted
    #     ptr += 1
    # timer = QtCore.QTimer()
    # timer.timeout.connect(update)
    # timer.start(50)


    # vp = (0,0,fs.width(),fs.height())
    # vp = (0,0,2*fs.width(),2*fs.height())
    width = canvas.frameGeometry().width()
    height = canvas.frameGeometry().height()
    pr = w.devicePixelRatio()
    height = int(height*1.41)
    width = int(width*1.41)
    vp = (-100, -100, width, height)
    print(vp)
    w.opts['viewport'] = vp
    print(w.opts['viewport'])
    print(w.devicePixelRatio())

    w.setWindowTitle(args.title)

    #w.setBackgroundColor('w')
    w.pan(0, 0, 1)
    w.setCameraPosition(distance=20)


    ax = gl.GLAxisItem()

    ax.setSize(3, 3, 3)
    ax.translate(0, 0, 1)
    w.addItem(ax)

    g = gl.GLGridItem()
    g.scale(2, 2, 1)
    g.translate(0, 0, 0)
    w.addItem(g)


    if args.stl == None:
        mesh = cubeMeshItem()
    else:
        stlmodel = stl.loader()
        stlmodel.load_stl(args.stl)
        # stlmodel.draw()
        mesh = stlmodel.mesh()
        # mesh.setColor(pg.mkColor('r'))

    mesh.translate(0,0,1)
    w.addItem(mesh)
    canvas.show()

    thread = stl.InputThread(mesh, args)
    thread.finished.connect(app.exit)
    thread.start()
    app.exec_()

if __name__ == '__main__':
    main()
