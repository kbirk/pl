#ifndef __PL_WINDOW_H__
#define __PL_WINDOW_H__

#include "plCommon.h"
#include "plMatrix44.h"
#include "plRenderingPipeline.h"

class plWindow
{
    public:                    

        static void      reshape       ( PLuint width, PLuint height );
        static plVector3 mouseToWorld  ( PLint x, PLint y, PLint z );
        static plVector3 worldToScreen ( PLfloat x, PLfloat y, PLfloat z );
        static void      mouseToRay    ( plVector3 &rayOrigin, plVector3 &rayDirection, PLint x, PLint y ); 

	private:
				
        plWindow() {};
				
};

#endif
