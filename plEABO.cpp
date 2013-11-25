#include "plEABO.h"


plEABO::plEABO()
{
}


plEABO::plEABO( const std::vector<GLuint>& data, PLuint mode, PLuint usage )
{
    set( data, mode, usage );
}


plEABO::plEABO( const plEABO& eabo )
{
    _copy( eabo ); 
}  

     
plEABO::plEABO( plEABO&& eabo )
{
    _move( std::move( eabo ) );
}


plEABO& plEABO::operator = ( const plEABO &eabo ) 
{ 
    _copy( eabo );
    return *this;
}


plEABO& plEABO::operator= ( plEABO&& eabo )
{
    _move( std::move( eabo ) );
    return *this;
}


void plEABO::set( const std::vector<PLuint>& data, PLuint mode, PLuint usage )
{
    _data = data;
    _numBytes = _data.size() * sizeof( PLuint );
    _type = GL_UNSIGNED_INT;
    _mode = mode;
    _usage = usage;
    //_flagChange();
}


void plEABO::clear()
{
    _data.clear();
    //_flagChange();
}


void plEABO::upload()
{
    if ( _data.empty() )
    {
        std::cerr << "plEABO::upload() error: data buffer empty, ignoring command" << std::endl;    
        return;
    }
    
    /*
    if ( !_changeFlag )
        return; // no need to re-upload
    */

    // if buffer not allocated, generate
	if ( !_id )
        glGenBuffers( 1, &_id );
	    
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _id );   
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, _numBytes, &_data[0], _usage );
  
    //_changeFlag = false; 
    // do not unbind buffer as this will most likely be called from a vao
}


void plEABO::drawElements( PLuint index ) const
{
    glDrawElements( _mode, _data.size(), _type, (GLvoid*)(index) ); 
}


void plEABO::_copy( const plEABO &eabo )
{
    set( eabo._data, eabo._mode, eabo._usage );
    
    // see if buffer to copy has been uploaded, if it has, upload the copy as well
    /*
    if ( !eabo._changeFlag )
        upload();
    */
}


void plEABO::_move( plEABO&& eabo )
{
    _id = eabo._id;
    _numBytes = eabo._numBytes;    
    _type = eabo._type;
    _mode = eabo._mode;
    _data = std::move( eabo._data );
    //_vao = eabo._vao;
    
    eabo._id = 0;  
    eabo._numBytes = 0;
    eabo._type = 0;
    eabo._mode = 0;
    //eabo._vao = nullptr;
}

