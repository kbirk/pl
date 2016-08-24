#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"
#include "plVBO.h"
#include "plEABO.h"
#include "plRenderingPipeline.h"
#include "plVertexSpecBuffer.h"

class plVAO
{
    public:

        plVAO();
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

        void _destroy();
};
