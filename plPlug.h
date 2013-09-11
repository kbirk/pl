#ifndef __PL_PLUG_H__
#define __PL_PLUG_H__

#include "plCommon.h"
#include "plModelSpecific.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"
#include "plTransform.h"

class plPlug : public plModelSpecific
{
    public:

        plTransform transform; 

        plPlug();
        plPlug( PLuint _modelID, const plBoneAndCartilage &_model, const plTransform &t );

        void translate  ( const plVector3 &translation );
        void translateX ( PLfloat distance, const plVector3 &planeNormal );
        void translateZ ( PLfloat distance, const plVector3 &planeNormal );
        void rotate     ( const plVector3 &axis, PLfloat angleDegrees );
           
};

#endif
