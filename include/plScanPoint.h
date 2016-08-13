#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plString.h"

class plScanPointData
{
    public:
        // attributes

        // members
        plScanPointData();
        plScanPointData(const plVector3&, const plVector3&, PLfloat);

        PLbool draw() const;

        plVector3 point;
        plVector3 normal;
        PLfloat   radius;
};

std::ostream& operator << (std::ostream &stream, const plScanPointData &p);

namespace plScanPointReader
{
    std::vector<plScanPointData> readPoints(const plString&);
}
