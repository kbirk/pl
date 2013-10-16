#ifndef PL_DONOR_SITE_H
#define PL_DONOR_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPickingTexture.h"

class plDonorSite : public plModelSpecific,
                    public plRenderable
{
    public:

        plBoundary boundary; 
                
        plDonorSite();
        plDonorSite( PLuint _modelID, const plBoneAndCartilage &_model );
        plDonorSite( PLuint _modelID, const plBoneAndCartilage &_model, const plBoundary &b );

        void draw() const;      
};


#endif

