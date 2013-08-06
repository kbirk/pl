#include "plPhongShader.h"
                   
plPhongShader::plPhongShader(const char *vertexFile, const char *fragmentFile) 
    : plMinimalShader(vertexFile, fragmentFile)
{  
    // get uniform locations
    getUniformLocations();
}

void plPhongShader::getUniformLocations()
{			
    plMinimalShader::getUniformLocations();
    // lighting uniform
    _lightPositionID = glGetUniformLocation(_shaderProgramID, "vLightPosition");  

}        

void plPhongShader::setLightUniform(const plVector3 &lightPosition) const
{	
    glUniform3fv(_lightPositionID, 1, (GLfloat*)(&lightPosition));	
}

