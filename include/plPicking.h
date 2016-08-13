#pragma once

#include "plCommon.h"
#include "plRenderResources.h"


enum plPickingRedEnums
{
    PL_PICKING_TYPE_NONE = 0,
    PL_PICKING_TYPE_BONE,
    PL_PICKING_TYPE_CARTILAGE,
    PL_PICKING_TYPE_GRAFT_PROJECTION,
    PL_PICKING_TYPE_GRAFT_HANDLE,
    PL_PICKING_TYPE_GRAFT_MARKER,
    PL_PICKING_TYPE_GRAFT,
    PL_PICKING_TYPE_GRAFT_ARTHRO_PROJECTION,
    PL_PICKING_TYPE_DEFECT_SPLINE,
    PL_PICKING_TYPE_DEFECT_CORNERS,
    PL_PICKING_TYPE_DEFECT_BOUNDARY,
    PL_PICKING_TYPE_DONOR_BOUNDARY,
    PL_PICKING_TYPE_IGUIDE_BOUNDARY,
    PL_PICKING_TYPE_IGUIDE
};


enum plPickingBlueEnums
{
    PL_PICKING_INDEX_GRAFT_DONOR = 1,
    PL_PICKING_INDEX_GRAFT_DEFECT
};


class plPickingInfo
{
    public:

        PLint r;
        PLint g;
        PLint b;

        plPickingInfo()
            : r(-1), g(-1), b(-1)
        {
        }

        plPickingInfo(PLint red, PLint green, PLint blue)
            : r(red), g(green), b(blue)
        {
        }
};


namespace plPicking
{
    const plPickingInfo& previousPick();
    const plPickingInfo& pickPixel(PLuint x, PLuint y);
}
