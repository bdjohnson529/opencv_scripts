import cv2
import numpy as np
import glob
import sys

# example usage
# python normalize.py '/home/user/dir/*.png' '/home/user/dir/save_dir/'

# path should have single quotations
# to prevent shell from expanding path
path = sys.argv[1]
save_dir = sys.argv[2]

files = glob.glob(path)

print "Converting images..."

for file in files:

	filename = file.rsplit('/', 1)[1]
	save_path = save_dir + filename

	img = cv2.imread(file)

	cv2.normalize(img, img, 0, 255, cv2.NORM_MINMAX)
	cv2.imwrite(save_path, img)

print "Finished converting images."