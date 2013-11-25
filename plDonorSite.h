#ifndef PL_DONOR_SITE_H
#define PL_DONOR_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plModelSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"

class plDonorSite : public plModelSpecific,
                    public plRenderable
{
    public:

        plBoundary boundary; 
                
        plDonorSite();
        plDonorSite( PLuint modelID, const plBoneAndCartilage& model );
        plDonorSite( PLuint modelID, const plBoneAndCartilage& model, const plBoundary &b );

        void extractRenderComponents( plRenderMap& renderMap ) const;  
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
        
        void draw() const;      
};


#endif

