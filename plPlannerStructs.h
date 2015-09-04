#ifndef PL_PLANNER_STRUCTS
#define PL_PLANNER_STRUCTS

#include "plCommon.h"
#include "plUtility.h"
#include "plVector4.h"
#include "plSSBO.h"

class plDefectSolution
{
    public:

        PLuint                  graftCount;
        std::vector<plVector4>  graftPositions;
        std::vector<plVector4>  graftNormals;         // normal / direction of graft
        std::vector<plVector4>  graftSurfaceNormals;  // actual normal of the surface
        std::vector<PLfloat>    graftRadii;

        plDefectSolution() {};
};


class plCapIndices
{
    public:

        plSSBO defectCapIndexSSBO;
        plSSBO donorCapIndexSSBO;

        plCapIndices() {};
};


class plRmsData
{
    public:

        plSSBO rmsSSBO;
        plSSBO rotationSSBO;

        plRmsData() {}
};


class plDonorSolution
{
    public:

        std::vector<plVector4>  graftPositions;
        std::vector<plVector4>  graftNormals;
        std::vector<plVector4>  graftSurfaceNormals;
        std::vector<plVector4>  graftXAxes;
        std::vector<PLuint>     graftSiteIndices;
        float                   rms;

        plDonorSolution() {};

};


#endif
