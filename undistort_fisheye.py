#####
# Input arguments : 	argv[1] = settings file
#			argv[2] = distorted image directory
#####


import numpy as np
import cv2
import glob
import sys
import os

# DEBUGGING - manually enter matrix and distortion
DIM = (1920, 1200)
K = np.array([[726.49727255,0,945.7804803],[0,727.06050889,603.13960861],[0,0,1]])
D = np.array([[-0.03016212],[-0.02889801],[0.0337639],[-0.0153955]])
#print mtx
#print dist

# get intrinsics and distortion from input file
#input_file = sys.argv[1]
#with open(input_file) as f:
#    line = f.readline()                 # line 1

    # read row 1
#    line = f.readline()
#    end = line.find("]")
#    row1 = line[1:end]                  # line 2
#    m11, m12, m13 = row1.split(',')

    # read row 2
#    line = f.readline()
#    end = line.find("]")
#    row2 = line[1:end]                  # line 3
#    m21, m22, m23 = row2.split(',')

#    f.readline()                        # line 4
#    f.readline()                        # line 5

    # read distortion
#    line = f.readline()
#    end = line.find("]]")
#    row5 = line[2:end]                  # line 6
#    d1, d2, d3, d4, d5 = row5.split(',')

# populate intrinsic and distortion matrix
#mtx = numpy.matrix([[float(m11),float(m12),float(m13)],[float(m21),float(m22),float(m23)],[0,0,1]])
#dist = numpy.matrix([float(d1),float(d2),float(d3),float(d4),float(d5)])

#print "intrinsic matrix: ", mtx
#print "distortion coefficients: ", dist

def undistort(img_path):
    # read distorted image
    img = cv2.imread(img_path)
    h,w = img.shape[:2]

    map1, map2 = cv2.fisheye.initUndistortRectifyMap(K, D, np.eye(3), K, DIM, cv2.CV_16SC2)
    undistorted_img = cv2.remap(img, map1, map2, interpolation=cv2.INTER_LINEAR, borderMode=cv2.BORDER_CONSTANT)
    cv2.imshow("undistorted", undistorted_img)
    cv2.imwrite("undistorted1.png",undistorted_img)
    cv2.waitKey(1000)
    cv2.destroyAllWindows()

if __name__ == '__main__':
    for p in sys.argv[1:]:
        undistort(p)
