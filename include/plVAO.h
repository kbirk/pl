#pragma once

#include "plCommon.h"
#include "plVBO.h"
#include "plEABO.h"
#include "plRenderingPipeline.h"
#include "plVertexSpecBuffer.h"

#include <epoxy/gl.h>

class plVAO
{
    public:

        plVAO();
        plVAO(const plVAO& vao);
        plVAO(plVAO&& vao);

        plVAO& operator= (const plVAO& vao);
        plVAO& operator= (plVAO&& vao);

        ~plVAO();

        std::shared_ptr<plVBO> vbos(uint32_t index) { return _vbos[index]; }
        std::shared_ptr<plEABO> eabo() { return _eabo; }

        void attach(std::shared_ptr<plVBO> vbo);
        void attach(std::shared_ptr<plEABO> eabo);
        void upload();
        void draw() const;
        void clear();

        friend class plVertexSpecBuffer;

    private:

        GLuint _id;

        std::vector<std::shared_ptr<plVBO>> _vbos;
        std::shared_ptr<plEABO> _eabo;

        void _copy(const plVAO& vao);
        void _move(plVAO&& vao);
        void _destroy();
};
