#include "plDefectSite.h" 

plDefectSite::plDefectSite() 
{
}


plDefectSite::plDefectSite( PLuint modelID, const plBoneAndCartilage& model )
    : plModelSpecific( modelID, model ), spline( model.cartilage.mesh() )
{
}


plDefectSite::plDefectSite( PLuint modelID, const plBoneAndCartilage& model, const plSpline &s, const plBoundary &b )
    : plModelSpecific( modelID, model ), spline( s ), boundary( b )
{
}


void plDefectSite::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;
      
    // draw spline boundary 
    plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_BOUNDARY );
    boundary.extractRenderComponents( renderMap, technique );   
     
    // draw spline corners
    spline.extractRenderComponents( renderMap, technique );
}


void plDefectSite::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


/*
void plDefectSite::draw() const
{      
    if ( !_isVisible )
        return;
      
    // draw spline boundary 
    plPickingStack::loadRed( PL_PICKING_TYPE_DEFECT_BOUNDARY );
    boundary.draw();   
     
    // draw spline corners
    spline.draw();
}
*/
