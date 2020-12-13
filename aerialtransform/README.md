# Aerial Perspective Transform

This program projects aerial imagery onto the ground, so images can be overlayed on a satellite map. You can use the functions in this library to project aerial images taken by a drone onto Google Earth tiles. To perform the transformation, you will need imagery, GPS and attitude measurements. We mounted a camera, IMU and GPS receiver on a drone to obtain our datasets.

### Algorithm

The PerspectiveTransform class finds the spatial coordinates of the image, by applying a rotation matrix R (line 47) to the imaging plane of the camera. Using the camera model, we are able to project the rotated image onto the ground. We then use the projected image coordinates, and the GPS-provided camera position, to obtain the latitude and longitude of image center.

Images are downsampled to fit the closest zoom level.


### Sample Code

Sample code can be found in test/testPerspective.cpp. The basic idea is to load the PerspectiveTransform class with all inputs, and retrieve the output image.


Load the input parameters:

    ptransform.LoadCamera(focal_length (m), pixel_size (m), width (px), height (px) );
    ptransform.LoadLLA(latitude (m), longitude (m), altitude (m) );
    ptransform.LoadAttitude(yaw (deg), pitch (deg), roll (deg) );

Transform the image:

    ptransform.ImageTransform();

Retrieve image as a cv::Mat:

   	cv::Mat tform = ptransform.GetTransformedImage();
