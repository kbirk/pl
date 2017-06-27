#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"

class plBufferObject {
public:
    plBufferObject();

    virtual ~plBufferObject();

    uint32_t numBytes() const { return _numBytes; }

    virtual void bind(uint32_t location) const = 0;
    virtual void unbind(uint32_t location) const = 0;

protected:
    GLuint _id;
    GLuint _numBytes;

    virtual void _destroy();
};
