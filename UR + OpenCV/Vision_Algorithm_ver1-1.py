# CV_Algorithm:
# 0. Installed camera drivers, set up camera in the right position, get 10 pics - DONE
# 1. Подготовительное преобразование изображения в сканирующих окнах - DONE
# 2. Формирование полигонов и нахождение центральных точек этих полигонов: A,B,C,D
# 3. В построенном ABCD: решить задачу нахождения угла поворота
# 4. В построенном ABCD: решить задачу нахождения линейного смещения
# 5. В случае AB: решить задачу нахождения угла поворота
# 6. В случае AB: решить задачу нахождения линейного смещения

import os
import cv2
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import math

# Step 0:

def getpicture(source):
    cv2.namedWindow("picture_cam")  # creating main window
    camera_cap = cv2.VideoCapture(source)

    # Check if the webcam is opened correctly
    if not camera_cap.isOpened():
        raise IOError("Cannot open webcam")

    while True:
        return_value, frame = camera_cap.read(source)

        cv2.imshow("picture_cam", frame)

        #print("We take a picture of you, check the folder")


        if cv2.waitKey(1) % 0xFF == ord('q'):
            cv2.imwrite('Line_Detected_NewCAM.jpg', frame)
            break

    camera_cap.release()
    cv2.destroyAllWindows()

def plot_pixel_analyzer():
    nframe = cv2.imread('Line_Detected.jpg')
    nframe = cv2.cvtColor(nframe, cv2.COLOR_BGR2RGB)
    plt.imshow(nframe)
    plt.show()

# Step 1:

def nothing(x):
    pass

def hsv_color_filter_callibration(source):
    '''
    Эта функция нужна для ручной настройки параметров фильтра
    :param source:
    выбор источника с которым работает функция, обычно это номер камеры:
    0 - устанволенная в ноутбук
    1 - внешняя веб-камера по USB
    2 - вторая внешняя по USB (если подключено больше одной веб - камеры)
    3 - третья внешняя по USB (если подключено больше двух веб - камер)
    8 - происходит работа с изображением, которое по умолчанию сохранено:
        как 'Line_Detected_CAM.jpg' в основной папке проекта
    :return:
    Возвращает значение коэффициентов фильтра:
    h_min = np.array((h1, s1, v1), np.uint8)
    h_max = np.array((h2, s2, v2), np.uint8)

    Которые могут использоваться в дальнейшем для формировании маски

    '''
    cv2.namedWindow('result')  # creating main window
    cv2.namedWindow('settings')  # for callibration

    if source == 8:
        cap = cv2.imread('Line_Detected_CAM.jpg')
    else:
        cap = cv2.VideoCapture(source)
    # создаем 6 бегунков:
    cv2.createTrackbar('h1', 'settings', 0, 255, nothing)
    cv2.createTrackbar('s1', 'settings', 0, 255, nothing)
    cv2.createTrackbar('v1', 'settings', 0, 255, nothing)
    cv2.createTrackbar('h2', 'settings', 255, 255, nothing)
    cv2.createTrackbar('s2', 'settings', 255, 255, nothing)
    cv2.createTrackbar('v2', 'settings', 255, 255, nothing)
    crange = [0, 0, 0, 0, 0, 0]

    while True:
        if source == 8:
            hsv = cv2.cvtColor(cap, cv2.COLOR_BGR2HSV)
        else:
            flag, img = cap.read(source)
            hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)


        # считываем значения бегунков
        h1 = cv2.getTrackbarPos('h1', 'settings')
        s1 = cv2.getTrackbarPos('s1', 'settings')
        v1 = cv2.getTrackbarPos('v1', 'settings')
        h2 = cv2.getTrackbarPos('h2', 'settings')
        s2 = cv2.getTrackbarPos('s2', 'settings')
        v2 = cv2.getTrackbarPos('v2', 'settings')

        # формируем начальный и конечный цвет фильтра
        h_min = np.array((h1, s1, v1), np.uint8)
        h_max = np.array((h2, s2, v2), np.uint8)

        #  накладываем фильтр на кадр в модели HSV
        thresh = cv2.inRange(hsv, h_min, h_max)

        cv2.imshow('result', thresh)

        ch = cv2.waitKey(5)
        if ch == 27:
            break

    if source == 8:
        cv2.destroyAllWindows()
        return h_min, h_max
    else:
        cap.release()
        cv2.destroyAllWindows()
        return h_min, h_max

