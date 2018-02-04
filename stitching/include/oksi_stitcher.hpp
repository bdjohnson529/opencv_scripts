#include <algorithm>
#include <numeric>
#include <map>
#include "opencv2/opencv.hpp"

/**
#if CV_VERSION_EPOCH == 2
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/imgproc/imgproc.hpp" // For calculating quadrilateral centroid
#else
**/


#include "opencv2/calib3d.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;
using namespace cv::xfeatures2d;

class OKSI_Stitcher
{

  int minHessian = 400;
  //SurfFeatureDetector detector( minHessian );
#if CV_VERSION_EPOCH == 2
  SurfFeatureDetector detector;
  SurfDescriptorExtractor extractor;
#else
  Ptr<SURF> detector = SURF::create( minHessian );
#endif
  std::vector<Point2f> obj_corners, scene_corners;
  Mat H_total = Mat::eye(3,3,CV_64F);
  // Setup that is independent of any group of images to be stitched
  const int poly_size = 4; // Number of corners in a quadrilateral polygon
  FlannBasedMatcher matcher;
  int num_img = 0;
  Mat Eye = Mat::eye(3,3,CV_64F);

  std::vector<Mat> img_arr; // For the original images
  std::vector<Mat> imgt_arr; // = {img_scene}; // For the warped images
  std::vector<std::vector<KeyPoint> > keypoints; //(num_img); // Keypoints for each image
  std::vector<Mat> descriptors; //(num_img); // Descriptors for each image
  std::vector<Mat> Hs, H_totals; // Homography matrices for each image based on the previous image(s)
  Mat im1t; // Initial output matrix
  //std::vector<int> xdataout(2), ydataout(2); // Describes how large the canvas needs to be for stitching
  std::vector<int> xdataout, ydataout; // Describes how large the canvas needs to be for stitching
  //std::vector<double> xoffset(num_img,0.0), yoffset(num_img,0.0), xoffset_summed(num_img), yoffset_summed(num_img); // Offsets for each image due to homography
  std::vector<double> xoffset, yoffset, xoffset_summed, yoffset_summed; // Offsets for each image due to homography
  std::vector<double> x_doubly_offset, y_doubly_offset;
  //std::vector<std::vector<Point2f> > obj_corners_vec(num_img, std::vector<Point2f>(poly_size)), obj_corners_vec_offset = obj_corners_vec; // Track each images corners. Needed for center calculation.
  std::vector<std::vector<Point2f> > obj_corners_vec, obj_corners_vec_offset = obj_corners_vec; // Track each images corners. Needed for center calculation.
  Size warped_obj_size; // Canvas size
  //Mat Image_Combined(img_arr[0].cols, img_arr[0].rows, img_arr[0].type(), 0.0);
  Rect bRect;
  Mat Image_Combined;
  std::map<int, Point2f> center_map;

public:

#if CV_VERSION_EPOCH == 2
  OKSI_Stitcher() : detector(minHessian), obj_corners(poly_size), scene_corners(poly_size), xdataout(2), ydataout(2),
#else
  OKSI_Stitcher() : obj_corners(poly_size), scene_corners(poly_size), xdataout(2), ydataout(2),
#endif
		    // CV_8UC1 is just a guess for initialization
		    Image_Combined(1,1,CV_8UC1)  {
#if CV_VERSION_EPOCH != 2
    //detector(minHessian);
    obj_corners.resize(poly_size);
    scene_corners.resize(poly_size);
#endif
  }
  

  void reset() {
    num_img = 0;
    img_arr.clear();
    imgt_arr.clear();
    keypoints.clear();
    descriptors.clear();
    Hs.clear(); H_totals.clear();
    xdataout[0] = 0;
    xdataout[1] = 0;
    ydataout[0] = 0;
    ydataout[1] = 0;
    xoffset.clear();
    yoffset.clear();
    xoffset_summed.clear();
    yoffset_summed.clear();
    x_doubly_offset.clear();
    y_doubly_offset.clear();
    warped_obj_size.height = ydataout[1]-ydataout[0];
    warped_obj_size.width = xdataout[1]-xdataout[0];
    // Do we need to reset obj_corners and scene_corners?
    //for(int i = 0; i < poly_size; i++) {
    //  obj_corners
    center_map.clear();
  }
  
