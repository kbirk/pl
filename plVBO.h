#ifndef PL_VERTEX_BUFFER_OBJECT_H
#define PL_VERTEX_BUFFER_OBJECT_H

#include "plCommon.h"
#include "plVertexSpecBuffer.h"
#include "plVector4.h"


// attribute locations, set by VBOs and mirrored in shader files
enum plVertexAttributeIndexEnum
{
    PL_POSITION_ATTRIBUTE = 0,
    PL_NORMAL_ATTRIBUTE,
    PL_COLOUR_ATTRIBUTE,
    PL_TEXCOORD_ATTRIBUTE
};


class plVertexAttributePointer
{
    public:

        plVertexAttributePointer( GLuint index, GLuint offset = 0 )
            : index( index ), offset( offset )
        {
        }
        
        GLuint index;
        GLuint offset;

};


class plVBO : public plVertexSpecBuffer
{

    public:                    
          
        plVBO();    
        plVBO( const std::vector<plVector4>& data, PLuint usage = GL_STATIC_DRAW );
        plVBO( const plVBO& vbo );        
        plVBO( plVBO&& vbo );

        plVBO& operator= ( const plVBO& vbo ); 
        plVBO& operator= ( plVBO&& vbo ); 
        
        void set( const std::vector<plVector3>& data, PLuint usage = GL_STATIC_DRAW );
        void set( const std::vector<plVector4>& data, PLuint usage = GL_STATIC_DRAW );        
        void set( const plVertexAttributePointer& attributePointer );

        void clear();

        void bind  () const { glBindBuffer( GL_ARRAY_BUFFER, _id ); }
        void unbind() const { glBindBuffer( GL_ARRAY_BUFFER, 0   ); }

        void upload();

        std::vector<plVector4> data() const { return _data; }

    private:

        std::vector<plVector4> _data;
        std::vector<plVertexAttributePointer> _attributePointers;

        void _copy( const plVBO& vbo );
        void _move( plVBO&& vbo );
};


#endif
