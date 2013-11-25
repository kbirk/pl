#ifndef PL_BONE_AND_CARTILAGE_MODEL_H
#define PL_BONE_AND_CARTILAGE_MODEL_H

#include "plCommon.h"
#include "plModel.h"

class plBoneAndCartilage : plRenderable
{
    public:
    
        plModel bone;
        plModel cartilage;
        plModel combined;

        plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combinedFile );

        void extractRenderComponents( plRenderMap& renderMap ) const;

        plVector3 getCentroid() const; 

    private:

        //plVAO _generateTransparencyVAO() const;
        
};


#endif
