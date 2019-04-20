import cv2
import numpy as np

#read the image
image = cv2.imread("../assets/putin.jpg")

#Convert to HSV color sppace
hsvImage = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
print(hsvImage[50,50])

#split the channels
h, s, v = cv2.split(hsvImage)

#initialize the histogram params
histogramSize = 180
bin_w = 4

#initialize the histogram image params
histogramImageWidth = histogramSize * 4
histogramImageHeight = histogramSize * 4

#caluculate histogram
histogram = cv2.calcHist([h], [0], None, [histogramSize], [0, histogramSize])
print(histogram)

#Normalize the histogram
cv2.normalize(histogram, histogram, 0, histogramSize*3, cv2.NORM_MINMAX, -1)
print(histogram)

#convert all float histogram values to int
histogram = np.int32(histogram)
print(histogram)

#Create empty image to draw histogram
histogramImage = 255*np.ones((histogramImageWidth, histogramImageHeight, 3))

#draw x axis
cv2.line(histogramImage, (0, histogramImageHeight - 30), (histogramImageWidth, histogramImageHeight - 30), (0, 0, 0), 2, 8, 0)

#draw origin
cv2.line(histogramImage, (0, histogramImageHeight - 20), (0, histogramImageHeight - 40), (0, 0, 0), 2, 8, 0)
cv2.putText(histogramImage, "0", (0, int(histogramImageHeight - (30/2))), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)

#draw histogram as line on empty image
for i in range(1, histogramSize):
    cv2.line(histogramImage, ((bin_w*(i-1)), histogramImageHeight - 30 - histogram[i-1]), ((bin_w*i), histogramImageHeight - 30 - histogram[i]), 
        (0, 0, 255), 2, 8, 0)
    if(i % 20 == 0):
        cv2.line(histogramImage, (i*bin_w, histogramImageHeight - 20), (i*bin_w, histogramImageHeight - 40), (0, 0, 0), 2, 8, 0)
        cv2.putText(histogramImage, str(i), (i*bin_w, histogramImageHeight - 5), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0), 2)
    

#create window to display images
cv2.namedWindow("image", cv2.WINDOW_NORMAL)
cv2.namedWindow("hue", cv2.WINDOW_NORMAL)
cv2.namedWindow("saturation", cv2.WINDOW_NORMAL)
cv2.namedWindow("value", cv2.WINDOW_NORMAL)
cv2.namedWindow("histogram", cv2.WINDOW_NORMAL)

#display images
cv2.imshow("image", image)
cv2.imshow("hue", h)
cv2.imshow("saturation", s)
cv2.imshow("value", v)
cv2.imshow("histogram", histogramImage)

#press esc to exit the program
cv2.waitKey(0)

#close all the opened windows
cv2.destroyAllWindows()