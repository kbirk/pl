/**
 *======================================================================================================
 * plDRBTransform class
 * Thomas Vaughan, Queen's University
 * August 16 2011
 *------------------------------------------------------------------------------------------------------
 *
 * Class used to greatly simplify the transformation process on the programmer's end.
 * There are two kinds of transforms:
 * 1. From files
 * 2. From DRB's
 * Each of these uses a different constructor to build the transformation matrices (using eccTransform)
 *
 * This class supports both forward and reverse transformations.
 * For more details, see the Programmer's Guide and API.
 *======================================================================================================
 */

#ifndef _PL_DRB_TRANSFORM_H_
#define _PL_DRB_TRANSFORM_H_

#include "plCommon.h"
#include "plMatrix44.h"
#include <string>
#include <fstream>


// This enum is used to indicate how much information is in a transformation file
typedef enum { TRANSLATION , TRANSLATION_ROTATION, TRANSLATION_ROTATION_SCALE } MarkerType;


class plDRBTransform 
{

    private:

        // These store the information about the translations.
        // (computed on instantiation)
        plMatrix44 fwdTransform, inverseTransform;
        plVector3 reverseTranslation;

        // Values to store the original information, provided by the file or the DRB.
        // Storing these isn't strictly necessary, though could be useful if ever
        // we wanted to get the original axis-angle representation values
    public:
    
        double calibpointx, calibpointy, calibpointz;
        double calibangle, calibaxisx, calibaxisy, calibaxisz;
        double calibscalex, calibscaley, calibscalez; // generally these will all be the same value


    public:

        plDRBTransform() 
        { 
            // default transform - this will do nothing
            calibpointx=calibpointy=calibpointz=calibangle=calibaxisy=calibaxisx=0;
            calibscalex=calibscaley=calibscalez=calibaxisz=1;
            initializeTransforms();
        }

        plDRBTransform( const std::string&, MarkerType = TRANSLATION );
        // File Transformation - string is the file, MarkerType specifies how much to read

        plDRBTransform( const plVector3&, const plVector3&, double ); // DRB Transformation

    //    plDRBTransform( const Matrix1<double> ); // DRB Transformation from a 4x4 matrix;

        plDRBTransform( const plMatrix44 );

        void initializeTransforms(); // Sets up the matrices
        plDRBTransform clone() const;
        plMatrix44     getTransform() const;
        plVector3      applyTransform         ( const plVector3& ) const; // forward transformation, returns resulting vector
        plVector3      applyInverseTransform  ( const plVector3& ) const; // reverse transformation, returns resulting vector

};

#endif // plDRBTransform_H
