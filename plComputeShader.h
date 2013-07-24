#ifndef __PL_COMPUTE_SHADER_H__
#define __PL_COMPUTE_SHADER_H__

#include "plCommon.h"
#include "plShader.h"

class plComputeShader : public plShader
{
    public:                    
          
        plComputeShader(const char *computeFile );
        ~plComputeShader();

        void getUniformLocations();
        void bufferTextures( const plSeq<plVector4> &input, PLuint outputFactor );        
        void dispatch();        
        void loadOutputTexture();
        
    private:
    
        PLuint        _inputSize;
        PLuint        _outputFactor;
        
        PLuint        _input;               
        PLuint        _output;

};

#endif
