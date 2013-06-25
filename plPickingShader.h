#ifndef __PL_PICKING_SHADER_H__
#define __PL_PICKING_SHADER_H__

#include "pl.h"
#include "plShader.h"
#include "plMatrix44.h"
#include "plPickingTexture.h"

class plPickingShader : public plShader
{
    public:                    
               
        plPickingShader(const char *vertexFile, const char *fragmentFile) : plShader(vertexFile, fragmentFile)
		{  
            // get uniform locations
            getUniformLocations();
		}
      
        void getUniformLocations()
        {			
			// transformation uniform locations
			//m_modelViewID  = glGetUniformLocation(m_shaderProgramID, "mModelView");			
            //m_projectionID = glGetUniformLocation(m_shaderProgramID, "mProjection");                
            _redID 	   = glGetUniformLocation(_shaderProgramID, "uRedBits"); 
			_greenID   = glGetUniformLocation(_shaderProgramID, "uGreenBits");  			
			_blueID    = glGetUniformLocation(_shaderProgramID, "uBlueBits");  			 
        }
        /*           
		virtual void setGlobalUniforms(const plMatrix44 &modelView,  
									   const plMatrix44 &projection) const
		{				
            // minimal transformation uniforms				
			glUniformMatrix4fv(m_modelViewID,  1, GL_FALSE, (GLfloat*)(&modelView) );
            glUniformMatrix4fv(m_projectionID, 1, GL_FALSE, (GLfloat*)(&projection));            
		}
		*/
		void setPickingUniforms(const plPickingInfo *pi) const
		{	
		    glUniform1i(_redID,   pi->type);	
            glUniform1i(_greenID, pi->id);	
			glUniform1i(_blueID,  pi->index);
		}
		
		void setPickingUniforms(int red, int green, int blue) const
		{	
		    glUniform1i(_redID,   red);	
            glUniform1i(_greenID, green);	
			glUniform1i(_blueID,  blue);
		}
		
    private:
	
        //GLuint m_modelViewID;
        //GLuint m_projectionID; 

		GLuint _redID;
		GLuint _greenID;
		GLuint _blueID;
		


};

#endif
