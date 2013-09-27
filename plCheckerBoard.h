#ifndef __PL_CHECKERBOARD_H__
#define __PL_CHECKERBOARD_H__

#include "plCommon.h"
#include "plRenderable.h"
#include "plRenderingPipeline.h"
#include "plTransform.h"
#include "plColourMesh.h"
#include "plDRBTransform.h"

class plCheckerBoard : public plRenderable
{

    public:
    
        plCheckerBoard( PLfloat blocksize );
        
        void toggleVisibility  ();
        void toggleTransparency();
        
        void updateTransform( const plDRBTransform &currentFemurDRBToWorld, const plDRBTransform &femurDRBToFemurSTL );
        
        void draw() const;

    private:

        PLbool       _isTransparent;
        plColourMesh _mesh;
        plTransform  _transform;
        
        plVector3   _calibOrigin, 
                    _calibXAxisPoint, 
                    _calibYAxisPoint;
        
        void _generate( PLfloat blocksize ); 
        bool _readCheckerBoardCalib();

};  




#endif
