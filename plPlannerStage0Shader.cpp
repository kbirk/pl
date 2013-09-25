#include "plPlannerStage0Shader.h"

plPlannerStage0Shader::plPlannerStage0Shader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{   
    getUniformLocations();         
}

void plPlannerStage0Shader::getUniformLocations()
{
    _siteMeshSizeID   = glGetUniformLocation(_shaderProgramID, "uSiteMeshSize");   
    _siteMeshAreaID   = glGetUniformLocation(_shaderProgramID, "uSiteMeshArea");
    _siteGridSizeID   = glGetUniformLocation(_shaderProgramID, "uSiteGridSize");
    _sitePerimSizeID  = glGetUniformLocation(_shaderProgramID, "uSitePerimSize");
    
    _temperatureID    = glGetUniformLocation(_shaderProgramID, "uStateTemperature");   
    _loadLocalID      = glGetUniformLocation(_shaderProgramID, "uLoadLocal");

    _seedID           = glGetUniformLocation(_shaderProgramID, "uSeed");   
}


void plPlannerStage0Shader::setSiteUniforms( PLuint  meshSize, 
                                             PLfloat meshArea, 
                                             PLuint  gridSize,
                                             PLuint  perimSize ) const
{
    glUniform1ui  ( _siteMeshSizeID,  meshSize );    
    glUniform1f   ( _siteMeshAreaID,  meshArea ); 
    glUniform1ui  ( _siteGridSizeID,  gridSize ); 
    glUniform1ui  ( _sitePerimSizeID, perimSize);
}                                                  


void plPlannerStage0Shader::setTemperatureUniform( PLfloat temp ) const
{
    glUniform1f   ( _temperatureID, temp ); 
}


void plPlannerStage0Shader::setLocalLoadUniform( PLuint load ) const
{
    glUniform1ui ( _loadLocalID, load   );   
    glUniform1ui ( _seedID,      rand() );  
}
