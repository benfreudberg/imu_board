import serial
import time
import struct
from math import acos as acos

from matplotlib import pyplot as plt
#from pylab import *
from numpy import array, mgrid, pi, ones, hstack, dot, linspace, zeros, cross, eye
import mpl_toolkits.mplot3d.axes3d as p3
import matplotlib.animation as animation

#the inverse of a quaternion 
def quatInv(q): 
    return array([q[0], -q[1], -q[2], -q[3]])

#multiplication of a quaternion with another quaternion    
def quatMult(p, q):
    p0 = p[0]
    pv = p[1:4]
    q0 = q[0]
    qv = q[1:4]
    new0 = p0*q0 - dot(pv,qv)
    newv = p0*qv + q0*pv + cross(pv,qv)
    return array([new0, newv[0], newv[1], newv[2]])

def vectorRotation(v,q):
    v = array([0,v[0],v[1],v[2]])
    return quatMult(quatMult(q,v),quatInv(q))    

#plot axes
def plotFrame(R,ax,color,label):
    ax.plot([0,R[0,0]], [0,R[1,0]], [0,R[2,0]],color)
    ax.plot([0,R[0,1]], [0,R[1,1]], [0,R[2,1]],color)
    ax.plot([0,R[0,2]], [0,R[1,2]], [0,R[2,2]],color)
    ax.text(R[0,0],R[1,0],R[2,0],"x_"+label)
    ax.text(R[0,1],R[1,1],R[2,1],"y_"+label)
    ax.text(R[0,2],R[1,2],R[2,2],"z_"+label)

def animate(num, lines, ser):
#def animate(num, line0, line1, line2, ser):
    variables = 4
    bytes_per_var = 4;
    sets_per_message = 1;
    read_val = ser.read(size=variables*bytes_per_var*sets_per_message)
    try:
        numS0 = read_val[bytes_per_var*(0*variables+0):bytes_per_var*(0*variables+1)]
        fpn0 = struct.unpack('f', numS0)
        
        numS1 = read_val[bytes_per_var*(0*variables+1):bytes_per_var*(0*variables+2)]
        fpn1 = struct.unpack('f', numS1)
        
        numS2 = read_val[bytes_per_var*(0*variables+2):bytes_per_var*(0*variables+3)]
        fpn2 = struct.unpack('f', numS2)
        
        numS3 = read_val[bytes_per_var*(0*variables+3):bytes_per_var*(0*variables+4)]
        fpn3 = struct.unpack('f', numS3)
            

        q = array([fpn0[0], fpn1[0], fpn2[0], fpn3[0]])
        R = eye(3)
        R0 = eye(3)
        R[0] = vectorRotation(R0[0],q)[1:4]
        R[1] = vectorRotation(R0[1],q)[1:4]
        R[2] = vectorRotation(R0[2],q)[1:4]
        
        data0 = array([[0, R[0,0]],
                 [0, R[0,1]],
                 [0, R[0,2]]])
        
        data1 = array([[0, R[1,0]],
                 [0, R[1,1]],
                 [0, R[1,2]]])
        
        data2 = array([[0, R[2,0]],
                 [0, R[2,1]],
                 [0, R[2,2]]])
    
        for i in range(0,3):
        
            lines[0].set_data(data0[0:2, :2])
            lines[0].set_3d_properties(data0[2, :2])
            
            lines[1].set_data(data1[0:2, :2])
            lines[1].set_3d_properties(data1[2, :2])
            
            lines[2].set_data(data2[0:2, :2])
            lines[2].set_3d_properties(data2[2, :2])
            

        
#        line0.set_data(data0[0:2, :2])
#        line0.set_3d_properties(data0[2, :2])
#        
#        line1.set_data(data1[0:2, :2])
#        line1.set_3d_properties(data1[2, :2])
#        
#        line2.set_data(data2[0:2, :2])
#        line2.set_3d_properties(data2[2, :2])
        
#        for i in range(0,3):
#            line0data[0][i] = R[0][i]
#            line1data[0][i] = R[1][i]
#            line2data[0][i] = R[2][i]

        
        

