#ifndef __PL_PHONG_SHADER_H__
#define __PL_PHONG_SHADER_H__

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
