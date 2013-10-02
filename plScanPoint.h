#ifndef PLSCANPOINTREADER_H
#define PLSCANPOINTREADER_H

#include "plVector3.h"
#include "plSeq.h"
#include "plString.h"
#include <fstream>
//#include "plRenderable.h"

class plScanPointData //: public plRenderable
{
public:

    plScanPointData();
    plScanPointData(const plVector3&, const plVector3&, PLfloat);

    plVector3 point;
    plVector3 normal;
    PLfloat   radius;
};

std::ostream& operator << ( std::ostream &stream, const plScanPointData &p );

namespace plScanPointReader
{
    plSeq<plScanPointData> readPoints(const plString&);
}

#endif // PLSCANPOINTREADER_H
