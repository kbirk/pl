#ifndef __PL_POLYGON_H__
#define __PL_POLYGON_H__

#include "pl.h"
#include "plSeq.h"
#include "plVector3.h"

class plPolygon {

  public:
    plPolygon() {}

    const PLuint    &getPolygonSize()          const { return _points.size(); }
    const plVector3 &getPoint ( PLuint index ) const { return _points[index]; }
    const plVector3 &getNormal()               const { return _normal;        }

    void  setPoint ( const PLuint index , const plVector3 &value );
    void  setNormal( const plVector3 &value );

    void  addPoint    ( const plVector3 &value );   // adds a point to the end of the plSeq
    void  removePoint ( const PLuint &index    );   // removes the last point in the plSeq of points

  private:
    plSeq<plVector3> _points;
    plVector3        _normal;

};

std::ostream& operator << ( std::ostream &stream, const plPolygon &p );

#endif // PLPOLYGON_H
