#include "plDRBTransform.h"

plDRBTransform::plDRBTransform()
{
    // default transform - this will do nothing
    calibpointx=calibpointy=calibpointz=calibangle=calibaxisy=calibaxisx=0;
    calibscalex=calibscaley=calibscalez=calibaxisz=1;
    initializeTransforms();
}

plDRBTransform::plDRBTransform( const std::string& inputFile, MarkerType t )
{
    std::ifstream calibFile;
    calibFile.open(inputFile.c_str());

    // Different files (calibration/registration/etc...) have different levels of detail provided
    if (calibFile.good() && t >= TRANSLATION)
    {
        calibFile >> calibpointx >> calibpointy >> calibpointz;
        if (t >= TRANSLATION_ROTATION)
        {
            calibFile >> calibaxisx >> calibaxisy >> calibaxisz >> calibangle;
            if (t >= TRANSLATION_ROTATION_SCALE)
            {
                calibFile >> calibscalex;
                calibscaley = calibscalez = calibscalex;
            }
            else
             {
                calibscaley = calibscalez = calibscalex = 1;
            }
        }
        else
        {
            calibaxisx = calibaxisy = calibangle = 0; calibaxisz = 1;
            calibscaley = calibscalez = calibscalex = 1;
        }
    }
    else
    {
        std::cout << "Error opening file " << inputFile << " - using default values of 0" << std::endl;
        calibpointx = calibpointy = calibpointz = calibaxisx = calibaxisy = calibangle = 0;
        calibaxisz = calibscaley = calibscalez = calibscalex = 1;
    }
    calibFile.close();
    initializeTransforms();
}

plDRBTransform::plDRBTransform( const plVector3& pos, const plVector3& ori, double ang)
{
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

plDRBTransform::plDRBTransform(plMatrix44 A)
{
    for (int i=0; i<4; i++){
        for (int j=0; j<4; j++){
            fwdTransform(i,j) = A(i,j);
        }
    }
}

void plDRBTransform::initializeTransforms()
{
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

plDRBTransform plDRBTransform::clone() const
{
    plDRBTransform temp = plDRBTransform(fwdTransform);
    return temp;
}

plVector3 plDRBTransform::applyTransform(const plVector3& pos) const
{
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

plMatrix44 plDRBTransform::getTransform() const
{
    return fwdTransform;
}

plVector3 plDRBTransform::applyInverseTransform(const plVector3& pos) const
{
    // All taken care of by the 4x4 matrix
    plVector4 position(pos, 1.0);
    plVector4 returnVal = inverseTransform * position;
    return plVector3(returnVal.x, returnVal.y, returnVal.z);
}
