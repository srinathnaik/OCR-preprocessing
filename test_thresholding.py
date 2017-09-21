import cv2
import numpy as np
from matplotlib import pyplot as plt
path = raw_input(":-")
pt = path.strip(".jpg")
img = cv2.imread(path,0)
imgc = cv2.imread(path,0)
img1 = cv2.medianBlur(img,5)
ret,th1 = cv2.threshold(img1,127,255,cv2.THRESH_BINARY)
th2 = cv2.adaptiveThreshold(img1,255,cv2.ADAPTIVE_THRESH_MEAN_C,\
            cv2.THRESH_BINARY,11,2)
th3 = cv2.adaptiveThreshold(img1,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
            cv2.THRESH_BINARY,11,2)

# Otsu's thresholding
ret2,th4 = cv2.threshold(imgc,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)
# Otsu's thresholding after Gaussian filtering
blur = cv2.GaussianBlur(imgc,(5,5),0)
ret3,th5 = cv2.threshold(blur,0,255,cv2.THRESH_BINARY+cv2.THRESH_OTSU)

titles = ['Global_Thresholding',
            'Adaptive_Mean_Thresholding', 'Adaptive_Gaussian_Thresholding', 'Otsu', 'G_Otsu']
images = [th1, th2, th3, th4, th5 ]
for i in xrange(len(images)):
    # plt.subplot(2,2,i+1)
    plt.figure()
    plt.imshow(images[i],'gray')
    plt.title(titles[i])
    plt.xticks([]),plt.yticks([])
    plt.show()
    cv2.imwrite(pt+"_"+titles[i]+".jpg",images[i])