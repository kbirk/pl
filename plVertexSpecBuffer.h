#ifndef PL_VERTEX_SPEC_BUFFER_H
#define PL_VERTEX_SPEC_BUFFER_H

#include "plCommon.h"

//class plVAO;    // yay, circular dependencies

class plVertexSpecBuffer
{

    public:                    
          
        plVertexSpecBuffer();   

        virtual ~plVertexSpecBuffer();

        virtual void bind  () const = 0;
        virtual void unbind() const = 0;
       
        //friend class plVAO;

    protected:

        GLuint _id;
        GLuint _numBytes;
        GLuint _usage;

        //PLbool _changeFlag;
        //plVAO *_vao;
        
        //virtual void _flagChange();
        virtual void _destroy();
};


#endif
