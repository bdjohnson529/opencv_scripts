# Camera Calibration

The C++ program is not maintained. Likely the CMakeLists needs to be modified, and the OpenCV C++ package needs to be installed for the code to work. Once CMake has been set up, the program can be built with g++ or another compiler.
```
$g++ -ggdb camera_calibration.cpp -o camera_calibration `pkg-config --cflags --libs opencv`
```

The Python program is maintained.

1. Camera calibration.
	* Supported for rectilinear and fisheye lenses.
	* `python calibrate_fisheye.py /path/to/image/directory/`
	* `python calibrate.py /path/to/image/directory/`

2. Image undistortion.
	* Supported for rectilinear and fisheye lenses.
	* `python undistort_fisheye.py /path/to/image/directory/`
	* `python undistort.py /path/to/image/directory/`

3. Read camera intrinsics.
	* `python read_intrinsics.py`
