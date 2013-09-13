#ifndef __PL_CHECKERBOARD_H__
#define __PL_CHECKERBOARD_H__

#include "plCommon.h"
#include "plRenderable.h"
#include "plRenderingPipeline.h"
#include "plTransform.h"
#include "plColourMesh.h"

class plCheckerBoard : public plRenderable
{
    public:

        plTransform transform;

        plCheckerBoard( PLfloat blocksize );
        
        void   toggleVisibility  ();
        void   toggleTransparency();
        
        void draw() const;

    private:

        PLbool _isTransparent;

        plColourMesh _mesh;
        
        void _generate( PLfloat blocksize ); 

};  

bool readCheckerBoardCalib(plVector3 &origin, plVector3 &xAxis, plVector3 &yAxis);


#endif
