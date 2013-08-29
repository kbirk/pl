#ifndef __PL_SHADER_H__
#define __PL_SHADER_H__

#include "plCommon.h"
#include "plSeq.h"

// attribute locations, set by VBOs and mirrored in shader files
#define PL_POSITION_ATTRIBUTE  0
#define PL_NORMAL_ATTRIBUTE    1
#define PL_COLOUR_ATTRIBUTE    2
#define PL_TEXCOORD_ATTRIBUTE  3

class plShader 
{

    public:                    
          
        plShader(const char *shaderFile, GLenum shaderType);           
        plShader(const char *vertexFile, const char *fragmentFile);

		~plShader();
                
		void bind()   const { glUseProgram(_shaderProgramID);  }	    
		void unbind() const { glUseProgram(0);                 }
		
		virtual void getUniformLocations() = 0;
		
    protected:

        GLuint _shaderProgramID;

        char*  _readShaderFile(const char *filename);                
        GLuint _createShader(const char *shaderFile, GLenum shaderType);        
        GLuint _compileShader(GLuint shader);        
        void   _printCompileError(GLuint shader);        
        GLuint _linkProgram();        
        void   _printLinkError();

};


template< class T >
PLuint createSSBO( PLuint count, const T &fill )
{
    plSeq<T> filler( fill, count );

    PLuint bufferID;

    glGenBuffers(1, &bufferID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, count*sizeof(T), &filler[0], GL_STREAM_COPY);
    
    return bufferID;
} 


template< class T >
T* readSSBO( PLuint index, PLuint count )
{
    return (T*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                  index*sizeof(T), 
                                  count*sizeof(T),
                                  GL_MAP_READ_BIT );   
}


#endif
