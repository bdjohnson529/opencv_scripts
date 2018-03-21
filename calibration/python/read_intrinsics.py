
#####
# undistorts images using opencv2
# modified from http://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_calib3d/py_calibration/py_calibration.html
# Ben Johnson
# December 11, 2017
#####

#####
# NOTES: flags in read_intrinsic_report have not been incorporated into error messages
#####

# TO EXECUTE: python undistort.py [intrinsic_report] [image_directory]

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
=======
intrinsic_report = sys.argv[1]
image_directory = sys.argv[2]
save_directory = sys.argv[3]

# read images in directory
filename_list = sorted(os.listdir(image_directory))
filepath_list = []
for file in filename_list:
    filepath_list.append( os.path.join(image_directory + file))


# populate intrinsic matrix from intrinsic_report text file
def read_intrinsic_report(report):
    # set flags
    distortion_flag1 = 0
    distortion_flag2 = 0
    projection_flag1 = 0
    projection_flag2 = 0
    reprojection_flag = 0

    # read data from KALIBR files
    if report.find("kalibr") >= 0:
        with open(report) as f:
            # search file line by line
            for line in f:
                # find focal length, principal point
                if "projection:" in line:
                    # set flags
                    projection_flag1 = True
                    projection_flag2 = True
                    # slice line
                    begin = line.find("[") + 1
                    end = line.find("]") - 1
                    projection_raw = line[begin:end]
                    projection_clean = " ".join(projection_raw.split())         # clean up line for double spaces
                    f1,f2,p1,p2 = projection_clean.split(" ")                   # split line into variables
                if "distortion" in line:
                    # set flags
                    distortion_flag1 = True
                    distortion_flag2 = True
                    # slice line
                    begin = line.find("[") + 1
                    end = line.find("]") - 1
                    distortion_raw = line[begin:end]
                    distortion_clean = " ".join(distortion_raw.split())         # clean up line for double spaces
                    k1,k2,r1,r2 = distortion_clean.split(" ")                   # split line into variables

    # read data from MATLAB files
    elif intrinsic_report.find("matlab") >= 0:
        with open(intrinsic_report) as f:
            for line in f:
                # slice line for radial distortion values
                if "RadialDistortion" in line and "Coefficients" not in line:
                    distortion_flag1 = True
                    begin = line.find("[") + 1
                    end = line.find("]") - 1
                    radial_distortion_raw = line[begin:end]
                    radial_distortion_clean = " ".join(radial_distortion_raw.split())       # clean up line for double spaces
                    r1,r2= radial_distortion_clean.split(" ")                         # split line into variables

                # slice line for tangential distortion values
                if "TangentialDistortion" in line and "Estimate" not in line:
                    distortion_flag2 = True
                    begin = line.find("[") + 1
                    end = line.find("]")
                    tangential_distortion_raw = line[begin:end]
                    tangential_distortion_clean = " ".join(tangential_distortion_raw.split())         # clean up line for double spaces
                    k1,k2= tangential_distortion_clean.split(" ")               # split line into variables

                # slice line for focal length
                if "FocalLength" in line:
                    projection_flag1 = True
                    begin = line.find("[") + 1
                    end = line.find("]") - 1
                    focal_length_raw = line[begin:end]
                    focal_length_clean = " ".join(focal_length_raw.split())         # clean up line for double spaces
                    f1,f2 = focal_length_clean.split(" ")                   # split line into variables

                # slice line for principal point
                if "PrincipalPoint" in line:
                    projection_flag2 = True
                    begin = line.find("[") + 1
                    end = line.find("]") - 1
                    principal_point_raw = line[begin:end]
                    principal_point_clean = " ".join(principal_point_raw.split())         # clean up line for double spaces
                    p1,p2 = principal_point_clean.split(" ")                   # split line into variables

    return(float(f1), float(f2), float(p1), float(p2), float(r1), float(r2), float(k1), float(k2))


def main():
    print "starting the main function"
    f1, f2, p1, p2, r1, r2, k1, k2 = read_intrinsic_report(intrinsic_report)

    # populate numpy array with data read from text file
    intrinsic_mtx = numpy.array([(f1,0,p1),(0,f2,p2),(0,0,1)])
    distortion_coeff = numpy.array([(k1,k2,r1,r2)])
    print "intrinsic matrix \n", intrinsic_mtx
    print "distortion coefficients \n", distortion_coeff
    #intrinsic_mtx = numpy.array([(746.81,0,590.670),(0,748.66,610.38),(0,0,1)])
    #distortion_coeff = numpy.array([-0.36,0.17,-0.0012,-0.00061])

    #undistort images
    for file in filename_list:

        # read image
        filepath_read = image_directory + file
        img = cv2.imread(filepath_read)

        h, w = img.shape[:2]
        newcameramtx, roi = cv2.getOptimalNewCameraMatrix(intrinsic_mtx,distortion_coeff,(w,h),1,(w,h))
        dst = cv2.undistort(img, intrinsic_mtx, distortion_coeff, None, newcameramtx)

        x,y,w,h = roi
        dst = dst[y:y+h, x:x+w]
        filepath_write = save_directory + file
        cv2.imwrite(filepath_write,dst)



if __name__ == "__main__":
    main()
>>>>>>> 7abf312e24dbd206923e77098a3f7173de80b0b5
