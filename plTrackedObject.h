#ifndef TRACKEDOBJECT_H
#define TRACKEDOBJECT_H

#include "plCommon.h"
#include "plRenderable.h"
#include "plDRBTransform.h"

class plTrackedObject : public plRenderable
{

    public:
        // Empty Constructor, allows for declaration of trackedObjects in headers
        plTrackedObject   () {};
        //  Real constructor requiring three DRB transforms (and an arthro flag)
        plTrackedObject   ( const plDRBTransform &ToTrackedPoint, const plDRBTransform &ToTrackedEnd,
                            const plDRBTransform &FemurDRBToFemurSTL, bool isArthro = false);
                            
        void            updatePosition( const plDRBTransform &DRBToWorld, const plDRBTransform &FemurToWorld );

        // Single line accessors
        PLbool          isArthroscope() const       { return _isArthroscope;     }
        const plVector3 &getPoint() const           { return trackedTip;        }
        const plVector3 &getEnd()   const           { return trackedEnd;        }
        const plVector3 &getTipWorldCoords() const  { return tTipWorldCoords;   }
        const plVector3 &getEndWorldCoords() const  { return tEndWorldCoords;   }
        const plVector3 &getRotationAxis()   const  { return rotationAxis;      }
        double          getRotationAngle()   const  { return rotationAngle;     }
        plVector4       getRotationInfo()    const  { return plVector4(rotationAxis, rotationAngle); }

    private:
        plDRBTransform    DRBToTrackedPoint;
        plDRBTransform    DRBToTrackedEnd;
        plDRBTransform    ToFemurSTL;

        plVector3       zeroVec;
        plVector3       trackedTip, tTipWorldCoords, rotationAxis;
        plVector3       trackedEnd, tEndWorldCoords;

        double          rotationAngle;
        PLbool          _isArthroscope;


};

#endif // TRACKEDOBJECT_H
