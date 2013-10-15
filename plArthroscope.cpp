#include "plArthroscope.h"

/*
        plMatrix44 aIntrin = arthroscope.getIntrinsics();
        float aWidth    = arthroscope.width;
        float aHeight   = arthroscope.height;
        float x0        = 0.0f;
        float y0        = 0.0f;
        float zNear     = 10.0f;
        float zFar      = 15000.0f;

        plMatrix44 arthroProj(2 * aIntrin(0,0) / aWidth,-2 * aIntrin(0,1) / aWidth , ( aWidth  - 2 * aIntrin(0,2) + 2 * x0) / aWidth , 0        ,
                              0                        , 2 * aIntrin(1,1) / aHeight, (-aHeight + 2 * aIntrin(1,2) + 2 * y0) / aHeight, 0        ,
                              0                        , 0                         , (-zFar - zNear)/(zFar - zNear)                  ,-2*zFar*zNear/(zFar - zNear),
                              0                        , 0                         ,-1                                               , 0        );
*/

plArthroscope::plArthroscope() 
//:    interpolationDeque( INTERPOLATION_LIMIT )
{ 
//    weights = { INTERPOLATION_WEIGHTS };
    cameraDiameter = ARTHRO_DIAM;
    textureScaleFactor = 0.1;
    textureSize = ARTHRO_DIAM;
    cameraRadius = ARTHRO_DIAM/2.;

    capture = cvCreateCameraCapture(0);
    if (!capture) 
    {
        std::cerr << "Error: Unable to read from camera - aborting" << std::endl;
        exit(1);
    }

    // image distortion stuff
    mapx = cvCreateImage(cvSize(CAPTURE_WIDTH, CAPTURE_HEIGHT), IPL_DEPTH_32F, 1);
    mapy = cvCreateImage(cvSize(CAPTURE_WIDTH, CAPTURE_HEIGHT), IPL_DEPTH_32F, 1);
    intrinsics = (CvMat*) cvLoad("./data/matrices/Intrinsics.xml");
    distortion = (CvMat*) cvLoad("./data/matrices/Distortion.xml");
    std::cout << "Intrinsics and Distortion matrices loaded successfully." << std::endl;

    image = cvCreateImage(cvSize(CAPTURE_WIDTH, CAPTURE_HEIGHT), COLOUR_DEPTH, NUM_CHANNELS); // added September 12 2011

    width   = CAPTURE_WIDTH;
    height  = CAPTURE_HEIGHT;
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
    if ( imageManipulation == CAMERA_IMAGE_UNDISTORT )
    {
//        callCircle();

//        if ( radius>0 )
//        {
            IplImage *t = cvCloneImage( image );
            cvRemap( t, image, mapx, mapy ); // undistort image
            cvReleaseImage( &t );

//            cv::Point center(xCenter,yCenter);
//            cv::Mat dst;
//            // center and radius are the results of HoughCircle
//            // mask is a CV_8UC1 image with 0
//            cv::Mat mask = cv::Mat::zeros( frameMatrix.rows, frameMatrix.cols, CV_8UC1 );
//            circle( mask, center, radius, cv::Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
//            frameMatrix.copyTo( dst, mask ); // copy values of img to dst if mask is > 0.
//            dst.copyTo(frameMatrix);
            // circle( frameMatrix, center, radius, Scalar(0,255,00), 3, 8, 0 );
//        }
    }

    img    = image->imageData;
    width  = image->width;
    height = image->height;

}


//void plArthroscope::callCircle()
// {
//        cv::Mat src_gray;
//        frameMatrix = image;
//
//        cvtColor( frameMatrix, src_gray, CV_BGR2GRAY ); // Convert it to gray
//
//        cv::GaussianBlur( src_gray, src_gray, cv::Size(9, 9), 2, 2 );  // Reduce the noise so we avoid false circle detection
//
//        // Apply the Hough Transform to find the circles
//        std::vector<cv::Vec3f> circles;
//
//        cv::HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 2, src_gray.rows/8, 200, 100, 356, 359 );
//
//        //  Draw the circles detected
//        for( size_t i = 0; i < circles.size(); i++ )
//        {
//            cv::Point center( cvRound(circles[i][0]), cvRound(circles[i][1]) );
//
//            if(  interpolationDeque.size() == INTERPOLATION_LIMIT )
//                interpolationDeque.pop_back();        // pop off back if at limit
//                
//            // add newest point  
//            interpolationDeque.push_front( center );
//                
//            float finalX = 0;
//            float finalY = 0;
//            for(int i = 0; i<interpolationDeque.size(); i++)
//            {
//                finalX += interpolationDeque[i].x * weights[i];
//                finalY += interpolationDeque[i].y * weights[i];
//            }
//
//            xCenter = finalX;
//            yCenter = finalY;
//            radius = cvRound( circles[i][2] );
//        }
// }
