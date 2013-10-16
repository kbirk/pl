#ifndef PL_PHONG_SHADER_H
#define PL_PHONG_SHADER_H

#include "plCommon.h"
#include "plMinimalShader.h"

class plPhongShader : public plMinimalShader
{
    public:                    
                         
        plPhongShader(const char *vertexFile, const char *fragmentFile);

        virtual void getUniformLocations();      
		virtual void setLightUniform    (const plVector3 &lightPosition) const;

    private:

        GLuint _lightPositionID;

};

#endif