#        plotFrame(R,ax,"-g","d")
        # update the plot
#        fig.canvas.draw()

            
        # when time's up, kill the collect+plot loop
        #if timepoints[-1] > duration: run=False
    
    # if the try statement throws an error, just do nothing
    except: pass
    return lines

plt.close('all')
#plt.run_line_magic('matplotlib','qt')

#ser = serial.Serial('COM3', 500000, timeout=1)
ser = serial.Serial('COM16', 460800, timeout=1)

ser.close()
time.sleep(1)
ser.open()
time.sleep(1)


#plt.ion()


fig0 = plt.figure(figsize=plt.figaspect(1.0)*2,facecolor="white")
#fig0 = plt.figure()
ax0 = p3.Axes3D(fig0)
#ax0 = fig0.add_subplot(111, projection='3d')
#ax0.view_init(elev=30., azim=45)
#ax0.set_xlim((-1,1))
#ax0.set_ylim((-1,1))
#ax0.set_zlim((-1,1))
#
R0 = eye(3)
# R1 = eye(3)
#
#plotFrame(R,ax0,"-g","d")

#line0, = ax0.plot([0,R[0,0]], [0,R[1,0]], [0,R[2,0]],"-g")
#line1, = ax0.plot([0,R[0,1]], [0,R[1,1]], [0,R[2,1]],"-r")
#line2, = ax0.plot([0,R[0,2]], [0,R[1,2]], [0,R[2,2]],"-b")

#lines = [ax0.plot([0,R[0,0]], [0,R[1,0]], [0,R[2,0]],"-g")[0], ax0.plot([0,R[0,1]], [0,R[1,1]], [0,R[2,1]],"-r")[0], ax0.plot([0,R[0,2]], [0,R[1,2]], [0,R[2,2]],"-b")[0]]
#texts = [ax0.text(R[0,0],R[1,0],R[2,0],"x_"+"d")[0], ax0.text(R[0,1],R[1,1],R[2,1],"y_"+"d")[0], ax0.text(R[0,2],R[1,2],R[2,2],"z_"+"d")[0]]
#lines = [ax0.plot([0,R[i,0]], [0,R[i,1]], [0,R[i,2]],"-g")[0] for i in range(0,3)]
#ax0.text(R[0,0],R[1,0],R[2,0],"x_"+"d")
#ax0.text(R[0,1],R[1,1],R[2,1],"y_"+"d")
#ax0.text(R[0,2],R[1,2],R[2,2],"z_"+"d")

ax0.view_init(elev=30., azim=225)
ax0.set_xlim((-1,1))
ax0.set_ylim((-1,1))
ax0.set_zlim((-1,1))
ser.flushInput()
#ani = animation.FuncAnimation(fig0,animate,fargs=(lines, ser),interval=100)
#ani = animation.FuncAnimation(fig0,animate,fargs=(line0,line1,line2, ser),interval=100)
#plt.show()
variables = 4
bytes_per_var = 4
sets_per_message = 1
#i=0
#q0_save = [0]*1000
run = True
while run:

    read_val = ser.read(size=variables*bytes_per_var*sets_per_message)
    try:
        numS0 = read_val[bytes_per_var*(0*variables+0):bytes_per_var*(0*variables+1)]
        fpn0 = struct.unpack('f', numS0)
        
        numS1 = read_val[bytes_per_var*(0*variables+1):bytes_per_var*(0*variables+2)]
        fpn1 = struct.unpack('f', numS1)
        
        numS2 = read_val[bytes_per_var*(0*variables+2):bytes_per_var*(0*variables+3)]
        fpn2 = struct.unpack('f', numS2)
        
        numS3 = read_val[bytes_per_var*(0*variables+3):bytes_per_var*(0*variables+4)]
        fpn3 = struct.unpack('f', numS3)
        
        # numS4 = read_val[bytes_per_var*(0*variables+4):bytes_per_var*(0*variables+5)]
        # fpn4 = struct.unpack('f', numS4)
        
        # numS5 = read_val[bytes_per_var*(0*variables+5):bytes_per_var*(0*variables+6)]
        # fpn5 = struct.unpack('f', numS5)
        
        # numS6 = read_val[bytes_per_var*(0*variables+6):bytes_per_var*(0*variables+7)]
        # fpn6 = struct.unpack('f', numS6)
        
        # numS7 = read_val[bytes_per_var*(0*variables+7):bytes_per_var*(0*variables+8)]
        # fpn7 = struct.unpack('f', numS7)
            

        q0 = array([fpn0[0], fpn1[0], fpn2[0], fpn3[0]])
        # q1 = array([fpn4[0], fpn5[0], fpn6[0], fpn7[0]])
        RR = eye(3)
        R0[0] = vectorRotation(RR[0],q0)[1:4]
        R0[1] = vectorRotation(RR[1],q0)[1:4]
        R0[2] = vectorRotation(RR[2],q0)[1:4]
        
        # R1[0] = vectorRotation(RR[0],q1)[1:4]
        # R1[1] = vectorRotation(RR[1],q1)[1:4]
        # R1[2] = vectorRotation(RR[2],q1)[1:4]
        
