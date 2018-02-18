This is a toolbox for OpenCV. OpenCV [a link](https://github.com/opencv/opencv)


1. Building cpp executables.

G++:	$ g++ -ggdb camera_calibration.cpp -o camera_calibration `pkg-config --cflags --libs opencv`


2. Modules.
	Camera calibration - calculate camera intrinsics 
	Inpainting - remove lines from an image 
	SFM - 3d reconstruction 
	Stitching - stitch overlapping images 
	Tracking - online image tracking 
