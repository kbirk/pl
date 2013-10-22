#ifndef PL_TEXTURE_2D_SHADER_H
#define PL_TEXTURE_2D_SHADER_H

#include "plCommon.h"
#include "plMinimalShader.h"

class plTexture2DShader : public plMinimalShader
{
    public:                    
                         
        plTexture2DShader(const char *vertexFile, const char *fragmentFile);
        
        void getUniformLocations();     
		void setTextureUniform  () const;

    private:

        GLuint _textureSamplerID;

};

#endif
