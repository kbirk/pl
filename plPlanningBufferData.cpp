#include "plPlanningBufferData.h"


plPlanningBufferData::plPlanningBufferData( const plDefectSite& defect, const std::vector<plDonorSite*>& donors )
{
    // generate defect site and buffer
    std::cout << "\tGenerating defect site planning data" << std::endl;    
    defectSite = plPlanningSite( defect.spline.triangles(), defect.boundary, false );
    
    std::cout << "\tGenerating defect site SSBO" << std::endl;   
    defectSiteSSBO = defectSite.getSSBO();       
      
    // generate donor sites and buffer   
    for (PLuint i=0; i<donors.size(); i++)
    {
        std::cout << "\tGenerating donor site planning data " << i << " planning data " << std::endl;
        donorSites.push_back( plPlanningSite( donors[i]->model().combined.triangles(), 
                                              donors[i]->boundary,
                                              true ) );                                 
    }
    std::cout << "\tGenerating donor sites SSBO" << std::endl;   
    donorSitesSSBO = _getGroupSSBO(); 
}


PLbool plPlanningBufferData::good() const 
{
    PLbool isGood = true;
    for (PLuint i=0; i < donorSites.size(); i++ )
    {
        isGood &= donorSites[i].good();
    }
    
    return isGood & defectSite.good();
}


PLuint plPlanningBufferData::totalDonorGridPoints() const
{
    PLuint totalGridPoints = 0;
    for ( const plPlanningSite& donorSite : donorSites )
    {    
        totalGridPoints +=  donorSite.gridPoints.size();
    }
    return totalGridPoints;
}


plSSBO plPlanningBufferData::_getGroupSSBO()
{
    // find total data size
    PLuint dataSize = 0;
    for (PLuint i=0; i < donorSites.size(); i++ )
    {
        dataSize += donorSites[i].totalSize();
    }

    // buffer all data
    std::vector<plVector4> data;    data.reserve( dataSize );    

    for (PLuint i=0; i < donorSites.size(); i++ )
    {
        donorSites[i].getData( data );  
    }
    
    PLuint numBytes = dataSize * sizeof( plVector4 );

    std::cout << "\t\tTotal buffer size: " << numBytes << " bytes " << std::endl;     

    return plSSBO( numBytes, (void*)(&data[0]) );
}

