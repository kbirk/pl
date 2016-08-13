#pragma once

#include "plCommon.h"

#include "plVector4.h"
#include "plPlanningSite.h"
#include "plDefectSite.h"
#include "plDonorSite.h"
#include "plSSBO.h"

#define PL_MAX_DONOR_SITES                   5

class plPlanningBufferData
{
    public:

        plPlanningSite defectSite;
        plSSBO         defectSiteSSBO;

        std::vector<plPlanningSite > donorSites;
        plSSBO                        donorSitesSSBO;

        plPlanningBufferData(const plDefectSite& defect, const std::vector<plDonorSite*>& donors);

        bool good() const;

        uint32_t totalDonorGridPoints() const;

    private:

        plSSBO _getGroupSSBO();
};
