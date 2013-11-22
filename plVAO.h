#ifndef PL_VERTEX_ARRAY_OBJECT_H
#define PL_VERTEX_ARRAY_OBJECT_H

#include "plCommon.h"
#include "plVBO.h"
#include "plEABO.h"
#include "plRenderingPipeline.h"
#include "plVertexSpecBuffer.h"

// TODO: remove _vao link from attachees, vao does not need to be redone every time the buffers are updated, only if the attribute pointers change.


class plVAO
{

    public:                    
          
        plVAO();               
        plVAO( const plVAO& vao );        
        plVAO( plVAO&& vao );

        plVAO& operator= ( const plVAO& vao ); 
        plVAO& operator= ( plVAO&& vao ); 

        ~plVAO();
        
        void attach( const std::shared_ptr< plVBO >&  vbo  );
        void attach( const std::shared_ptr< plEABO >& eabo );        
        void upload();        
        void draw() const;
        void clear();
        
        //void draw( const std::vector<PLuint> &indices );
        
        friend class plVertexSpecBuffer;
        
    private:

        GLuint _id;
        PLbool _changeFlag;
        
        std::vector< std::shared_ptr< plVBO > > _vbos;
        std::shared_ptr< plEABO > _eabo;   
        
        void _copy( const plVAO& vao );
        void _move( plVAO&& vao );
        void _destroy();
};


#endif