  void add_image(const Mat &img) {
    num_img++;
    keypoints.resize(num_img);
    descriptors.resize(num_img);
    xoffset.resize(num_img,0.0);
    yoffset.resize(num_img,0.0);
    obj_corners_vec.push_back( std::vector<Point2f>(poly_size));
    obj_corners_vec_offset.push_back( std::vector<Point2f>(poly_size));
    img_arr.push_back(img);

    if(num_img == 1) {
      Image_Combined.convertTo(Image_Combined, img_arr[0].type());
      // Analyse the first image
      //-- Step 1: Detect the keypoints using SURF Detector
#if CV_VERSION_EPOCH == 2
      detector.detect(img_arr[0], keypoints[0]);
      extractor.compute(img_arr[0], keypoints[0], descriptors[0]);
#else
      detector->detect(img_arr[0], keypoints[0]);
      detector->compute(img_arr[0], keypoints[0], descriptors[0]); 
#endif
      H_totals.push_back(Eye.clone());
      xdataout[0] = 0;
      ydataout[0] = 0;
      xdataout[1] = img_arr[0].cols;
      ydataout[1] = img_arr[0].rows;
      warped_obj_size.height = ydataout[1]-ydataout[0];
      warped_obj_size.width = xdataout[1]-xdataout[0];
      imgt_arr.push_back(img_arr[0]);
      //-- Get the corners from the image_1 ( the object to be "detected" )
      scene_corners[0] = cvPoint(0,0); scene_corners[1] = cvPoint( img_arr[0].cols, 0 );
      scene_corners[2] = cvPoint( img_arr[0].cols, img_arr[0].rows ); scene_corners[3] = cvPoint( 0, img_arr[0].rows );
      for(int i = 0; i < poly_size; i++)
	obj_corners_vec[0][i] = scene_corners[i];
    }
    else {
      int img_idx = num_img-1;

#if CV_VERSION_EPOCH == 2
      detector.detect( img_arr[img_idx], keypoints[img_idx] );
      //-- Step 2: Calculate descriptors (feature vectors)
      extractor.compute(img_arr[img_idx], keypoints[img_idx], descriptors[img_idx]);
#else
      detector->detect( img_arr[img_idx], keypoints[img_idx] );
      //-- Step 2: Calculate descriptors (feature vectors)
      detector->compute(img_arr[img_idx], keypoints[img_idx], descriptors[img_idx]);
#endif
 
      //-- Step 3: Matching descriptor vectors using FLANN matcher
      std::vector< DMatch > matches;
      matcher.match( descriptors[img_idx], descriptors[img_idx-1], matches );
    
      double max_dist = 0; double min_dist = 100;
    
      //-- Quick calculation of max and min distances between keypoints
      for( int i = 0; i < descriptors[img_idx].rows; i++ ) {
	double dist = matches[i].distance;
	if( dist < min_dist ) min_dist = dist;
	if( dist > max_dist ) max_dist = dist;
      }
    
      //-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
      std::vector< DMatch > good_matches;    
      for( int i = 0; i < descriptors[img_idx].rows; i++ )
	if( matches[i].distance < 3*min_dist )
	  good_matches.push_back(matches[i]);
    
      //-- Localize the object
      std::vector<Point2f> obj, scene;
    
      for( int i = 0; i < good_matches.size(); i++ ) {
	//-- Get the keypoints from the good matches
	obj.push_back( keypoints[img_idx][ good_matches[i].queryIdx ].pt );
	scene.push_back( keypoints[img_idx-1][ good_matches[i].trainIdx ].pt );
      }
    
      Mat H = findHomography( obj, scene, CV_RANSAC );    
      // RANSAC (RANSAC-based robust method)
      // LMEDS (Leadt-Median robust method)
      // RHO (PROSAC-based robust method)
      Hs.push_back(H);
      Mat H_total = Eye.clone();
      for( int i = 0; i < Hs.size(); i++)
	H_total *= Hs[i];
      H_totals.push_back(H_total);
    
      // Calculate the corners
      perspectiveTransform( scene_corners, obj_corners, H);
      std::vector<int> xdataout_old = xdataout, ydataout_old = ydataout;
      for( int i = 0; i < poly_size; i++) {
	xdataout[0] = std::min(xdataout[0],(int)floor(obj_corners[i].x));
	ydataout[0] = std::min(ydataout[0],(int)floor(obj_corners[i].y));
	xdataout[1] = std::max(xdataout[1],(int)ceil(obj_corners[i].x));
	ydataout[1] = std::max(ydataout[1],(int)ceil(obj_corners[i].y));
	obj_corners_vec[img_idx][i] = obj_corners[i];
      }
      if(xdataout[0] < 0) {
	xoffset[img_idx] = xdataout[0];
      }
      if(ydataout[0] < 0) {
	yoffset[img_idx] = ydataout[0];
      }
      Mat offset = Mat::eye(3,3,CV_64F);
      double x_net_offset = accumulate(xoffset.begin(), xoffset.end(), 0.0);
      double y_net_offset = accumulate(yoffset.begin(), yoffset.end(), 0.0);
      offset.at<double>(1,2) -= y_net_offset;
      offset.at<double>(0,2) -= x_net_offset;
      int offset_sign = -1;
      for( int i = 0; i < poly_size; i++) {
	xdataout[0] = std::min(xdataout[0],(int)floor(obj_corners[i].x+offset_sign*x_net_offset));
	ydataout[0] = std::min(ydataout[0],(int)floor(obj_corners[i].y+offset_sign*y_net_offset));
	xdataout[1] = std::max(xdataout[1],(int)ceil(obj_corners[i].x+offset_sign*x_net_offset));
	ydataout[1] = std::max(ydataout[1],(int)ceil(obj_corners[i].y+offset_sign*y_net_offset));
	Point2f offset_point = obj_corners[i];
	offset_point.x=offset_sign*x_net_offset;
	offset_point.y=offset_sign*y_net_offset;
	obj_corners_vec_offset[img_idx][i] = obj_corners[i];
	//std::cout << "Object corner " << obj_corners[i] << "\tScene Corner: " << scene_corners[i] << std::endl;
      }
      warped_obj_size.height = ydataout[1]-ydataout[0];
      warped_obj_size.width = xdataout[1]-xdataout[0];
      //std::cout << "XDataout: " << xdataout[0] << "\t" << xdataout[1] << "\tYDataout: " << ydataout[0] << "\t" << ydataout[1] << "\tX Net Offset: " << x_net_offset << "\tY Net Offset: " << y_net_offset << std::endl;      
      Mat im2t;
      //std::cout << offset << std::endl << H_total << std::endl;
      H_total = offset*H_total;
      warpPerspective(img_arr[img_idx], im2t, H_total, warped_obj_size);
      imgt_arr.push_back(im2t);
      scene_corners = obj_corners;

    }

  }

