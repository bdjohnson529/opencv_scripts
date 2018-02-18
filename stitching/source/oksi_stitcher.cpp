#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "oksi_stitcher.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
	printf("test output \n");
	cv::Mat image1 = cv::imread("/home/oksi/Pictures/stitching/1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image2 = cv::imread("/home/oksi/Pictures/stitching/2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image3 = cv::imread("/home/oksi/Pictures/stitching/3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image4 = cv::imread("/home/oksi/Pictures/stitching/4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image5 = cv::imread("/home/oksi/Pictures/stitching/5.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	/**
	cv::imshow("Display window", image1);
	cv::waitKey(1000);
	cv::imshow("Display window", image2);
	cv::waitKey(1000);
	cv::imshow("Display window", image3);
	cv::waitKey(1000);
	cv::imshow("Display window", image4);
	cv::waitKey(1000);
	cv::imshow("Display window", image5);
	cv::waitKey(1000);
	**/

	OKSI_Stitcher Stitcher;
	Stitcher.reset();
	Stitcher.add_image(image1);
	Stitcher.add_image(image2);

	Stitcher.calc_offsets();
	Stitcher.stitch();
	Stitcher.crop();
	Stitcher.calc_center();

	cv::Mat stitched_image;
	stitched_image = Stitcher.get_image();
	cv::imwrite("stitched.jpg", stitched_image);
	return 1;
}