#ifndef PL_DEBUG_TRANSFORM_H
#define PL_DEBUG_TRANSFORM_H

#include "plCommon.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plDraw.h"

class plDebugTransform
{

    public:

        plDebugTransform( const plVector3 &x, const plVector3 &y, const plVector3 &origin );

        void draw() const;
       
    private:
                    
        plVector3 _origin;
        plVector3 _x;
        plVector3 _y;
        plVector3 _z;         

};

#endif
