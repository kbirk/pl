#include "plPlannerStage0Shader.h"


plPlannerStage0Shader::plPlannerStage0Shader ( const std::vector< plString > &computeFiles )
    : plShader( computeFiles, GL_COMPUTE_SHADER )              
{   
    getUniformLocations();         
}


void plPlannerStage0Shader::getUniformLocations()
{
    _defectSiteTriangleCountID      = glGetUniformLocation(_shaderProgramID, "uDefectSiteTriangleCount");   
    _defectSiteAreaID               = glGetUniformLocation(_shaderProgramID, "uDefectSiteArea");
    _defectSiteGridPointCountID     = glGetUniformLocation(_shaderProgramID, "uDefectSiteGridPointCount");
    _defectSiteBoundaryPointCountID = glGetUniformLocation(_shaderProgramID, "uDefectSiteBoundaryPointCount");
    _defectSiteAvgNormalID          = glGetUniformLocation(_shaderProgramID, "uDefectSiteAvgNormal");
    
    _temperatureID = glGetUniformLocation(_shaderProgramID, "uTemperature");   
    _loadLocalID   = glGetUniformLocation(_shaderProgramID, "uLoadLocal");

    _seedID        = glGetUniformLocation(_shaderProgramID, "uSeed");   
}


void plPlannerStage0Shader::setDefectSiteUniforms( const plPlanningSite &defectSite ) const
{
    glUniform1ui  ( _defectSiteTriangleCountID,      defectSite.triangles.size() );    
    glUniform1f   ( _defectSiteAreaID,               defectSite.area ); 
    glUniform1ui  ( _defectSiteGridPointCountID,     defectSite.gridPoints.size() ); 
    glUniform1ui  ( _defectSiteBoundaryPointCountID, defectSite.boundaryPoints.size() );
    glUniform4fv  ( _defectSiteAvgNormalID, 1, &defectSite.avgNormal.x);  
}                                                  


void plPlannerStage0Shader::setTemperatureUniform( PLfloat temperature ) const
{
    glUniform1f   ( _temperatureID, temperature ); 
}


void plPlannerStage0Shader::setLocalLoadUniform( PLuint loadLocal ) const
{
    glUniform1ui ( _loadLocalID, loadLocal   );   
    glUniform1ui ( _seedID,      rand()      );  
}
