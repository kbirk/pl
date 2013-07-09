#ifndef __PL_POLYGON_H__
#define __PL_POLYGON_H__

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"

class plPolygon {

  public:
    plPolygon() {}

    plSeq<plVector3> _points;
    plVector3        _normal;

};

std::ostream& operator << ( std::ostream &stream, const plPolygon &p );

#endif // PLPOLYGON_H
