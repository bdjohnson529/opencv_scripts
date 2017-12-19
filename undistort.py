import numpy
import cv2
import glob
import sys
import os


mtx = numpy.matrix([[747.23,0,587.15],[0,749.75,609.161],[0,0,1]])
dist = numpy.matrix([-0.3827,-0.2301,-0.001305,-0.00019864,-0.08813])
dist1 = numpy.matrix([-0.02,-0.01,-0.001305,-0.00019864,0])


print mtx
print dist


img = cv2.imread("/home/ben/camera_calibration_datasets/checkerboard_11302017_intrinsics/cam0/00000000051.jpg")
h,  w = img.shape[:2]
newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),0.9,(w,h))
print "newcameramtx: ", newcameramtx
print "roi: ", roi
mapx,mapy = cv2.initUndistortRectifyMap(mtx,dist,None,newcameramtx,(w,h),5)
dst = cv2.remap(img,mapx,mapy,cv2.INTER_LINEAR)
cv2.imwrite('calibresult.png',dst)


# crop the image
#x,y,w,h = roi
#dst = dst[y:y+h, x:x+w]
#cv2.imwrite('calibresult.png',dst)



#newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
#print h
#print w
#print roi
#print newcameramtx
#print img
