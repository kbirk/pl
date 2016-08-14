#pragma once

#include "plCommon.h"
#include "plVector3.h"

#include <epoxy/gl.h>

template<typename T>
class plPixel
{
    public:

        T r, g, b, a;

        plPixel()
            : r(T()), g(T()), b(T()), a(T())
        {
        }

        plPixel(T red, T green, T blue, T alpha)
            : r(red), g(green), b(blue), a(alpha)
        {
        }
};


class plTexture2D
{

    public:

        plTexture2D(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, GLvoid* image = nullptr);
        plTexture2D(const plTexture2D& texture);
        plTexture2D(plTexture2D&& texture);

        ~plTexture2D();

        plTexture2D& operator = (const plTexture2D& texture);
        plTexture2D& operator = (plTexture2D&& texture);

        void bind  () const;   // bind textures AFTER binding shader AND BEFORE drawing arrays
        void unbind() const;

        uint32_t width()  const { return _width;  }
        uint32_t height() const { return _height; }

        void set(uint32_t width, uint32_t height, GLvoid* image = nullptr);
        void set(uint32_t width, uint32_t height, uint32_t internalFormat, uint32_t format, uint32_t type, GLvoid* image = nullptr);
        void setParameter(uint32_t pname, uint32_t param);

        friend class plFBO;

    private:

        GLuint _id;
        GLuint _width;
        GLuint _height;
        GLuint _internalFormat;
        GLuint _format;
        GLuint _type;

        uint32_t _getFormatSize() const;

        void _destroy();
        void _copy(const plTexture2D& texture);
        void _move(plTexture2D&& texture);
};