  void calc_offsets() {
    xoffset_summed.resize(num_img);
    yoffset_summed.resize(num_img);
    x_doubly_offset.resize(num_img); y_doubly_offset.resize(num_img);
    std::partial_sum(xoffset.begin(), xoffset.end(), xoffset_summed.begin()); 
    std::partial_sum(yoffset.begin(), yoffset.end(), yoffset_summed.begin());
    std::reverse(xoffset_summed.begin(), xoffset_summed.end());
    std::reverse(yoffset_summed.begin(), yoffset_summed.end());
    std::reverse(xoffset.begin(), xoffset.end());
    std::reverse(yoffset.begin(), yoffset.end());
    warped_obj_size.height -= yoffset_summed[0];
    warped_obj_size.width -= xoffset_summed[0];
    for(int i = 0; i < num_img; i++) {
      x_doubly_offset[i] = -xoffset_summed[num_img-1-i]+xoffset_summed[0];
      y_doubly_offset[i] = -yoffset_summed[num_img-1-i]+yoffset_summed[0];
    }

  }

  void stitch() {
    //std::cout << "Stitch: Warped Object Size " << warped_obj_size << std::endl;
    resize(Image_Combined, Image_Combined, warped_obj_size);
    //#ifdef TIMER
    //std::cout << "Image resize " << tmr.elapsed() << std::endl;
    //#endif
    for ( int img_idx = 0; img_idx < num_img; img_idx++) {
      Mat im1t, im2t;
      //Mat offset = Mat::eye(2,3,CV_64F);
      //offset.at<double>(1,2) -= y_doubly_offset[img_idx];
      //offset.at<double>(0,2) -= x_doubly_offset[img_idx];
      im1t = imgt_arr[img_idx];
      Size im_size = im1t.size();
      //std::cout << "Image " << img_idx << " size: " << im_size << "\t" <<  -x_doubly_offset[img_idx] << "\t" << -y_doubly_offset[img_idx] << std::endl;
      //Rect dstRect(Point(offset.at<double>(0,2),offset.at<double>(1,2)), im_size);
      Rect dstRect(Point(-x_doubly_offset[img_idx], -y_doubly_offset[img_idx]), im1t.size());
      im1t.copyTo(Image_Combined(dstRect),im1t);
      // #ifdef TIMER
      //       std::cout << "End of Overlay Loop " << img_idx << " " << tmr.elapsed() << std::endl;
      // #endif
    }
  }


