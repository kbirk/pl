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

void plPickingShader::setPickingUniforms( PLint type, PLint id, PLint index ) const // const plPickingInfo &pi ) const
{            
    glUniform1i( _redID,   type  );	
    glUniform1i( _greenID, id    );	
	glUniform1i( _blueID,  index );
}
		
