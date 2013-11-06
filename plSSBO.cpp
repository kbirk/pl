#include "plSSBO.h"


plSSBO::plSSBO()
    : _id( 0 )
{
    //_create( 0 );
}


plSSBO::plSSBO( PLuint numBytes, const void *buffer  )
    : _id( 0 )
{
    _create( numBytes, buffer );     
}  


plSSBO::plSSBO( const plSSBO& ssbo )  
{    
    _copy( ssbo );       
}


plSSBO::plSSBO( plSSBO&& ssbo )
    : _id ( ssbo._id ), _numBytes( ssbo._numBytes )

{
    ssbo._numBytes = 0;
    ssbo._id = 0;
}


plSSBO& plSSBO::operator = ( plSSBO&& ssbo )
{
    _destroy();
    _numBytes = ssbo._numBytes;
    _id = ssbo._id;
    ssbo._numBytes = 0;
    ssbo._id = 0;
    return *this;
}


plSSBO& plSSBO::operator = ( const plSSBO& ssbo )
{
    _destroy();
    _copy( ssbo ); 
    return *this;
}


plSSBO::~plSSBO()
{
    _destroy();
}


void plSSBO::_destroy() 
{
    glDeleteBuffers( 1, &_id );
    _id = 0;
}


void plSSBO::_create( PLuint numBytes, const void *buffer )
{
    // set bytes
    _numBytes = numBytes;
    // generate handle if needed
    if ( !_id )
        glGenBuffers(1, &_id);   

    // bind and buffer data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, _id);

    if ( !buffer )
    {
        // initialize empty buffer to all 0's
        PLuint *temp = new PLuint[ numBytes ];
        memset( temp, 0, numBytes );
        glBufferData( GL_SHADER_STORAGE_BUFFER, numBytes, temp, GL_STREAM_COPY );
        delete [] temp;
    }
    else
    {
        glBufferData( GL_SHADER_STORAGE_BUFFER, numBytes, buffer, GL_STREAM_COPY );
    }
    
    // unbind
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    
} 


void plSSBO::_copy( const plSSBO &ssbo )
{
    // read data from previous ssbo
    //std::vector<PLchar> buffer;
    PLchar *buffer = new PLchar[ _numBytes ];

    ssbo.readBytes( buffer, _numBytes );
    // copy number of bytes and create buffer on gpu
    _create( ssbo._numBytes, (void*)buffer );
    
    delete [] buffer;
    // set this data
    //setBytes( buffer, _numBytes );  
}

