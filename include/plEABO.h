#pragma once

#include "plCommon.h"
#include "plVertexSpecBuffer.h"
#include "plOpenGLInfo.h"

class plEABO : public plVertexSpecBuffer
{

    public:

        plEABO();
        plEABO(const std::vector<uint32_t>& data, uint32_t mode = GL_TRIANGLES, uint32_t usage = GL_STATIC_DRAW);
        plEABO(const plEABO& eabo);
        plEABO(plEABO&& eabo);

        plEABO& operator= (const plEABO& eabo);
        plEABO& operator= (plEABO&& eabo);

        std::vector<uint32_t> data() const { return _data; }

        void set(const std::vector<uint32_t>& data, uint32_t mode = GL_TRIANGLES, uint32_t usage = GL_STATIC_DRAW);

        void upload();

        void clear();

        void bind  () const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id); }
        void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

        void drawElements(uint32_t index = 0) const;

    private:

        std::vector<uint32_t> _data;
        uint32_t _type;           // eventually use this for templated version of class
        uint32_t _mode;

        void _copy(const plEABO& eabo);
        void _move(plEABO&& eabo);
};
