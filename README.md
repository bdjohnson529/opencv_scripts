This is a toolbox for OpenCV. OpenCV installation is documented on website. For example, to install 3.3.1, download the zip source file, and follow the "installation instructions" in the documentation.


1. Building cpp executables.

G++:	$ g++ -ggdb camera_calibration.cpp -o camera_calibration `pkg-config --cflags --libs opencv`


2. Modules.
	Camera calibration - calculate camera intrinsics
	Inpainting - remove lines from an image
	SFM - 3d reconstruction
	Stitching - stitch overlapping images
	Tracking - online image tracking
