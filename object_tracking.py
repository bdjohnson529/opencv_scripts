#####
# OG code pulled from https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/
# Pascal 2012 format pulled from http://host.robots.ox.ac.uk/pascal/VOC/voc2012/htmldoc/index.html
# Modified by Ben Johnson
# January 3, 2017
#####

# input arguments
# videoFile = sys.argv[1]
# saveDirectory = sys.argv[2]

import cv2
import sys
import xml.etree.ElementTree as ET

def InitializeTracker():
    (major_ver, minor_ver, subminor_ver) = (cv2.__version__).split('.')

    # select tracker type
    tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW', 'GOTURN']
    tracker_type = tracker_types[2]

    # initialize opencv tracker
    if int(minor_ver) < 3:
        tracker = cv2.Tracker_create(tracker_type)
    else:
        if tracker_type == 'BOOSTING':
            tracker = cv2.TrackerBoosting_create()
        if tracker_type == 'MIL':
            tracker = cv2.TrackerMIL_create()
        if tracker_type == 'KCF':
            tracker = cv2.TrackerKCF_create()
        if tracker_type == 'TLD':
            tracker = cv2.TrackerTLD_create()
        if tracker_type == 'MEDIANFLOW':
            tracker = cv2.TrackerMedianFlow_create()
        if tracker_type == 'GOTURN':
            tracker = cv2.TrackerGOTURN_create()

    return tracker


def InitializeVideo():
    videoFile = sys.argv[1]
    # Read video
    video = cv2.VideoCapture(videoFile)
    if not video.isOpened():
        print "Could not open video"
        sys.exit()
    # Read first frame
    ok, frame = video.read()
    if not ok:
        print 'Cannot read video file'
        sys.exit()

    return video, ok, frame


def InitializeAnnotation(videoName):
    saveDirectory = sys.argv[2]
    outfilePath = saveDirectory + videoName + ".xml"
    annotationFile = open(outfilePath, 'w')

    results = ET.Element('results')

    return results, annotationFile


def InitializeErrorLog():
    errorLog = open('errorlog.txt', 'w')
    errorLog.write('Tracking failure for these frames\n')
    return errorLog


def AddAnnotation(results, imageName, p1, p2):
    layout = ET.SubElement(results, 'layout')
    image = ET.SubElement(layout, 'image')
    image.text = imageName
    obj = ET.SubElement(layout, 'object')
    obj.text = str(1)
    part = ET.SubElement(layout, 'part')
    objClass = ET.SubElement(part, 'class')
    objClass = 'drone'
    bndbox = ET.SubElement(part, 'bndbox')
    xmin = ET.SubElement(bndbox, 'xmin')
    xmin.text = str(p1[0])
    ymin = ET.SubElement(bndbox, 'ymin')
    ymin.text = str(p1[1])
    xmax = ET.SubElement(bndbox, 'xmax')
    xmax.text = str(p2[0])
    ymax = ET.SubElement(bndbox, 'ymax')
    ymax.text = str(p2[1])

    return results

def SaveImage(frame, imageName):
    saveDirectory = sys.argv[2]
    imageFilePath = saveDirectory + "img/" + imageName + ".jpg"

    # Display and save result
    cv2.imshow("Tracking", frame)
    cv2.imwrite(imageFilePath, frame)


def main():
    videoName = "drone_1"

    tracker = InitializeTracker()
    video, ok, frame = InitializeVideo()
    resultsXML, annotationFile = InitializeAnnotation(videoName)
    errorLog = InitializeErrorLog()

    # Select and initialize bounding box
    bbox = cv2.selectROI(frame, False)
    ok = tracker.init(frame, bbox)

    index = 0

    while True:
        index += 1
        imageName = "%08d" % index

        # Read a new frame
        ok, frame = video.read()
        frameSize = frame.shape

        if not ok:
            break
         
        # Start timer
        timer = cv2.getTickCount()

        # Update tracker
        ok, bbox = tracker.update(frame)

        # Calculate Frames per second (FPS)
        fps = cv2.getTickFrequency() / (cv2.getTickCount() - timer);

        # Draw bounding box
        if ok:
            # Tracking success
            p1 = (int(bbox[0]), int(bbox[1]))
            p2 = (int(bbox[0] + bbox[2]), int(bbox[1] + bbox[3]))
            cv2.rectangle(frame, p1, p2, (255,0,0), 2, 1)

            # Save image and annotation
            SaveImage(frame, imageName)
            results = AddAnnotation(resultsXML, imageName, p1, p2)

        else :
            # Tracking failure
            cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)
            errorLog.write(imageName)
            errorLog.write("\n")

        # Exit if ESC pressed
        k = cv2.waitKey(1) & 0xff
        if k == 27 : break
        


    mydata = ET.tostring(results)
    print mydata
    # Push tree to file
    annotationFile.write(mydata)  

 

if __name__ == "__main__":
    main()