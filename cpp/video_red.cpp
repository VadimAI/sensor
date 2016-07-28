#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;


int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat hsv_image;
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    Mat red_hue_image;

    namedWindow("line laser",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, hsv_image, COLOR_BGR2HSV);

        inRange(hsv_image, Scalar(0,100,100), Scalar(10,255,255), lower_red_hue_range);
        inRange(hsv_image, Scalar(160,100,100), Scalar(179,255,255), upper_red_hue_range);

        addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

        imshow("line laser", red_hue_image);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

