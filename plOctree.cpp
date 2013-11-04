#include "plOctree.h"


plOctree::plOctree()
    : _root(0)
{
} 
 
 
plOctree::plOctree( const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, PLuint depth)   
    : _root(0)                                    
{
    build( min, max, triangles, depth );
}


plOctree::~plOctree()
{
    _clear();
}


void plOctree::_clear()
{
    // clear all children recursively
    if ( _root )
        _root->clear();
    // delete root and set pointer to null    
    delete _root;
    _root = 0;
}


void plOctree::build(  const plVector3 &min, const plVector3 &max, const std::vector<plTriangle> &triangles, PLuint depth )
{
    if (depth == 0)
    {
        std::cerr << "plOctree::build() error: depth must be greater than 0\n";
        return;
    }
    
    std::cout << "Building octree for " << triangles.size() << " triangles (depth = " << depth << ")...";
    
    // centre point of octree
    plVector3 centre = 0.5f * (min+max);
    
    // find largest distance component, becomes half width
    plVector3 minDiff = min - centre;
    plVector3 maxDiff = max - centre;
    PLfloat minMax    = PL_MAX_OF_3( fabs(minDiff.x), fabs(minDiff.y), fabs(minDiff.z) );
    PLfloat maxMax    = PL_MAX_OF_3( fabs(maxDiff.x), fabs(maxDiff.y), fabs(maxDiff.z) );       
    // half width of octree cell
    PLfloat halfWidth = PL_MAX_OF_2( minMax, maxMax );
    
    // clear, construct, and fill in _root 
    _clear();
    _root = new plOctreeNode( centre, halfWidth );
 
    // traverse and fill sub cubes with triangles until desired depth is reached  
    _fill( triangles, depth ); 
    
    std::cout << "\tComplete.\n";
}


void plOctree::_fill( const std::vector<plTriangle> &triangles, PLuint depth )
{
    for (PLuint i=0; i< triangles.size(); i++)
    {
        _root->insert( triangles[i], depth-1 );
    }
    
}


void plOctree::draw() const
{
    if ( !_isVisible )
        return;

    _root->draw();
}


plIntersection plOctree::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay, PLbool backFaceCull) const
{
    if ( _root == NULL )
        return plIntersection(false);

    plIntersection closestIntersection( false );

    // find set of potentially intersected triangles
    plSet<const plTriangle*> triangles;
    if ( _root->rayIntersect( triangles, rayOrigin, rayDirection, 0, ignoreBehindRay ) )
    {
        // find closest triangle in set
        PLfloat min = FLT_MAX;

        for ( const plTriangle* tri : triangles )
        {  
            // intersect triangle
            plIntersection intersection = tri->rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );           
            if (intersection.exists)
            {
                PLfloat tAbs = fabs(intersection.t);
                if ( tAbs < min) 
                {
                    min = tAbs;
                    closestIntersection = intersection;
                }
            }

        }
    }
    
    return closestIntersection;     
}


void plOctree::graftIntersect( plSet<const plTriangle*> &triangles, const plTransform &transform, PLfloat radius ) const
{
    _root->rayIntersect( triangles, transform.origin(), transform.y(), radius, false );
}


plOctreeNode::plOctreeNode( const plVector3 &c, PLfloat hw)
    : _children( 8, (plOctreeNode*)(NULL) ), _centre( c ), _halfWidth( hw )
{
    // create mesh
    _updateMesh();
}


plOctreeNode::~plOctreeNode()
{
    clear();
}


void plOctreeNode::clear()
{
    for (PLuint i=0; i < 8; i++)
    { 
        if ( _children[i] )
            _children[i]->clear();
        delete _children[i];
    }
}


void plOctreeNode::draw() const
{
    PLint count = 0;

    for (PLuint i=0; i < 8; i++)
    {
        if ( _children[i] != NULL )
        {
            _children[i]->draw();
            count++;
        }
    }

    if ( _contained.size() > 0 || count > 0 )    // only draw if contains objects, or has children that contain
        _mesh.draw();
    
}


