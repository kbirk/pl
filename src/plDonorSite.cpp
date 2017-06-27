#include "plDonorSite.h"

plDonorSite::plDonorSite(std::shared_ptr<plMesh> mesh)
{
    boundary = std::make_shared<plBoundary>(PL_PICKING_TYPE_DONOR_BOUNDARY, mesh);
}

plDonorSite::plDonorSite(std::shared_ptr<plBoundary> boundary)
    : boundary(boundary)
{
}

plDonorSite::~plDonorSite()
{
}

void plDonorSite::extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const
{
    if (!_isVisible) {
        return;
    }
    // draw spline boundary
    boundary->extractRenderComponents(renderMap);
}

void plDonorSite::extractRenderComponents(plRenderMap& renderMap) const
{
    extractRenderComponents(renderMap, PL_PLAN_TECHNIQUE);
}
