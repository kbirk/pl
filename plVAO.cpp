#include "plVAO.h"

plVAO::plVAO() 
    : _id( 0 ), _changeFlag( false )
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
    // give attached vbo a pointer to its vao, this allows it to modify the vao change flag
    //vbo->_vao = this;
    //vbo->_attachedToVAO = true;
    _vbos.push_back( vbo );    
    //_changeFlag = true;
}


void plVAO::attach( const std::shared_ptr< plEABO >& eabo )
{
    // if eabo is already attached, unattach it
    /*
    if ( _eabo )
    {
        _eabo->_vao = nullptr;
    }
    */

    // give attached vbo a pointer to its vao, this allows it to modify the vao change flag
    //eabo->_vao = this;
    //eabo->_attachedToVAO = true;
    _eabo = eabo;
    //_changeFlag = true;
}


void plVAO::clear()
{
    // if eabo is already attached, unattach it
    /*
    if ( _eabo )
        _eabo->_vao = nullptr;   
    */    
    _eabo = nullptr;
    
    /*
    for ( auto vbo : _vbos )
    {
        vbo->_vao = nullptr;
    }    
    */
    _vbos.clear();
    //_changeFlag = true;
}


void plVAO::draw() const
{		
	// bind vertex array object
	glBindVertexArray( _id );
	// draw
	_eabo->drawElements();
    // unbind vao
	glBindVertexArray( 0 ); 	
}

/*
void plVAO::draw( const std::vector<PLuint> &indices )
{
    // bind vertex array object
	glBindVertexArray( _id );		
    // buffer new index data
    _eabo->set( indices );       
	// draw batch
	glDrawElements( _mode, _eabo->numIndices(), GL_UNSIGNED_INT, 0 ); 
    // unbind vao
	glBindVertexArray( 0 ); 
}
*/

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

    //_changeFlag = false;
}


void plVAO::_copy( const plVAO& vao )
{
    attach( std::shared_ptr< plEABO >( new plEABO( *vao._eabo ) ) );
    for ( auto& vbo : vao._vbos )
    {
        attach( std::shared_ptr< plVBO >( new plVBO( *vbo ) ) );
    }

    // see if object to copy has been uploaded, if it has, upload the copy as well
    //if ( !vao._changeFlag )
        upload();
}


void plVAO::_move( plVAO&& vao )
{
    // move vao
    _id = vao._id;
    //_changeFlag = vao._changeFlag;        
    
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


