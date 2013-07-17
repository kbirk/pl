#ifndef __PL_POLYGON_H__
#define __PL_POLYGON_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"

class plPolygon 
{

    public:
    
        plSeq<plVector3> points;
        plVector3        normal;

};

std::ostream& operator << ( std::ostream &stream, const plPolygon &p );

#endif