def img_preparation(source, hsv_min, hsv_max, output_parametr):
    '''
    Эта функция формирует скнаирующие окна и подгатавливает изображения в них
    :param source: (то же что и для hsv_color_filter_callibration()
    выбор источника с которым работает функция, обычно это номер камеры:
    0 - устанволенная в ноутбук
    1 - внешняя веб-камера по USB
    2 - вторая внешняя по USB (если подключено больше одной веб - камеры)
    3 - третья внешняя по USB (если подключено больше двух веб - камер)
    8 - происходит работа с изображением, которое по умолчанию сохранено:
        как 'Line_Detected_CAM.jpg' в основной папке проекта
    :param hsv_min: матрица параметров фильтра: h1, s1, v1
    :param hsv_max: матрица параметров фильтра: h2, s2, v3
    :param output_parametr: Параметр настройки выхода функции:
    0 - на выходе формируется только массив(MAT), который можно далее использовать для анализа
    1 - сохраняется изображение в папка проекта: 'IMG_Prepare.jpg'

    :return: Зависит от output_parametr
    0 - Только матрица
    1 - Только картинка с подготовленными к дальнейшему анализу сканирующими окнами
    '''

    if source == 8:
        cap = cv2.imread('Line_Detected_CAM.jpg')
    else:
        cap = cv2.VideoCapture(source)

    while True:
        if source == 8:
            base_frame = cap
        else:
            ret, base_frame = cap.read(source)

        frame = base_frame[30:150, 220:380]
        # It is useful for calculations to work with small areas:
        top_frame = frame[30:60, 0:150]
        bottom_frame = frame[90:120, 0:150]
        #print('top_frame: ', np.shape(top_frame))

        # Blurring - it is need for reality
        # NB:  cv2.BORDER_DEFAULT vs cv2.BORDER_CONSTANT
        top_frame_blur = cv2.GaussianBlur(top_frame, (1, 5), cv2.BORDER_CONSTANT)
        bottom_frame_blur = cv2.GaussianBlur(bottom_frame, (1, 5), cv2.BORDER_CONSTANT)
        #print('top_frame_blur: ', np.shape(top_frame_blur))

        # Change ColorSpace in small areas to HSV:
        top_frame_hsv = cv2.cvtColor(top_frame_blur, cv2.COLOR_BGR2HSV)
        bottom_frame_hsv = cv2.cvtColor(bottom_frame_blur, cv2.COLOR_RGB2HSV)
        #print('top_frame_hsv: ', np.shape(top_frame_hsv))

        # Create Mask, based on pre-calculation:
        top_frame_mask = cv2.inRange(top_frame_hsv, hsv_min, hsv_max)
        bottom_frame_mask = cv2.inRange(bottom_frame_hsv, hsv_min, hsv_max)
        #print('top_frame_mask: ', np.shape(top_frame_mask))

        # Putting mask on the Image:
        top_frame_result = cv2.bitwise_and(top_frame_hsv, top_frame_hsv, mask=top_frame_mask)
        bottom_frame_result = cv2.bitwise_and(bottom_frame_hsv, bottom_frame_hsv, mask=bottom_frame_mask)
        #print('top_frame_results: ', np.shape(top_frame_result))

        # Binarisation
        #top_frame_bin = cv2.adaptiveThreshold(top_frame_result, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        #bottom_frame_bin = cv2.adaptiveThreshold(bottom_frame_result, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, 11, 2)
        #print('top_frame_bin: ', np.shape(top_frame_bin))

        # Return these pieces to the main picture:
        # ----------------------------------------
        # here is the changeable code:
        top_frame = top_frame_result
        bottom_frame = bottom_frame_result
        frame[30:60, 0:150] = top_frame
        frame[90:120, 0:150] = bottom_frame

        # frame2 = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
        # kernel = np.ones((3, 3), np.float32) / 25
        # dst = cv2.filter2D(frame2, -1, kernel)

        # Output_parametr:
        if output_parametr == 0:
                cap.release()
                cv2.destroyAllWindows()
                return frame

        else:
            base_frame[30:150, 220:380] = frame

        cv2.imshow('base_frame', base_frame)
        cv2.imwrite('Line_Detected.jpg', base_frame)

        if cv2.waitKey(1) % 0xFF == ord('q'):
            break

    if source == 8:
        cv2.destroyAllWindows()

    else:
        cap.release()
        cv2.destroyAllWindows()
    return 0

