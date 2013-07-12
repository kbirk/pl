#ifndef __PL_COMPUTE_SHADER_H__
#define __PL_COMPUTE_SHADER_H__

#include "plCommon.h"
#include "plShader.h"

class plComputeShader : public plShader
{
    public:                    
          
        plComputeShader(const char *computeFile) : plShader(computeFile, GL_COMPUTE_SHADER) {}

        void getUniformLocations()
        {	
        }

};

#endif
