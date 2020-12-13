#include "PerspectiveTransform.hpp"


PerspectiveTransform::PerspectiveTransform()
{
	this->lat = 0;
	this->lon = 0;
	this->alt = 0;

	this->focal_length = 0;
	this->pixel_size = 0;
	this->num_pix_x = 0;
	this->num_pix_y = 0;
}

void PerspectiveTransform::LoadLLA(double lat, double lon, double alt)
{
	this->lat = lat;
	this->lon = lon;
	this->alt = alt;
}

void PerspectiveTransform::LoadAttitude(float yaw, float pitch, float roll)
{
	this->yaw = yaw * M_PI / 180;
	this->pitch = pitch * M_PI / 180;
	this->roll = roll * M_PI / 180;
}

void PerspectiveTransform::LoadCamera(double focal_length, double pixel_size, int num_pix_x, int num_pix_y)
{
	this->focal_length = focal_length;
	this->pixel_size = pixel_size;
	this->num_pix_x = num_pix_x;
	this->num_pix_y = num_pix_y;
}

void PerspectiveTransform::LoadImage(cv::Mat image)
{
	this->image_raw = image;
}

void PerspectiveTransform::ImageTransform()
{
	Eigen::Matrix<float, 3, 3> R;

	R = Eigen::AngleAxisf(roll, Eigen::Vector3f::UnitX())
		* Eigen::AngleAxisf(pitch, Eigen::Vector3f::UnitY())
		* Eigen::AngleAxisf(yaw, Eigen::Vector3f::UnitZ());

	float width = pixel_size * image_raw.cols;
	float height = pixel_size * image_raw.rows;

	Eigen::Matrix<float, 4, 3> PTS, PTS2;

	// Set the origin as image center
	PTS(0,0) = -0.5 * width;
	PTS(0,1) = -0.5 * height;
	PTS(1,0) = 0.5 * width;
	PTS(1,1) = -0.5 * height;
	PTS(2,0) = 0.5 * width;
	PTS(2,1) = 0.5 * height;
	PTS(3,0) = -0.5 * width;
	PTS(3,1) = 0.5 * height;
	// z axis is perpendicular to image plane
	for(int i=0; i<4; i++)
		PTS(i,2) = focal_length;

	// Apply rotation to image points
	PTS2 = ( R * PTS.transpose() ).transpose();

	std::cout << "PTS2\n" << PTS2 << std::endl;

	// Project images onto the ground
	// We appoximate all image corners at the same altitude
	for(int i=0; i<4; i++)
		for(int j=0; j<3; j++)
			PTS2(i,j) = PTS2(i,j) * alt / PTS2(i,2);

	// Cast Eigen matrices as vector<cv::Point2f>
	std::vector<cv::Point2f> px_corners(4), image_corners(4), ned_corners(4), world_corners_scaled(4), pix_corners_transformed(4);
	for(int i = 0; i< 4; i++) {
		image_corners[i].x = PTS(i,0);
		image_corners[i].y = PTS(i,1);
		ned_corners[i].x = PTS2(i,0);
		ned_corners[i].y = PTS2(i,1);
	}

	// Compute bounding rectangle on world corners
	float min_x = ned_corners[0].x, min_y = ned_corners[0].y;
	float max_x = ned_corners[0].x, max_y = ned_corners[0].y;
	for(int i=1; i<4; i++) {
		min_x = std::min(min_x, ned_corners[i].x);
		max_x = std::max(max_x, ned_corners[i].x);
		min_y = std::min(min_y, ned_corners[i].y);
		max_y = std::max(max_y, ned_corners[i].y);
	}

	std::cout << "PTS\n" << PTS << std::endl;
	std::cout << "PTS2\n" << PTS2 << std::endl;

	/*
	// optional NED to LLA corner coordinate conversion
	Eigen::Vector3d center(lat, lon, alt);
	EigenCoordConverter coordinateConverter(center);
	for(int i=0; i<4; i++) {
		Eigen::Vector3d ned_coordinates(ned_corners[i].x, ned_corners[i].y, alt);
		std::cout << "ned coordinates = " << ned_coordinates << std::endl;
		Eigen::Vector3d lla_coordinates = coordinateConverter.nedToGeodetic(ned_coordinates);
		std::cout << "lla coordinates = " << lla_coordinates << std::endl << std::endl;
	}
	*/

	// Scale the output
	double delta_x = max_x - min_x;
	double delta_y = max_y - min_y;
	double aspect_ratio = delta_y / delta_x;

	float scaled_width = FindImageWidth(lat, delta_x, num_pix_x);
	float scaled_height = scaled_width * aspect_ratio;

	double x_scale = scaled_width / delta_x;
	double y_scale = scaled_height / delta_y;

	// Scale world corners into pixel space
	// Translate so origin is top left
	for(int i = 0; i < 4; i++) {
		world_corners_scaled[i].x = (ned_corners[i].x - min_x)*x_scale;
		world_corners_scaled[i].y = (ned_corners[i].y - min_y)*y_scale;
	}

	// Raw image corners in pixel space
	px_corners[0] = cv::Point2f(0,0);
	px_corners[1] = cv::Point2f(image_raw.cols, 0);
	px_corners[2] = cv::Point2f(image_raw.cols, image_raw.rows);
	px_corners[3] = cv::Point2f(0, image_raw.rows);

	/* Debugging output
	for(int i = 0; i< 4; i++) {
		std::cout << "image corners: " << "\t" << i << "\t\t\t" << image_corners[i] << std::endl;
		std::cout << "px corners: " << "\t" << i << "\t\t\t" << px_corners[i] << std::endl;
		std::cout << "world corners: " << "\t" << i << "\t\t\t" << ned_corners[i] << std::endl;
		std::cout << "world corners scaled = " << i << "\t\t" << world_corners_scaled[i] << std::endl << std::endl;
	}
	*/

	// Apply transformation
	// Transformation must take place between two sets of corners in pixel space
	cv::Mat map_matrix = cv::getPerspectiveTransform(px_corners, world_corners_scaled);

	std::cout << "raw image size = " << image_raw.size() << std::endl;

	cv::Size scaled_size(scaled_width,scaled_height);
	//cv::warpPerspective(image_raw, image_t, map_matrix, image_raw.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar() );
	cv::warpPerspective(image_raw, image_t, map_matrix, scaled_size, cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar() );

	// Cast as vector<Point> for fillConvexPoly()
	std::vector<cv::Point> bounding_rect;
	for(int i=0; i<4; i++)
		bounding_rect.push_back(world_corners_scaled[i]);

	// Build mask
	cv::Mat mask = cv::Mat::zeros(image_t.size(), CV_8UC1);
	cv::fillConvexPoly(mask, bounding_rect, cv::Scalar(255) );

	// Source images for channel mixing
	std::vector<cv::Mat> mixSrc(2);
	mixSrc[0] = image_t;
	mixSrc[1] = mask;

	// Destination image for channel mixing
	image_final = cv::Mat(image_t.size(), image_t.type() );

	// Add transparency mask and output RGBA image
	int from_to[] = { 0,0, 1,1, 2,2, 4,3 };
	cv::mixChannels( mixSrc, image_final, from_to, 4);
	cv::cvtColor(image_final, image_final, cv::COLOR_BGRA2RGBA);

}

