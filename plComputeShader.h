#ifndef __PL_COMPUTE_SHADER_H__
#define __PL_COMPUTE_SHADER_H__

#include "pl.h"
#include "plShader.h"

class plComputeShader : public plShader
{
    public:                    
          
        plComputeShader(const char *computeFile) : plShader(computeFile, GL_COMPUTE_SHADER)
        {
        }

};

#endif
