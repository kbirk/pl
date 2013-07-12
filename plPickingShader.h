#ifndef __PL_PICKING_SHADER_H__
#define __PL_PICKING_SHADER_H__

#include "plCommon.h"
#include "plMinimalShader.h"
#include "plPickingTexture.h"

class plPickingShader : public plMinimalShader
{
    public:                    
               
        plPickingShader(const char *vertexFile, const char *fragmentFile) : plMinimalShader(vertexFile, fragmentFile)
		{  
            // get uniform locations
            getUniformLocations();
		}
      
        void getUniformLocations()
        {			
            plMinimalShader::getUniformLocations();            
            _redID 	   = glGetUniformLocation(_shaderProgramID, "uRedBits"); 
			_greenID   = glGetUniformLocation(_shaderProgramID, "uGreenBits");  			
			_blueID    = glGetUniformLocation(_shaderProgramID, "uBlueBits");  			 
        }

		void setPickingUniforms(const plPickingInfo &pi) const
        {            
		    glUniform1i(_redID,   pi.type);	
            glUniform1i(_greenID, pi.id);	
			glUniform1i(_blueID,  pi.index);
		}
		
    private:
	
		GLuint _redID;
		GLuint _greenID;
		GLuint _blueID;
		


};

#endif
