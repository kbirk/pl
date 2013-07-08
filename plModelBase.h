#ifndef PLMODELBASE_H
#define PLMODELBASE_H

#include "pl.h"
#include "plSeq.h"
#include "plTriangle.h"
#include "plMesh.h"


class plModelBase
{

public:

    plModelBase() {}
    plModelBase( std::string filename );

    const plSeq<plTriangle> &triangles()     const { return _triangles; }

    void      getMinMax(plVector3 &min, plVector3 &max) const;
    plVector3 getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const;
    plString  getFilenameWithoutPath ();

    friend std::ostream& operator << ( std::ostream& out, const plModelBase &m );

protected:

    plSeq<plTriangle>     _triangles;
    plString              _filename;

};

#endif // PLMODELBASE_H
