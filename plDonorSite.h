#ifndef PL_DONOR_SITE_H
#define PL_DONOR_SITE_H

#include "plCommon.h"
#include "plVector3.h"
#include "plMeshSpecific.h"
#include "plBoundary.h"
#include "plRenderable.h"
#include "plPicking.h"

class plDonorSite : public plRenderable
{
    public:

        plBoundary boundary;

        plDonorSite();
        plDonorSite( const plMesh& mesh );
        plDonorSite( const plBoundary &boundary );

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;
};


#endif
