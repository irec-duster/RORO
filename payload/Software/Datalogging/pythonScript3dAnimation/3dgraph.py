from pyqtgraph.Qt import QtCore, QtGui
import pyqtgraph as pg
import pyqtgraph.opengl as gl
import sys
import argparse
import numpy
import math

def cubeMeshItem():
    verts = numpy.array([
        [-1, -1, -1],
        [ 1, -1, -1],
        [-1,  1, -1],
        [ 1,  1, -1],
        [-1, -1,  1],
        [ 1, -1,  1],
        [-1,  1,  1],
        [ 1,  1,  1],
    ])
    faces = numpy.array([
        [0, 1, 2], # bottom
        [1, 2, 3],
        [4, 5, 6], # top
        [5, 6, 7],
        [0, 1, 4], # right
        [1, 4, 5],
        [2, 3, 6], # left
        [3, 6, 7],
        [0, 2, 4], # back
        [2, 4, 6],
        [1, 3, 5], # front
        [3, 5, 7]
    ])
    colors = numpy.array([
        [0.5, 1, 0.5, 1],
        [0.5, 1, 0.5, 1],
        [0, 1, 0, 1],
        [0, 1, 0, 1],
        [1, 1, 0.5, 1],
        [1, 1, 0.5, 1],
        [1, 1, 0, 1],
        [1, 1, 0, 1],
        [0.5, 0.5, 1, 1],
        [0.5, 0.5, 1, 1],
        [0, 0, 1, 1],
        [0, 0, 1, 1]
    ])
    return gl.GLMeshItem(vertexes=verts, faces=faces, faceColors=colors, smooth=False)



################################################################################
import struct

from OpenGL.GL import *
from OpenGL.GLU import *

#class for a 3d point
class createpoint:
    def __init__(self,p,c=(1,0,0)):
        self.point_size=0.5
        self.color=c
        self.x=p[0]
        self.y=p[1]
        self.z=p[2]
      
    def glvertex(self):
        glVertex3f(self.x,self.y,self.z)

#class for a 3d face on a model
class createtriangle:
    points=None
    normal=None

    def __init__(self,p1,p2,p3,n=None):
        #3 points of the triangle
        self.points=createpoint(p1),createpoint(p2),createpoint(p3)
      
        #triangles normal
        self.normal=createpoint(self.calculate_normal(self.points[0],self.points[1],self.points[2]))#(0,1,0)#
  
    #calculate vector / edge
    def calculate_vector(self,p1,p2):
        return -p1.x+p2.x,-p1.y+p2.y,-p1.z+p2.z
      
    def calculate_normal(self,p1,p2,p3):
        a=self.calculate_vector(p3,p2)
        b=self.calculate_vector(p3,p1)
        #calculate the cross product returns a vector
        return self.cross_product(a,b)    
  
    def cross_product(self,p1,p2):
        return (p1[1]*p2[2]-p2[1]*p1[2]) , (p1[2]*p2[0])-(p2[2]*p1[0]) , (p1[0]*p2[1])-(p2[0]*p1[1])

