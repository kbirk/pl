#include "plPlanningBufferData.h"

plPlanningBufferData::plPlanningBufferData(
    std::shared_ptr<plDefectSite> defect,
    const std::vector<std::shared_ptr<plDonorSite>>& donors)
{
    // generate defect site and buffer
    LOG_INFO("  Generating defect site planning data");
    defectSite = std::make_shared<plPlanningSite>(
        defect->boundary->mesh()->triangles(),
        defect->boundary,
        false);

    LOG_INFO("  Generating defect site SSBO");
    defectSiteSSBO = defectSite->getSSBO();

    // ensure number of sites remains within maximum
    uint32_t donorSiteCount = (donors.size() < PL_MAX_DONOR_SITES) ? donors.size()  : PL_MAX_DONOR_SITES;

    // generate donor sites and buffer
    for (uint32_t i=0; i<donorSiteCount; i++)
    {
        LOG_INFO("  Generating donor site " << i << " planning data");
        auto donorSite = std::make_shared<plPlanningSite>(
            donors[i]->boundary->mesh()->triangles(),
            donors[i]->boundary,
            true);
        donorSites.push_back(donorSite);
    }
    LOG_INFO("  Generating donor sites SSBO");
    donorSitesSSBO = _getGroupSSBO();
}


bool plPlanningBufferData::good() const
{
    if (!defectSite->good())
    {
        return false;
    }
    for (auto donorSite : donorSites)
    {
        if (!donorSite->good())
        {
            return false;
        }
    }
    return true;
}


uint32_t plPlanningBufferData::totalDonorGridPoints() const
{
    uint32_t totalGridPoints = 0;
    for (auto donorSite : donorSites)
    {
        totalGridPoints += donorSite->gridPoints.size();
    }
    return totalGridPoints;
}


std::shared_ptr<plSSBO> plPlanningBufferData::_getGroupSSBO()
{
    // find total data size
    uint32_t dataSize = 0;
    for (auto donorSite : donorSites)
    {
        dataSize += donorSite->totalSize();
    }

    // buffer all data
    std::vector<plVector4> data;
    data.reserve(dataSize);

    for (auto donorSite : donorSites)
    {
        donorSite->getData(data);
    }

    uint32_t numBytes = dataSize * sizeof(plVector4);

    LOG_INFO("    Total buffer size: " << numBytes << " bytes");

    return std::make_shared<plSSBO>(numBytes, (void*)(&data[0]));
}
