#pragma once

#include "plCommon.h"
#include "plVector3.h"
#include "plTexture2D.h"
#include "plOpenGLInfo.h"

#include <epoxy/gl.h>

class plFBO
{
    public:

        plFBO();
        plFBO(const plFBO& fbo);
        plFBO(plFBO&& fbo);

        ~plFBO();

        plFBO& operator= (const plFBO& fbo);
        plFBO& operator= (plFBO&& fbo);

        void bind() const;
        void unbind() const;

        void setDrawBuffers(const std::vector<GLenum>& buffers) const;

        std::vector<GLenum> drawBuffers() const;

        const std::shared_ptr<plTexture2D >& texture2DAttachment(uint32_t attachment) const;

        template<typename T>
        plPixel<T> readPixel(GLenum attachment, uint32_t x, uint32_t y) const;

        void attach(uint32_t attachment, const std::shared_ptr<plTexture2D>& texture);
        void attach(uint32_t attachment0, uint32_t attachment1, const std::shared_ptr<plTexture2D>& texture); // depth-stencil shared texture

    private:

        GLuint _id;

        std::map<GLenum, std::shared_ptr<plTexture2D>> _textureAttachments;

        bool _checkAttachmentError() const;

        void _create();
        void _destroy();
        void _move(plFBO&& fbo);
        void _copy(const plFBO& fbo);
};


template<typename T>
plPixel<T> plFBO::readPixel(GLenum attachment, uint32_t x, uint32_t y) const
{
    if (_textureAttachments.find(attachment) == _textureAttachments.end())
    {
        std::cerr << "plFBO::readPixel() error: attachment does not exist" << std::endl;
        return plPixel<T>();
    }

    // get pointer to texture
    const std::shared_ptr<plTexture2D>& texture = _textureAttachments.find(attachment)->second;

    GLuint format = texture->_format;
    uint32_t type   = texture->_type;
    uint32_t size   = texture->_getFormatSize();

    if (sizeof(plPixel<T>) < size)
    {
        std::cerr << "plFBO::readPixel() error: specified pixel type is too small for respective texture format" << std::endl;
        return plPixel<T>();
    }

    plPixel<T> pixel;

    glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
    glReadBuffer     (attachment);

    glReadPixels(x, y, 1, 1, format, type, &pixel.r);

    glReadBuffer     (GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    return pixel;
}
