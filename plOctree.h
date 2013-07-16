#ifndef __PL_OCTREE_H__
#define __PL_OCTREE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plMath.h"
#include "plLineMesh.h"
#include "plModel.h"

class plOctreeNode 
{
    public:

        plOctreeNode( const plVector3 &c, PLfloat hw); 

        plVector3                 centre;        // center point of octree node (not strictly needed)    
        PLfloat                   halfWidth;     // half the width of the node volume (not strictly needed)    
        plSeq<plOctreeNode*>      children;      // pointers to the eight children nodes   
        plSeq<const plTriangle*>  contained;     // linked list of objects contained at this node 
        
        void draw  () const;        
        void insert( const plTriangle &tri, PLuint depth, PLbool exclusive );
        
        const plSeq<const plTriangle*> &rayIntersect ( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay = false ) const;
        
    private:
                           
        plLineMesh  _mesh;

        void _insertChild( PLuint index, const plTriangle &tri, PLuint depth, PLbool exclusive);
        void _updateMesh();
        
};

class plOctree
{
    public:

        plOctreeNode *root;
        plOctree( const plModel &model, PLuint depth, PLbool exclusive = false );

        const plSeq<const plTriangle*> &rayIntersect ( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay = false ) const;

        void draw() const;
        
    private:
    
        PLbool _exclusive;  // if each triangle is exclusive to a single cube
                            // exclusive octree     = better memory efficiency
                            // non-exclusive octree = better performance
    
        void _fill(const plSeq<plTriangle> &triangles, PLuint depth, PLbool exclusive);

};

std::ostream& operator << ( std::ostream &stream, const plOctree     &o );
std::ostream& operator << ( std::ostream &stream, const plOctreeNode &n );

#endif
