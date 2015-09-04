#ifndef PL_VERTEX_SPEC_BUFFER_H
#define PL_VERTEX_SPEC_BUFFER_H

#include "plCommon.h"


class plVertexSpecBuffer
{

    public:

        plVertexSpecBuffer();

        virtual ~plVertexSpecBuffer();

        PLuint id() const  { return _id; }

        virtual void bind  () const = 0;
        virtual void unbind() const = 0;

    protected:

        GLuint _id;
        GLuint _numBytes;
        GLuint _usage;

        virtual void _destroy();
};


#endif
