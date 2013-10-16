#ifndef PL_SCAN_H
#define PL_SCAN_H

#include "plScanPoint.h"
#include "plScanVolume.h"

class plScan
{

    public:
        // methods
        // constructors
        plScan();
        plScan( const plScanPointData& point, const PLfloat& resolutionW );
        plScan( const plString& filename    , const PLfloat& resolutionW );
        ~plScan();

        // controlling the volume and points
        PLbool addPoint( const plScanPointData& );
        PLbool initializeVolume( const plVector3& centerW, PLfloat radiusW, PLfloat resolutionW );
        PLbool clearAll();

        // output/visualization
        PLbool printPoints() const;
        PLbool printField() const;
        PLbool draw() const;

    private:
        // attributes
        plSeq<plScanPointData> _points;
        plScanField*           _field;

    
};

#endif // PLSCAN_H
