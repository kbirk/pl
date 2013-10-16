#ifndef PL_TEXTURE_SHADER_H
#define PL_TEXTURE_SHADER_H

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
