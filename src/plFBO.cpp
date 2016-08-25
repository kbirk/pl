#include "plFBO.h"

plFBO::plFBO()
    : _id(0)
{
    _create();
}


plFBO::~plFBO()
{
    _destroy();
}


void plFBO::attach(uint32_t attachment, std::shared_ptr<plTexture2D> texture)
{
    // bind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    LOG_OPENGL("glBindFramebuffer");

    // bind texture to attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->_id, 0);
    LOG_OPENGL("glFramebufferTexture2D");

    if (_checkAttachmentError())
    {
        // successful, add to map
        _textureAttachments[attachment] = texture;
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    LOG_OPENGL("glBindFramebuffer");
}


void plFBO::attach(uint32_t attachment0, uint32_t attachment1, std::shared_ptr<plTexture2D> texture)
{
    // bind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    LOG_OPENGL("glBindFramebuffer");

    // bind texture to attachments
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment0, GL_TEXTURE_2D, texture->_id, 0);
    LOG_OPENGL("glFramebufferTexture2D");
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment1, GL_TEXTURE_2D, texture->_id, 0);
    LOG_OPENGL("glFramebufferTexture2D");

    if (_checkAttachmentError())
    {
        // successful, add to map
        _textureAttachments[attachment0] = texture;
        _textureAttachments[attachment1] = texture;
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    LOG_OPENGL("glBindFramebuffer");
}


bool plFBO::_checkAttachmentError() const
{
    // check for errors
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        LOG_WARN("Framebuffer status error: " << status);
        return false;
    }
    return true;
}


void plFBO::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    LOG_OPENGL("glBindFramebuffer");
}


void plFBO::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    LOG_OPENGL("glBindFramebuffer");
}


std::shared_ptr<plTexture2D> plFBO::texture2DAttachment(uint32_t attachment) const
{
    auto iter = _textureAttachments.find(attachment);
    if (iter == _textureAttachments.end())
    {
        LOG_WARN("Attachment enumeration `" << attachment << "` does not exist for this fbo");
        return nullptr;
    }
    return iter->second;
}


void plFBO::setDrawBuffers(const std::vector<GLenum>& buffers) const
{
    glDrawBuffers(buffers.size(), &buffers[0]);
    LOG_OPENGL("glDrawBuffers");
}


void plFBO::_create()
{
    if (!_id)
    {
        glGenFramebuffers(1, &_id);
        LOG_OPENGL("glGenFramebuffers");
    }
}


void plFBO::_destroy()
{
    glDeleteFramebuffers(1, &_id);
    LOG_OPENGL("glDeleteFramebuffers");
}
