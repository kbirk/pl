#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"

class plVertexSpecBuffer
{
    public:

        plVertexSpecBuffer();

        virtual ~plVertexSpecBuffer();

        uint32_t id() const { return _id; }

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

    protected:

        GLuint _id;
        GLuint _numBytes;
        GLuint _usage;

        virtual void _destroy();
};
