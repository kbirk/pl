#ifndef PL_BONE_AND_CARTILAGE_MODEL_H
#define PL_BONE_AND_CARTILAGE_MODEL_H

#include "plCommon.h"
#include "plModel.h"

class plBoneAndCartilage : public plRenderable,
                           public plArthroViewable
{
    public:

        plModel bone;
        plModel cartilage;
        plModel combined;

        plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combinedFile );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

        plVector3 getCentroid() const;

        void toggleArthroView();

};


#endif
