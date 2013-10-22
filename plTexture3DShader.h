#ifndef PL_TEXTURE_3D_SHADER_H
#define PL_TEXTURE_3D_SHADER_H

#include "plCommon.h"
#include "plMinimalShader.h"

class plTexture3DShader : public plMinimalShader
{
    public:                    
                         
        plTexture3DShader(const char *vertexFile, const char *fragmentFile);
        
        void getUniformLocations();     
		void setTextureUniform  () const;

    private:

        GLuint _textureSamplerID;

};

#endif
