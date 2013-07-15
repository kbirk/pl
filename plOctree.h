#ifndef __PL_OCTREE_H__
#define __PL_OCTREE_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plLineMesh.h"
#include "plTriangle.h"

//template<class T>
class plOctreeNode 
{
    public:

        plOctreeNode( const plVector3 &c, PLfloat hw, PLuint depth );

        plVector3            centre;       // center point of octree node (not strictly needed)    
        PLfloat              halfWidth;    // half the width of the node volume (not strictly needed)    
        plSeq<plOctreeNode*> children;     // pointers to the eight children nodes   
  
        void insert( const plTriangle &tri);
  
    //private:
         
        plSeq<const plTriangle*>     contained;    // linked list of objects contained at this node 
          
        plLineMesh           mesh;
        
        void draw() const;
        
        void _updateMesh();
        
};

//template<class T>
class plOctree
{
    public:

        plOctreeNode *root;

        plOctree( const plVector3 &min, const plVector3 &max, PLuint depth);

        void fill(const plSeq<plTriangle> &triangles);

        void draw() const;
    
    
    private:
    
        

};

std::ostream& operator << ( std::ostream &stream, const plOctree     &o );
std::ostream& operator << ( std::ostream &stream, const plOctreeNode &n );

#endif
