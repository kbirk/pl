#ifndef PL_BUFFER_OBJECT_H
#define PL_BUFFER_OBJECT_H

#include "plCommon.h"

class plBufferObject
{

    public:

        plBufferObject();

        virtual ~plBufferObject();

        PLuint numBytes() const { return _numBytes; }

        virtual void bind  ( PLuint location ) const = 0;
        virtual void unbind( PLuint location ) const = 0;

    protected:

        GLuint _id;
        GLuint _numBytes;

        virtual void _destroy();
};


#endif
