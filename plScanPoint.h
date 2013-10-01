#ifndef PLSCANPOINTREADER_H
#define PLSCANPOINTREADER_H

#include "plVector3.h"
#include "plSeq.h"
#include "plString.h"
#include <fstream>

class plScanPointData
{
public:

    plScanPointData();
    plScanPointData(const plVector3&, const plVector3&, PLfloat);

    plVector3 point;
    plVector3 normal;
    PLfloat   radius;
};

namespace plScanPointReader
{
    plSeq<plScanPointData> readPoints(const plString&);
}

#endif // PLSCANPOINTREADER_H
