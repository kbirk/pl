#include "plTFO.h"

plTFO::plTFO()
    : _id( 0 )
{
}


plTFO::plTFO( const plTFO& tfo )
    : _id( 0 )
{
    _copy( tfo );
}


plTFO::plTFO( plTFO&& tfo )
    : _id( 0 )
{
    _move( std::move( tfo ) );
}


plTFO& plTFO::operator = ( const plTFO &tfo )
{
    _copy( tfo );
    return *this;
}


plTFO& plTFO::operator= ( plTFO&& tfo )
{
    _move( std::move( tfo ) );
    return *this;
}


plTFO::~plTFO()
{
    _destroy();
}


void plTFO::attach( const std::shared_ptr< plVBO >& vbo, const std::vector< std::string >& varyings )
{
    // add the vbo
    _vbos.push_back( vbo );

    // for each new vbo, ensure varyinngs switch to new buffer
    if ( !_varyings.empty() )
    {
        _varyings.push_back( "gl_NextBuffer" );
    }

    // add varyings to vector
    for ( const std::string& varying : varyings )
    {
        _varyings.push_back( varying );
    }
}


void plTFO::clear()
{
    _vbos.clear();
}


void beginTransformFeedback() const
{
    if ( _discardRasterizer )
        glEnable( GL_RASTERIZER_DISCARD );



}


void endTransformFeedback() const
{
    if ( _discardRasterizer )
        glDisable( GL_RASTERIZER_DISCARD );
}


void plTFO::draw() const
{
	// bind vertex array object
	glBindVertexArray( _id );
	// draw
	_eabo->drawElements();
    // unbind tfo
	glBindVertexArray( 0 );
}


void plTFO::upload( const plShader& shader )
{
    if ( _vbos.empty() )
    {
        std::cerr << " plTFO::upload() error: cannot upload to GPU, tfo has no VBOs attached, command ignored " << std::endl;
        return;
    }

    // create and bind tfo
    if ( !_id )
	    glGenTransformFeedbacks( 1, &_id );

	// bind the tfo
    glBindTransformFeedback( GL_TRANSFORM_FEEDBACK, _id );

    // assemble varyings into c-style char arrays
    std::vector< std::vector<char> > varyings;
    for ( PLuint i=0; i<_varyings.size() i++ )
    {
        varyings[i] = new char[ _varyings[i].length() + 1 ];
        std::strcpy( varyings[i], _varyings[i] );
    }

    // set varyings
    glTransformFeedbackVaryings( _id, varyings.size(), varyings, GL_INTERLEAVED_ATTRIBS );

    // add varyings to vector
    for ( const std::string& varying : varyings )
    {
        _varyings.push_back( varying );
    }

    // set vbos to bindings
    PLint location = 0;
    for ( auto& vbo : _vbos )
    {
        vbo->upload();
        glBindBufferBase( GL_TRANSFORM_FEEDBACK_BUFFER, location++, vbo->id() );
    }




    /*
    for (unsigned int i = 0; i < 2 ; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback[i]);
        glBindBuffer(GL_ARRAY_BUFFER, m_particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Particles), Particles, GL_DYNAMIC_DRAW);

    }
    */

    glLinkProgram(

    glBindTransformFeedback( 0 );
}


void plTFO::_copy( const plTFO& tfo )
{
    attach( std::shared_ptr< plEABO >( new plEABO( *tfo._eabo ) ) );
    for ( auto& vbo : tfo._vbos )
    {
        attach( std::shared_ptr< plVBO >( new plVBO( *vbo ) ) );
    }

    upload();
}


void plTFO::_move( plTFO&& tfo )
{
    // move tfo
    _id = tfo._id;

    // move vbo and eabo
    _vbos = std::move( tfo._vbos );
    _eabo = std::move( tfo._eabo );

    tfo._id = 0;
}


void plTFO::_destroy()
{

    glDeleteVertexArrays( 1, &_id) ;	// delete tfo
    _id = 0;
}
