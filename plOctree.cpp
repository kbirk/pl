#include "plOctree.h"
 
plOctree::plOctree()
    : _root(0)
{
} 
 
plOctree::plOctree( const plVector3 &min, 
                    const plVector3 &max, 
                    const plSeq<plTriangle> &triangles, 
                    PLuint depth)   
    : _root(0)                                    
{
    build( min, max, triangles, depth );
}

plOctree::~plOctree()
{
    clear();
}

void plOctree::clear()
{
    if ( _root )
        _root->clear();
    delete _root;
    _root = 0;
}

void plOctree::build(  const plVector3 &min, const plVector3 &max, const plSeq<plTriangle> &triangles, PLuint depth )
{
    if (depth == 0)
    {
        std::cerr << "plOctree::build() error: depth must be greater than 0\n";
        exit(1);
    }
    // centre point of octree
    plVector3 centre = 0.5f * (min+max);
    
    // find largest distance component, becomes half width
    plVector3 minDiff = min - centre;
    plVector3 maxDiff = max - centre;
    PLfloat minMax = PL_MAX_OF_3( fabs(minDiff.x), fabs(minDiff.y), fabs(minDiff.z) );
    PLfloat maxMax = PL_MAX_OF_3( fabs(maxDiff.x), fabs(maxDiff.y), fabs(maxDiff.z) );       
    PLfloat halfWidth = PL_MAX_OF_2( minMax, maxMax );
    
    // Construct and fill in _root 
    clear();
    _root = new plOctreeNode( centre, halfWidth );
 
    // traverse and fill sub cubes with triangles until desired depth is reached  
    _fill( triangles, depth ); 
}

void plOctree::draw() const
{
    if ( !_isVisible )
        return;

    _root->draw();
}


plIntersection plOctree::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay, PLbool backFaceCull) const
{
    if (_root == NULL)
        return plIntersection(false);

    plIntersection closestIntersection(false);

    plSet<const plTriangle*> triangles;

    if ( _root->rayIntersect( triangles, rayOrigin, rayDirection, 0, ignoreBehindRay ) )
    {
        PLfloat min = FLT_MAX;

        //std::cout << "triCount: " << triangles.size() << "\n";

        // must use iterators for plSet
        plSet<const plTriangle*>::iterator tri_itr = triangles.begin();
        plSet<const plTriangle*>::iterator tri_end = triangles.end();
        for ( ; tri_itr != tri_end; ++tri_itr)
        {  
            plIntersection intersection = (*tri_itr)->rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );
            
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


void plOctree::_fill(const plSeq<plTriangle> &triangles, PLuint depth)
{
    for (PLuint i=0; i< triangles.size(); i++)
    {
        _root->insert( triangles[i], depth-1 );
    }
    
}


plOctreeNode::plOctreeNode( const plVector3 &c, PLfloat hw)
    : children(NULL, 8), centre(c), halfWidth(hw)
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
        if ( children[i] )
            children[i]->clear();
        delete children[i];
    }
}

void plOctreeNode::draw() const
{
    PLint count = 0;

    for (PLuint i=0; i < 8; i++)
    {
        if (children[i] != NULL)
        {
            children[i]->draw();
            count++;
        }
    }

    if (contained.size() > 0 || count > 0)    // only draw if contains objects, or has children that contain
        _mesh.draw();
    
}


void plOctreeNode::insert( const plTriangle &tri, PLuint depth)
{    
    // add to current node
    //      assumes the given triangle is already bound by the current node,
    //      there will never be the case where it should not be added
    contained.add( &tri );

    // contains the 0-7 index of the child, determined using bit wise addition
    PLint child = 0;

    // distance from each axis
    PLfloat dx = tri.centroid().x - centre.x;
    PLfloat dy = tri.centroid().y - centre.y;
    PLfloat dz = tri.centroid().z - centre.z;

    // if distance is less than radius, then the triangle straddles a boundary
    if ( fabs(dx) < tri.radius() || fabs(dy) < tri.radius() || fabs(dz) < tri.radius() )
    {
        // straddles a boundary
        // non exclusive, add to intersected children as well
        for (PLuint i=0; i<8; i++)
        {
            if ( sphereCheck( tri.centroid(), tri.radius(), i) )
            {
                // part of bounding sphere intersects child, insert
                _insertChild( i, tri, depth ); 
            }
        }    
    }    
    else
    {    
        // fully contained in a single child, find child         
        if ( dx > 0 ) child += 1;
        if ( dy > 0 ) child += 2;
        if ( dz > 0 ) child += 4;
         
        _insertChild( child, tri, depth ); 
    } 
}

void plOctreeNode::_insertChild( PLuint index, const plTriangle &tri, PLuint depth)
{
    if (children[index] != NULL)    
    {     
        // child already exists, recursively insemaxAABB  
        children[index]->insert( tri, depth-1 );
    }    
    else   
    { 
        // child does not exist, if terminal depth has not been reached, create child node
        if (depth > 0)
        {
        
            plVector3 offset;
            PLfloat step = halfWidth * 0.5f;
            offset.x = ((index & 1) ? step : -step);
            offset.y = ((index & 2) ? step : -step);
            offset.z = ((index & 4) ? step : -step);
            children[index] = new plOctreeNode( centre + offset, step);
            children[index]->insert( tri, depth - 1 );
        }
    }
}