class loader:
    model=[]

    #return the faces of the triangles
    def get_triangles(self):
        if self.model:
            for face in self.model:
                yield face

    #draw the models faces
    def draw(self):
        glBegin(GL_TRIANGLES)
        for tri in self.get_triangles():
            glNormal3f(tri.normal.x,tri.normal.y,tri.normal.z)
            glVertex3f(tri.points[0].x,tri.points[0].y,tri.points[0].z)
            glVertex3f(tri.points[1].x,tri.points[1].y,tri.points[1].z)
            glVertex3f(tri.points[2].x,tri.points[2].y,tri.points[2].z)
        glEnd()

    def mesh(self):
        verts = []
        faces = []
        for i, tri in enumerate(self.get_triangles()):
            faces.append([0 + 3*i, 1 + 3*i, 2 + 3*i])
            verts.append([tri.points[1].x, tri.points[1].y, tri.points[1].z])
            verts.append([tri.points[0].x, tri.points[0].y, tri.points[0].z])
            verts.append([tri.points[2].x, tri.points[2].y, tri.points[2].z])
        return gl.GLMeshItem(vertexes=numpy.array(verts), faces=numpy.array(faces), shader='shaded')

    #load stl file detects if the file is a text file or binary file
    def load_stl(self,filename):
        #read start of file to determine if its a binay stl file or a ascii stl file
        fp=open(filename,'rb')
        h=fp.read(80)
        type=h[0:5]
        fp.close()

        if type=='solid':
            print( "reading text file"+str(filename) )
            self.load_text_stl(filename)
        else:
            print( "reading binary stl file "+str(filename,) )
            self.load_binary_stl(filename)

    #read text stl match keywords to grab the points to build the model
    def load_text_stl(self,filename):
        fp=open(filename,'r')

        for line in fp.readlines():
            words=line.split()
            if len(words)>0:
                if words[0]=='solid':
                    self.name=words[1]

                if words[0]=='facet':
                    center=[0.0,0.0,0.0]
                    triangle=[]
                    normal=(eval(words[2]),eval(words[3]),eval(words[4]))

                if words[0]=='vertex':
                    triangle.append((eval(words[1]),eval(words[2]),eval(words[3])))


                if words[0]=='endloop':
                    #make sure we got the correct number of values before storing
                    if len(triangle)==3:
                        self.model.append(createtriangle(triangle[0],triangle[1],triangle[2],normal))
        fp.close()

    #load binary stl file check wikipedia for the binary layout of the file
    #we use the struct library to read in and convert binary data into a format we can use
    def load_binary_stl(self,filename):
        fp=open(filename,'rb')
        h=fp.read(80)

        l=struct.unpack('I',fp.read(4))[0]
        count=0
        while True:
            try:
                p=fp.read(12)
                if len(p)==12:
                    n=struct.unpack('f',p[0:4])[0],struct.unpack('f',p[4:8])[0],struct.unpack('f',p[8:12])[0]

                p=fp.read(12)
                if len(p)==12:
                    p1=struct.unpack('f',p[0:4])[0],struct.unpack('f',p[4:8])[0],struct.unpack('f',p[8:12])[0]

                p=fp.read(12)
                if len(p)==12:
                    p2=struct.unpack('f',p[0:4])[0],struct.unpack('f',p[4:8])[0],struct.unpack('f',p[8:12])[0]

                p=fp.read(12)
                if len(p)==12:
                    p3=struct.unpack('f',p[0:4])[0],struct.unpack('f',p[4:8])[0],struct.unpack('f',p[8:12])[0]

                new_tri=(n,p1,p2,p3)

                if len(new_tri)==4:
                    tri=createtriangle(p1,p2,p3,n)
                    self.model.append(tri)
                count+=1
                fp.read(2)

                if len(p)==0:
                    break
            except EOFError:
                break
        fp.close()
################################################################################



def rad2deg(rad):
    return 360*rad/2/math.pi

class InputThread(QtCore.QThread):
    def __init__(self, mesh_item, args):
        self.mesh_item = mesh_item
        super(InputThread, self).__init__()

    def run(self):
        while True:
            line = sys.stdin.readline()
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
        tr.scale(0.02, 0.02, 0.02)

        self.mesh_item.setTransform(tr)



def main():
    parser = argparse.ArgumentParser(description='Plot 3D Orientation',
                    epilog='reading from stdin, format: q0,q1,q2,q3[,x,y,z]')
    parser.add_argument('--title', default='3D Orientation\n',
                       help='set window title')
    parser.add_argument('--stl', default=None)
    args = parser.parse_args()


    app = QtGui.QApplication([])
    w = gl.GLViewWidget()
    w.show()
    w.setWindowTitle(args.title)
    w.pan(0, 0, 1)
    w.setCameraPosition(distance=20)

    ax=gl.GLAxisItem()
    ax.setSize(3,3,3)
    ax.translate(0, 0, 1)
    w.addItem(ax)

    g = gl.GLGridItem()
    g.scale(2,2,1)
    g.translate(0, 0, 0)
    w.addItem(g)

    if args.stl == None:
        mesh = cubeMeshItem()
    else:
        stlmodel = loader()
        stlmodel.load_stl(args.stl)
        # stlmodel.draw()
        mesh = stlmodel.mesh()
        # mesh.setColor(pg.mkColor('r'))

    mesh.translate(0,0,1)
    w.addItem(mesh)

    thread = InputThread(mesh, args)
    thread.finished.connect(app.exit)
    thread.start()

    if (sys.flags.interactive != 1) or not hasattr(QtCore, 'PYQT_VERSION'):
        QtGui.QApplication.instance().exec_()


if __name__ == '__main__':
    main()
