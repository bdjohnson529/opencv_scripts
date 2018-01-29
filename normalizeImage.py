import cv2 as cv
import numpy as np
import sys

# image path = sys.argv[1]


path = sys.argv[1]
outfile = open('outfile.txt','w')
img = cv.imread(path)
outfile.write(img)


print img

#normalizedImg = np.zeros((800, 800))
#normalizedImg = cv.normalize(img,  normalizedImg, 0, 255, cv.NORM_MINMAX)
#cv.imshow('dst_rt', normalizedImg)
#cv.waitKey(0)
#cv.destroyAllWindows()
