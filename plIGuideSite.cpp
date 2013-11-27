#include "plIGuideSite.h" 

plIGuideSite::plIGuideSite() 
{
}


plIGuideSite::plIGuideSite( const plMesh& mesh )
    : boundary( PL_PICKING_TYPE_IGUIDE_BOUNDARY, mesh )
{
}


plIGuideSite::plIGuideSite( const plBoundary& boundary )
    : boundary( boundary )
{
}


void plIGuideSite::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    plPickingStack::loadRed( PL_PICKING_TYPE_IGUIDE_BOUNDARY );
    boundary.extractRenderComponents( renderMap, technique );
}  


void plIGuideSite::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}  


PLbool plIGuideSite::generateTemplateBase()
{
    _templateBase.clear();

    // find the surface for the iGuide
    if ( !plMeshCutter::findInteriorMesh( _templateBase, boundary.mesh().triangles(), boundary ) ) // find surface
    {
        std::cerr << "Error in plIGuideSite::generateTemplateBase(): findInteriorMesh() failed. Aborting iGuideSite surface calculation." << std::endl;
        return false;
    }

    float magnitudeOfOffset      ( 5.10f);
    float preTranslationOfSurface(-0.10f);
    _templateBase = plMeshExtruder::extrudeMesh( _templateBase, magnitudeOfOffset, preTranslationOfSurface, boundary.getAverageNormal() );

	return true;
}