# Step 2:

def ABCD_calculation():
    '''
    Эта функция находит центры полигонов, это точки: A,B,C,D
    На данном этапе вижу три способа решения данной задачи:
    а) Разбить на прямоугольник и две криволинейные трапеции:
       Левую и Правую. Через двойные интегралы (для каждой трапеции)
       и пересечения диаганолей найти центр масс полигона.
       Прогнав эту операцию 4 раза найдем искомые точки.
       Проблема: не знаю как описать уравнения кривых для трапеций

    б) Работа с псевдоматрицами.
       Создать две нулевые матрицы (идентичные областям анализа по кол-ву элементов)
       top_matrix = np.zeros((30, 150))
       bottom_matrix = np.zeros((30, 150))
       Переобозначим элементы этих матриц, которым соответвуют ненулевые пиксели,
       с нуля на единицы. Таким образом получим матрицы, которые описывают кадр.
       Используя матричные методы найти центры масс этих матриц
       Проблема: не знаю матричные методы нахождения такого центра

    в) Прогон в циклах.
       Пройтись циклом от 30 до 60 и найти средние точки в каждом ряду
       Выделить их белым цветом [255, 255, 255]
       Далее работать с полученным списком

    :return:
    Возвращает координаты центров полигонов: A,B,C,D

    Так получилось, что это EF - calculation
    '''

    A,B,C,D = [0,0], [0,0], [0,0], [0,0]
    img_base = cv2.imread('Line_Detected.jpg')
    img_work = cv2.cvtColor(img_base, cv2.COLOR_BGR2GRAY)
    pixel_top = []
    pixel_bottom = []
    j_aver_top = 0
    j_aver_bottom = 0

    # матрица верхнего окна:
    for i in range (60, 90, 1):
        Sum = 0
        Del = 0
        for j in range (220, 370, 1):
            if img_work[i,j] >= 10:
                Sum = Sum + j
                Del +=1
                j_aver_top = int(Sum / Del)

        if j_aver_top != 0:
            pixel_top.append(j_aver_top)
            #img_work[i, j_aver_top] = 255
    #print(pixel_top)

    print("There is a bottom matrix: \n")

    # матрица нижнего окна:
    for i in range(120, 150, 1):
        Sum = 0
        Del = 0
        for j in range(220, 370, 1):
            if img_work[i, j] >= 10:
                Sum = Sum + j
                Del += 1
                j_aver_bottom = int(Sum / Del)

        if j_aver_bottom != 0:
            pixel_bottom.append(j_aver_bottom)
            #img_work[i, j_aver_bottom] = 255
    #print(pixel_bottom)

    # для pixel_top:
    Ex = 0
    for i in range (0, len(pixel_top), 1):
        Ex = Ex + pixel_top[i]
    Ex = int(Ex / 30)
    print (Ex)
    Ey = 75

    # для pixel_bottom:
    Fx = 0
    for i in range(0, len(pixel_bottom), 1):
        Fx = Fx + pixel_bottom[i]
    Fx = int(Fx / 30)
    print(Fx)
    Fy = 135

    img_work[Ey, Ex] = 125
    img_work[Fy, Fx] = 125

    # Этот код потом вынесу в отдельную функцию:

    # Step 3 нахождение угла:
    H = Fy - Ey # 135 - 75 = 60
    EF = math.sqrt((Fx-Fy)+H^2)
    alfa = 90 - math.acos(EF/H)
    print(alfa)

    # Step 4 нахождение смещения:




    cv2.imwrite('New_One.jpg', img_work)

    print(A,B,C,D)
    plt.imshow(img_work)
    plt.show()

    return (Ey, Ex, Fy, Fx)


