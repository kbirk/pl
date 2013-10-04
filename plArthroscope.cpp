#include "plArthroscope.h"


plArthroscope::plArthroscope() 
:     mydeque(5)
{ 
    weights = {0.2, 0.2, 0.2, 0.2, 0.2};
    myCount = 0;
    CAMERA_DIAMETER = ARTHRO_DIAM;
    TEXTURE_SCALE_FACTOR = 0.1;
    TEXTURE_SIZE = ARTHRO_DIAM;
    CAMERA_RADIUS = ARTHRO_DIAM/2.;

    capture = cvCreateCameraCapture(0);
    if (!capture) 
    {
        std::cerr << "Error: Unable to read from camera - aborting" << std::endl;
        exit(1);
    }

    // image distortion stuff
    mapx = cvCreateImage(cvSize(CAPTUREWIDTH, CAPTUREHEIGHT), IPL_DEPTH_32F, 1);
    mapy = cvCreateImage(cvSize(CAPTUREWIDTH, CAPTUREHEIGHT), IPL_DEPTH_32F, 1);
    intrinsics = (CvMat*) cvLoad("./data/matrices/Intrinsics.xml");
    distortion = (CvMat*) cvLoad("./data/matrices/Distortion.xml");
    std::cout << "Intrinsics and Distortion matrices loaded successfully." << std::endl;

    image = cvCreateImage(cvSize(CAPTUREWIDTH, CAPTUREHEIGHT), COLORDEPTH, NUMCHANNELS); // added September 12 2011

    WIDTH   = CAPTUREWIDTH;
    HEIGHT  = CAPTUREHEIGHT;
    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            plIntrinsics(row, col) = cvGetReal2D( intrinsics, row, col );
        }
    }

}

plArthroscope::~plArthroscope() 
{
    cvReleaseCapture( &capture );
    cvReleaseImage  ( &image   );
    delete [] img;
}

const PLchar* plArthroscope::getImage() const
{
    return img;
}

const plMatrix44& plArthroscope::getIntrinsics() const
{
    return plIntrinsics;
}

void plArthroscope::updateImage( PLuint imageManipulation )
{
    // setting up the undistort map is really slow, we want to do it only once
    static bool firstTime = true;
    if (firstTime) 
    {
        cvInitUndistortMap( intrinsics, distortion, mapx, mapy ); // SLOW SLOW SLOW
        firstTime = false;
    }

    frame = cvQueryFrame( capture );
    image = frame;


    // undistortion step
    if ( imageManipulation == CAMERA_IMAGE_UNDISTORT && previousRadius>0 )
    {
        callCircle();

//        if (previousRadius>0)
//        {
            IplImage *t = cvCloneImage( image );
            cvRemap( t, image, mapx, mapy ); // undistort image
            cvReleaseImage( &t );


            cv::Point center(xCenter,yCenter);
            cv::Mat dst;
            // center and radius are the results of HoughCircle
            // mask is a CV_8UC1 image with 0
            cv::Mat mask = cv::Mat::zeros( frameMatrix.rows, frameMatrix.cols, CV_8UC1 );
            circle( mask, center, previousRadius, cv::Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
            frameMatrix.copyTo( dst, mask ); // copy values of img to dst if mask is > 0.
            dst.copyTo(frameMatrix);
            // circle( frameMatrix, center, previousRadius, Scalar(0,255,00), 3, 8, 0 );
//        }
    }
    else
    {
         callCircle();
    }

    img    = image->imageData;
    WIDTH  = image->width;
    HEIGHT = image->height;

}


void plArthroscope::callCircle()
 {
        cv::Mat src_gray;
        frameMatrix = image;

        cvtColor( frameMatrix, src_gray, CV_BGR2GRAY ); // Convert it to gray

        GaussianBlur( src_gray, src_gray, cv::Size(9, 9), 2, 2 );  // Reduce the noise so we avoid false circle detection

        // Apply the Hough Transform to find the circles
        std::vector<cv::Vec3f> circles;

        HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 2, src_gray.rows/8, 200, 100, 356/*356*/, 359/*356*/);

        if (circles.size() == 0){
             cv::Point previousCenter(xCenter, yCenter);
          //    circle( frameMatrix, previousCenter, previousRadius, cv::Scalar(0,0,255), 3, 8, 0 ); // circle outline
             if (previousRadius > 0){
                 cv::Point rectangleLeftpt(previousCenter.x-previousRadius,previousCenter.y-previousRadius);
                 cv::Point rectangleRighttpt(previousCenter.x+previousRadius,previousCenter.y+previousRadius);
            }
         }

        else{
            //  Draw the circles detected
            for( size_t i = 0; i < circles.size(); i++ )
            {
                cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));

                mydeque.push_front(center);
                if(myCount >= LIMIT)
                {
                    double finalX, finalY;
                    for(int i = 0; i<=LIMIT; i++)
                    {
                       finalX = mydeque.at(i).x *weights[i] + finalX;
                       finalY = mydeque.at(i).y *weights[i] + finalY;
                    }
                    xCenter = finalX;
                    yCenter = finalY;
                }
                else
                {
                    xCenter = cvRound(circles[i][0]);
                    yCenter = cvRound(circles[i][1]);
                    myCount = myCount +1;
                }

                int radius = cvRound(circles[i][2]);

            //    circle( frameMatrix, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );

                cv::Point rectangleLeftpt(xCenter-radius,yCenter-radius);
                cv::Point rectangleRighttpt(xCenter+radius,yCenter+radius);

                previousRectangleLeftptX = rectangleLeftpt.x;
                previousRectangleLeftptY = rectangleLeftpt.y;
                previousRectangleRightptX = rectangleRighttpt.x;
                previousRectangleRightptY = rectangleRighttpt.y;
                previousRadius = radius;
             }


        }
 }
