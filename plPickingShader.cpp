#include "plPickingShader.h"
            
plPickingShader::plPickingShader(const char *vertexFile, const char *fragmentFile) 
    : plMinimalShader(vertexFile, fragmentFile)
{  
    // get uniform locations
    getUniformLocations();
}

void plPickingShader::getUniformLocations()
{			
    plMinimalShader::getUniformLocations();            
    _redID 	   = glGetUniformLocation(_shaderProgramID, "uRedBits"); 
	_greenID   = glGetUniformLocation(_shaderProgramID, "uGreenBits");  			
	_blueID    = glGetUniformLocation(_shaderProgramID, "uBlueBits");  			 
}

void plPickingShader::setPickingUniforms(const plPickingInfo &pi) const
{            
    glUniform1i(_redID,   pi.type);	
    glUniform1i(_greenID, pi.id);	
	glUniform1i(_blueID,  pi.index);
}
		