#        for i in range(0,3):
#            line0data[0][i] = R[0][i]
#            line1data[0][i] = R[1][i]
#            line2data[0][i] = R[2][i]
        ax0.clear()
        ax0.view_init(elev=30., azim=225)
        ax0.set_xlim((-1,1))
        ax0.set_ylim((-1,1))
        ax0.set_zlim((-1,1))
#        plotFrame(R,ax0,"-g","d")
        ax0.plot([0,R0[0,0]], [0,R0[0,1]], [0,R0[0,2]],"-g")
        ax0.plot([0,R0[1,0]], [0,R0[1,1]], [0,R0[1,2]],"-r")
        ax0.plot([0,R0[2,0]], [0,R0[2,1]], [0,R0[2,2]],"-b")
        ax0.text(R0[0,0],R0[0,1],R0[0,2],"x_s")
        ax0.text(R0[1,0],R0[1,1],R0[1,2],"y_s")
        ax0.text(R0[2,0],R0[2,1],R0[2,2],"z_s")
        
        # ax0.plot([0,R1[0,0]], [0,R1[0,1]], [0,R1[0,2]],"--g")
        # ax0.plot([0,R1[1,0]], [0,R1[1,1]], [0,R1[1,2]],"--r")
        # ax0.plot([0,R1[2,0]], [0,R1[2,1]], [0,R1[2,2]],"--b")
        # ax0.text(R1[0,0],R1[0,1],R1[0,2],"x_b")
        # ax0.text(R1[1,0],R1[1,1],R1[1,2],"y_b")
        # ax0.text(R1[2,0],R1[2,1],R1[2,2],"z_b")
        
#        data0 = array([[0, R[0,0]],
#                 [0, R[0,1]],
#                 [0, R[0,2]]])
#        
#        data1 = array([[0, R[1,0]],
#                 [0, R[1,1]],
#                 [0, R[1,2]]])
#        
#        data2 = array([[0, R[2,0]],
#                 [0, R[2,1]],
#                 [0, R[2,2]]])
#    
#        for i in range(0,3):
#        
#            lines[0].set_data(data0[0:2, :2])
#            lines[0].set_3d_properties(data0[2, :2])
#            
#            lines[1].set_data(data1[0:2, :2])
#            lines[1].set_3d_properties(data1[2, :2])
#            
#            lines[2].set_data(data2[0:2, :2])
#            lines[2].set_3d_properties(data2[2, :2])
#        

    
        # update the plot
        fig0.canvas.draw()

#        i = i+1
#        if (i==1000) :
#            run = False
#        q0_save[i] = q0
        # when time's up, kill the collect+plot loop
        #if timepoints[-1] > duration: run=False
    
    # if the try statement throws an error, just do nothing
    except:
        run = False
    

# http://matplotlib.org/users/text_props.html


ser.close()
print('exiting')
#angs = [0]*999
#for i in range(1,1000):
#    angs[i-1] = acos(q0_save[i][0])*360/pi