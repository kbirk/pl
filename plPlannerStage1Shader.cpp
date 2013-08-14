#include "plPlannerStage1Shader.h"

plPlannerStage1Shader::plPlannerStage1Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}


plPlannerStage1Shader::~plPlannerStage1Shader()
{
}


void plPlannerStage1Shader::getUniformLocations()
{
    _siteMeshSizeID = glGetUniformLocation(_shaderProgramID, "uSiteMeshSize");   
    _siteAreaID     = glGetUniformLocation(_shaderProgramID, "uSiteArea");

    _temperatureID  = glGetUniformLocation(_shaderProgramID, "uTemperature");
    
    _stateEnergyID  = glGetUniformLocation(_shaderProgramID, "uStateEnergy");
    _stateGraftsID  = glGetUniformLocation(_shaderProgramID, "uStateGrafts");
}


void plPlannerStage1Shader::setAnnealingUniforms( PLuint size, PLfloat area, PLfloat temp, PLfloat energy, const plSeq<PLuint> &grafts )
{
    glUniform1ui  ( _siteMeshSizeID, size );    
    glUniform1f   ( _siteAreaID,     area ); 
    glUniform1f   ( _temperatureID,  temp ); 
    glUniform1f   ( _stateEnergyID,  energy ); 
    glUniform1uiv ( _stateGraftsID, grafts.size(), &grafts[0] );     
}
