#include "plPlanningBufferData.h"

plPlanningBufferData::plPlanningBufferData(const plDefectSite& defect, const std::vector<plDonorSite*>& donors)
{
    // generate defect site and buffer
    std::cout << "    Generating defect site planning data" << std::endl;
    defectSite = plPlanningSite(defect.boundary.mesh().triangles(), defect.boundary, false);

    std::cout << "    Generating defect site SSBO" << std::endl;
    defectSiteSSBO = defectSite.getSSBO();

    // ensure number of sites remains within maximum
    uint32_t donorSiteCount = (donors.size() < PL_MAX_DONOR_SITES) ? donors.size()  : PL_MAX_DONOR_SITES;

    // generate donor sites and buffer
    for (uint32_t i=0; i<donorSiteCount; i++)
    {
        std::cout << "    Generating donor site " << i << " planning data " << std::endl;
        donorSites.push_back(
            plPlanningSite(
                donors[i]->boundary.mesh().triangles(),
                donors[i]->boundary,
                true));
    }
    std::cout << "    Generating donor sites SSBO" << std::endl;
    donorSitesSSBO = _getGroupSSBO();
}


bool plPlanningBufferData::good() const
{
    bool isGood = true;
    for (uint32_t i=0; i < donorSites.size(); i++)
    {
        isGood &= donorSites[i].good();
    }

    return isGood & defectSite.good();
}


uint32_t plPlanningBufferData::totalDonorGridPoints() const
{
    uint32_t totalGridPoints = 0;
    for (const plPlanningSite& donorSite : donorSites)
    {
        totalGridPoints +=  donorSite.gridPoints.size();
    }
    return totalGridPoints;
}


plSSBO plPlanningBufferData::_getGroupSSBO()
{
    // find total data size
    uint32_t dataSize = 0;
    for (uint32_t i=0; i < donorSites.size(); i++)
    {
        dataSize += donorSites[i].totalSize();
    }

    // buffer all data
    std::vector<plVector4> data;    data.reserve(dataSize);

    for (uint32_t i=0; i < donorSites.size(); i++)
    {
        donorSites[i].getData(data);
    }

    uint32_t numBytes = dataSize * sizeof(plVector4);

    std::cout << "\t\tTotal buffer size: " << numBytes << " bytes " << std::endl;

    return plSSBO(numBytes, (void*)(&data[0]));
}
