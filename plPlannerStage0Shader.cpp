#include "plPlannerStage0Shader.h"

plPlannerStage0Shader::plPlannerStage0Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


plPlannerStage0Shader::~plPlannerStage0Shader()
{
}


void plPlannerStage0Shader::getUniformLocations()
{
    _siteMeshSizeID       = glGetUniformLocation(_shaderProgramID, "uSiteMeshSize");   
    _siteMeshAreaID       = glGetUniformLocation(_shaderProgramID, "uSiteMeshArea");
    _siteGridSizeID       = glGetUniformLocation(_shaderProgramID, "uSiteGridSize");
    
    _stateTemperatureID   = glGetUniformLocation(_shaderProgramID, "uStateTemperature");   
    _stateEnergyID        = glGetUniformLocation(_shaderProgramID, "uStateEnergy");
    _stateGraftCountID    = glGetUniformLocation(_shaderProgramID, "uStateGraftCount"); 
    _stateIndicesID       = glGetUniformLocation(_shaderProgramID, "uStateIndices");
    _statePerturbationsID = glGetUniformLocation(_shaderProgramID, "uStatePerturbations");    
}

void plPlannerStage0Shader::setAnnealingUniforms( PLuint  meshSize, 
                                                  PLfloat meshArea, 
                                                  PLuint  gridSize,
                                                  PLfloat temp, 
                                                  PLfloat energy, 
                                                  PLfloat count,
                                                  const plSeq<PLuint>     &indices,
                                                  const plSeq<plMatrix44> &perturbations )
{
    glUniform1ui  ( _siteMeshSizeID, meshSize );    
    glUniform1f   ( _siteMeshAreaID, meshArea ); 
    glUniform1ui  ( _siteGridSizeID, gridSize ); 
    
    glUniform1f        ( _stateTemperatureID, temp   );     
    glUniform1f        ( _stateEnergyID,      energy ); 
    glUniform1ui       ( _stateGraftCountID,  count  ); 
    glUniform1uiv      ( _stateIndicesID, indices.size(), &indices[0] );   
    glUniformMatrix4fv ( _statePerturbationsID, perturbations.size(), false, &perturbations[0][0] );  
}
