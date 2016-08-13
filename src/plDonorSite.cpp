#include "plDonorSite.h"

plDonorSite::plDonorSite()
{
}


plDonorSite::plDonorSite( const plMesh& mesh )
    : boundary( PL_PICKING_TYPE_DONOR_BOUNDARY, mesh )
{
}


plDonorSite::plDonorSite( const plBoundary& boundary  )
    : boundary( boundary )
{
}


plDonorSite::~plDonorSite() 
{
}


void plDonorSite::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    // draw spline boundary
    boundary.extractRenderComponents( renderMap );
}


void plDonorSite::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}
