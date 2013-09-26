#include "plIGuideSite.h" 

plIGuideSite::plIGuideSite() 
{
}


plIGuideSite::plIGuideSite( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model )
{
}


plIGuideSite::plIGuideSite( PLuint modelID, const plBoneAndCartilage &model, const plBoundary &b )
    : plModelSpecific( modelID, model ), boundary( b )
{
}


void plIGuideSite::draw() const
{
    plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}


PLbool plIGuideSite::generateTemplateBase()
{
    _templateBase.clear();

    // find the surface for the iGuide
    if ( !plMeshCutter::findInteriorMesh( _templateBase, _model->combined.triangles(), boundary ) ) // find surface
    {
        std::cerr << "Error in plIGuideSite::generateTemplateBase(): findInteriorMesh() failed. Aborting iGuideSite surface calculation." << std::endl;
        return false;
    }

    float magnitudeOfOffset      ( 5.10f);
    float preTranslationOfSurface(-0.10f);
    _templateBase = plMeshExtruder::extrudeMesh( _templateBase, magnitudeOfOffset, preTranslationOfSurface, boundary.getAverageNormal() );

	return true;
}

