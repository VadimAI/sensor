#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat hsv_image;
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    Mat red_hue_image;
    Mat cdst;

    namedWindow("line laser",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, hsv_image, COLOR_BGR2HSV);

        inRange(hsv_image, Scalar(0,100,100), Scalar(10,255,255), lower_red_hue_range);
        inRange(hsv_image, Scalar(160,100,100), Scalar(179,255,255), upper_red_hue_range);

        addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

        vector<Vec4i> lines;
        HoughLinesP(red_hue_image, lines, 1, CV_PI/180, 50, 50, 10);
        cvtColor(red_hue_image, cdst, COLOR_GRAY2BGR);

        for (int i = 0; i < lines.size(); i++)
        {
        	Vec4i l = lines[i];
        	line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
        }

        imshow("line laser", cdst);
        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}

