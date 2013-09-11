#include "plModel.h"

plModel::plModel( const plSeq<plTriangle> &triangles, const plString &filename, PLuint octreeDepth )
    : _triangles(triangles), _filename(filename), _isTransparent(false)
{
    if ( !filename.compare( ".stl", filename.length()-4, 4) )
    {
        std::cout << "plModel error: Unrecognized suffix on filename '" << filename
                  << "'. plModel filenames should have suffix .stl" << std::endl;
        return;
    }
    if ( triangles.size() == 0 )
    {
        std::cout << "plModel error: No triangles provided as input. Aborting creation." << std::endl;
        return;
    }
    // build mesh
    _mesh = plMesh(_triangles);
    // get min and max extents of model
    plVector3 min, max;
    getMinMax(min,max);
    // build octree
    std::cout << "Building octree for " << _triangles.size() << " triangles (depth = " << octreeDepth << ")...";
    _octree.build( min, max, _triangles, octreeDepth );
    std::cout << "\tComplete.\n";
}


plModel::plModel( const plString &filename, PLuint octreeDepth )
    : _filename(filename), _isTransparent(false)
{
    if ( !filename.compare( ".stl", filename.length()-4, 4) )
    {
        std::cout << "plModel error: Unrecognized suffix on filename '" << filename
                  << "'. plModel filenames should have suffix .stl" << std::endl;  		
        return;
    }
    // import STL file
    plSTL::importFile( _triangles, filename );
    // build mesh
    _mesh = plMesh(_triangles);  
    // get min and max extents of model
    plVector3 min, max;
    getMinMax(min,max);
    // build octree    
    std::cout << "Building octree for " << _triangles.size() << " triangles (depth = " << octreeDepth << ")...";
    _octree.build( min, max, _triangles, octreeDepth );      
    std::cout << "\tComplete.\n";  
}


void plModel::getMinMax(plVector3 &min, plVector3 &max) const
{
    min = plVector3(FLT_MAX, FLT_MAX, FLT_MAX);
    max = -1 * min;

    for ( PLuint i = 0; i < _triangles.size(); i++)
    {  
        const plVector3 &v = _triangles[i].centroid();

        if (v.x < min.x) min.x = v.x;
        if (v.y < min.y) min.y = v.y;
        if (v.z < min.z) min.z = v.z;

        if (v.x > max.x) max.x = v.x;
        if (v.y > max.y) max.y = v.y;
        if (v.z > max.z) max.z = v.z;
    }
}


plVector3 plModel::getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const
{
    plVector3 normal(0,0,0);
    PLint count = 0;
    float radiusSquared = radius * radius;
    
    // Find polygons on top of graft
    for (PLuint i=0; i<_triangles.size(); i++) 
    {
        if (_triangles[i].normal() * up > 0.001)
        {        
            PLfloat dist1 = (_triangles[i].point0() - origin).squaredLength();
            PLfloat dist2 = (_triangles[i].point1() - origin).squaredLength();
            PLfloat dist3 = (_triangles[i].point2() - origin).squaredLength();
           
            // if any point of triangle is withing radial sphere, accept
            float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

            if (minDist <= radiusSquared)
            {        
                normal = normal + _triangles[i].normal();
                count++;
            }
        }
    } 

    if (count == 0)
    {
        // no triangles in radial sphere, just assume previous normal, (this can be bad.....)
        std::cout << "No normal found\n";
        return up;
    }    

    return (1.0f/(PLfloat)(count) * normal).normalize();
}


PLbool _compareOrderPairs( const plOrderPair &a, const plOrderPair &b )
{
    return (a.distance > b.distance);
}


void plModel::draw( const plVector3 &colour ) const
{
    if ( !_isVisible )
        return;

    if ( !_isTransparent ) 
    {
        glDisable( GL_STENCIL_TEST );            // if opaque, allow overwriting pixels during picking
        plColourStack::load( colour.x, colour.y, colour.z, 1.0); 
        _mesh.draw();
    }
    else
    {
        glEnable( GL_STENCIL_TEST );             // if transparent, prevent overwriting pixels during picking
        plColourStack::load( colour.x, colour.y, colour.z, 0.2);

        // Sort by distance
        plVector3 viewDir = plCameraStack::direction();

        std::vector<plOrderPair> order;     order.reserve(_triangles.size() );
        for (PLuint i=0; i<_triangles.size(); i++)
        {
            order.push_back( plOrderPair( i, _triangles[i].centroid() * viewDir) );
        }
        std::sort(order.begin(), order.end(), _compareOrderPairs);

        plSeq<PLuint> indices(_triangles.size()*3 );
        for (PLuint i = 0; i < order.size(); i++)
        {
            indices.add( order[i].index*3 );
            indices.add( order[i].index*3+1 );
            indices.add( order[i].index*3+2 );
        }

        _mesh.draw(indices);
        
        glDisable( GL_STENCIL_TEST ); 
    } 
}


void plModel::toggleVisibility()
{
    if (_isTransparent) 
    {
        _isVisible = false;
        _isTransparent = false;
    } 
    else if (!_isVisible) 
    {
        _isVisible = true;
        _isTransparent = false;
    } 
    else 
    {
        _isVisible = true;
        _isTransparent = true;
    }
}


void plModel::toggleTransparency()
{
    if (_isTransparent)
    {
        _isVisible = true;
        _isTransparent = false;
    }
    else
    {
        _isVisible = true;
        _isTransparent = true;
    }
}


plIntersection plModel::rayIntersect( const plVector3 &rayOrigin, const plVector3 &rayDirection, PLbool ignoreBehindRay, PLbool backFaceCull ) const        
{
    // intersect the octree
    return _octree.rayIntersect( rayOrigin, rayDirection, ignoreBehindRay, backFaceCull );
}


std::ostream& operator << ( std::ostream& out, const plModel &m )
{
    out << m.filename();
    return out;
}



