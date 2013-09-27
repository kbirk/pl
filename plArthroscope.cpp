#include "plArthroscope.h"

              
plArthroscope::plArthroscope() 
{ 
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
    cvResize( frame, image, 0 );

    // undistortion step
    if ( imageManipulation == CAMERA_IMAGE_UNDISTORT )
    {
        IplImage *temp;
        temp = cvCloneImage( image );
        cvRemap( temp, image, mapx, mapy ); // undistort image
        cvReleaseImage( &temp );
    }

    img    = image->imageData;
    WIDTH  = image->width;
    HEIGHT = image->height;

}