void plOctreeNode::insert( const plTriangle &tri, PLuint depth )
{    
    // only add triangle if leaf node
    if ( depth == 0 )
        _contained.push_back( &tri );

    // distance from each axis
    PLfloat dx = tri.centroid().x - _centre.x;
    PLfloat dy = tri.centroid().y - _centre.y;
    PLfloat dz = tri.centroid().z - _centre.z;

    // if distance is less than radius, then the triangle straddles a boundary
    if ( fabs(dx) < tri.radius() || fabs(dy) < tri.radius() || fabs(dz) < tri.radius() )
    {
        // straddles a boundary try to add to intersected children
        for (PLuint i=0; i<8; i++)
        {               
            if ( _sphereCheck( tri.centroid(), tri.radius(), i) )
            {
                // part of bounding sphere intersects child, insert
                _insertChild( i, tri, depth ); 
            }
        }    
    }    
    else
    {            
        // fully contained in a single child, find child index
        // contains the 0-7 index of the child, determined using bit wise addition
        PLint child = 0;        
        if ( dx > 0 ) child += 1;
        if ( dy > 0 ) child += 2;
        if ( dz > 0 ) child += 4;
         
        _insertChild( child, tri, depth );
    } 
}


void plOctreeNode::_insertChild( PLuint index, const plTriangle &tri, PLuint depth)
{
    if ( _children[index] != NULL )    
    {     
        // child already exists, recursively insert
        _children[index]->insert( tri, depth-1 );
    }    
    else   
    { 
        // child does not exist, if terminal depth has not been reached, create child node
        if (depth > 0)
        {        
            plVector3 offset;
            PLfloat step = _halfWidth * 0.5f;
            offset.x = ((index & 1) ? step : -step);
            offset.y = ((index & 2) ? step : -step);
            offset.z = ((index & 4) ? step : -step);
            _children[index] = new plOctreeNode( _centre + offset, step);
            _children[index]->insert( tri, depth - 1 );
        }
    }
}


PLfloat plOctreeNode::_squaredDistanceFromPoint( const plVector3 &point, PLint child ) const
{
    // shift AABB dimesions based on which child cell is begin tested  
    plVector3 offsetCentre = _centre;
    PLfloat step = 0.5f * _halfWidth;
    offsetCentre.x += ( (child & 1) ? step : -step );
    offsetCentre.y += ( (child & 2) ? step : -step );
    offsetCentre.z += ( (child & 4) ? step : -step );
    plVector3 minAABB = plVector3( offsetCentre.x - step, offsetCentre.y - step, offsetCentre.z - step );
    plVector3 maxAABB = plVector3( offsetCentre.x + step, offsetCentre.y + step, offsetCentre.z + step );
    
    // For each axis count any excess distance outside box extents
    PLfloat sqrDist = 0.0f;
    // x
    if (point.x < minAABB.x) sqrDist += (minAABB.x - point.x) * (minAABB.x - point.x);
    if (point.x > maxAABB.x) sqrDist += (point.x - maxAABB.x) * (point.x - maxAABB.x);
    // y
    if (point.y < minAABB.y) sqrDist += (minAABB.y - point.y) * (minAABB.y - point.y);
    if (point.y > maxAABB.y) sqrDist += (point.y - maxAABB.y) * (point.y - maxAABB.y);
    // z
    if (point.z < minAABB.z) sqrDist += (minAABB.z - point.z) * (minAABB.z - point.z);
    if (point.z > maxAABB.z) sqrDist += (point.z - maxAABB.z) * (point.z - maxAABB.z);
        
    return sqrDist;
}


plVector3 plOctreeNode::_closestPointInBox( const plVector3 &point, PLint child ) const
{
    // shift AABB dimesions based on which child cell is begin tested      
    PLfloat step = 0.5f * _halfWidth;
    plVector3 offsetCentre = _centre;
    offsetCentre.x += ( (child & 1) ? step : -step );
    offsetCentre.y += ( (child & 2) ? step : -step );
    offsetCentre.z += ( (child & 4) ? step : -step );
    plVector3 minAABB = plVector3( offsetCentre.x - step, offsetCentre.y - step, offsetCentre.z - step );
    plVector3 maxAABB = plVector3( offsetCentre.x + step, offsetCentre.y + step, offsetCentre.z + step );

    plVector3 closestPoint;
    closestPoint.x = (point.x < minAABB.x)? minAABB.x : (point.x > maxAABB.x)? maxAABB.x : point.x;
    closestPoint.y = (point.y < minAABB.y)? minAABB.y : (point.y > maxAABB.y)? maxAABB.y : point.y;
    closestPoint.z = (point.z < minAABB.z)? minAABB.z : (point.z > maxAABB.z)? maxAABB.z : point.z;

    return closestPoint;
}


PLbool plOctreeNode::_sphereCheck( const plVector3 &centre, PLfloat radius, PLint child ) const
{   
    //plVector3 closestPointInBox = _closestPointInBox( centre, child );
    //return (centre - closestPointInBox).squaredLength() <= radius*radius;
      
    // compute squared distance between sphere center and AABB
    PLfloat dist = _squaredDistanceFromPoint( centre, child );
    // sphere and AABB intersect if the distance is less than the sphere radius
    return dist <= radius*radius;
    

}


