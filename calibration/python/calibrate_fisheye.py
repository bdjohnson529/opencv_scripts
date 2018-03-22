#####
# Input arguments : argv[1] = image directory
#####

import cv2
assert cv2.__version__[0] == '3', 'The fisheye module requires opencv version >= 3.0.0'
import numpy as np
import os
import glob
import sys


CHECKERBOARD = (6,8)    # (column,row)

subpix_criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
calibration_flags = cv2.fisheye.CALIB_RECOMPUTE_EXTRINSIC+cv2.fisheye.CALIB_CHECK_COND+cv2.fisheye.CALIB_FIX_SKEW

# prepare object points
objp = np.zeros((1, CHECKERBOARD[0]*CHECKERBOARD[1], 3), np.float32)
objp[0,:,:2] = np.mgrid[0:CHECKERBOARD[0], 0:CHECKERBOARD[1]].T.reshape(-1, 2)

_img_shape = None

# Arrays to store object points and image points.
objpoints = [] # 3d point in real world space
imgpoints = [] # 2d points in image plane.

# define list of filepaths for processing
filepath_list = []
directory = sys.argv[1]
filename_list = sorted(os.listdir(directory))
for file in filename_list:
    filepath_list.append( os.path.join(directory + file) )

# inititalize error log file
log = open('calibration_log.txt', 'w')
log.write("Corners not found for these images: \n")

print "begin processing images..................."

# read images
for fname in filepath_list:
    img = cv2.imread(fname,0)
    print "reading: ", fname
    # check that image was read
    if _img_shape == None:
        _img_shape = img.shape[:2]
    else:
        assert _img_shape == img.shape[:2], "All images must share the same size."

    # Find the chess board corners
    ret, corners = cv2.findChessboardCorners(img, CHECKERBOARD, cv2.CALIB_CB_ADAPTIVE_THRESH+cv2.CALIB_CB_FAST_CHECK+cv2.CALIB_CB_NORMALIZE_IMAGE)
    print ret

    # If found, add object points, image points (after refining them)
    if ret == True:
        print "corners found"
        objpoints.append(objp)

        # resolve corners at sub-pixel accuracy
        corners2 = cv2.cornerSubPix(img,corners,(3,3),(-1,-1),subpix_criteria)
        imgpoints.append(corners2)

        # Draw and display the corners
        #img_display = cv2.drawChessboardCorners(img, (CHECKERBOARD[0],CHECKERBOARD[1]), corners2, ret)
        #cv2.imshow('img',img_display)
        #cv2.waitKey(500)

    else:
        print "corners not found: ", fname
        log.write("\nrm")
        log.write(fname)

print "Calculating camera intrinsics........................................"
N_OK = len(objpoints)
print "Found ", str(N_OK), " valid images for calibration"

# initialize camera and distortion matrices
K = np.zeros((3, 3))
D = np.zeros((4, 1))
rvecs = [np.zeros((1, 1, 3), dtype=np.float64) for i in range(N_OK)]
tvecs = [np.zeros((1, 1, 3), dtype=np.float64) for i in range(N_OK)]
# calibrate camera
rms, K, D, rvecs, tvecs = cv2.fisheye.calibrate(objpoints, imgpoints, img.shape[::-1], K, D, rvecs, tvecs, calibration_flags, (cv2.TERM_CRITERIA_EPS+cv2.TERM_CRITERIA_MAX_ITER, 30, 1e-6))
print "rms: ", rms
print "K: ", K
print "D: ", D
print "shape: ", img.shape[:2]

# convert matrices to lists
K1 = K[0].tolist()
K2 = K[1].tolist()
K3 = K[2].tolist()
print "K1: ", K1
dist_list = D.tolist()
print dist_list

#write output to text file
output = open('results_fisheye.txt', 'w')
output.write("Intrinsic matrix\n")
output.write("%s\n" % K1)
output.write("%s\n" % K2)
output.write("%s\n" % K3)

output.write("Distortion coefficients \n")
output.write("%s\n" % dist_list)
