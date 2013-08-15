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
    _siteMeshSizeID = glGetUniformLocation(_shaderProgramID, "uSiteMeshSize");   
    _siteAreaID     = glGetUniformLocation(_shaderProgramID, "uSiteArea");

    _stateTemperatureID   = glGetUniformLocation(_shaderProgramID, "uStateTemperature");   
    _stateEnergyID        = glGetUniformLocation(_shaderProgramID, "uStateEnergy");
    _stateIndicesID       = glGetUniformLocation(_shaderProgramID, "uStateIndices");
    _statePerturbationsID = glGetUniformLocation(_shaderProgramID, "uStatePerturbations");
    _stateGraftCountsID   = glGetUniformLocation(_shaderProgramID, "uStateGraftCount");    
}

void plPlannerStage0Shader::setAnnealingUniforms( PLuint size, 
                                                  PLfloat area, 
                                                  PLfloat temp, 
                                                  PLfloat energy, 
                                                  PLfloat count,
                                                  const plSeq<PLuint> &indices,
                                                  const plSeq<plMatrix44> &perturbations )
{
    glUniform1ui  ( _siteMeshSizeID, size );    
    glUniform1f   ( _siteAreaID,     area ); 
    
    glUniform1f        ( _stateTemperatureID, temp );     
    glUniform1f        ( _stateEnergyID, energy ); 
    glUniform1ui       ( _stateGraftCountsID,  count ); 
    glUniform1uiv      ( _stateIndicesID, indices.size(), &indices[0] );   
    glUniformMatrix4fv ( _statePerturbationsID, perturbations.size(), false, &perturbations[0][0] );   
}
