To build camera calibration with G++:

$g++ -ggdb camera_calibration.cpp -o camera_calibration `pkg-config --cflags --libs opencv`