# --------Previous Algorithm (Denmark and few weeks after)

def color_hist(img, nbins=256, bins_range=(0, 256)):
    rh = np.histogram(img[:, :, 0], nbins, bins_range)
    gh = np.histogram(img[:, :, 1], nbins, bins_range)
    bh = np.histogram(img[:, :, 2], nbins, bins_range)

    # Generating bin centers
    bin_edges = rh[1] + gh[1] + bh[1]
    bincen = (bin_edges[1:] + bin_edges[0:len(bin_edges) - 1]) / 6

    feature_vec = np.concatenate((rh, gh, bh))

    # Return the individual histograms, bin_centers and feature vector
    return rh, gh, bh, bincen, feature_vec

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
    G - середина AB, H - середина DC
    От прошлой программы отличается тем, что:
    все В и D меняю местами, а также меняю местами x и y
    """
    # general case:
    if (Ay != By) and (Dy != Cy):
        Gy = min(Ay, By) + abs(By - Ay) / 2  # потому, что наклон не предсказуем
        Gx = Ax + ((Bx - Ax) / 2)            # потому что:  Bx > Ax
        Hy = min(Dy, Cy) + abs(Dy - Cy) / 2  # потому, что наклон не предсказуем
        Hx = Dx + ((Cx - Dx) / 2)            # потому что:  Cx > Dx
        print("GH general case: ", Gx, Gy, Hx, Hy)

    # Top - equal  case:
    elif (Ay == By) and (Dy != Cy):
        Gy = Ay
        Gx = Ax + ((Bx - Ax) / 2)
        Hy = min(Dy, Cy) + abs(Dy - Cy) / 2  # потому, что наклон не предсказуем
        Hx = Dx + ((Cx - Dx) / 2)
        print("GH left-equal case: ", Gx, Gy, Hy, Hx)

    # Bottom - equal Case:
    elif (Ay != By) and (Dy == Cy):
        Gy = min(Ay, By) + abs(By - Ay) / 2  # потому, что наклон не предсказуем
        Gx = Ax + ((Bx - Ax) / 2)
        Hy = Cy
        Hx = Dx + ((Cx - Dx) / 2)
        print("GH right - equal case: ", Gx, Gy, Hx, Hy)

    # ideal case:
    elif (Ay == By) and (Dy == Cy):
        Gy = By
        Gx = Ax + ((Bx - Ax) / 2)
        Hy = Dy
        Hx = Dx + ((Cx - Dx) / 2)
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
    if (Fy == Ey):
        Kfe = Fx - Ex
    else:
        Kfe = (Fx - Ex) / (Fy - Ey)

    if (Hy == Gy):
        Khg = Hx - Gx
    else:
        Khg = (Hx - Gx) / (Hy - Gy)

    Oy = 1 / (Kfe - Khg) * (Ey*Kfe - Ex - Gy*Khg + Gx)

    Ox = Oy * Kfe - Ey * Kfe + Ex

    print(Ox, Oy)

    return int(Ox), int(Oy)

def coordinates_detector(frame, Ax, Bx, Ay,By):
    """
    Это упрощенный вариант функции, тем что по известным осям:
    Ax = Dx и Bx = Cx мы ищем Аy, Dy, By, Cy
    для этого составляем два списка значений пикселей вдоль оси Ax - Dx и вдоль оси Bx - Cx
    затем для левого списка:
    первое вхождение нуля - координата Ay
    последнее вхождение нуля - координата Dy
    для правого списка:
    первое вхождение нуля - координата By
    последнее вхождение нуля - координата Cy
    :param frame:
    :return: Ay, Dy, By, Cy
    """
    # Решение через списки:
    left_list, right_list = list(), list()

    # Решение через строки:
    #left_list, right_list = str(), str()

    # Далее для увеличения скорости работы - работать с массивами

    for y in range(60, 200):
        # Решение через списки:
        left_list.append(frame[y, Ax])
        right_list.append(frame[y, Bx])
    Ay, Dy, By, Cy = 0, 0, 0, 0
    for i in range(len(left_list) - 1):
        if (left_list[i] == 0) and (left_list[i-1] == 0):
            if Ay == 0:
                Ay = i
                i = i + 10
            elif Ay != 0:
                Dy = i

    print('left_list: ', left_list)

    for i in range(len(right_list) - 1):
        if (right_list[i] == 0) and (right_list[i - 1] == 0):
            if By == 0:
                By = i
                i = i + 10
            elif By != 0:
                Cy = i
    print ('right_list: ', right_list)

    Ay = 60 + Ay #int(left_list.find('0'))
    Dy = 60 + Dy #int(left_list.rfind('0'))
    By = 60 + By #int(right_list.find('0'))
    Cy = 60 + Cy #int(right_list.rfind('0'))


        # решение через строки:
        #left_list = left_list + ' ' + str(frame[y, Ax])
        #right_list = right_list + ' ' +str(frame[y,Bx])

    #Ay = 60 + int(left_list.find('0') / 3)
    #Dy = Ay + int(left_list.rfind('0') / 3)
    #By = 60 + int(right_list.find('0') / 3)
    #Cy = By + int(right_list.rfind('0') / 3)
    #print(right_list)
    print ('Ay,Dy,By,Cy: ', Ay,Dy,By,Cy )

    return Ay,Dy,By,Cy

def styding_project(bright, brightHSV, brightYCB, brightLAB):
    # python
    bgr = [40, 158, 16]
    thresh = 40

    minBGR = np.array([bgr[0] - thresh, bgr[1] - thresh, bgr[2] - thresh])
    maxBGR = np.array([bgr[0] + thresh, bgr[1] + thresh, bgr[2] + thresh])

    maskBGR = cv2.inRange(bright, minBGR, maxBGR)
    resultBGR = cv2.bitwise_and(bright, bright, mask=maskBGR)

    # convert 1D array to 3D, then convert it to HSV and take the first element
    # this will be same as shown in the above figure [65, 229, 158]
    hsv = cv2.cvtColor(np.uint8([[bgr]]), cv2.COLOR_BGR2HSV)[0][0]

    minHSV = np.array([hsv[0] - thresh, hsv[1] - thresh, hsv[2] - thresh])
    maxHSV = np.array([hsv[0] + thresh, hsv[1] + thresh, hsv[2] + thresh])

    maskHSV = cv2.inRange(brightHSV, minHSV, maxHSV)
    resultHSV = cv2.bitwise_and(brightHSV, brightHSV, mask=maskHSV)

    # convert 1D array to 3D, then convert it to YCrCb and take the first element
    ycb = cv2.cvtColor(np.uint8([[bgr]]), cv2.COLOR_BGR2YCrCb)[0][0]

    minYCB = np.array([ycb[0] - thresh, ycb[1] - thresh, ycb[2] - thresh])
    maxYCB = np.array([ycb[0] + thresh, ycb[1] + thresh, ycb[2] + thresh])

    maskYCB = cv2.inRange(brightYCB, minYCB, maxYCB)
    resultYCB = cv2.bitwise_and(brightYCB, brightYCB, mask=maskYCB)

    # convert 1D array to 3D, then convert it to LAB and take the first element
    lab = cv2.cvtColor(np.uint8([[bgr]]), cv2.COLOR_BGR2LAB)[0][0]

    minLAB = np.array([lab[0] - thresh, lab[1] - thresh, lab[2] - thresh])
    maxLAB = np.array([lab[0] + thresh, lab[1] + thresh, lab[2] + thresh])

    maskLAB = cv2.inRange(brightLAB, minLAB, maxLAB)
    resultLAB = cv2.bitwise_and(brightLAB, brightLAB, mask=maskLAB)

    cv2.imshow("Result BGR", resultBGR)
    cv2.imshow("Result HSV", resultHSV)
    cv2.imshow("Result YCB", resultYCB)
    cv2.imshow("Output LAB", resultLAB)

# -----------------------------------------------------


# Main code here:

# step 0:
# getpicture(1)

# step1:
# настроили фильтр: (0,19,84) (255,62,227)
# получили изображение LineDetected.jpg
# hsv_min, hsv_max = hsv_color_filter_callibration(8) #8 - picture
# print(hsv_min, hsv_max)
# img_preparation(8, hsv_min, hsv_max, 1) #8 -picture

# step 2:
# Работаем с LineDetected.jpg
ABCD_calculation()

# step 3:

def angle():
    # https://xn--90aeniddllys.xn--p1ai/opencv-mashinnoe-zrenie-na-python-pryamougolnye-obekty-chast-3/
    import numpy as np
    import cv2 as cv
    import math

    hsv_min = np.array((86, 11, 0), np.uint8)
    hsv_max = np.array((132, 255, 255), np.uint8)

    color_blue = (255, 0, 0)
    color_yellow = (0, 255, 255)

    if __name__ == '__main__':
        fn = '34562.jpg'  # имя файла, который будем анализировать
        img = cv.imread(fn)

        hsv = cv.cvtColor(img, cv.COLOR_BGR2HSV)  # меняем цветовую модель с BGR на HSV
        thresh = cv.inRange(hsv, hsv_min, hsv_max)  # применяем цветовой фильтр
        contours0, hierarchy = cv.findContours(thresh.copy(), cv.RETR_TREE, cv.CHAIN_APPROX_SIMPLE)

        # перебираем все найденные контуры в цикле
        for cnt in contours0:
            rect = cv.minAreaRect(cnt)  # пытаемся вписать прямоугольник
            box = cv.boxPoints(rect)  # поиск четырех вершин прямоугольника
            box = np.int0(box)  # округление координат
            center = (int(rect[0][0]), int(rect[0][1]))
            area = int(rect[1][0] * rect[1][1])  # вычисление площади

            # вычисление координат двух векторов, являющихся сторонам прямоугольника
            edge1 = np.int0((box[1][0] - box[0][0], box[1][1] - box[0][1]))
            edge2 = np.int0((box[2][0] - box[1][0], box[2][1] - box[1][1]))

            # выясняем какой вектор больше
            usedEdge = edge1
            if cv.norm(edge2) > cv.norm(edge1):
                usedEdge = edge2
            reference = (1, 0)  # горизонтальный вектор, задающий горизонт

            # вычисляем угол между самой длинной стороной прямоугольника и горизонтом
            angle = 180.0 / math.pi * math.acos(
                (reference[0] * usedEdge[0] + reference[1] * usedEdge[1]) / (cv.norm(reference) * cv.norm(usedEdge)))

            if area > 100:
                cv.drawContours(img, [box], 0, (255, 0, 0), 2)  # рисуем прямоугольник
                cv.circle(img, center, 5, color_yellow, 2)  # рисуем маленький кружок в центре прямоугольника
                # выводим в кадр величину угла наклона
                cv.putText(img, "%d" % int(angle), (center[0] + 20, center[1] - 20), cv.FONT_HERSHEY_SIMPLEX, 1,
                           color_yellow, 2)

        cv.imshow('contours', img)
        cv.waitKey()
        cv.destroyAllWindows()

# step 4:


# Control Step:
# plot_pixel_analyzer()







print('End')
