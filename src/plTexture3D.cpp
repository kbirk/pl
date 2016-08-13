#include "plTexture3D.h"

plTexture3D::plTexture3D(PLuint width, PLuint height, PLuint depth)
    : _textureID(0), _width(width), _height(height), _depth(depth)
{
    setTexture(nullptr, _width, _height, _depth);
}


plTexture3D::~plTexture3D()
{
    _destroy();
}


plTexture3D::plTexture3D(const plTexture3D &texture)
    : _textureID(0)
{
    _copyTexture(texture);
}


plTexture3D& plTexture3D::operator = (const plTexture3D &texture)
{
    _copyTexture(texture);
    return *this;
}


void plTexture3D::_destroy()
{
    glDeleteTextures(1, &_textureID);
    _textureID = 0;
}


void plTexture3D::_copyTexture(const plTexture3D &texture)
{
    // destroy previous buffers
    _destroy();

    std::vector<PLchar> texData(texture._width * texture._height * 3, '0');

    // copy vertex data
    glBindTexture(GL_TEXTURE_3D, texture._textureID);
    glGetTexImage(GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texData[0]);
    glBindTexture(GL_TEXTURE_3D, 0);

    // set texture
    setTexture(&texData[0], texture._width, texture._height, texture._depth);
}


void plTexture3D::bind() const
{
    // bind textures AFTER binding shader AND BEFORE drawing arrays
    glDisable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, _textureID);
}


void plTexture3D::unbind() const
{
    // unbind textures after drawing
    glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, 0);
}


void plTexture3D::setTexture(const PLchar *image, PLint width, PLint height, PLint depth)
{
    _width  = width;
    _height = height;
    _depth  = depth;

    _destroy();

    glGenTextures(1, &_textureID);
    glBindTexture(GL_TEXTURE_3D, _textureID);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, _width, _height, _depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glBindTexture(GL_TEXTURE_3D, 0);
}
