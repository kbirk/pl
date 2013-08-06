#include "plArthroscope.h"

plArthroscope::plArthroscope() 
{ 

    CAMERA_DIAMETER = ARTHRO_DIAM;
    TEXTURE_SCALE_FACTOR = 0.1;
    TEXTURE_SIZE = ARTHRO_DIAM ;//* (1 + TEXTURE_SCALE_FACTOR);
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
//    img = new PLchar[ TEXTURE_SIZE  * TEXTURE_SIZE  * 3 ];
    
//    for (int i = 0; i < TEXTURE_SIZE  * TEXTURE_SIZE  * 4; i+=4)
//    {
//        img[i  ] = 0;
//        img[i+1] = 255;
//        img[i+2] = 0;
//        img[i+3] = 255;
//    }

}

plArthroscope::~plArthroscope() 
{
    cvReleaseCapture(&capture);
    cvReleaseImage(&image); // prevent memory leaks...
    delete [] img;
}

PLchar *plArthroscope::getImage()
{
    return &img[0];
}

CvMat* plArthroscope::getIntrinsics() 
{ 
    return intrinsics; 
}

void plArthroscope::updateImage(ImageManipulation im)
{
    // setting up the undistort map is really slow, we want to do it only once
    static bool firstTime = true;
    if (firstTime) 
    {
        cvInitUndistortMap(intrinsics, distortion, mapx, mapy); // SLOW SLOW SLOW
        firstTime = false;
        std::cout << 63 << std::endl;
    }

    frame = cvQueryFrame(capture);

    cvResize(frame, image, 0);
    image->origin = frame->origin;

    // undistortion step
    if (im == CAMERA_IMAGE_UNDISTORT)
    {
        static IplImage *temp;
        temp = cvCloneImage(image);
        cvRemap(temp, image, mapx, mapy); // undistort image
        cvReleaseImage(&temp);
    }

    int image_center_x = image->width/2;
    int image_center_y = image->height/2;

    int texture_center = TEXTURE_SIZE/2;
    int texture_offset_y = texture_center - CAMERA_RADIUS;
    int texture_offset_x = texture_center - CAMERA_RADIUS * (1 + TEXTURE_SCALE_FACTOR);

    int colOffset = image_center_x - CAMERA_RADIUS * (1 + TEXTURE_SCALE_FACTOR / 2) ;
    int rowOffset = image_center_y - CAMERA_RADIUS;

    img = image->imageData;
    WIDTH = image->width;
    HEIGHT = image->height;

/*
    PLbyte *imgPtr = &img[0];

    for (int i = 0; i < (texture_offset_y * TEXTURE_SIZE + texture_offset_x); i++)
    {
        *imgPtr++ = 0;
        *imgPtr++ = 0;
        *imgPtr++ = 0;
        *imgPtr++ = 255;
    }

    for (int row = 0; row < TEXTURE_SIZE ; row++)
    {
        PLbyte *cvPtr = (PLbyte*) &image->imageData[(row + rowOffset) * image->widthStep + colOffset*3];

        for (int col = 0; col < TEXTURE_SIZE ; col++)
        {

            if (( row + rowOffset < 0 ) || ( row + rowOffset >= image->height ))
            {
                *imgPtr++ = 0;
                *imgPtr++ = 0;
                *imgPtr++ = 0;
                *imgPtr++ = 255;
                cvPtr += 3;
            }
            else
            {
                *imgPtr++ = *cvPtr++;
                *imgPtr++ = *cvPtr++;
                *imgPtr++ = *cvPtr++;
                *imgPtr++ = 255;
            }
        }
    }
    */
}
