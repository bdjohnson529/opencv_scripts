
import numpy
import cv2
import glob
import sys
import os

# DEBUGGING - manually enter matrix and distortion
#mtx = numpy.matrix([[740.36535481,0,947.33511156],[0,741.57034976,610.70286756],[0,0,1]])
#dist = numpy.matrix([-0.35636705,0.16373423,-0.00045106,-0.00084871,-0.03945055])
#print mtx
#print dist

# get intrinsics and distortion from input file
input_file = sys.argv[1]
with open(input_file) as f:
    line = f.readline()                 # line 1

    # read row 1
    line = f.readline()
    end = line.find("]")
    row1 = line[1:end]                  # line 2
    m11, m12, m13 = row1.split(',')

    # read row 2
    line = f.readline()
    end = line.find("]")
    row2 = line[1:end]                  # line 3
    m21, m22, m23 = row2.split(',')

    f.readline()                        # line 4
    f.readline()                        # line 5

    # read distortion
    line = f.readline()
    end = line.find("]]")
    row5 = line[2:end]                  # line 6
    d1, d2, d3, d4, d5 = row5.split(',')

# populate intrinsic and distortion matrix
mtx = numpy.matrix([[float(m11),float(m12),float(m13)],[float(m21),float(m22),float(m23)],[0,0,1]])
dist = numpy.matrix([float(d1),float(d2),float(d3),float(d4),float(d5)])

print "intrinsic matrix: ", mtx
print "distortion coefficients: ", dist


# undistort image
directory = sys.argv[2]
filename_list = sorted(os.listdir(directory))
filepath_list = []
for file in filename_list:
    filepath_list.append( os.path.join(directory + file) )

for i, fname in enumerate(filepath_list):

    print i

    img = cv2.imread(fname)
    h,  w = img.shape[:2]
    print "h: ", h
    print "w: ", w
    newcameramtx, roi=cv2.getOptimalNewCameraMatrix(mtx,dist,(w,h),1,(w,h))
    print "newcameramtx: ", newcameramtx
    print "roi: ", roi
    mapx,mapy = cv2.initUndistortRectifyMap(mtx,dist,None,newcameramtx,(w,h),5)
    dst = cv2.remap(img,mapx,mapy,cv2.INTER_LINEAR)

    new_filename = "undistorted_" + str(i) + ".png"
    cv2.imwrite(new_filename,dst)


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