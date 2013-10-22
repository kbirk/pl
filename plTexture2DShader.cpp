#include "plTexture2DShader.h"
                     
plTexture2DShader::plTexture2DShader(const char *vertexFile, const char *fragmentFile) 
    : plMinimalShader(vertexFile, fragmentFile)
{  
    // get uniform locations
    getUniformLocations();
}


void plTexture2DShader::getUniformLocations()
{			
    plMinimalShader::getUniformLocations();
    // texture uniform
    _textureSamplerID = glGetUniformLocation( _shaderProgramID, "textureSampler");
}
    

void plTexture2DShader::setTextureUniform() const
{	
    // set texture uniforms
    glUniform1i(_textureSamplerID,  0);  // texture unit 0
}

