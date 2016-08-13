#include "plDefectSite.h"

plDefectSite::plDefectSite()
{
}


plDefectSite::plDefectSite(const plMesh& mesh)
    : spline(mesh)
{
    boundary = plBoundary(PL_PICKING_TYPE_DEFECT_BOUNDARY, spline.surfaceMesh());
}


plDefectSite::plDefectSite(const plSpline& splne, const std::vector<plString> &row)
    : spline(splne)
{
    boundary = plBoundary(PL_PICKING_TYPE_DEFECT_BOUNDARY, spline.surfaceMesh(), row);
}


plDefectSite::~plDefectSite()
{
}


void plDefectSite::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    if (!_isVisible)
        return;

    // draw spline boundary
    boundary.extractRenderComponents(renderMap, technique);

    // draw spline corners
    spline.extractRenderComponents(renderMap, technique);
}


void plDefectSite::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}


void plDefectSite::recastBoundary()
{
    for (int32_t i=boundary.size()-1; i>=0; i--)
    {
        plIntersection intersection = spline.surfaceMesh().rayIntersect(boundary.points(i), boundary.normals(i));

        if (intersection.exists)
        {
            boundary.movePointAndNormal(i, intersection.point, intersection.normal);
        }
        else
        {
            boundary.removePointAndNormal(i);
        }
    }
}
