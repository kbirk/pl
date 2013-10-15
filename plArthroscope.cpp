#include "plArthroscope.h"


plArthroscope::plArthroscope() 
    :    _interpolationDeque( INTERPOLATION_LIMIT )
{ 
//   _weights = { INTERPOLATION_WEIGHTS };

    _capture = cvCreateCameraCapture(0);
    if ( !_capture ) 
    {
        std::cerr << "Error: Unable to read from camera - aborting" << std::endl;
        exit(1);
    }

    // image distortion stuff
    _mapx = cvCreateImage( cvSize( CAPTURE_WIDTH, CAPTURE_HEIGHT ), IPL_DEPTH_32F, 1 );
    _mapy = cvCreateImage( cvSize( CAPTURE_WIDTH, CAPTURE_HEIGHT ), IPL_DEPTH_32F, 1 );
    _intrinsicsCV = (CvMat*) cvLoad("./data/matrices/Intrinsics.xml");
    _distortion   = (CvMat*) cvLoad("./data/matrices/Distortion.xml");
    std::cout << "Intrinsics and Distortion matrices loaded successfully." << std::endl;

    _image = cvCreateImage( cvSize( CAPTURE_WIDTH, CAPTURE_HEIGHT ), COLOUR_DEPTH, NUM_CHANNELS ); // added September 12 2011

    for (int row = 0; row < 3; row++)
    {
        for (int col = 0; col < 3; col++)
        {
            _intrinsicsPL(row, col) = cvGetReal2D( _intrinsicsCV, row, col );
        }
    }

}


plArthroscope::~plArthroscope() 
{
    cvReleaseCapture( &_capture );
    cvReleaseImage  ( &_image   );
}


const PLchar* plArthroscope::image() const
{
    return _image->imageData;
}


PLuint plArthroscope::width() const
{
    return _image->width;
}


PLuint plArthroscope::height() const
{
    return _image->height;
}


const plMatrix44& plArthroscope::intrinsics() const
{
    return _intrinsicsPL;
}


plMatrix44 plArthroscope::getProjectionMatrix() const
{
    const PLfloat x0        = 0.0f;
    const PLfloat y0        = 0.0f;
    const PLfloat zNear     = 10.0f;
    const PLfloat zFar      = 15000.0f;
    const PLfloat w         = (PLfloat) width();
    const PLfloat h         = (PLfloat) height();

    return plMatrix44 (2 * _intrinsicsPL(0,0) / w,-2 * _intrinsicsPL(0,1) / w , ( w  - 2 * _intrinsicsPL(0,2) + 2 * x0) / w , 0        ,
                       0                         , 2 * _intrinsicsPL(1,1) / h , (-h + 2 * _intrinsicsPL(1,2) + 2 * y0) / h  , 0        ,
                       0                         , 0                          , (-zFar - zNear)/(zFar - zNear)              ,-2*zFar*zNear/(zFar - zNear),
                       0                         , 0                          ,-1                                           , 0        );
}


void plArthroscope::updateImage( PLuint imageManipulation )
{
    // setting up the undistort map is really slow, we want to do it only once
    static bool firstTime = true;
    if (firstTime) 
    {
        cvInitUndistortMap( _intrinsicsCV, _distortion, _mapx, _mapy ); // SLOW SLOW SLOW
        firstTime = false;
    }

    _frame = cvQueryFrame( _capture );
    _image = _frame;

    // undistortion step
    if ( imageManipulation == CAMERA_IMAGE_UNDISTORT )
    {
//        _callCircle();
//        if ( _radius > 0 )
//        {
            IplImage *t = cvCloneImage( _image );
            cvRemap( t, _image, _mapx, _mapy ); // undistort image
            cvReleaseImage( &t );
            
            /*
            cv::Point center( _xCenter, _yCenter);
            cv::Mat dst;
            // center and radius are the results of HoughCircle
            // mask is a CV_8UC1 image with 0
            cv::Mat mask = cv::Mat::zeros( _frameMatrix.rows, _frameMatrix.cols, CV_8UC1 );
            circle( mask, center, _radius, cv::Scalar(255,255,255), -1, 8, 0 ); //-1 means filled
            _frameMatrix.copyTo( dst, mask ); // copy values of img to dst if mask is > 0.
            dst.copyTo(frameMatrix);
            // circle( _frameMatrix, center, _radius, Scalar(0,255,00), 3, 8, 0 );
            */
//        }
    }

}

/*
void plArthroscope::_callCircle()
{       
    _frameMatrix = _image;
    
    // convert to grayscale
    cv::Mat grayScale;
    cvtColor( _frameMatrix, grayScale, CV_BGR2GRAY );

    // Reduce the noise so we avoid false circle detection
    cv::GaussianBlur( grayScale, grayScale, cv::Size(9, 9), 2, 2 );  

    // Apply the Hough Transform to find the circles
    std::vector<cv::Vec3f> circles;
    cv::HoughCircles( grayScale, circles, CV_HOUGH_GRADIENT, 2, grayScale.rows/8, 200, 100, 356, 359 );

    //  Draw the circles detected
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center( cvRound( circles[i][0] ), cvRound( circles[i][1] ) );

        if ( interpolationDeque.size() == INTERPOLATION_LIMIT )
        {
            interpolationDeque.pop_back();        // pop off back if at limit
        }
            
        // add newest point  
        interpolationDeque.push_front( center );
            
        PLfloat finalX = 0;
        PLfloat finalY = 0;
        for( PLuint i = 0; i<interpolationDeque.size(); i++ )
        {
            finalX += interpolationDeque[i].x * _weights[i];
            finalY += interpolationDeque[i].y * _weights[i];
        }

        _xCenter = finalX;
        _yCenter = finalY;
        _radius = cvRound( circles[i][2] );
    }
}
*/
