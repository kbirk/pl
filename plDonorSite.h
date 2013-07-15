#ifndef __PL_DONOR_SITE_H__
#define __PL_DONOR_SITE_H__

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

        void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models );

        void draw() const;      
};


#endif

