#ifndef TRACKEDOBJECT_H
#define TRACKEDOBJECT_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plDRBTransform.h"

class plTrackedObject : public plRenderable
{

    public:
           
        plTrackedObject () {};    // Empty Constructor, allows for declaration of trackedObjects in headers        
        plTrackedObject ( const plDRBTransform &ToTrackedPoint, const plDRBTransform &ToTrackedEnd,   //  Real constructor requiring three DRB transforms (and an arthro flag)
                          const plDRBTransform &FemurDRBToFemurSTL, bool isArthro = false);
                            
        void            updatePosition( const plDRBTransform &DRBToWorld, const plDRBTransform &FemurToWorld );

        // Single line accessors
        PLbool          isArthroscope() const       { return _isArthroscope;  }
        const plVector3 &getPoint() const           { return _trackedTip;     }
        const plVector3 &getEnd()   const           { return _trackedEnd;     }
        const plVector3 &getTipWorldCoords() const  { return _tipWorldCoords; }
        const plVector3 &getEndWorldCoords() const  { return _endWorldCoords; }
        const plVector3 &getRotationAxis()   const  { return _rotationAxis;   }
        double          getRotationAngle()   const  { return _rotationAngle;  }
        plVector4       getRotationInfo()    const  { return plVector4( _rotationAxis, _rotationAngle); }

    private:
    
        plDRBTransform   _DRBToTrackedPoint;
        plDRBTransform   _DRBToTrackedEnd;
        plDRBTransform   _toFemurSTL;

        plVector3       _zeroVec;
        plVector3       _trackedTip, _tipWorldCoords, _rotationAxis;
        plVector3       _trackedEnd, _endWorldCoords;

        double          _rotationAngle;
        PLbool          _isArthroscope;

};

#endif