PLbool plOctreeNode::rayIntersect( plSet<const plTriangle*> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLfloat boxInflation, PLbool ignoreBehindRay ) const
{ 
    // box inflation is used to intersect cylinder with box rather than ray, used for grafts
    PLfloat boxExtents = _halfWidth + boxInflation;  
    
    // check if ray origin is inside box
    plVector3 diff = rayOrigin - _centre;
    
    if( ignoreBehindRay )
    {
	    if( fabs(diff.x) > boxExtents && diff.x*rayDirection.x >= 0.0f)	return false;
	    if( fabs(diff.y) > boxExtents && diff.y*rayDirection.y >= 0.0f)	return false;
	    if( fabs(diff.z) > boxExtents && diff.z*rayDirection.z >= 0.0f)	return false;
    }
	plVector3 fabsDirection( fabs(rayDirection.x), fabs(rayDirection.y), fabs(rayDirection.z) );

	float f;
	f = rayDirection.y * diff.z - rayDirection.z * diff.y;	if( fabs(f) > boxExtents*fabsDirection.z + boxExtents*fabsDirection.y ) return false;
	f = rayDirection.z * diff.x - rayDirection.x * diff.z;	if( fabs(f) > boxExtents*fabsDirection.z + boxExtents*fabsDirection.x )	return false;
	f = rayDirection.x * diff.y - rayDirection.y * diff.x;	if( fabs(f) > boxExtents*fabsDirection.y + boxExtents*fabsDirection.x )	return false;

	// intersection exists, recurse further
    PLuint childCount = 0;
    
    for (PLuint i=0; i<_children.size(); i++)
    {       
        if (_children[i] != NULL)
        {
            // not a leaf, recurse
            _children[i]->rayIntersect( triangles, rayOrigin, rayDirection, boxInflation, ignoreBehindRay ); 
            childCount++;            
        }           
    }

    if (childCount == 0)
    {
        // leaf, add triangles
        for (PLuint i=0; i<_contained.size(); i++)
        {
            triangles.insert( _contained[i] );
        }        
    }

    return true;
}


void plOctreeNode::_updateMesh()
{
    // DEBUG MESH
    std::vector<plVector3> vertices;    vertices.reserve( 8 );
    std::vector<PLuint>    indices;     indices.reserve( 8*3 );
    
    // front face
    vertices.push_back( plVector3( _centre.x - _halfWidth, _centre.y - _halfWidth, _centre.z + _halfWidth) );
    vertices.push_back( plVector3( _centre.x + _halfWidth, _centre.y - _halfWidth, _centre.z + _halfWidth) );
    vertices.push_back( plVector3( _centre.x + _halfWidth, _centre.y + _halfWidth, _centre.z + _halfWidth) );
    vertices.push_back( plVector3( _centre.x - _halfWidth, _centre.y + _halfWidth, _centre.z + _halfWidth) );
    
    // back face
    vertices.push_back( plVector3( _centre.x - _halfWidth, _centre.y - _halfWidth, _centre.z - _halfWidth) );
    vertices.push_back( plVector3( _centre.x + _halfWidth, _centre.y - _halfWidth, _centre.z - _halfWidth) );
    vertices.push_back( plVector3( _centre.x + _halfWidth, _centre.y + _halfWidth, _centre.z - _halfWidth) );
    vertices.push_back( plVector3( _centre.x - _halfWidth, _centre.y + _halfWidth, _centre.z - _halfWidth) );
    
    // front
    indices.push_back( 0 );   indices.push_back( 1 );   
    indices.push_back( 1 );   indices.push_back( 2 );   
    indices.push_back( 2 );   indices.push_back( 3 );   
    indices.push_back( 3 );   indices.push_back( 0 );   

    // sides 
    indices.push_back( 0 );   indices.push_back( 4 );   
    indices.push_back( 1 );   indices.push_back( 5 );   
    indices.push_back( 2 );   indices.push_back( 6 );   
    indices.push_back( 3 );   indices.push_back( 7 ); 

    // back
    indices.push_back( 4 );   indices.push_back( 5 );   
    indices.push_back( 5 );   indices.push_back( 6 );   
    indices.push_back( 6 );   indices.push_back( 7 );   
    indices.push_back( 7 );   indices.push_back( 4 );

    _mesh.setBuffers( vertices, indices );
}

