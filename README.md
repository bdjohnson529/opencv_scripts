# This is a toolbox for OpenCV.
#### Builds were tested in Ubuntu 16.04 on a 64 bit processor.
[OpenCV](https://github.com/opencv/opencv) should be built with the [contrib](https://github.com/opencv/opencv_contrib) modules for full support. Run [`install.sh`](install.sh) to install to fully install the latest OpenCV release.

#### To check OpenCV version:
	$ pkg-config --modversion opencv

API reference can be found on the [OpenCV website](https://docs.opencv.org/3.4.0/)

## Camera Calibration
The C++ program is not maintained. Likely the CMakeLists needs to be modified, and the OpenCV C++ package needs to be installed for the code to work.  Once CMake has been set up, the program can be built with g++ or another compiler.
```
$g++ -ggdb camera_calibration.cpp -o camera_calibration `pkg-config --cflags --libs opencv`
```

The Python program is maintained. The following scripts are located in the folder [`calibration/python`](calibration/python).

1. **Camera calibration.**
	* Supported for rectilinear and fisheye lenses.
	* `python calibrate_fisheye.py /path/to/image/directory/`
	* `python calibrate.py /path/to/image/directory/`

2. **Image undistortion.**
	* Supported for rectilinear and fisheye lenses.
	* `python undistort_fisheye.py /path/to/image/directory/`
	* `python undistort.py /path/to/image/directory/`

3. **Read camera intrinsics.**
	* `python read_intrinsics.py`

## Inpainting

1. Inpainting.
	* Custom code to inpaint over bounding boxes drawn in CV.


## Object Tracking

1. Tracking.
	* Single object tracking. User inputs a bounding box defining the object. Files are saved with bounding box and PASCAL VOC XML file used for deep learning.
	* `python object_tracking.py /path/to/video/file/ /path/to/save/directory/`
