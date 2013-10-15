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

        plArthroscope();
        ~plArthroscope();

        const PLchar*      image()      const;
        const plMatrix44&  intrinsics() const;
        PLuint             width()      const;
        PLuint             height()     const;
        plMatrix44         getProjectionMatrix() const;
        
        void  updateImage( PLuint imageManipulation );
                
    private:
    
        CvCapture *_capture;
        IplImage  *_image;
        IplImage  *_frame;

        // for image distortion
        IplImage* _mapx;
        IplImage* _mapy;

        //Input camera intrinsics
        CvMat*     _intrinsicsCV;
        plMatrix44 _intrinsicsPL;
        CvMat*     _distortion;
        
        // Variables for circle tracking and undistortion function
        std::deque<cv::Point> _interpolationDeque;
        std::vector<PLfloat>  _weights;
//        cv::Mat _frameMatrix;
        PLfloat _xCenter, _yCenter;
        PLuint _radius;
        
        void _callCircle();

};


#endif
