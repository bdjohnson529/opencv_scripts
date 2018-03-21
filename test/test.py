import cv2

image = cv2.imread("/media/nuc3/numbers/1.png")

cv2.imshow("image", image)

print image.elemSize()

cv2.waitKey(1000)