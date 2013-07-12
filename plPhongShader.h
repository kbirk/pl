#ifndef __PL_PHONG_SHADER_H__
#define __PL_PHONG_SHADER_H__

#include "plCommon.h"
#include "plMinimalShader.h"

class plPhongShader : public plMinimalShader
{
    public:                    
                         
        plPhongShader(const char *vertexFile, const char *fragmentFile) : plMinimalShader(vertexFile, fragmentFile)
		{  
            // get uniform locations
            getUniformLocations();
		}

        virtual void getUniformLocations()
        {			
            plMinimalShader::getUniformLocations();
            // lighting uniform
            _lightPositionID = glGetUniformLocation(_shaderProgramID, "vLightPosition");  

        }        

		void setLightUniform(const plVector3 &lightPosition) const
		{	
		    glUniform3fv(_lightPositionID, 1, (GLfloat*)(&lightPosition));	
		}


    private:

        GLuint _lightPositionID;

};

#endif
