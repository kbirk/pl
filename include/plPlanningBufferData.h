#pragma once

#include "plCommon.h"
#include "plDefectSite.h"
#include "plDonorSite.h"
#include "plPlanningSite.h"
#include "plSSBO.h"
#include "plVector4.h"

#define PL_MAX_DONOR_SITES 5

class plPlanningBufferData {
public:
    std::shared_ptr<plPlanningSite> defectSite;
    std::shared_ptr<plSSBO> defectSiteSSBO;

    std::vector<std::shared_ptr<plPlanningSite> > donorSites;
    std::shared_ptr<plSSBO> donorSitesSSBO;

    plPlanningBufferData(
        std::shared_ptr<plDefectSite> defect,
        const std::vector<std::shared_ptr<plDonorSite> >& donors);

    bool good() const;

    uint32_t totalDonorGridPoints() const;

private:
    std::shared_ptr<plSSBO> _getGroupSSBO();
};
