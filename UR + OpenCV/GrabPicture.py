import os
import cv2
import numpy as np
from matplotlib import pyplot as plot

def webCamera():
    cap = cv2.VideoCapture(0)
    while(True):
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        cv2.imshow('frame', gray)

        print("angle is ")
        print("shift is ")
        if cv2.waitKey(1) % 0xFF == ord('q'):
            cv2.imwrite(r'C:\Users\a.pimenov\PycharmProjects\UR_Robotics\Source_Vision\capture.jpg', gray)
            break

    cap.release()
    cv2.destroyAllWindows()
    return 0

def Line_Boarder_Estimation(cadr):
    # ROI
    frame = cadr[160:480, 120:360]
    cv2.imshow("ROI", frame)
    #cv2.imwrite('ROI.png', frame)

    # Blurring - it is need for reality

    # Binarisation

    # Detecting the points
    return frame

def middle_line(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy):
    """
    Идея моего алгоритма опирается на рассмотрение прямоугольника: ABCD
    В каждом кадре:
    A - левый верхний угол
    B - правый верхний угол
    C - правый нижний угол
    D - левый нижний угол
    :return:
    Координыты точек E и F:
    E - середина AD, F - середина BC
    """
    # general case:
    if (Ay != Dy) and (By != Cy):
        Ey = (Dy - Ay) / 2
        Ex = abs(Dx - Ax) / 2  # потому, что наклон не предсказуем
        Fy = (Cy - By) / 2
        Fx = abs(Cx - Bx) / 2  # потому, что наклон не предсказуем
        print("general case: ", Ex, Ey, Fx, Fy)

    # left - equal  case:
    elif (Ay == By) and (Dy != Cy):
        Ey = Ay
        Ex = abs(Dx - Ax) / 2  # потому, что наклон не предсказуем
        Fy = (Cy - By) / 2
        Fx = abs(Cx - Bx) / 2  # потому, что наклон не предсказуем
        print("left-equal case: ", Ex, Ey, Fx, Fy)

    # right - equal Case:
    elif (Ay != By) and (Dy != Cy):
        Ey = (Dy - Ay) / 2
        Ex = abs(Dx - Ax) / 2  # потому, что наклон не предсказуем
        Fy = Cy
        Fx = abs(Cx - Bx) / 2  # потому, что наклон не предсказуем
        print("right - equal case: ", Ex, Ey, Fx, Fy)

    # ideal case:
    elif (Ay != By) and (Dy != Cy):
        Ey = Dy
        Ex = abs(Dx - Ax) / 2  # потому, что наклон не предсказуем
        Fy = Cy
        Fx = abs(Cx - Bx) / 2  # потому, что наклон не предсказуем
        print("ideal case: ", Ex, Ey, Fx, Fy)

    # unreal case:
    else:
        print("Interesting Exception")
        Ex, Ey, Fx, Fy = 1, 2, 3, 4
    return int(Ex), int(Ey), int(Fx), int(Fy)
# -----------------------------


Ax, Ay = 0, 70
Bx, By = 239, 75
Cx, Cy = 0, 172
Dx, Dy = 239, 178
color_purple = (255,0,255)

# Upload picture
cadr = cv2.imread(r'C:\Users\a.pimenov\PycharmProjects\UR_Robotics\Source_Vision\capture.jpg')#, cv2.IMREAD_GRAYSCALE)

while (True):

    # ROI
    frame = cadr[160:480, 120:360]
    cv2.imshow("ROI", frame)

    Ex, Ey, Fx, Fy = middle_line(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)
    cv2.line(frame, (Ex, Ey), (Fx, Fy), color_purple, thickness=1, lineType=8, shift=0)

    if cv2.waitKey(1) % 0xFF == ord('q'):
        break

print('End')
