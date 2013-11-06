#include "plPlannerStage1Shader.h"


plPlannerStage1Shader::plPlannerStage1Shader ( const std::vector< plString > &computeFiles )
    : plShader( computeFiles, GL_COMPUTE_SHADER )              
{   
    getUniformLocations();         
}


void plPlannerStage1Shader::getUniformLocations()
{   
    _defectSiteGridPointCountID     = glGetUniformLocation(_shaderProgramID, "uDefectSiteGridPointCount");
    _defectSiteBoundaryPointCountID = glGetUniformLocation(_shaderProgramID, "uDefectSiteBoundaryPointCount");
    _defectSiteTriangleCountID      = glGetUniformLocation(_shaderProgramID, "uDefectSiteTriangleCount"); 
    _defectSiteAreaID               = glGetUniformLocation(_shaderProgramID, "uDefectSiteArea"); 
    _defectSiteAvgNormalID          = glGetUniformLocation(_shaderProgramID, "uDefectSiteAvgNormal");
     
    _donorSiteCountID               = glGetUniformLocation(_shaderProgramID, "uDonorSiteCount");    
    _donorSiteTotalGridPointsID     = glGetUniformLocation(_shaderProgramID, "uDonorTotalGridPointCount");      
    _donorSiteTriangleCountsID      = glGetUniformLocation(_shaderProgramID, "uDonorSiteTriangleCounts");   
    _donorSiteGridPointCountsID     = glGetUniformLocation(_shaderProgramID, "uDonorSiteGridPointCounts");
    _donorSiteBoundaryPointCountsID = glGetUniformLocation(_shaderProgramID, "uDonorSiteBoundaryPointCounts");
    _donorSiteDataOffsetsID         = glGetUniformLocation(_shaderProgramID, "uDonorSiteDataOffsets"); 
      
    _defectSolutionGraftCountID      = glGetUniformLocation(_shaderProgramID, "uDefectSolutionGraftCount");
    _defectSolutionGraftPositionsID  = glGetUniformLocation(_shaderProgramID, "uDefectSolutionGraftPositions");
    _defectSolutionGraftNormalsID    = glGetUniformLocation(_shaderProgramID, "uDefectSolutionGraftNormals");
    _defectSolutionGraftRadiiID      = glGetUniformLocation(_shaderProgramID, "uDefectSolutionGraftRadii");
    _defectSolutionSurfaceNormalsID  = glGetUniformLocation(_shaderProgramID, "uDefectSolutionSurfaceNormals"); 
}


void plPlannerStage1Shader::setDefectSiteUniforms( const plPlanningSite &defectSite ) const
{
    glUniform1ui  ( _defectSiteTriangleCountID,      defectSite.triangles.size() );    
    glUniform1f   ( _defectSiteAreaID,               defectSite.area ); 
    glUniform1ui  ( _defectSiteGridPointCountID,     defectSite.gridPoints.size() ); 
    glUniform1ui  ( _defectSiteBoundaryPointCountID, defectSite.boundaryPoints.size() );
    glUniform4fv  ( _defectSiteAvgNormalID, 1,      &defectSite.avgNormal.x );  
}                                                  


void plPlannerStage1Shader::setDonorSiteUniforms( const std::vector<plPlanningSite> &donorSites ) const
{
    PLuint totalGridPoints = 0;
    PLuint dataIndexOffset = 0;
    std::vector<PLuint> gridPointCounts;
    std::vector<PLuint> triangleCounts;   
    std::vector<PLuint> boundaryPointCounts;       
    std::vector<PLuint> dataOffsets;

    for ( const plPlanningSite& donorSite : donorSites )
    {    
        totalGridPoints +=  donorSite.gridPoints.size();
        gridPointCounts.push_back     ( donorSite.gridPoints.size()     );
        triangleCounts.push_back      ( donorSite.triangles.size()      );            
        boundaryPointCounts.push_back ( donorSite.boundaryPoints.size() );          
        dataOffsets.push_back( dataIndexOffset );        
        dataIndexOffset += donorSite.totalSize();
    }

    glUniform1ui  ( _donorSiteCountID,               donorSites.size() );  
    glUniform1ui  ( _donorSiteTotalGridPointsID,     totalGridPoints   ); 
    glUniform1uiv ( _donorSiteGridPointCountsID,     gridPointCounts.size(),     &gridPointCounts[0]     ); 
    glUniform1uiv ( _donorSiteTriangleCountsID,      triangleCounts.size(),      &triangleCounts[0]      );
    glUniform1uiv ( _donorSiteBoundaryPointCountsID, boundaryPointCounts.size(), &boundaryPointCounts[0] );   
    glUniform1uiv ( _donorSiteDataOffsetsID,         dataOffsets.size(),         &dataOffsets[0]         );
}                                                  


void plPlannerStage1Shader::setDefectSolutionUniforms ( const plDefectSolution &solution ) const
{   
    glUniform1ui  ( _defectSolutionGraftCountID,     solution.graftCount );     
    glUniform4fv  ( _defectSolutionGraftPositionsID, solution.graftPositions.size(),      &solution.graftPositions[0].x      );   
    glUniform4fv  ( _defectSolutionGraftNormalsID,   solution.graftNormals.size(),        &solution.graftNormals[0].x        );
    glUniform1fv  ( _defectSolutionGraftRadiiID,     solution.graftRadii.size(),          &solution.graftRadii[0]            );
    glUniform4fv  ( _defectSolutionSurfaceNormalsID, solution.graftSurfaceNormals.size(), &solution.graftSurfaceNormals[0].x );
}


