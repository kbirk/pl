#ifndef _PL_ARTHROSCOPE_H__
#define _PL_ARTHROSCOPE_H__

#include "headers.h"
#include "matOps.h"
#include <opencv.hpp>

#include "plCommon.h"

// set the camera dimensions here
#define CAPTURE_WIDTH           1280
#define CAPTURE_HEIGHT          720
#define COLOUR_DEPTH            8         // 8 bit color
#define NUM_CHANNELS            3         // 3 color channels (RGB)
#define ARTHRO_DIAM	            720
#define INTERPOLATION_LIMIT     4
#define INTERPOLATION_WEIGHTS   0.2, 0.2, 0.2, 0.2, 0.2


enum ImageManipulation
{ 
    CAMERA_IMAGE_RAW, 
    CAMERA_IMAGE_UNDISTORT 
};


class plArthroscope 
{


    public:

        double  cameraRadius;
        int     cameraDiameter;
        int     textureSize, width, height;
        float   textureScaleFactor;

        plArthroscope();
        ~plArthroscope();

        const PLchar*      getImage()      const;
        const plMatrix44&  getIntrinsics() const;
        void  updateImage( PLuint imageManipulation );
        
        
    private:
    
        CvCapture *capture;
        IplImage  *image;
        IplImage  *frame;
        PLchar    *img;

        // for image distortion
        IplImage* mapx;
        IplImage* mapy;

        //Input camera intrinsics
        CvMat* intrinsics;
        CvMat* distortion;

        plMatrix44 plIntrinsics;

        // Variables for circle tracking and undistortion function

//        std::deque<cv::Point> interpolationDeque;
//        std::vector<float> weights;
//        cv::Mat frameMatrix;
        float xCenter, yCenter;
        int radius;
        
        void callCircle();

};


#endif
