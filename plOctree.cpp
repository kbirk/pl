#include "plOctree.h"
 
plOctree::plOctree( const plVector3 &min, const plVector3 &max, PLuint depth )
{
    if (depth == 0)
    {
        std::cerr << "plOctree plOctree() error: depth must be greater than 0\n";
        exit(1);
    }
    
    // centre point of octree
    plVector3 centre = 0.5f * (min+max);
    
    // find largest distance component becomes half width
    plVector3 minDiff = min - centre;
    plVector3 maxDiff = max - centre;
    PLfloat minMax = PL_MAX_OF_3( fabs(minDiff.x), fabs(minDiff.y), fabs(minDiff.z) );
    PLfloat maxMax = PL_MAX_OF_3( fabs(maxDiff.x), fabs(maxDiff.y), fabs(maxDiff.z) );       
    PLfloat halfWidth = PL_MAX_OF_2( minMax, maxMax );
    
    // Construct and fill in ’root’ of this subtree
    root = new plOctreeNode( centre, halfWidth, depth );
}

void plOctree::draw() const
{
    root->draw();
}

void plOctree::fill(const plSeq<plTriangle> &triangles)
{
    for (PLuint i=0; i< triangles.size(); i++)
    {
        root->insert( triangles[i] );
    }
}





plOctreeNode::plOctreeNode( const plVector3 &c, PLfloat hw, PLuint depth)
    : children(NULL, 8), centre(c), halfWidth(hw)
{
    // create mesh
    _updateMesh();
    
    // Recursively construct the eight children of the subtree
    if (depth > 1)
    {               
        plVector3 offset;
        PLfloat step = halfWidth * 0.5f;
        for (PLuint i = 0; i < 8; i++) 
        {
            offset.x = ((i & 1) ? step : -step);
            offset.y = ((i & 2) ? step : -step);
            offset.z = ((i & 4) ? step : -step);
            children[i] = new plOctreeNode( centre + offset, step, depth - 1);
        }
    }
}

void plOctreeNode::draw() const
{
    if (contained.size() > 0)    
        mesh.draw();
    
    for (PLuint i=0; i < 8; i++)
    {
        if (children[i] == NULL)
            break;
            
        children[i]->draw();
    }
    
}

void plOctreeNode::insert( const plTriangle &tri)
{
    int index = 0, straddle = 0;
    
    // Compute the octant number [0..7] the object sphere center is in
    // If straddling any of the dividing x, y, or z planes, exit directly

    PLint child = 0;

    PLfloat dx = tri.centroid().x - centre.x;
    PLfloat dy = tri.centroid().y - centre.y;
    PLfloat dz = tri.centroid().z - centre.z;

    if ( fabs(dx) < tri.radius() || fabs(dy) < tri.radius() || fabs(dz) < tri.radius() )
    {
        // straddles a boundary
        contained.add( &tri );
        return;
    }    
    
    // fully contained in child, find child         
    if ( dx > 0 ) child += 1;
    if ( dy > 0 ) child += 2;
    if ( dz > 0 ) child += 4;
     
    if (children[child] != NULL)    
    {   
        contained.add( &tri );   
        children[child]->insert( tri );
    }    
    else    
        contained.add( &tri );
        
        /*
        plVector3 offset;
        PLfloat step = halfWidth * 0.5f;
        offset.x = ((child & 1) ? step : -step);
        offset.y = ((child & 2) ? step : -step);
        offset.z = ((child & 4) ? step : -step);
        children[i] = new plOctreeNode( centre + offset, step, depth - 1);
        */
}


void plOctreeNode::_updateMesh()
{
    // DEBUG MESH
    plSeq<plVector3> vertices( 8  );
    plSeq<PLuint>    indices ( 12 );
    
    // front face
    vertices.add( plVector3( centre.x - halfWidth, centre.y - halfWidth, centre.z + halfWidth) );
    vertices.add( plVector3( centre.x + halfWidth, centre.y - halfWidth, centre.z + halfWidth) );
    vertices.add( plVector3( centre.x + halfWidth, centre.y + halfWidth, centre.z + halfWidth) );
    vertices.add( plVector3( centre.x - halfWidth, centre.y + halfWidth, centre.z + halfWidth) );
    
    // back face
    vertices.add( plVector3( centre.x - halfWidth, centre.y - halfWidth, centre.z - halfWidth) );
    vertices.add( plVector3( centre.x + halfWidth, centre.y - halfWidth, centre.z - halfWidth) );
    vertices.add( plVector3( centre.x + halfWidth, centre.y + halfWidth, centre.z - halfWidth) );
    vertices.add( plVector3( centre.x - halfWidth, centre.y + halfWidth, centre.z - halfWidth) );
    
    // front
    indices.add( 0 );   indices.add( 1 );   indices.add( 2 );
    indices.add( 0 );   indices.add( 2 );   indices.add( 3 );
    
    // right
    indices.add( 1 );   indices.add( 5 );   indices.add( 6 );
    indices.add( 1 );   indices.add( 6 );   indices.add( 2 );
    
    // back
    indices.add( 5 );   indices.add( 4 );   indices.add( 7 );
    indices.add( 5 );   indices.add( 7 );   indices.add( 6 );
    
    // left
    indices.add( 4 );   indices.add( 0 );   indices.add( 3 );
    indices.add( 4 );   indices.add( 3 );   indices.add( 7 );
    
    // top
    indices.add( 3 );   indices.add( 2 );   indices.add( 6 );
    indices.add( 3 );   indices.add( 6 );   indices.add( 7 );
    
    // bottem
    indices.add( 4 );   indices.add( 5 );   indices.add( 1 );
    indices.add( 4 );   indices.add( 1 );   indices.add( 0 );
    
    mesh.setBuffers( vertices, indices );
}

std::ostream& operator << ( std::ostream& stream, const plOctree &o )
{
    stream << *o.root;
    return stream;
}

std::ostream& operator << ( std::ostream& stream, const plOctreeNode &n )
{
    for (PLuint i=0; i < 8; i++)
    {
        if (n.children[i] != NULL)
            stream << "child " << *n.children[i] << "\n"; 
        else
            stream << "leaf "; 
            
    }
    stream << "\n"; 

    return stream;
}