float PerspectiveTransform::FindImageWidth(float lat, float gsd, int num_pix_x)
{

	std::cout << "compare to = " << gsd / num_pix_x << std::endl;
	std::cout << "num pix x = " << num_pix_x << std::endl;

	// Only downsample images
	if ( GroundResolution(lat, 24) > (gsd / num_pix_x) ) {
		printf("Image smaller than zoom level 24. Not supported.\n");
		return 0;
	}
	else if ( GroundResolution(lat, 23) > (gsd / num_pix_x) ) {
		printf("Zoom level 24.\n");
		this->zoom = 24;
	}
	else if ( GroundResolution(lat, 22) > (gsd / num_pix_x) ) {
		printf("Zoom level 23.\n");
		this->zoom = 23;
	}
	else if ( GroundResolution(lat, 21) > (gsd / num_pix_x) ) {
		printf("Zoom level 22.\n");
		this->zoom = 22;
	}
	else if ( GroundResolution(lat, 20) > (gsd / num_pix_x) ) {
		printf("Zoom level 21.\n");
		this->zoom = 21;
	}
	else if ( GroundResolution(lat, 19) > (gsd / num_pix_x) ) {
		printf("Zoom level 20.\n");
		this->zoom = 20;
	}
	else if ( GroundResolution(lat, 18) > (gsd / num_pix_x) ) {
		printf("Zoom level 19.\n");
		this->zoom = 19;
	}
	else if ( GroundResolution(lat, 17) < (gsd / num_pix_x) ) {
		printf("Image larger than zoom level 17. Not supported.\n");
		return 0;
	}

	std::cout << "gsd = " << gsd << std::endl;

	// zoom level does not sync with QGC
	float scaled_width = gsd / GroundResolution(lat, zoom);

	std::cout << "scaled width = " << scaled_width << std::endl;

	return scaled_width;

}

float PerspectiveTransform::GroundResolution(float lat, int zoom)
{
	float ground_res;
	ground_res = ( 2 * M_PI * 6378137 * cos( lat*M_PI / 180) ) / (256 * pow(2, zoom));

	std::cout << "zoom = " << zoom << "\tres = " << ground_res << std::endl;

	return ground_res;
}

bool PerspectiveTransform::CheckValues()
{
	if ( this->lat == 0)
		return false;
	
	if ( this->lon == 0)
		return false;

	if ( this->alt == 0)
		return false;

	if ( this->focal_length == 0)
		return false;

	if ( this->pixel_size == 0)
		return false;

	if ( this->num_pix_x == 0)
		return false;

	if ( this->num_pix_y == 0)
		return false;

	return true;
}