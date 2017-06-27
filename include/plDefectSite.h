#pragma once

#include "plBoundary.h"
#include "plCommon.h"
#include "plMeshSpecific.h"
#include "plPicking.h"
#include "plRenderable.h"
#include "plSpline.h"
#include "plVector3.h"

class plDefectSite : public plRenderable {
public:
    plDefectSite(std::shared_ptr<plMesh> mesh);
    plDefectSite(std::shared_ptr<plSpline> spline, const std::vector<plString>& row);

    virtual ~plDefectSite();

    void extractRenderComponents(plRenderMap& renderMap) const;
    void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

    void recastBoundary();

    std::shared_ptr<plSpline> spline;
    std::shared_ptr<plBoundary> boundary;
};
