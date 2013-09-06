#ifndef __PL_BONE_AND_CARTILAGE_MODEL_H__
#define __PL_BONE_AND_CARTILAGE_MODEL_H__

#include "plCommon.h"
#include "plModel.h"

class plBoneAndCartilage
{
    public:
       
        plModel bone;
		plModel	cartilage;
        //plModel	combined;

        plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile );
        //plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combinedFile );

        void draw() const;

        void       getMinMax  ( plVector3 &min, plVector3 &max ) const;        
        plVector3  getCentroid() const;     

    private:

        // prevent empty constructor, copy constructor, and assignment, which will invalidate the octree's pointers if rhs is scoped
        plBoneAndCartilage();
        plBoneAndCartilage( const plBoneAndCartilage &m );
        plBoneAndCartilage operator= ( const plBoneAndCartilage &m ) const;    
        
};


#endif
