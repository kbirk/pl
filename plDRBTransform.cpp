// See header or API for class information
// Thomas Vaughan, August 16, 2011

#include "plDRBTransform.h"

DRBTransform::DRBTransform( const std::string& inputFile, MarkerType t ) {

    std::ifstream calibFile;
    calibFile.open(inputFile.c_str());
    // Different files (calibration/registration/etc...) have different levels of detail provided
    if (calibFile.good() && t >= TRANSLATION) {
        calibFile >> calibpointx >> calibpointy >> calibpointz;
        if (t >= TRANSLATION_ROTATION) {
            calibFile >> calibaxisx >> calibaxisy >> calibaxisz >> calibangle;
            if (t >= TRANSLATION_ROTATION_SCALE) {
                calibFile >> calibscalex;
                calibscaley = calibscalez = calibscalex;
            } else {
                calibscaley = calibscalez = calibscalex = 1;
            }
        } else {
            calibaxisx = calibaxisy = calibangle = 0; calibaxisz = 1;
            calibscaley = calibscalez = calibscalex = 1;
        }
    } else {
        std::cout << "Error opening file " << inputFile << " - using default values of 0" << std::endl;
        calibpointx = calibpointy = calibpointz = calibaxisx = calibaxisy = calibangle = 0;
        calibaxisz = calibscaley = calibscalez = calibscalex = 1;
    }
    calibFile.close();
    initializeTransforms();
}

DRBTransform::DRBTransform( const plVector3& pos, const plVector3& ori, double ang) {

    calibpointx = pos.x;
    calibpointy = pos.y;
    calibpointz = pos.z;
    calibaxisx = ori.x;
    calibaxisy = ori.y;
    calibaxisz = ori.z;
    calibangle = ang;
    calibscalex = calibscaley = calibscalez = 1.;

    initializeTransforms();
}

DRBTransform::DRBTransform(plMatrix44 A) {
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            fwdTransform(i,j) = A(i,j);
        }
    }
}

void DRBTransform::initializeTransforms() {

    // Forward transformation is carried out via means of:
    // Scale -> Rotation -> Translation
    // This can all be done in one 4x4 matrix

    plMatrix44 scaleMat44;
    scaleMat44.setIdentity();
    scaleMat44.setScale(calibscalex, calibscaley, calibscalez);

    plMatrix44 rotMat44;
    rotMat44.setIdentity();
    rotMat44.setRotationD(calibangle, plVector3(calibaxisx, calibaxisy, calibaxisz));

    fwdTransform = rotMat44 * scaleMat44;
    fwdTransform.setColumn(3, calibpointx, calibpointy, calibpointz, 1);

    inverseTransform = fwdTransform.inverse();
}

DRBTransform DRBTransform::clone(){
    DRBTransform temp   = DRBTransform(fwdTransform);
    return temp;
}

plVector3 DRBTransform::applyTransform(const plVector3& pos) {

    // Forward transform is carried out in the order:
    // Scale -> Rotation -> Translation
    // All taken care of by the 4x4 matrix

    plVector4 position(1.0,1.0,1.0,1.0);
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;
    plVector4 returnVal = fwdTransform * position;
    return plVector3(returnVal.x, returnVal.y, returnVal.z);

}

plMatrix44 DRBTransform::getTransform(){
    return fwdTransform;
}

plVector3 DRBTransform::applyInverseTransform(const plVector3& pos){
    // All taken care of by the 4x4 matrix

    plVector4 position(pos, 1.0);
    plVector4 returnVal = inverseTransform * position;
    return plVector3(returnVal.x, returnVal.y, returnVal.z);
}