PLfloat plOctreeNode::squaredDistanceFromPoint( const plVector3 &point, PLint child ) const
{
    plVector3 minAABB, maxAABB;    
    if (child != -1)
    {
        // check for child      
        plVector3 offsetCentre = centre; 
        PLfloat step = 0.5f * halfWidth;
        offsetCentre.x += ((child & 1) ? step : -step);
        offsetCentre.y += ((child & 2) ? step : -step);
        offsetCentre.z += ((child & 4) ? step : -step);
        minAABB = plVector3( offsetCentre.x - step, offsetCentre.y - step, offsetCentre.z - step );
        maxAABB = plVector3( offsetCentre.x + step, offsetCentre.y + step, offsetCentre.z + step );
    }
    else
    {
        // check for this node
        minAABB = plVector3( centre.x - halfWidth, centre.y - halfWidth, centre.z - halfWidth );
        maxAABB = plVector3( centre.x + halfWidth, centre.y + halfWidth, centre.z + halfWidth );   
    }

    PLfloat sqDist = 0.0f;
    // For each axis count any excess distance outside box extents
    // x
    if (point.x < minAABB.x) sqDist += (minAABB.x - point.x) * (minAABB.x - point.x);
    if (point.x > maxAABB.x) sqDist += (point.x - maxAABB.x) * (point.x - maxAABB.x);
    // y
    if (point.y < minAABB.y) sqDist += (minAABB.y - point.y) * (minAABB.y - point.y);
    if (point.y > maxAABB.y) sqDist += (point.y - maxAABB.y) * (point.y - maxAABB.y);
    // z
    if (point.z < minAABB.z) sqDist += (minAABB.z - point.z) * (minAABB.z - point.z);
    if (point.z > maxAABB.z) sqDist += (point.z - maxAABB.z) * (point.z - maxAABB.z);
        
    return sqDist;
}


PLbool plOctreeNode::sphereCheck( const plVector3 &centre, PLfloat radius, PLint child ) const
{
    // compute squared distance between sphere center and AABB
    PLfloat sqDist = squaredDistanceFromPoint(centre, child);
    // sphere and AABB intersect if the distance is less than the sphere radius
    return sqDist <= radius * radius;
}


PLbool plOctreeNode::rayIntersect( plSet<const plTriangle*> &triangles, const plVector3 &rayOrigin, const plVector3 &rayDirection, PLfloat boxInflation, PLbool ignoreBehindRay ) const
{
    plVector3 diff;

    PLfloat boxExtents = halfWidth + boxInflation;

	diff.x = rayOrigin.x - centre.x;
	if( fabs(diff.x) > boxExtents && diff.x*rayDirection.x>=0.0f)	return false;

	diff.y = rayOrigin.y - centre.y;
	if( fabs(diff.y) > boxExtents && diff.y*rayDirection.y>=0.0f)	return false;

	diff.z = rayOrigin.z - centre.z;
	if( fabs(diff.z) > boxExtents && diff.z*rayDirection.z>=0.0f)	return false;

	float fAWdU[3];
	fAWdU[0] = fabs(rayDirection.x);
	fAWdU[1] = fabs(rayDirection.y);
	fAWdU[2] = fabs(rayDirection.z);

	float f;
	f = rayDirection.y * diff.z - rayDirection.z * diff.y;	if(fabs(f)>boxExtents*fAWdU[2] + boxExtents*fAWdU[1])	return false;
	f = rayDirection.z * diff.x - rayDirection.x * diff.z;	if(fabs(f)>boxExtents*fAWdU[2] + boxExtents*fAWdU[0])	return false;
	f = rayDirection.x * diff.y - rayDirection.y * diff.x;	if(fabs(f)>boxExtents*fAWdU[1] + boxExtents*fAWdU[0])	return false;

	// intersection exists, recurse further
    PLuint childCount = 0;
    
    for (PLuint i=0; i<children.size(); i++)
    {       
        if (children[i] != NULL)
        {
            // not a leaf, recurse
            children[i]->rayIntersect( triangles, rayOrigin, rayDirection, boxInflation, ignoreBehindRay ); 
            childCount++;            
        }           
    }

    if (childCount == 0)
    {
        // leaf, add triangles
        for (PLuint i=0; i<contained.size(); i++)
        {
            triangles.insert( contained[i] );
        }        
    }
    return true;
}


void plOctreeNode::_updateMesh()
{
    // DEBUG MESH
    plSeq<plVector3> vertices( 8  );
    plSeq<PLuint>    indices ( 8*3 );
    
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
    indices.add( 0 );   indices.add( 1 );   
    indices.add( 1 );   indices.add( 2 );   
    indices.add( 2 );   indices.add( 3 );   
    indices.add( 3 );   indices.add( 0 );   

    // sides 
    indices.add( 0 );   indices.add( 4 );   
    indices.add( 1 );   indices.add( 5 );   
    indices.add( 2 );   indices.add( 6 );   
    indices.add( 3 );   indices.add( 7 ); 

    // back
    indices.add( 4 );   indices.add( 5 );   
    indices.add( 5 );   indices.add( 6 );   
    indices.add( 6 );   indices.add( 7 );   
    indices.add( 7 );   indices.add( 4 );

    _mesh.setBuffers( vertices, indices );
}

