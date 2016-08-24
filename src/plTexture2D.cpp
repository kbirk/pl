#include "plTexture2D.h"

plTexture2D::plTexture2D(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, GLvoid* image)
    : _id(0)
{
    set(width, height, internalFormat, format, type, image);
}


plTexture2D::~plTexture2D()
{
    _destroy();
}


void plTexture2D::bind() const
{
    // bind textures AFTER binding shader AND BEFORE drawing arrays
    glBindTexture(GL_TEXTURE_2D, _id);
    LOG_OPENGL("glBindTexture");
}


void plTexture2D::unbind() const
{
    // unbind textures after drawing
    glBindTexture(GL_TEXTURE_2D, 0);
    LOG_OPENGL("glBindTexture");
}


void plTexture2D::set(uint32_t width, uint32_t height, GLvoid* image)
{
    set(width, height, _internalFormat, _format, _type, image);
}


void plTexture2D::set(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, GLvoid* image)
{
    _width  = width;
    _height = height;
    _internalFormat = internalFormat;
    _format = format;
    _type = type;

    if (!_id)
    {
        glGenTextures(1, &_id);
        LOG_OPENGL("glGenTextures");
    }

    glBindTexture(GL_TEXTURE_2D, _id);
    LOG_OPENGL("glBindTexture");
    glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _format, _type, image);
    LOG_OPENGL("glTexImage2D");

    // default linear interpolate
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    LOG_OPENGL("glTexParameterf");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    LOG_OPENGL("glTexParameterf");

    // default repeat wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    LOG_OPENGL("glTexParameteri");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    LOG_OPENGL("glTexParameteri");

    glBindTexture(GL_TEXTURE_2D, 0);
    LOG_OPENGL("glBindTexture");
}


void plTexture2D::setParameter(uint32_t pname, uint32_t param)
{
    glBindTexture(GL_TEXTURE_2D, _id);
    LOG_OPENGL("glBindTexture");
    glTexParameteri(GL_TEXTURE_2D, pname, param);
    LOG_OPENGL("glTexParameteri");
    glBindTexture(GL_TEXTURE_2D, 0);
    LOG_OPENGL("glBindTexture");
}


void plTexture2D::_destroy()
{
    glDeleteTextures(1, &_id);
    LOG_OPENGL("glDeleteTextures");
    _id = 0;
}


uint32_t plTexture2D::_getFormatSize() const
{
    uint32_t multiplier = 0;

    switch (_format)
    {
        case GL_RED:                multiplier = 1;     break;
        case GL_RG:                 multiplier = 2;     break;
        case GL_RGB:                multiplier = 3;     break;
        case GL_BGR:                multiplier = 3;     break;
        case GL_RGBA:               multiplier = 4;     break;
        case GL_BGRA:               multiplier = 4;     break;
        case GL_RED_INTEGER:        multiplier = 1;     break;
        case GL_RG_INTEGER:         multiplier = 2;     break;
        case GL_RGB_INTEGER:        multiplier = 3;     break;
        case GL_BGR_INTEGER:        multiplier = 3;     break;
        case GL_RGBA_INTEGER:       multiplier = 4;     break;
        case GL_BGRA_INTEGER:       multiplier = 4;     break;
        case GL_STENCIL_INDEX:      multiplier = 1;     break;  // not 100% sure this is correct
        case GL_DEPTH_COMPONENT:    multiplier = 3;     break;  // not 100% sure this is correct
        case GL_DEPTH_STENCIL:      multiplier = 4;     break;
    }

    uint32_t size = 0;

    switch (_type)
    {
        case GL_UNSIGNED_BYTE:      size = sizeof(GLubyte);     break;
        case GL_BYTE:               size = sizeof(GLbyte);      break;
        case GL_UNSIGNED_SHORT:     size = sizeof(GLushort);    break;
        case GL_SHORT:              size = sizeof(GLshort);     break;
        case GL_UNSIGNED_INT:       size = sizeof(GLuint);      break;
        case GL_INT:                size = sizeof(GLint);       break;
        case GL_FLOAT:              size = sizeof(GLfloat);     break;
        case GL_DOUBLE:             size = sizeof(GLdouble);    break;

        // not 100% sure about these
        case GL_UNSIGNED_BYTE_3_3_2:
        case GL_UNSIGNED_BYTE_2_3_3_REV:
            return 1;
            break;

        case GL_UNSIGNED_SHORT_5_6_5:
        case GL_UNSIGNED_SHORT_5_6_5_REV:
        case GL_UNSIGNED_SHORT_4_4_4_4:
        case GL_UNSIGNED_SHORT_4_4_4_4_REV:
        case GL_UNSIGNED_SHORT_5_5_5_1:
        case GL_UNSIGNED_SHORT_1_5_5_5_REV:
            return 2;
            break;

        case GL_UNSIGNED_INT_8_8_8_8:
        case GL_UNSIGNED_INT_8_8_8_8_REV:
        case GL_UNSIGNED_INT_10_10_10_2:
        case GL_UNSIGNED_INT_2_10_10_10_REV:
            return 4;
            break;
    }

    return multiplier * size;
}
