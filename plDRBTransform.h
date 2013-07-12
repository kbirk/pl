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

// This enum is used to indicate how much information is in a transformation file
enum MarkerType 
{ 
    TRANSLATION , 
    TRANSLATION_ROTATION, 
    TRANSLATION_ROTATION_SCALE 
};

class plDRBTransform 
{
 
    public:
    
        // Values to store the original information, provided by the file or the DRB.
        // Storing these isn't strictly necessary, though could be useful if ever
        // we wanted to get the original axis-angle representation values
        double calibpointx, calibpointy, calibpointz;
        double calibangle,  calibaxisx, calibaxisy, calibaxisz;
        double calibscalex, calibscaley, calibscalez; // generally these will all be the same value

        plDRBTransform();
        
        plDRBTransform( const plMatrix44 );
        plDRBTransform( const std::string&, MarkerType = TRANSLATION ); // File Transformation - string is the file, MarkerType specifies how much to read
        plDRBTransform( const plVector3&, const plVector3&, double ); // DRB Transformation
        
        void           initializeTransforms(); // Sets up the matrices
        
        plDRBTransform clone()        const;
        plMatrix44     getTransform() const;
        
        plVector3      applyTransform       ( const plVector3& ) const; // forward transformation, returns resulting vector
        plVector3      applyInverseTransform( const plVector3& ) const; // reverse transformation, returns resulting vector

    private:

        // These store the information about the translations.
        // (computed on instantiation)
        plMatrix44 fwdTransform, inverseTransform;
        plVector3 reverseTranslation;

        

};

#endif // plDRBTransform_H
