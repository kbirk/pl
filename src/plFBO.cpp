#include "plFBO.h"

static uint32_t currentBoundFBO = 0;

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

    // bind texture to attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture->_id, 0);

    if (_checkAttachmentError())
    {
        // successful, add to map
        _textureAttachments[attachment] = texture;
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void plFBO::attach(uint32_t attachment0, uint32_t attachment1, std::shared_ptr<plTexture2D> texture)
{
    // bind fbo
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    // bind texture to attachments
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment0, GL_TEXTURE_2D, texture->_id, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment1, GL_TEXTURE_2D, texture->_id, 0);

    if (_checkAttachmentError())
    {
        // successful, add to map
        _textureAttachments[attachment0] = texture;
        _textureAttachments[attachment1] = texture;
    }

    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    if (currentBoundFBO == _id)
        return;

    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    std::vector<GLenum> buffers = drawBuffers();
    glDrawBuffers(buffers.size(), &buffers[0]);

    currentBoundFBO = _id;
}


void plFBO::unbind() const
{
    currentBoundFBO = 0;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


std::shared_ptr<plTexture2D> plFBO::texture2DAttachment(uint32_t attachment) const
{
    if (_textureAttachments.find(attachment) == _textureAttachments.end())
    {
        LOG_WARN("Attachment enumeration `" << attachment << "` does not exist for this fbo");
        return nullptr;
    }
    return _textureAttachments.find(attachment)->second;
}


void plFBO::setDrawBuffers(const std::vector<GLenum>& buffers) const
{
    glDrawBuffers(buffers.size(), &buffers[0]);
}


std::vector<GLenum> plFBO::drawBuffers() const
{
    std::vector<GLenum> drawBuffers;
    for (auto iter : _textureAttachments)
    {
        auto attachment = static_cast<GLint>(iter.first);
        // check if attachment is a color attachment
        if (attachment >= GL_COLOR_ATTACHMENT0 &&
            attachment < GL_COLOR_ATTACHMENT0 + plOpenGLInfo::maxColorAttachments)
        {
            // if color attachment, add to draw buffers
            drawBuffers.push_back(attachment);
        }
    }
    return drawBuffers;
}


void plFBO::_create()
{
    if (!_id)
        glGenFramebuffers(1, &_id);
}


void plFBO::_destroy()
{
    glDeleteFramebuffers(1, &_id);
}
