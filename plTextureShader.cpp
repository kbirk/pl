#include "plTextureShader.h"
                     
plTextureShader::plTextureShader(const char *vertexFile, const char *fragmentFile) 
    : plMinimalShader(vertexFile, fragmentFile)
{  
    // get uniform locations
    getUniformLocations();
}


void plTextureShader::getUniformLocations()
{			
    plMinimalShader::getUniformLocations();
    // texture uniform
    _textureSamplerID = glGetUniformLocation( _shaderProgramID, "textureSampler");
}
    

void plTextureShader::setTextureUniform() const
{	
    // set texture uniforms
    glUniform1i(_textureSamplerID,  0);  // texture unit 0
}

