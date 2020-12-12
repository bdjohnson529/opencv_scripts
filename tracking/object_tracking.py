#####
# Modified from https://www.learnopencv.com/object-tracking-using-opencv-cpp-python/
# Pascal 2012 format pulled from http://host.robots.ox.ac.uk/pascal/VOC/voc2012/htmldoc/index.html
# Modified by Ben Johnson
# January 3, 2017
#####

# input arguments
# base directory = /media/oksi/Data/datasets_drone_images/
# videoName = sys.argv[1]

import cv2
import sys
import xml.etree.ElementTree as ET

def InitializeTracker():
    (major_ver, minor_ver, subminor_ver) = (cv2.__version__).split('.')

    # select tracker type
    tracker_types = ['BOOSTING', 'MIL','KCF', 'TLD', 'MEDIANFLOW', 'GOTURN']
    tracker_type = tracker_types[2]

    # initialize KCF tracker
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


def InitializeVideo(videoName):
    directory = "/media/oksi/Data/datasets_drone_images/"
    videoFile = directory + videoName + ".mp4"
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
    directory = "/media/oksi/Data/datasets_drone_images/" 
    outfilePath = directory + videoName + "/" + videoName + ".xml"
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

def SaveImage(image, frame, imageName):
    videoName = sys.argv[1]
    directory = "/media/oksi/Data/datasets_drone_images/"
    imageFilePath = directory + videoName + "/img/" + imageName + ".jpg"
    imageAnnotatedFilePath = directory + videoName + "/img_annotated/" + imageName + ".jpg"

    print imageFilePath

    # Save result
    cv2.imwrite(imageFilePath, image)
    cv2.imwrite(imageAnnotatedFilePath, frame)

def TrackObject(index, video, frame, tracker, resultsXML, errorLog):
    while True:
        index += 1
        imageName = "%08d" % index

        # Read a new frame
        ok, image = video.read()
        frame = image.copy()
        if not ok:
            repeat = False
            skip = False
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
            cv2.imshow("Tracking", frame)
            SaveImage(image, frame, imageName)
            resultsXML = AddAnnotation(resultsXML, imageName, p1, p2)

        else :
            # Tracking failure
            cv2.putText(frame, "Tracking failure detected", (100,80), cv2.FONT_HERSHEY_SIMPLEX, 0.75,(0,0,255),2)
            cv2.imshow("Tracking", frame)
            errorLog.write(imageName)
            errorLog.write("\n")

        # key press actions
        # 32 = space, 27 = escape, 114 = s
        k = cv2.waitKey(1) & 0xff
        if k == 32 :
            repeat = True
            skip = False
            break
        if k == 27 :
            repeat = False
            skip = False
            break
        if k == 115:
            repeat = True
            skip = True
            break

    return repeat, skip, index, resultsXML

def SkipFrames(video, index):
    while True:
        index += 1
        ok, frame = video.read()
        cv2.imshow("Tracking", frame)
        if not ok:
            repeat = False
            break

        # key press actions
        # 32 = space, 27 = escape, 114 = s
        k = cv2.waitKey(0) & 0xff
        if k == 32 :
            repeat = True
            break
        if k == 27:
            repeat = False
            break

    return repeat, index


def main():
    videoName = sys.argv[1]
    video, ok, frame = InitializeVideo(videoName)
    resultsXML, annotationFile = InitializeAnnotation(videoName)
    errorLog = InitializeErrorLog()

    index = 0

    while True:
        # Select and initialize bounding box
        ok, frame = video.read()
        tracker = InitializeTracker()
        bbox = cv2.selectROI(frame, False)
        ok = tracker.init(frame, bbox)

        repeat, skip, index, resultsXML = TrackObject(index, video, frame, tracker, resultsXML, errorLog)

        if skip:
            repeat, index = SkipFrames(video, index)
        if not repeat:
            break

    # write XML data to file
    mydata = ET.tostring(resultsXML)
    print mydata
    annotationFile.write(mydata)



if __name__ == "__main__":
    main()
