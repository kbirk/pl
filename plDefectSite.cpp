#include "plDefectSite.h" 

plDefectSite::plDefectSite() 
{
}


plDefectSite::plDefectSite( const plMesh& mesh )
    : spline( mesh )
{
    boundary = plBoundary( PL_PICKING_TYPE_DEFECT_BOUNDARY, spline.surfaceMesh() );
}


plDefectSite::plDefectSite( const plSpline& splne, const std::vector<plString> &row )
    : spline( splne )
{
    boundary = plBoundary( PL_PICKING_TYPE_DEFECT_BOUNDARY, spline.surfaceMesh(), row );
}


void plDefectSite::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;
      
    // draw spline boundary 
    boundary.extractRenderComponents( renderMap, technique );   
     
    // draw spline corners
    spline.extractRenderComponents( renderMap, technique );
}


void plDefectSite::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}