  void crop(bool actually_crop=true) {
    // Calculate bounding rectangle (maybe convex hull) to get image center
    std::vector<Point2f> obj_corners_total;
    for(int img_idx = 0; img_idx < num_img; img_idx++) 
      for(int j = 0; j < poly_size; j++) 
	obj_corners_total.push_back(obj_corners_vec[img_idx][j]);
    bRect = boundingRect(obj_corners_total);
    Rect bRect_subimage = bRect;
    if(bRect_subimage.x < 0) {
      // SHF 1/16/2018
      // I have NO idea why I need to include the Y amounts in width and X, but it seems to work....
      // I should not need to do so. And nor do I know why it is different than when .x >= 0
      bRect_subimage.width += -xoffset[1]+y_doubly_offset[0];
      bRect_subimage.x = -y_doubly_offset[0]; 
      bRect_subimage.y = -yoffset[1];
    }
    else {
      //bRect_subimage.width -= 10;
      bRect_subimage.y = -y_doubly_offset[1];
    }
    // #ifdef TIMER
    //     std::cout << "Bounding box " << tmr.elapsed() << std::endl;
    // #endif
    
    //std::cout << "Bounding Box " << bRect << "\t" << bRect_subimage << std::endl;
    // Crop the image
    if(actually_crop)
      Image_Combined = Image_Combined(bRect_subimage);
    // #ifdef TIMER
    //     std::cout << "Image crop " << tmr.elapsed() << std::endl;
    // #endif

  }

  void calc_center() {
    // Find the center of the cropped image
    Point2f center;
    center.x = bRect.width/2.0;
    center.y = bRect.height/2.0;
    // Use the center of the bounding box as the image center
    // Find out which images the center lies within
    std::vector<double> image_center_test(num_img);
    std::vector<Point2f> center_arr(1,center), center_t_arr;
    for(int img_idx = 0; img_idx < num_img; img_idx++) {
      for(int j = 0; j < poly_size; j++) {
	obj_corners_vec_offset[img_idx][j].x = obj_corners_vec[img_idx][j].x - bRect.x;
	obj_corners_vec_offset[img_idx][j].y = obj_corners_vec[img_idx][j].y - bRect.y;
      }
      image_center_test[img_idx] = pointPolygonTest(obj_corners_vec_offset[img_idx], center, false);
      // If the center of the image lies within the corresponding image,
      // convert that location into the frame of the untransformed original image
      if(image_center_test[img_idx] >= 0) {
	Mat offset = Eye.clone();
	offset.at<double>(0,2) -= bRect.x;
	offset.at<double>(1,2) -= bRect.y;
	Mat inv_offset_homog = (offset*Hs[img_idx-1]).inv();
	perspectiveTransform(center_arr, center_t_arr, inv_offset_homog);
	//std::cout << "New center. Image Index: " << img_idx << "\tCenter location: " << center_t_arr[0] << std::endl;
	center_map[img_idx] = center_t_arr[0];
      }
    }
    
  }

  void stitch_and_center() {
    calc_offsets();
    stitch();
    crop();
    calc_center();
  }

  Mat get_image() { return Image_Combined; }

  std::map<int, Point2f> get_centers() { return center_map; }
private:

};
