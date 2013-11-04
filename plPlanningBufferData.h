#ifndef PL_PLANNING_BUFFER_DATA_H
#define PL_PLANNING_BUFFER_DATA_H

#include "plCommon.h"

#include "plVector4.h"
#include "plPlanningSite.h"
#include "plDefectSite.h"
#include "plDonorSite.h"
#include "plSSBO.h"

class plPlanningBufferData
{
    public:
    
        plPlanningSite          defectSite;
        plSSBO                  defectSiteSSBO;               
        std::vector< plPlanningSite > donorSites;
        plSSBO                  donorSitesSSBO;

        plPlanningBufferData( const plDefectSite& defect, const std::vector<plDonorSite*>& donors );        

        PLbool good() const;

        PLuint totalDonorGridPoints() const;

    private:
        
        plSSBO _getGroupSSBO(); 
};






#endif
