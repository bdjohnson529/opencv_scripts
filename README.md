# This is a toolbox for OpenCV.
#### Builds were tested in Ubuntu 16.04 on a 64 bit processor.
[OpenCV](https://github.com/opencv/opencv) should be built with the [contrib](https://github.com/opencv/opencv_contrib) modules for full support. Run the install script in a chosen directory to fully install the latest OpenCV release.

## To check OpenCV version:
	$ pkg-config --modversion opencv

API reference can be found on the [OpenCV website](https://docs.opencv.org/3.4.0/)

## Features:


1. Camera calibration.
	* Calibration is supported for rectilinear and fisheye lenses.
	* `python calibrate_fisheye.py /path/to/image/directory/`
	* `python calibrate.py /path/to/image/directory/`

2. Inpainting.
	* Custom code to inpaint over bounding boxes drawn in CV.

3. Tracking.
	* Single object tracking. User inputs a bounding box defining the object. Files are saved with bounding box and PASCAL VOC XML file used for deep learning.
	* `python object_tracking.py /path/to/video/file/ /path/to/save/directory/`
