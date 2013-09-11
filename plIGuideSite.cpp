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
        std::cerr << "Error in plIGuideSite::_create_templateBaseShape(): findInteriorMesh() failed. Aborting iGuideSite surface calculation." << std::endl;
        return false;
    }

    float magnitudeOfOffset(5.0);
    std::cout << "Entering extrudeMesh()" << std::endl;
    _templateBase = plMeshExtruder::extrudeMesh(_templateBase, magnitudeOfOffset, boundary.getAverageNormal());
    std::cout << "Exiting extrudeMesh()" << std::endl;

	return true;
}

