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
    frame = cv2.GaussianBlur(frame, (5, 5), cv2.BORDER_DEFAULT)
    #cv2.blur(frame,5)
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
    if (Ax != Dx) and (Bx != Cx):
        Ex = min(Ax, Dx) + abs(Dx - Ax) / 2  # потому, что наклон не предсказуем
        Ey = Ay + ((Dy - Ay) / 2)
        Fx = min(Bx, Cx) + abs(Bx - Cx) / 2  # потому, что наклон не предсказуем
        Fy = By + ((Cy - By) / 2)
        print("EF general case: ", Ex, Ey, Fx, Fy)

    # left - equal  case:
    elif (Ax == Dx) and (Bx != Cx):
        Ex = Dx
        Ey = Ay + ((Dy - Ay) / 2)
        Fx = min(Bx, Cx) + abs(Bx - Cx) / 2  # потому, что наклон не предсказуем
        Fy = By + ((Cy - By) / 2)
        print("EF left-equal case: ", Ex, Ey, Fx, Fy)

    # right - equal Case:
    elif (Ax != Dx) and (Bx == Cx):
        Ex = min(Ax, Dx) + abs(Dx - Ax) / 2  # потому, что наклон не предсказуем
        Ey = Ay + ((Dy - Ay) / 2)
        Fx = Cx
        Fy = By + ((Cy - By) / 2)
        print("EF right - equal case: ", Ex, Ey, Fx, Fy)

    # ideal case:
    elif (Ax == Dx) and (Bx == Cx):
        Ex = Dx
        Ey = Ay + ((Dy - Ay) / 2)
        Fx = Cx
        Fy = By + ((Cy - By) / 2)
        print("EF ideal case: ", Ex, Ey, Fx, Fy)

    # unreal case:
    else:
        print("EF Interesting Exception")
        Ex, Ey, Fx, Fy = 1, 2, 3, 4
    return int(Ex), int(Ey), int(Fx), int(Fy)

def middle_line_vertical(Ax,Ay,Bx,By,Cx,Cy,Dx,Dy):
    """
    Координыты точек G и H:
    G - середина AD, F - середина BC
    От прошлой программы отличается тем, что:
    все В и D меняю местами, а также меняю местами x и y
    """
    # general case:
    if (Ay != By) and (Dy != Cy):
        Gy = min(Ay, By) + abs(By - Ay) / 2  # потому, что наклон не предсказуем
        Gx = Ax + ((Bx - Ax) / 2)
        Hy = min(Dy, Cy) + abs(Dy - Cy) / 2  # потому, что наклон не предсказуем
        Hx = Dx + ((Cx - Dx) / 2)
        print("GH general case: ", Gx, Gy, Hx, Hy)

    # left - equal  case:
    elif (Ay == By) and (Dy != Cy):
        Gy = By
        Gx = Ax + ((Bx - Ax) / 2)
        Hy = min(Dy, Cy) + abs(Dy - Cy) / 2  # потому, что наклон не предсказуем
        Hx = Dx + ((Cx - Dx) / 2)
        print("GH left-equal case: ", Gx, Gy, Hy, Hx)

    # right - equal Case:
    elif (Ay != By) and (Dy == Cy):
        Gy = min(Ay, By) + abs(By - Ay) / 2  # потому, что наклон не предсказуем
        Gx = Ax + ((Bx - Ax) / 2)
        Hy = Cy
        Hx = Dx + ((Cx - Dx) / 2)
        print("GH right - equal case: ", Gx, Gy, Hx, Hy)

    # ideal case:
    elif (Ax == Bx) and (Dx == Cx):
        Gx = Bx
        Gy = Ay + ((Dy - Ay) / 2)
        Hx = Cx
        Hy = Dy + ((Cy - Dy) / 2)
        print("GH ideal case: ", Gx, Gy, Hx, Hy)

    # unreal case:
    else:
        print("GH Interesting Exception")
        Gx, Gy, Hx, Hy = 1, 2, 3, 4
    return int(Gx), int(Gy), int(Hx), int(Hy)

def center_finding(Ex, Ey, Fx, Fy, Gx, Gy, Hx, Hy):
    """
    Solve this problem through common line equation
    1. Write them for EF & GH
    2. Express x through y from the EF
    3. x = x, so equate left & right
    4. find y
    5. find x
    :return: Ox, Oy
    """
    Kfe = (Fx - Ex) / (Fy - Ey)
    Khg = (Hx - Gx) / (Hy - Gy)

    Oy = 1 / (Kfe - Khg) * (Ey*Kfe - Ex - Gy*Khg + Gx)

    Ox = Oy * Kfe - Ey * Kfe + Ex

    print(Ox, Oy)

    return int(Ox), int(Oy)

# -----------------------------


# Our case:
Ax, Ay = 10, 70
Bx, By = 230, 75
Cx, Cy = 230, 178
Dx, Dy = 10, 172

color_purple = (255, 0, 255)

# Upload picture
#cadr = cv2.imread(r'C:\Users\a.pimenov\PycharmProjects\UR_Robotics\Source_Vision\capture.jpg')#, cv2.IMREAD_GRAYSCALE)

# Take picture from web camera:
capture = cv2.VideoCapture(0)

while True:
    ret, frame = capture.read()  # делаем снимок в случае захвата с камеры
    cadr = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  #RGB)   #HSV)   #GRAY)

    # ROI
    frame = cadr[160:480, 120:360]

    # For blink materials is it necessary to use histogram redistribution
    small_frame_left = frame[60:180, 0:20]
    small_frame_right = frame[60:180, 220:240]

    # Blurring - it is need for reality
    blur_left_frame = cv2.GaussianBlur(small_frame_left, (3, 3), cv2.BORDER_DEFAULT)
    blur_right_frame = cv2.GaussianBlur(small_frame_right, (3,3),cv2.BORDER_DEFAULT)

    # Binarisation
    left_frame = cv2.adaptiveThreshold(blur_left_frame, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
    right_frame = cv2.adaptiveThreshold(blur_right_frame, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)

    # Finalisation:
    frame[60:180, 0:20] = left_frame
    frame[60:180, 220:240] = right_frame



    Ex, Ey, Fx, Fy = middle_line(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)
    cv2.line(frame, (Ex, Ey), (Fx, Fy), color_purple, thickness=1, lineType=8, shift=0)

    Gx, Gy, Hx, Hy = middle_line_vertical(Ax, Ay, Bx, By, Cx, Cy, Dx, Dy)
    cv2.line(frame, (Gx, Gy), (Hx, Hy), color_purple, thickness=1, lineType=8, shift=0)

    Ox, Oy = center_finding(Ex, Ey, Fx, Fy, Gx, Gy, Hx, Hy)
    cv2.circle(frame, (Ox, Oy), 2, (0, 255, 0), 4)

    cv2.imshow("ROI", frame)

    if cv2.waitKey(1) % 0xFF == ord('q'):
        cv2.imwrite("working.jpg", frame)
        break

print('End')
