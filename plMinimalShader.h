#ifndef __PL_MINIMAL_SHADER_H__
#define __PL_MINIMAL_SHADER_H__

#include "plCommon.h"
#include "plShader.h"

class plMinimalShader : public plShader
{
    public:                    
             
        plMinimalShader(const char *vertexFile, const char *fragmentFile) : plShader(vertexFile, fragmentFile)
		{  
            // get uniform locations
            getUniformLocations();
		}
      
        virtual void getUniformLocations()
        {			
			// transformation uniform locations
			_modelID      = glGetUniformLocation(_shaderProgramID, "mModel"      );	
            _viewID       = glGetUniformLocation(_shaderProgramID, "mView"       );
            _projectionID = glGetUniformLocation(_shaderProgramID, "mProjection" );                     
            _colourID     = glGetUniformLocation(_shaderProgramID, "cColour"     );   
        }
        
		virtual void setTransformUniforms( const plMatrix44 &model, const plMatrix44 &view, const plMatrix44 &projection ) const
		{				
            // minimal transformation uniforms				
		    glUniformMatrix4fv(_modelID,      1, GL_FALSE, (GLfloat*)(&model) );
            glUniformMatrix4fv(_viewID,       1, GL_FALSE, (GLfloat*)(&view) );
            glUniformMatrix4fv(_projectionID, 1, GL_FALSE, (GLfloat*)(&projection));           
		}

        virtual void setColourUniform( const plVector4 colour ) const
        {
            glUniform4fv(_colourID, 1, (GLfloat*)(&colour));	
        }


    private:
	
        GLuint _modelID;
        GLuint _viewID;
        GLuint _projectionID; 
        GLuint _colourID; 
        

};

#endif
