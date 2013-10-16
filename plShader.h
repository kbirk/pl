#ifndef PL_SHADER_H
#define PL_SHADER_H

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

void reportOpenGLError        ( const std::string &str );
void checkOpenGLImplementation();   


PLuint createSSBO( PLuint numBytes, const GLvoid *buffer );


template< class T >
PLuint createSSBO( PLuint count )
{
    PLuint bufferID;
    glGenBuffers(1, &bufferID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, count*sizeof(T), NULL, GL_STREAM_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    return bufferID;
} 


template< class T >
PLuint createSSBO( PLuint count, const T &fill )
{
    plSeq<T> filler( count, fill );

    PLuint bufferID;

    glGenBuffers(1, &bufferID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, count*sizeof(T), &filler[0], GL_STREAM_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    return bufferID;
} 


template< class T >
PLuint createSSBO( const plSeq<T> &array )
{
    PLuint bufferID;

    glGenBuffers(1, &bufferID);     
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, bufferID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, array.size()*sizeof(T), &array[0], GL_STREAM_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
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


template< class T >
plSeq<T> readSSBO( PLuint bufferID, PLuint index, PLuint count )
{
    plSeq<T> ts( count, T() );
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, bufferID );  
    T *temp = readSSBO<T>( index, count );                                  
    memcpy( &ts[0], &temp[0], count*sizeof( T ) );    
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);      
    return ts;                              
}


template< class T >
void copyToSSBO( PLuint bufferID, const plSeq<T> &ts, PLuint byteOffset )
{
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, bufferID );            
    T *mappedBuffer = (T*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                              byteOffset, 
                                              ts.size()*sizeof(T), 
                                              GL_MAP_READ_BIT );                                                                                           
    memcpy( mappedBuffer, &ts[0], ts.size()*sizeof( T ) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );     
}


template< class T >   
void copyFromSSBO( plSeq<T> &ts, PLuint bufferID, PLuint byteOffset )
{
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, bufferID );    
    T *mappedBuffer = (T*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                              byteOffset, 
                                              ts.size()*sizeof(T), 
                                              GL_MAP_READ_BIT );                                                  
    memcpy( &ts[0], &mappedBuffer[0], ts.size()*sizeof(T) );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
}


template< class T >   
void copyFromSSBO( T *ts, PLuint bufferID, PLuint byteOffset, PLuint numBytes )
{
    glBindBuffer( GL_SHADER_STORAGE_BUFFER, bufferID );    
    T *mappedBuffer = (T*) glMapBufferRange( GL_SHADER_STORAGE_BUFFER, 
                                             byteOffset, 
                                             numBytes, 
                                             GL_MAP_READ_BIT );                                                                                      
    memcpy( ts, &mappedBuffer[0], numBytes );
    glUnmapBuffer( GL_SHADER_STORAGE_BUFFER );
}



#endif
