#ifndef __PL_TEXTURE_SHADER_H__
#define __PL_TEXTURE_SHADER_H__

#include "plCommon.h"
#include "plMinimalShader.h"

class plTextureShader : public plMinimalShader
{
    public:                    
                         
        plTextureShader(const char *vertexFile, const char *fragmentFile) : plMinimalShader(vertexFile, fragmentFile)
		{  
            // get uniform locations
            getUniformLocations();
		}

        virtual void getUniformLocations()
        {			
            plMinimalShader::getUniformLocations();
            // texture uniform
            _textureSamplerID = glGetUniformLocation( _shaderProgramID, "textureSampler");
        }        

		void setTextureUniform() const
		{	
		    // set texture uniforms
            glUniform1i(_textureSamplerID,  0);  // texture unit 0
		}

    private:

        GLuint _textureSamplerID;

};

#endif
