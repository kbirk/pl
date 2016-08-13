#include "plIGuideSite.h"

plIGuideSite::plIGuideSite()
{
}


plIGuideSite::plIGuideSite(const plMesh& mesh)
    : boundary(PL_PICKING_TYPE_IGUIDE_BOUNDARY, mesh)
{
}


plIGuideSite::plIGuideSite(const plBoundary& boundary)
    : boundary(boundary)
{
}


plIGuideSite::~plIGuideSite()
{
}


void plIGuideSite::extractRenderComponents(plRenderMap& renderMap, PLuint technique) const
{
    boundary.extractRenderComponents(renderMap, technique);
}


void plIGuideSite::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}


PLbool plIGuideSite::generateTemplateBase()
{
    // find the surface for the iGuide

    std::vector<plTriangle> triangles;
    if (!plMeshCutter::findInteriorMesh(triangles, boundary.mesh().triangles(), boundary)) // find surface
    {
        std::cerr << "Error in plIGuideSite::generateTemplateBase(): findInteriorMesh() failed. Aborting iGuideSite surface calculation." << std::endl;
        return false;
    }

    float magnitudeOfOffset      (5.10f);
    float preTranslationOfSurface(-0.10f);

    plMatrix44 translation; translation.setTranslation(preTranslationOfSurface * boundary.getAverageNormal());

    _templateBase = plMeshExtruder::extrudeMesh(translation * plMesh(triangles), magnitudeOfOffset, boundary.getAverageNormal());

    return true;
}
