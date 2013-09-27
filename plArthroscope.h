#ifndef _PL_ARTHROSCOPE_H__
#define _PL_ARTHROSCOPE_H__

#include "headers.h"
#include "matOps.h"
#include <opencv.hpp>

#include "plCommon.h"

// set the camera dimensions here
#define CAPTUREWIDTH    1280
#define CAPTUREHEIGHT   720
#define COLORDEPTH      8         // 8 bit color
#define NUMCHANNELS     3         // 3 color channels (RGB)
#define ARTHRO_DIAM	    720

enum ImageManipulation
{ 
    CAMERA_IMAGE_RAW, 
    CAMERA_IMAGE_UNDISTORT 
};

class plArthroscope 
{

    public:

        double  CAMERA_RADIUS;
        int     CAMERA_DIAMETER;
        int     TEXTURE_SIZE, WIDTH, HEIGHT;
        float   TEXTURE_SCALE_FACTOR;

        plArthroscope();
        ~plArthroscope();

        const PLchar*      getImage()      const;
        const plMatrix44&  getIntrinsics() const;
        
        void updateImage( PLuint imageManipulation );

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
};


#endif
