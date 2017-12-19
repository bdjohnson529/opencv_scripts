import numpy as np
import cv2
import glob
import sys
import os

row = 7
col = 11

# termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((row*col,3), np.float32)
objp[:,:2] = np.mgrid[0:col,0:row].T.reshape(-1,2)

# Arrays to store object points and image points.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

# define list of filepaths for processing
filepath_list = []
directory = sys.argv[1]
filename_list = sorted(os.listdir(directory))
for file in filename_list:
    filepath_list.append( os.path.join(directory + file) )



# DEBUGGING
#img = cv2.imread(filepath_list[1],0)
#ret, corners = cv2.findChessboardCorners(img, (col,row),None)
#objpoints.append(objp)
#corners2 = cv2.cornerSubPix(img,corners,(11,11),(-1,-1),criteria)
#imgpoints.append(corners2)
#ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, img.shape[::-1],None,None)

#print ret
#print mtx
#print dist
#print rvecs
#print tvecs


for fname in filepath_list:
    print fname
    img = cv2.imread(fname,0)
    print img

    # Find the chess board corners
    ret, corners = cv2.findChessboardCorners(img, (col,row),None)
    print ret

    # If found, add object points, image points (after refining them)
    if ret == True:
        objpoints.append(objp)

        # resolve corners at sub-pixel accuracy
        corners2 = cv2.cornerSubPix(img,corners,(11,11),(-1,-1),criteria)
        imgpoints.append(corners2)

        # Draw and display the corners
        #img = cv2.drawChessboardCorners(img, (col,row), corners2,ret)
        #cv2.imshow('img',img)
        #cv2.waitKey(500)

cv2.destroyAllWindows()

# calibrate camera
ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, img.shape[::-1],None,None)
print "camera matrix: ", mtx
print "camera distortion: ", dist


# undistort image
img = cv2.imread(filepath_list[1])
h,  w = img.shape[:2]
newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
dst = cv2.undistort(img, mtx, dist, None, newcameramtx)

# crop the image
x,y,w,h = roi
dst = dst[y:y+h, x:x+w]
cv2.imwrite('calibresult.png',dst)
