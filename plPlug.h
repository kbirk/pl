#ifndef __PL_PLUG_H__
#define __PL_PLUG_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"
#include "plTransform.h"

class plPlug
{
    public:

        plPlug();

        void translate  ( const plVector3 &translation );
        void translateX ( PLfloat distance, const plVector3 &planeNormal );
        void translateZ ( PLfloat distance, const plVector3 &planeNormal );
        void rotate     ( const plVector3 &axis, PLfloat angle_degrees );
        
        const plBoneAndCartilage  *model;
        PLuint                     modelID;
        plTransform                transform; 

    private:
    
};

#endif
