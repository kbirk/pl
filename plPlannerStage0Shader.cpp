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
}


void plPlannerStage0Shader::setMeshSizeUniform( PLuint size )
{
    glUniform1ui(_siteMeshSizeID, size );
}


