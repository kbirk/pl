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

/*

template< class T >
PLuint createBO( PLuint type, PLuint size, const T *array )
{
    PLuint bufferID;
    glGenBuffers(1, &bufferID);     
    glBindBuffer(type, bufferID);
    glBufferData(type, size*sizeof(T), array, GL_STREAM_COPY);
    glBindBuffer(type, 0);
    return bufferID;
} 


template< class T >
PLuint createBO( PLuint type, plSeq<T> array )
{
    return createBO( type, array.size(), &array[0] );
} 


template< class T >
PLuint createBO( PLuint type, PLuint count, const T &fill )
{
    plSeq<T> array( count, fill );
    return createBO( type, array );
} 


template< class T >
PLuint createBO( PLuint type, PLuint count )
{
    return createBO( type, count, NULL );
} 


template< class T >
T* readBO( PLuint type, PLuint index, PLuint count )
{
    return (T*) glMapBufferRange( type, 
                                  index*sizeof(T), 
                                  count*sizeof(T),
                                  GL_MAP_READ_BIT );   
}
*/


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


PLuint createSSBO( PLuint numBytes, const GLvoid *buffer );


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




#endif
