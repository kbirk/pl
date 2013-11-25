#include "plVertexFragmentShader.h"

plVertexFragmentShader::plVertexFragmentShader( const char *vertexFile, const char *fragmentFile ) 
{  
    // create and compile shader, tests for errors
    GLuint vertexShader   = _createAndCompileShader( vertexFile,   GL_VERTEX_SHADER   );
    GLuint fragmentShader = _createAndCompileShader( fragmentFile, GL_FRAGMENT_SHADER );

	// create program
	_id = glCreateProgram();

	// attach shaders
	glAttachShader( _id, vertexShader   );
	glAttachShader( _id, fragmentShader );
	
    // link program, check for error
	_linkProgram();

	// free shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	   
    // get uniform locations
    _getUniformLocations();
}


void plVertexFragmentShader::_getUniformLocations()
{			
	// transformations        
	_modelMatrixUniformID      = glGetUniformLocation( _id, PL_MODEL_MATRIX_UNIFORM_NAME );	
	_viewMatrixUniformID       = glGetUniformLocation( _id, PL_VIEW_MATRIX_UNIFORM_NAME );	
	_projectionMatrixUniformID = glGetUniformLocation( _id, PL_PROJECTION_MATRIX_UNIFORM_NAME );
    // colour
	_colourUniformID           = glGetUniformLocation( _id, PL_COLOUR_UNIFORM_NAME );
	// light position
	_lightPositionUniformID    = glGetUniformLocation( _id, PL_LIGHT_POSITION_UNIFORM_NAME );
	// picking
	_pickingUniformID          = glGetUniformLocation( _id, PL_PICKING_UNIFORM_NAME );
	// outline
	_outlineUniformID          = glGetUniformLocation( _id, PL_OUTLINE_UNIFORM_NAME );
    // texturing
	_textureUnit0SamplerID     = glGetUniformLocation( _id, PL_TEXTURE_UNIT_0_SAMPLER_UNIFORM_NAME );
	_textureUnit1SamplerID     = glGetUniformLocation( _id, PL_TEXTURE_UNIT_1_SAMPLER_UNIFORM_NAME );
	_textureUnit2SamplerID     = glGetUniformLocation( _id, PL_TEXTURE_UNIT_2_SAMPLER_UNIFORM_NAME );
	_textureUnit3SamplerID     = glGetUniformLocation( _id, PL_TEXTURE_UNIT_3_SAMPLER_UNIFORM_NAME );	
}	


void plVertexFragmentShader::setUniform( const plUniform& uniform ) const
{
    switch ( uniform.type() )
    {
        // minimal
        case PL_MODEL_MATRIX_UNIFORM:         glUniformMatrix4fv( _modelMatrixUniformID,      1, GL_FALSE, (GLfloat*)( uniform.data() ) );    break;
        case PL_VIEW_MATRIX_UNIFORM:          glUniformMatrix4fv( _viewMatrixUniformID,       1, GL_FALSE, (GLfloat*)( uniform.data() ) );    break;
        case PL_PROJECTION_MATRIX_UNIFORM:    glUniformMatrix4fv( _projectionMatrixUniformID, 1, GL_FALSE, (GLfloat*)( uniform.data() ) );    break;
        case PL_COLOUR_UNIFORM:               glUniform4fv      ( _colourUniformID,           1, (GLfloat*)( uniform.data() ) );	          break;
        // phong
        case PL_LIGHT_POSITION_UNIFORM:       glUniform3fv      ( _lightPositionUniformID,    1, (GLfloat*)( uniform.data() ) );	          break;
        // picking
        case PL_PICKING_UNIFORM:              glUniform3iv      ( _pickingUniformID,          1, (GLint*)( uniform.data() ) );	              break;
        // outline 
        case PL_OUTLINE_UNIFORM:              glUniform3iv      ( _outlineUniformID,          1, (GLint*)( uniform.data() ) );                break;
        // texture
        case PL_TEXTURE_UNIT_0_UNIFORM: 
        {   
            glActiveTexture( GL_TEXTURE0 );   
            const plTexture2D* tex = *( plTexture2D** )( uniform.data() );
            tex->bind( 0 );
            glUniform1i( _textureUnit0SamplerID, 0 );    
            break;
        }    
        case PL_TEXTURE_UNIT_1_UNIFORM:
        {   
            glActiveTexture( GL_TEXTURE1 );   
            const plTexture2D* tex = *( plTexture2D** )( uniform.data() );
            tex->bind( 1 );
            glUniform1i( _textureUnit1SamplerID, 1 );    
            break;
        }    
        case PL_TEXTURE_UNIT_2_UNIFORM:
        {
            glActiveTexture( GL_TEXTURE2 );
            const plTexture2D* tex = *( plTexture2D** )( uniform.data() );
            tex->bind( 2 );
            glUniform1i( _textureUnit2SamplerID, 2 );
            break;
        }
        case PL_TEXTURE_UNIT_3_UNIFORM:    glUniform1i( _textureUnit3SamplerID,  0 );    break;
    }
}

