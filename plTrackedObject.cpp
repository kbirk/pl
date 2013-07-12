#include "plTrackedObject.h"


trackedObject::trackedObject(DRBTransform ToTrackedPoint,
                             DRBTransform ToTrackedEnd,
                             DRBTransform FemurDRBToFemurSTL, bool isArthro) {
    DRBToTrackedPoint       = ToTrackedPoint.clone();
    DRBToTrackedEnd         = ToTrackedEnd.clone();
    ToFemurSTL              = FemurDRBToFemurSTL.clone();
    trackedTip              = plVector3(0, 0, 0);
    trackedEnd              = plVector3(0, 0, 0);
    rotationAxis            = plVector3(0, 0, 0);
    tTipWorldCoords         = plVector3(0, 0, 0);
    tEndWorldCoords         = plVector3(0, 0, 0);
    zeroVec                 = plVector3(0, 0, 0);
    visible                 = false;
    isArthroscope           = isArthro;
}

void trackedObject::updatePosition(DRBTransform DRBToWorld, DRBTransform FemurToWorld) {

    tTipWorldCoords   = DRBToTrackedPoint.applyTransform(zeroVec);
    tTipWorldCoords   = DRBToWorld.applyTransform(tTipWorldCoords);
    trackedTip        = FemurToWorld.applyInverseTransform(tTipWorldCoords);
    trackedTip        = ToFemurSTL.applyTransform(trackedTip);

    if (!isArthroscope) {
        tEndWorldCoords = DRBToTrackedEnd.applyTransform(zeroVec);
    }
    else {
        // arthroscope end is rotated by 30 degrees from camera view direction
        // and must be handled differently
        tEndWorldCoords = DRBToTrackedEnd.applyTransform(
                plVector3(0, 0, -10));
    }
    tEndWorldCoords     = DRBToWorld.applyTransform(tEndWorldCoords);
    trackedEnd          = FemurToWorld.applyInverseTransform(
            tEndWorldCoords);
    trackedEnd          = ToFemurSTL.applyTransform(trackedEnd);
    rotationAxis    = (((trackedEnd-trackedTip).normalize())
                       ^ plVector3(0,0,1)).normalize();
                                     // 180 / PI = 57.295779513082320876798154
    rotationAngle   = - acos((trackedEnd-trackedTip).normalize()
                             * plVector3(0,0,1)) * 57.295779513082320876798154;

}

void trackedObject::setVisibility(bool currentVisibility) {
    visible = currentVisibility;
}
