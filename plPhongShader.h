#ifndef __PL_PHONG_SHADER_H__
#define __PL_PHONG_SHADER_H__

#include "pl.h"
#include "plShader.h"
//#include "plMatrix44.h"

class plPhongShader : public plShader
{
    public:                    
             
        //plMinimalShader() {}
             
        plPhongShader(const char *vertexFile, const char *fragmentFile) : plShader(vertexFile, fragmentFile)
		{  
            // get uniform locations
            getUniformLocations();
		}
      
        virtual void getUniformLocations()
        {			
			// transformation uniform locations
			//m_modelViewID     = glGetUniformLocation(m_shaderProgramID, "mModelView"    );			
            //m_projectionID    = glGetUniformLocation(m_shaderProgramID, "mProjection"   );                
            _lightPositionID = glGetUniformLocation(_shaderProgramID, "vLightPosition");              
            //_colourID        = glGetUniformLocation(_shaderProgramID, "cColour"       );  
        }
        /*
		virtual void setGlobalUniforms(const matrix &modelView,  
		                               const matrix &projection, 
		                               const vector &lightPosition) const
		{				
            // minimal transformation uniforms				
		    glUniformMatrix4fv(m_modelViewID,  1, GL_FALSE, (GLfloat*)(&modelView) );
            glUniformMatrix4fv(m_projectionID, 1, GL_FALSE, (GLfloat*)(&projection));
           
            // lighting uniforms	
            glUniform3fv(m_lightPositionID,    1, (GLfloat*)(&lightPosition));	
            glUniform3fv(m_colourID,           1, colour);		
            
		}
		*/
		virtual void setLightUniform(const plVector3 &lightPosition) const
		{	
		    glUniform3fv(_lightPositionID, 1, (GLfloat*)(&lightPosition));	
		}
		/*
		virtual void setColourUniform(const float colour[4]) const
		{	
		    glUniform4fv(_colourID, 1, colour);
		}
	    */
    private:
	/*
        GLuint m_modelViewID;
        GLuint m_projectionID; */
        GLuint _lightPositionID;
        //GLuint _colourID;  
        

};

#endif
