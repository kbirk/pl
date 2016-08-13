#include "plFBO.h"


static PLuint currentBoundFBO = 0;


plFBO::plFBO()
    : _id( 0 )
{
    _create();
}


plFBO::plFBO( plFBO&& fbo )
    : _id( 0 )
{
    _move( std::move( fbo ) );
}


plFBO::plFBO( const plFBO& fbo )
    : _id( 0 )
{
    _copy( fbo );
}


plFBO::~plFBO()
{
    _destroy();
}


plFBO& plFBO::operator= ( plFBO&& fbo )
{
    _move( std::move( fbo ) );
    return *this;
}


plFBO& plFBO::operator= ( const plFBO& fbo )
{
    _copy( fbo );
    return *this;
}


void plFBO::attach( PLuint attachment, const std::shared_ptr<plTexture2D>& texture )
{
    // bind fbo
    glBindFramebuffer( GL_FRAMEBUFFER, _id );

    // bind texture to attachment
    glFramebufferTexture2D( GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->_id, 0);

    if ( _checkAttachmentError() )
    {
        // successful, add to map
        _textureAttachments[ attachment ] = texture;
    }

    // unbind
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}


void plFBO::attach( PLuint attachment0, PLuint attachment1, const std::shared_ptr<plTexture2D>& texture )
{
    // bind fbo
    glBindFramebuffer( GL_FRAMEBUFFER, _id );

    // bind texture to attachments
    glFramebufferTexture2D( GL_FRAMEBUFFER, attachment0, GL_TEXTURE_2D, texture->_id, 0);
    glFramebufferTexture2D( GL_FRAMEBUFFER, attachment1, GL_TEXTURE_2D, texture->_id, 0);

    if ( _checkAttachmentError() )
    {
        // successful, add to map
        _textureAttachments[ attachment0 ] = texture;
        _textureAttachments[ attachment1 ] = texture;
    }

    // unbind
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}


PLbool plFBO::_checkAttachmentError() const
{
    // check for errors
    GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
    if ( status != GL_FRAMEBUFFER_COMPLETE )
    {
        printf( "plFBO::attach() error: 0x%x\n", status );
        return false;
    }
    return true;
}


void plFBO::bind() const
{
    if ( currentBoundFBO == _id )
        return;

    glBindFramebuffer( GL_FRAMEBUFFER, _id );
    std::vector<GLenum> buffers = drawBuffers();
    glDrawBuffers( buffers.size(), &buffers[0] );

    currentBoundFBO = _id;
}


void plFBO::unbind() const
{
    currentBoundFBO = 0;
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}


const std::shared_ptr< plTexture2D >& plFBO::texture2DAttachment( PLuint attachment ) const
{
    if ( _textureAttachments.find( attachment ) == _textureAttachments.end() )
    {
        std::cerr << "plFBO::texture2DAttachment() error: attachment enumeration does not exist for this fbo" << std::endl;
        static std::shared_ptr< plTexture2D > emptyAttachment;
        return emptyAttachment;
    }
    return _textureAttachments.find( attachment )->second;
}


void plFBO::setDrawBuffers( const std::vector<GLenum>& buffers ) const
{
    glDrawBuffers( buffers.size(), &buffers[0]  );
}


std::vector<GLenum> plFBO::drawBuffers() const
{
    std::vector<GLenum> drawBuffers;

    for ( auto &iter : _textureAttachments )
    {
        auto attachment = static_cast<GLint>(iter.first);
        // check if attachment is a color attachment
        if ( attachment >= GL_COLOR_ATTACHMENT0 &&
             attachment < GL_COLOR_ATTACHMENT0 + plOpenGLInfo::maxColorAttachments )
        {
            // if color attachment, add to draw buffers
            drawBuffers.push_back( attachment );
        }
    }

    return drawBuffers;
}


void plFBO::_create()
{
    if ( !_id )
        glGenFramebuffers( 1, &_id );
}


void plFBO::_destroy()
{
    glDeleteFramebuffers (1, &_id);
}


void plFBO::_move( plFBO&& fbo )
{
    _id = fbo._id;
    fbo._id = 0;
}


void plFBO::_copy( const plFBO& fbo )
{
    _create();

    for ( auto &attachment : _textureAttachments )
    {
        // copy texture to a new shared pointer
        std::shared_ptr<plTexture2D> texture( new plTexture2D( *( attachment.second ) ) );

        // attach new texture
        attach( attachment.first, texture );
    }
}
