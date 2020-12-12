#include <opencv2/opencv.hpp>

class Settings
{

public:
    enum Pattern { NOT_EXISTING, CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };
    enum InputType {INVALID, CAMERA, VIDEO_FILE, IMAGE_LIST};

    void write(cv::FileStorage& fs);
    void read(const cv::FileNode& node);
    void interprate();
    cv::Mat nextImage();
    static bool readStringList( const std::string& filename, std::vector<std::string>& l );
    static bool isListOfImages( const std::string& filename);

    std::string getRawImage() {return imageList[atImageList-1]; };

public:
    cv::Size boardSize;            // The size of the board -> Number of items by width and height
    Pattern calibrationPattern;// One of the Chessboard, circles, or asymmetric circle pattern
    float squareSize;          // The size of a square in your defined unit (point, millimeter,etc).
    int nrFrames;              // The number of frames to use from the input for calibration
    float aspectRatio;         // The aspect ratio
    int delay;                 // In case of a video input
    bool bwritePoints;         //  Write detected feature points
    bool bwriteExtrinsics;     // Write extrinsic parameters
    bool calibZeroTangentDist; // Assume zero tangential distortion
    bool calibFixPrincipalPoint;// Fix the principal point at the center
    bool flipVertical;          // Flip the captured images around the horizontal axis
    std::string outputFileName;      // The name of the file where to write
    bool showUndistorsed;       // Show undistorted images after calibration
    std::string input;               // The input ->


    int cameraID;
    std::vector<std::string> imageList;
    int atImageList;
    cv::VideoCapture inputCapture;
    InputType inputType;
    bool goodInput = false;
    int flag;

private:
    std::string patternToUse;

};