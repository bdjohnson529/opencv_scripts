This is a toolbox for OpenCV. OpenCV installation is documented on website. For example, to install 3.3.1, download the zip source file, and follow the "installation instructions" in the documentation.


-----
camera_calibration.cpp
-----

To build executable, use g++ and link the opencv libraries.

	$ g++ -ggdb camera_calibration.cpp -o camera_calibration `pkg-config --cflags --libs opencv`

Camera calibration execution requires one argument.

	$ ./camera_calibration_executable /path/to/camera_settings.xml

Camera settings XML file is passed the image_list.xml file, which details the image paths.
