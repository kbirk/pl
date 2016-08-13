#include "plTexture2D.h"

plTexture2D::plTexture2D(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, GLvoid* image)
    : _id(0)
{
    set(width, height, internalFormat, format, type, image);
}


plTexture2D::plTexture2D(const plTexture2D& texture)
    : _id(0)
{
    _copy(texture);
}


plTexture2D::plTexture2D(plTexture2D&& texture)
    : _id(0)
{
    _move(std::move(texture));
}


plTexture2D::~plTexture2D()
{
    _destroy();
}


plTexture2D& plTexture2D::operator= (const plTexture2D& texture)
{
    _copy(texture);
    return *this;
}


plTexture2D& plTexture2D::operator= (plTexture2D&& texture)
{
    _move(std::move(texture));
    return *this;
}


void plTexture2D::bind() const
{
    // bind textures AFTER binding shader AND BEFORE drawing arrays
    glBindTexture(GL_TEXTURE_2D, _id);
}


void plTexture2D::unbind() const
{
    // unbind textures after drawing
    glBindTexture(GL_TEXTURE_2D, 0);
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
        glGenTextures(1, &_id);

    glBindTexture(GL_TEXTURE_2D, _id);

    // THIS IS OPENCV TYPE: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
    glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _format, _type, image);

    // default linear interpolate
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // default repeat wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void plTexture2D::setParameter(uint32_t pname, uint32_t param)
{
    glBindTexture(GL_TEXTURE_2D, _id);
    glTexParameteri(GL_TEXTURE_2D, pname, param);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void plTexture2D::_destroy()
{
    glDeleteTextures(1, &_id);
    _id = 0;
}


void plTexture2D::_copy(const plTexture2D &texture)
{
    uint8_t *buffer = new uint8_t[texture._width * texture._height * texture._getFormatSize()];

    // copy vertex data
    glBindTexture(GL_TEXTURE_2D, texture._id);
    glGetTexImage(GL_TEXTURE_2D, 0, texture._format, texture._type, buffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    // set texture
    set(texture._width, texture._height, texture._internalFormat, texture._format, texture._type, buffer);

    delete [] buffer;
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


void plTexture2D::_move(plTexture2D&& texture)
{
    _id = texture._id;
    _width = texture._width;
    _height = texture._height;
    _internalFormat = texture._internalFormat;
    _format = texture._format;
    _type = texture._type;

    texture._id = 0;
}
