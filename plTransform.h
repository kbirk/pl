#ifndef __PL_TRANSFORM_H__
#define __PL_TRANSFORM_H__

#include "pl.h"
#include "plVector3.h"

class plTransform 
{
    public:

        PLfloat transform[16];
        plVector3 x,y,z;
        plVector3 origin; 

        plTransform();

        void compute();

        void apply() const;

        plVector3 applyInverse( const plVector3 &v ) const;
        plVector3 applyNormalInverse( const plVector3 &v ) const;
        PLfloat squaredDistToAxis( const plVector3 &v ) const;
        PLfloat projectedDistOnAxis( const plVector3 &v ) const;
};

#endif 
