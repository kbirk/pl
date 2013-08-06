#include "plMinimalShader.h"

plMinimalShader::plMinimalShader(const char *vertexFile, const char *fragmentFile) 
    : plShader(vertexFile, fragmentFile)
{  
    // get uniform locations
    getUniformLocations();
}

void plMinimalShader::getUniformLocations()
{			
	// transformation uniform locations
	_modelID      = glGetUniformLocation(_shaderProgramID, "mModel"      );	
    _viewID       = glGetUniformLocation(_shaderProgramID, "mView"       );
    _projectionID = glGetUniformLocation(_shaderProgramID, "mProjection" );                     
    _colourID     = glGetUniformLocation(_shaderProgramID, "cColour"     );   
}

void plMinimalShader::setTransformUniforms( const plMatrix44 &model, const plMatrix44 &view, const plMatrix44 &projection ) const
{				
    // minimal transformation uniforms				
    glUniformMatrix4fv(_modelID,      1, GL_FALSE, (GLfloat*)(&model) );
    glUniformMatrix4fv(_viewID,       1, GL_FALSE, (GLfloat*)(&view) );
    glUniformMatrix4fv(_projectionID, 1, GL_FALSE, (GLfloat*)(&projection));           
}

void plMinimalShader::setColourUniform( const plVector4 colour ) const
{
    glUniform4fv(_colourID, 1, (GLfloat*)(&colour));	
}


