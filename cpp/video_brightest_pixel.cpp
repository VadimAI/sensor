#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/calib3d.hpp>
#include <iostream>
#include <math.h>
#include <fstream>

#define PI 3.14159265

using namespace cv;
using namespace std;



int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;

Mat rawImage, greyImage, cannyImage;
Mat detected_edges;

// Main print method which includes the switch for types
void printMat(const Mat& M){  //http://stackoverflow.com/questions/6479154/read-pixel-value-in-c-opencv
        switch ( (M.dataend-M.datastart) / (M.cols*M.rows*M.channels())){

        case sizeof(char):
             cout << "char" << endl;
             break;
        case sizeof(float):
        cout << "float" << endl;
             break;
        case sizeof(double):
        cout << "double" << endl;
             break;
        }
    }

/////////////////////////////////////////////////


void CannyThreshold(int, void*)  //http://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
{
  /// Reduce noise with a kernel 3x3
  blur( greyImage, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
    cannyImage = Scalar::all(0);

  rawImage.copyTo( cannyImage, detected_edges);
  imshow( "canny Image", cannyImage );
 }


std::vector<float> FindLineCentroid(const Mat& image)
{
    std::vector<float> result(image.cols,0);

    for ( int i = 0; i < image.cols ; i++)
    {
        float max = 0;
        float maxIsAtY = image.rows/2;

        for ( int j = 0; j < image.rows; j++)
        {
            float dotSum = image.at<Vec3b>(j,i)[0]+ image.at<Vec3b>(j,i)[1]+image.at<Vec3b>(j,i)[2];

            if ( dotSum > max)  //finds the x that corresponds to the max{R+G+B}
            {
                max = dotSum;
                maxIsAtY = j;
            }
        }

        result[i] = maxIsAtY;
    }
    return result;
}

int main(int argc, char* argv[])
{
    bool isUSBCameraThere = true;
    VideoCapture cap;

    if ( argc == 1  )
    {
        VideoCapture testCam(1);

        if(!testCam.isOpened())  // check if we succeeded
            isUSBCameraThere = false;

        testCam.~VideoCapture();

        cap.open( isUSBCameraThere ? 1 : 0);// open the default camera
        cout << ( isUSBCameraThere ? "USB Camera" : "WebCam" ) << endl;
    }
    else
    {
        cout << "Loading from file" << endl;

        cap.open( argv[1] );
        //cap.open( "/home/peter/workspace/EdgeDetection/Debug/input_video.avi");

        if ( cap.isOpened() == false )
        {
            cout << "Can't open file with that name" << endl;
            return -1;
        }

        const float WindowSize = 0.5;
        namedWindow("raw image", WindowSize );
        namedWindow("grey image", WindowSize );
        //namedWindow("canny Image", WindowSize);
        namedWindow("line centroid", WindowSize);

        int currentFrame = 0;
        createTrackbar( "threshold", "canny Image", &lowThreshold, max_lowThreshold, CannyThreshold );
        createTrackbar( "video position", "raw image", &currentFrame,  cap.get(CV_CAP_PROP_FRAME_COUNT) );

        Mat imageWithLine;
        const float f = 3.6; // focal length, [mm]
        const float s = 68.0; // length between laser and imager, [mm]
        const float pxl_size = 0.00375; // pixel size, [mm / pixel]

        ofstream myfile;
        myfile.open("dist.txt");

        ofstream myfile2;
        myfile2.open("angle.txt");

        while ( waitKey(30) < 0 && cap.isOpened() )
        {
            cap >> rawImage;

            //updates video position
            setTrackbarPos( "video position", "raw image", cap.get(CV_CAP_PROP_POS_FRAMES) );

             if (cap.get(CV_CAP_PROP_FRAME_COUNT) <= cap.get(CV_CAP_PROP_POS_FRAMES) )
             {
                 cap.release();
                 cap.open(argv[1]);
             }

            imshow("raw image", rawImage);
            rawImage.copyTo(imageWithLine);

            std::vector<float> lineCentroid = FindLineCentroid(rawImage);
            std::vector<float> dist(imageWithLine.cols,0);
            std::vector<float> angle(imageWithLine.cols,0);

            //draws lineCentroid into imageWithLine and computes distance
            for ( int i = 0; i < lineCentroid.size()-1; i++ )
            {
            	dist[i] = (f * s) / ((imageWithLine.rows - lineCentroid[i]) * pxl_size);
                int thickness = 2;
                  int lineType = 8;
                  line( imageWithLine,
                        Point(i,lineCentroid[i]) ,
                        Point(i+1,lineCentroid[i+1]),
                        Scalar( 255 , 0, 0 ),
                        thickness,
                        lineType );


                angle[i] = atan(dist[i]/2*f) * 180 / PI;
	            cout << "dist " << i << " : " << dist[i] << endl;
	            cout << "angle " << i << " : " << angle[i] << endl;
	            myfile << dist[i] << ", ";
	            myfile2 << angle[i] <<", ";
            }

            myfile << endl;
            myfile2 << endl;


            imshow("line centroid", imageWithLine);
        }

        myfile.close();
        myfile2.close();
        cap.release();
        return -1;
    }
}

