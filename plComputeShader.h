#ifndef __PL_COMPUTE_SHADER_H__
#define __PL_COMPUTE_SHADER_H__

#include "plCommon.h"
#include "plShader.h"


class plComputeShader : public plShader
{
    public:                    
          
        plComputeShader(const char *computeFile ) 
            : plShader(computeFile, GL_COMPUTE_SHADER)              
        {            
        }

        void clearBuffers()
        {
            //_data.clear();
            
            for (PLuint i=0; i<_dataBuffers.size(); i++)
            {
                glDeleteBuffers(1, &_dataBuffers[i]);	    // delete buffer objects 
            }
            
            _dataBuffers.clear();
        }

        void getUniformLocations()
        {	
        }

        template< class T>
        void bufferUniformData( const plSeq<T> &data )
        {
            // load data
            _dataBuffers.add( 0 );
            //glGenBuffers(1, &_dataBuffers.back() );
            //glBindBuffer(GL_ARRAY_BUFFER, _dataBuffers.back() );
            //glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], GL_DYNAMIC_COPY); // not sure what to put as usage, they are only hints anyway
     
     
            // load data
            glGenBuffers(1, &_dataBuffers.back());
            glBindBuffer(GL_UNIFORM_BUFFER, _dataBuffers.back());
            glBufferData(GL_UNIFORM_BUFFER, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);
            
            
     
     
        }

        void dispatch( PLuint numGroupsX, PLuint numGroupsY, PLuint numGroupsZ )
        {
            // bind compute shader
            bind();
            
            // bind uniform buffers
            for (PLuint i=0; i<_dataBuffers.size(); i++)
            {
                glBindBufferBase(GL_UNIFORM_BUFFER, i, _dataBuffers[i]);
            }
            
            
            
            glDispatchCompute( numGroupsX, numGroupsY, numGroupsZ );
            
            // memory barrier
            
            // clear data sequence
            clearBuffers(); 
        }

    private:
    
        plSeq<PLuint>        _dataBuffers;

};

#endif
