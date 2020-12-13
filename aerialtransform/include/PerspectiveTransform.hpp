#pragma once
//#include "coordinates.hpp"
#include "EigenCoordConverter.h"
#include  <Eigen/Core>
#include  <Eigen/Dense>
#include  <Eigen/Eigen>
#include  <opencv2/highgui/highgui.hpp>
#include  <opencv2/core/core.hpp>
#include  <opencv2/opencv.hpp>
#include  <opencv2/core/eigen.hpp>
#include  <opencv2/highgui/highgui.hpp>
#include  <opencv2/imgproc/imgproc.hpp>

#include <math.h>
#include <iostream>
#include <cmath>

typedef Eigen::Matrix<double,3,Eigen::Dynamic> MatrixD;


class PerspectiveTransform
{
	float yaw, pitch, roll;
	double focal_length, pixel_size;	// camera params
	int num_pix_x, num_pix_y, zoom;			// image size
	double lat, lon, alt;
	cv::Mat image_raw, image_t, image_final;

public:
	PerspectiveTransform();
	void LoadLLA(double lat, double lon, double alt);
	void LoadAttitude(float yaw, float pitch, float roll);
	void LoadCamera(double focal_length, double pixel_size, int num_pix_x, int num_pix_y);
	void ImageTransform();
	void LoadImage(cv::Mat image);
	float FindImageWidth(float lat, float gsd, int num_pix_x);
	float GroundResolution(float lat, int zoom);

	cv::Mat GetRawImage(){return image_raw;};
	cv::Mat GetTransformedImage(){return image_final;};
	int GetZoom(){return zoom;};
	
	bool CheckValues();

};