#ifndef __PL_TEXTURE_SHADER_H__
#define __PL_TEXTURE_SHADER_H__

#include "plCommon.h"
#include "plMinimalShader.h"

class plTextureShader : public plMinimalShader
{
    public:                    
                         
        plTextureShader(const char *vertexFile, const char *fragmentFile);
        
        void getUniformLocations();     
		void setTextureUniform  () const;

    private:

        GLuint _textureSamplerID;

};

#endif
