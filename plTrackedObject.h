#ifndef TRACKEDOBJECT_H
#define TRACKEDOBJECT_H

#include "kevinsCode/pl.h"
#include "DRBTransform.h"

class trackedObject
{
private:
    DRBTransform    DRBToTrackedPoint;
    DRBTransform    DRBToTrackedEnd;
    DRBTransform    ToFemurSTL;


    plVector3       zeroVec;
    plVector3       trackedTip, tTipWorldCoords, rotationAxis;
    plVector3       trackedEnd, tEndWorldCoords;

    double          rotationAngle;
    bool            visible;
    bool            isArthroscope;

public:
    // Empty Constructor, allows for declaration of trackedObjects in headers
    trackedObject   () {};
    //  Real constructor requiring three DRB transforms (and an arthro flag)
    trackedObject   (DRBTransform ToTrackedPoint, DRBTransform ToTrackedEnd,
                  DRBTransform FemurDRBToFemurSTL, bool isArthro = false);
    void            updatePosition(DRBTransform DRBToWorld, DRBTransform FemurToWorld);
    void            setVisibility(bool currentVisibility);


    // Single line accessors
    bool            getVisibility()     { return visible;           }
    plVector3       getPoint()          { return trackedTip;        }
    plVector3       getEnd()            { return trackedEnd;        }
    plVector3       getTipWorldCoords() { return tTipWorldCoords;   }
    plVector3       getEndWorldCoords() { return tEndWorldCoords;   }
    plVector3       getRotationAxis()   { return rotationAxis;      }
    double          getRotationAngle()  { return rotationAngle;     }
    plVector4       getRotationInfo()   { return
                                          plVector4(rotationAxis,
                                                    rotationAngle); }



};

#endif // TRACKEDOBJECT_H
