#include "plVAO.h"

plVAO::plVAO() 
    : _id( 0 )
{
}


plVAO::plVAO( const plVAO& vao )
    : _id( 0 ) 
{
    _copy( vao ); 
}  

     
plVAO::plVAO( plVAO&& vao )
    : _id( 0 ) 
{
    _move( std::move( vao ) );
}


plVAO& plVAO::operator = ( const plVAO &vao ) 
{
    _copy( vao );
    return *this;
}


plVAO& plVAO::operator= ( plVAO&& vao )
{
    _move( std::move( vao ) );
    return *this;
}


plVAO::~plVAO()
{
    _destroy();
}


void plVAO::attach( const std::shared_ptr< plVBO >& vbo )
{
    _vbos.push_back( vbo );    

}


void plVAO::attach( const std::shared_ptr< plEABO >& eabo )
{
    _eabo = eabo;
}


void plVAO::clear()
{
    _eabo = nullptr;
    _vbos.clear();
}


void plVAO::draw() const
{	
    if ( _eabo && !_vbos.empty() )
	{
        // bind vertex array object
	    glBindVertexArray( _id );
	    // draw
        _eabo->drawElements();
        // unbind vao
	    glBindVertexArray( 0 ); 	
    }
}


void plVAO::upload()
{
    if ( !_eabo )
    {
        std::cerr << " plVAO::upload() error: cannot upload to GPU, VAO has no EABO attached, command ignored " << std::endl;
        return;
    }
    
    if ( _vbos.empty() )
    {
        std::cerr << " plVAO::upload() error: cannot upload to GPU, VAO has no VBOs attached, command ignored " << std::endl;
        return;
    }
    
    // create and bind VAO
    if ( !_id )
	    glGenVertexArrays( 1, &_id );
	    
    glBindVertexArray( _id );
    
    for ( auto& vbo : _vbos )
    {
        vbo->upload();
    }
    
    _eabo->upload();
    
    glBindVertexArray( 0 );
}


void plVAO::_copy( const plVAO& vao )
{
    attach( std::shared_ptr< plEABO >( new plEABO( *vao._eabo ) ) );
    for ( auto& vbo : vao._vbos )
    {
        attach( std::shared_ptr< plVBO >( new plVBO( *vbo ) ) );
    }

    upload();
}


void plVAO::_move( plVAO&& vao )
{
    // move vao
    _id = vao._id;

    // move vbo and eabo
    _vbos = std::move( vao._vbos );
    _eabo = std::move( vao._eabo );   
    
    vao._id = 0; 
}


void plVAO::_destroy()
{   
    glDeleteVertexArrays( 1, &_id) ;	// delete vao
    _id = 0;
}


