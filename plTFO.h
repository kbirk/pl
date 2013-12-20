#ifndef PL_TRANSFORM_FEEDBACK_OBJECT_H
#define PL_TRANSFORM_FEEDBACK_OBJECT_H

#include "plCommon.h"
#include "plVBO.h"
#include "plEABO.h"
#include "plRenderingPipeline.h"
#include "plVertexSpecBuffer.h"


class plTFO
{

    public:                    
          
        plTFO();               
        plTFO( const plTFO& vao );        
        plTFO( plTFO&& vao );

        plTFO& operator= ( const plTFO& vao ); 
        plTFO& operator= ( plTFO&& vao ); 

        ~plTFO();
        
        std::shared_ptr< plVBO >& vbos( PLuint index ) { return _vbos[ index ]; }
       
        void beginTransformFeedback( PLuint ) const;
        void endTransformFeedback() const;

        void attach( const std::shared_ptr< plVBO >&  vbo  );
        
        void attachVarying( const std::string& varyingName );

        void attach( const std::shared_ptr< plEABO >& eabo );        
        void upload();        

        void clear();
        
        friend class plVertexSpecBuffer;
        
    private:

        GLuint _id;
        GLbool _discardRasterizer;
        
        std::vector< std::shared_ptr< plVBO > > _vbos; 
        std::Vector< std::string > _varyings;
        
        void _copy( const plTFO& vao );
        void _move( plTFO&& vao );
        void _destroy();
};


#endif
