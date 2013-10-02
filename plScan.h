#ifndef PLSCAN_H
#define PLSCAN_H

#include "plScanPoint.h"
#include "plScanVolume.h"

class plScan
{
  public:
  // attributes
    plSeq<plScanPointData> points;
    plScanField*           field;

  // methods
    // constructors
    plScan();
    plScan( const plScanPointData& point, const PLfloat& resolutionW );
    plScan( const plString& filename    , const PLfloat& resolutionW );

    // controlling the volume and points
    PLbool addPoint( const plScanPointData& );
    PLbool initializeVolume( const plVector3& centerW, PLfloat radiusW, PLfloat resolutionW );
    PLbool clearAll();

    // output/visualization
    PLbool printField();
};

#endif // PLSCAN_H
