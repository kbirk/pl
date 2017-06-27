#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"
#include "plVector4.h"
#include "plVertexSpecBuffer.h"

// attribute locations, set by VBOs and mirrored in shader files
enum plVertexAttributeIndexEnum {
    PL_POSITION_ATTRIBUTE = 0,
    PL_NORMAL_ATTRIBUTE,
    PL_COLOR_ATTRIBUTE,
    PL_TEXCOORD_ATTRIBUTE
};

class plVertexAttributePointer {
public:
    plVertexAttributePointer(GLuint index, GLuint stride, GLuint offset)
        : index(index)
        , stride(stride)
        , offset(offset)
    {
    }

    GLuint index;
    GLuint stride;
    GLuint offset;
};

class plVBO : public plVertexSpecBuffer {
public:
    plVBO();
    plVBO(
        const std::vector<plVector4>& data,
        uint32_t usage = GL_STATIC_DRAW);

    void set(const std::vector<plVector3>& data, uint32_t usage = GL_STATIC_DRAW);
    void set(const std::vector<plVector4>& data, uint32_t usage = GL_STATIC_DRAW);
    void set(const plVertexAttributePointer& attributePointer);

    void clear();

    void bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, _id);
        LOG_OPENGL("glBindBuffer");
    }
    void unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        LOG_OPENGL("glBindBuffer");
    }

    void upload();

    std::vector<plVector4> data() const { return _data; }

private:
    std::vector<plVector4> _data;
    std::vector<plVertexAttributePointer> _attributePointers;
};
