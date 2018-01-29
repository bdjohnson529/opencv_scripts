# Custom code to inpaint over bounding boxes
# Ben Johnson
# January 28, 2018

import sys
import cv2
import numpy
from matplotlib import pyplot as plt
import xml.etree.ElementTree as ET

def ParseXML():
	data = []
	filepath = sys.argv[1]
	xmlFile = sys.argv[2]
	imgFilepath = filepath + "img/"
	xmlFilepath = filepath + xmlFile

	tree = ET.parse(xmlFilepath)
	root = tree.getroot()
	for layout in root.findall('layout'):
	    image = layout[0].text
	    xmin = layout[2][1][0].text
	    ymin = layout[2][1][1].text
	    xmax = layout[2][1][2].text
	    ymax = layout[2][1][3].text

	    p1 = (int(xmin), int(ymin))
	    p2 = (int(xmax), int(ymax))

	    imageFile = imgFilepath + image + ".jpg"

	    information = (imageFile, p1, p2)

	    data.append(information)

	    print imageFile
	    print xmin
	    print ymin
	    print xmax
	    print ymax
	    print information

	return data


def main():
	data = ParseXML()

	for entry in data:
		imageFile = str(entry[0])
		p1 = entry[1]
		p2 = entry[2]


		original = cv2.imread(imageFile)

		# generate mask
		mask = numpy.zeros((original.shape[0], original.shape[1]), dtype = "uint8")
		cv2.rectangle(mask, p1, p2, (255,255,255), 6, 1)

		# inpaint using mask
		dst = cv2.inpaint(original, mask, 3, cv2.INPAINT_NS)


		cv2.imshow('original', original)
		cv2.imshow('mask', mask)
		cv2.imshow('dst', dst)
		cv2.waitKey(100)



if __name__ == "__main__":
    main()

#img = cv2.imread('rectangle.jpg')
#mask = numpy.zeros((img.shape[0], img.shape[1]), dtype = "uint8")
#print(img.shape)
#print(mask.shape)
#cv2.rectangle(mask, p1, p2, (255,255,255), 8, 1)

#dst = cv2.inpaint(img, mask, 3, cv2.INPAINT_TELEA)

#cv2.imshow('mask', mask)
#cv2.imshow('dst', dst)
#cv2.waitKey()
