import numpy
import cv2
import glob
import sys
import os

row = 8
col = 5

# termination criteria
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = numpy.zeros((row*col,3), numpy.float32)
objp[:,:2] = numpy.mgrid[0:col,0:row].T.reshape(-1,2)

# Arrays to store object points and image points.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

# define list of filepaths for processing
filepath_list = []
directory = sys.argv[1]
filename_list = sorted(os.listdir(directory))
for file in filename_list:
    filepath_list.append( os.path.join(directory + file) )

# error logging
log = open('calibration_log.txt', 'w')
log.write("Corners not found for these images: \n")

# read images
for fname in filepath_list:
    img = cv2.imread(fname,0)

    # Find the chess board corners
    ret, corners = cv2.findChessboardCorners(img, (col,row),None)

    # If found, add object points, image points (after refining them)
    if ret == True:
        print "corners found "
        objpoints.append(objp)

        # resolve corners at sub-pixel accuracy
        corners2 = cv2.cornerSubPix(img,corners,(11,11),(-1,-1),criteria)
        imgpoints.append(corners2)

        # Draw and display the corners
        #img = cv2.drawChessboardCorners(img, (col,row), corners2,ret)
        #cv2.imshow('img',img)
        #cv2.waitKey(500)
    else:
        print "corners not found: ", fname
        # log errors
        log.write("\nrm")
        log.write(fname)


cv2.destroyAllWindows()

# calibrate camera
ret, mtx, dist, rvecs, tvecs = cv2.calibrateCamera(objpoints, imgpoints, img.shape[::-1],None,None)
print "camera matrix: ", mtx
print "camera distortion: ", dist


# convert matrices to lists
mtx1 = mtx[0].tolist()
mtx2 = mtx[1].tolist()
mtx3 = mtx[2].tolist()
dist_list = dist.tolist()

#write output to text file
output = open('results.txt', 'w')
output.write("Intrinsic matrix\n")
output.write("%s\n" % mtx1)
output.write("%s\n" % mtx2)
output.write("%s\n" % mtx3)


output.write("Distortion coefficients \n")
output.write("%s\n" % dist_list)


# undistort image
#img = cv2.imread(filepath_list[1])
#h,  w = img.shape[:2]
#newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
#dst = cv2.undistort(img, mtx, dist, None, newcameramtx)

# crop the image
#x,y,w,h = roi
#dst = dst[y:y+h, x:x+w]
#cv2.imwrite('calibresult.png',dst)
