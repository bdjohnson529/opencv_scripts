# This is a computer vision toolbox.

This project leverages [OpenCV](https://opencv.org/about/), an "open source computer vision and machine learning software library." OpenCV is the most popular open-source library used by computer vision researchers. [OpenCV](https://github.com/opencv/opencv) should be built with the [contrib](https://github.com/opencv/opencv_contrib) modules for full support. API reference can be found on the [OpenCV website](https://docs.opencv.org/3.4.0/).
#### To check OpenCV version:
	$ pkg-config --modversion opencv


## Installation
| Script | Description| 
| -- | -- |
|[`install/install_cpp.sh`](install/install_cpp.sh) | C++ installation |
|[`install/install_python.sh`](install/install_python.sh) | Python installation |


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

1. **Inpainting.**
	* Custom code to inpaint over bounding boxes drawn in CV.


## Object Tracking

1. **Tracking.**
	* Single object tracking. User inputs a bounding box defining the object. Files are saved with bounding box and PASCAL VOC XML file used for deep learning.
	* `python object_tracking.py /path/to/video/file/ /path/to/save/directory/`
