#include "plPlannerStage0Shader.h"

plPlannerStage0Shader::plPlannerStage0Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}

void plPlannerStage0Shader::getUniformLocations()
{
    _siteMeshSizeID        = glGetUniformLocation(_shaderProgramID, "uSiteMeshSize");   
    _siteMeshAreaID        = glGetUniformLocation(_shaderProgramID, "uSiteMeshArea");
    _siteGridSizeID        = glGetUniformLocation(_shaderProgramID, "uSiteGridSize");
    _sitePerimSizeID       = glGetUniformLocation(_shaderProgramID, "uSitePerimSize");
    
    _stateTemperatureID    = glGetUniformLocation(_shaderProgramID, "uStateTemperature");   
    _stateEnergyID         = glGetUniformLocation(_shaderProgramID, "uStateEnergy");   
    _stateGraftCountID     = glGetUniformLocation(_shaderProgramID, "uStateGraftCount"); 
    _stateGraftPositionsID = glGetUniformLocation(_shaderProgramID, "uStateGraftPositions"); 
    _stateGraftNormalsID   = glGetUniformLocation(_shaderProgramID, "uStateGraftNormals"); 
    _stateGraftRadiiID     = glGetUniformLocation(_shaderProgramID, "uStateGraftRadii"); 

    _loadLocalID           = glGetUniformLocation(_shaderProgramID, "uLoadLocal");

    _seedID                = glGetUniformLocation(_shaderProgramID, "uSeed");   
}


void plPlannerStage0Shader::setSiteUniforms     ( PLuint  meshSize, 
                                                  PLfloat meshArea, 
                                                  PLuint  gridSize,
                                                  PLuint  perimSize ) const
{
    glUniform1ui  ( _siteMeshSizeID,  meshSize );    
    glUniform1f   ( _siteMeshAreaID,  meshArea ); 
    glUniform1ui  ( _siteGridSizeID,  gridSize ); 
    glUniform1ui  ( _sitePerimSizeID, perimSize);
}                                                  

void plPlannerStage0Shader::setAnnealingUniforms( PLfloat temp, 
                                                  PLfloat energy, 
                                                  PLfloat count,
                                                  const plSeq<plVector4> &positions,
                                                  const plSeq<plVector4> &normals,
                                                  const plSeq<PLfloat>   &radii ) const
{
    glUniform1f   ( _stateTemperatureID, temp   );     
    glUniform1f   ( _stateEnergyID,      energy ); 
    glUniform1ui  ( _stateGraftCountID,  count  ); 
    
    glUniform4fv  ( _stateGraftPositionsID, positions.size(), &positions[0].x );
    glUniform4fv  ( _stateGraftNormalsID,   normals.size(),   &normals[0].x   );
    glUniform1fv  ( _stateGraftRadiiID,     radii.size(),     &radii[0]       );  
}

void plPlannerStage0Shader::setLocalLoadUniform( PLuint load ) const
{
    glUniform1i  ( _loadLocalID, load );   
    glUniform1ui ( _seedID, rand() );  
}
