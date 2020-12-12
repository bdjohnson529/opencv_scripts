#include <iostream>
#include <fstream>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
    vector<Mat> imgs;
    Stitcher::Mode mode = Stitcher::PANORAMA;

    cv::Mat image1 = cv::imread("/home/oksi/Pictures/stitching/1.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat image2 = cv::imread("/home/oksi/Pictures/stitching/2.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat image3 = cv::imread("/home/oksi/Pictures/stitching/3.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat image4 = cv::imread("/home/oksi/Pictures/stitching/4.jpg", CV_LOAD_IMAGE_COLOR);
    cv::Mat image5 = cv::imread("/home/oksi/Pictures/stitching/5.jpg", CV_LOAD_IMAGE_COLOR);
    imgs.push_back(image1);
    imgs.push_back(image2);
    imgs.push_back(image3);
    imgs.push_back(image4);
    imgs.push_back(image5);

    cv::Mat pano;
    Ptr<Stitcher> stitcher = Stitcher::create(mode, false);
    Stitcher::Status status = stitcher->stitch(imgs, pano);
    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return -1;
    }

    cv::imwrite("result.jpg", pano);
    cv::imshow("panorama", pano);
    cv::waitKey(5000);
    return 0;
}
