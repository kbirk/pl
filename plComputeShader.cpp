#include "plComputeShader.h"

plComputeShader::plComputeShader(const char *computeFile ) 
    : plShader(computeFile, GL_COMPUTE_SHADER)              
{            
}

plComputeShader::~plComputeShader()
{
    glDeleteTextures(1, &_input);
    glDeleteTextures(1, &_output);

}

void plComputeShader::getUniformLocations()
{
}

void plComputeShader::bufferTextures( const plSeq<plVector4> &input, PLuint outputFactor )
{
    // load input data
    _inputSize = input.size();
    
    glGenTextures(1, &_input);
    glBindTexture(GL_TEXTURE_1D, _input);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA32F, input.size(), 0, GL_RGBA, GL_FLOAT, &input[0]);

    // create empty texture for output
    _outputFactor = outputFactor;
                           
    glGenTextures(1, &_output);                              
    glBindTexture(GL_TEXTURE_2D, _output);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, input.size(), _outputFactor, 0, GL_RGBA, GL_FLOAT, NULL);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void plComputeShader::dispatch()
{
    // bind compute shader
    bind();
    
    // bind input/output buffers            
    glBindImageTexture(0, _input, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, _output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute( ceil(_inputSize / 1024), 1, 1 );
    
    // memory barrier
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
                
    // read output into array            
    glBindTexture(GL_TEXTURE_2D, _output);
    
    plVector4 *pixels = new plVector4[_inputSize * _outputFactor];
          
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, &pixels[0]);

    for (int i=0; i<_inputSize*_outputFactor; i+= _inputSize)
    {
        std::cout << "v1: " << pixels[i] << "\n";
    }
}

void plComputeShader::loadOutputTexture()
{
    

}
        

