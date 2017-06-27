#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"
#include "plOpenGLInfo.h"
#include "plTexture2D.h"
#include "plVector3.h"

class plFBO {
public:
    plFBO();
    ~plFBO();

    void bind() const;
    void unbind() const;

    void setDrawBuffers(const std::vector<GLenum>& buffers) const;

    std::shared_ptr<plTexture2D> texture2DAttachment(uint32_t attachment) const;

    template <typename T>
    plPixel<T> readPixel(GLenum attachment, uint32_t x, uint32_t y) const;

    void attach(
        uint32_t attachment,
        std::shared_ptr<plTexture2D> texture);
    void attach(
        uint32_t attachment0,
        uint32_t attachment1,
        std::shared_ptr<plTexture2D> texture); // depth-stencil shared texture

private:
    GLuint _id;

    std::map<GLenum, std::shared_ptr<plTexture2D> > _textureAttachments;

    bool _checkAttachmentError() const;

    void _create();
    void _destroy();
};

template <typename T>
plPixel<T> plFBO::readPixel(GLenum attachment, uint32_t x, uint32_t y) const
{
    // get texture by the attachment
    auto iter = _textureAttachments.find(attachment);
    if (iter == _textureAttachments.end()) {
        LOG_WARN("Attachment `" << attachment << "` does not exist");
        return plPixel<T>();
    }
    // get texture
    auto texture = iter->second;

    GLuint format = texture->_format;
    uint32_t type = texture->_type;
    uint32_t size = texture->_getFormatSize();

    if (sizeof(plPixel<T>) < size) {
        LOG_WARN("Specified pixel type is too small for respective texture format");
        return plPixel<T>();
    }

    plPixel<T> pixel;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
    LOG_OPENGL("glBindFramebuffer");
    glReadBuffer(attachment);
    LOG_OPENGL("glReadBuffer");

    glReadPixels(x, y, 1, 1, format, type, &pixel.r);
    LOG_OPENGL("glReadPixels");

    glReadBuffer(GL_NONE);
    LOG_OPENGL("glReadBuffer");
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    LOG_OPENGL("glBindFramebuffer");

    return pixel;
}
