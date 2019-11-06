# AP_alfoil_list: (all coordinates in a BASE)
# MoveJ:
#   Home_position (x, y, z, Rx, Ry, Rz)
#   base_point(649.20mm; -785.40mm; -45.80mm; 2.5015, -1.8669, 0.0252)
#   Move joints: 120.21, -33.89, 82.34, -139.75, -91.19, -76.43

# MoveL: (25mm/s, 48mm/s^2)
#   grab_point(649.20mm; -785.40mm; -302.20mm; 2.5013, -1.8669, 0.0252)
#   Move joints: 120.21, -14.00, 73.18, -150.49, -91.19, -76.43

# wait = 1 sec
# Set DO[2] = On
# wait = 1 sec

# MoveL: (15mm/s, 48mm/s^2)
#   ready_point(649.27mm; -785.29mm; -277.48mm; 2.5016, -1.8669, 0.0250)
#   Move joints: 120.21, -16.00, 74.54, -149.80, -91.19, -76.54

# MoveP: (25mm/s, 25mm/s^2, Blend radipus = 15mm)
#   transport_point(128.10mm; -891.72mm; -236.98mm; 2.5014, -1.8671, 0.0252)
#   Move joints: 87.51, -24.53, 93.02, -159.96, -90.29, -109.23

# MoveL: (15mm/s, 48mm/s^2)
#   ready_point(128.10mm; -891.72mm; -294.88mm; 2.5014, -1.8671, 0.0252)
#   Move joints: 87.51, -19.45, 90.13, -162.16, -90.29, -109.23

# wait = 1 sec
# Set DO[2] = On
# wait = 1 sec

# MoveL: (15mm/s, 48mm/s^2)
# transport_point

import time
import logging
import math
import socket
import urx
import math3d as m3d      #the library, used by urx to represent transformations

def check_connection():
    while 1:
        # help(robot)  # Let's list all URX functions
        #robot.get_pos()  # let's get robot's tool tcp position (x,y,z)
        #trans = robot.get_pose()  # get current transformation matrix (tool to base)
        #angle = robot.get_orientation()
        #real = robot.get_realtime_monitor()

        tcp_position = str(robot.get_pos())
        base_position = str(robot.get_pose())
        an_orientation = str(robot.get_orientation())
        a_real = str(robot.get_realtime_monitor())
        print(a_real)

# ТУТ ПРОБЛЕМА В ТОМ, ЧТО НЕ ПОНИМАЮ КАК ЗАДАТЬ КООРДИНАТЫ В БАЗОВОЙ СИСТЕМЕ КООРДИНАТ:
def transportation():
    # step 1. Go to a Home_position:
    # syntax of this command:
    robot.movej((0.64920, -0.78540, -0.04580, 2.5015, -1.8669, 0.0252), 0.1, 0.05)

    # step 2. разрешение на захват листа - подумай как лучше организовать!
    # для теста: robot.stopl() or sleep() or robot._wait_for_move()
    # а в основной программе сделать через камеру: если есть изображение с камеры и
    # это изображение листок с полосами
    # robot.stopj(0.5)

    # step 3.
    robot.movel(0.64920, -0.78540, -0.30220, 2.5013, -1.8669, 0.0252)
    #  не ясно где прописывать параметры движения: 25мм/с и 48мм/с^2

    # robot.stop(0.5)
    robot.set_digital_out(2, True)
    # robot.stop(0.5)

    robot.movel(0.64927, -0.78529, -0.27748, 2.5016, -1.8669, 0.0250) # ready_point
    robot.movel(0.12810, -0.89172, -0.23698, 2.5014, -1.8671, 0.0252) # transport_point1
    # robot.movel() # transport_point2

    # step 4 First if-else (Minimisation of angle error)

    # step 5 First if-else (Minimisation of transport error)

    # step 6
    robot.movel(0.64927, -0.78529, -0.27748, 2.5016, -1.8669, 0.0250) # ready_point

    # robot.stop(0.5)
    robot.set_digital_out(0, False)
    #robot.stop(0.5)

# Женин пример:
def Z_transportation():
    mytcp = m3d.Transform()  # create a matrix for our tool tcp
    mytcp.pos.z = 0.18
    mytcp.orient.rotate_zb(3.14 / 3)
    robot.set_tcp(mytcp)
    time.sleep(0.2)

    # get current pose, transform it and move robot to new pose
    trans = robot.get_pose()  # get current transformation matrix (tool to base)
    trans.pos.z += 0.3
    trans.orient.rotate_yb(3.14 / 2)
    robot.set_pose(trans, acc=0.5, vel=0.2)  # apply the new pose

    # or only work with orientation part
    o = robot.get_orientation()
    o.rotate_yb(3.14)
    robot.set_orientation(o)


# Main code here:
# step 0 Connection to the robot:
robot = urx.Robot("10.42.0.111", use_rt=True)
# Movement Function:
# transportation()
Z_transportation()










