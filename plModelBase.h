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


class plBoneAndCartilageBase
{
public:
    plModelBase bone;
    plModelBase	cartilage;

    plBoneAndCartilageBase() {}
    plBoneAndCartilageBase( plString bone_file, plString cartilage_file );

    void readFromCSV( const plSeq<plString> &row , const plString &directory );

    void getMinMax(plVector3 &min, plVector3 &max) const;

    friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
};

#endif // PLMODELBASE_H
