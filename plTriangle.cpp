#include "plTriangle.h"

plTriangle::plTriangle()
    :   _normal(0,0,0), _points( 3, plVector3(0,0,0) ), _centroid(0,0,0)
{
    _calcRadius();
}


plTriangle::plTriangle(const plVector3 &n, const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 )
    :   _centroid( 0.333333f * (p0 + p1 + p2) )
{
    _points.push_back( p0 );
    _points.push_back( p1 );
    _points.push_back( p2 );

    if (n.x == 0.0f && n.y == 0.0f && n.z == 0.0f)
    {
        _normal = ((p1-p0)^(p2-p0)).normalize();
    }
    else
    {
        _normal = n;
    }
    _calcRadius();
}


plTriangle::plTriangle(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2 )
    :   _normal(((p1 - p0) ^ (p2 - p0)).normalize()),
        _centroid(0.333333f * (p0 + p1 + p2))
{
    _points.push_back( p0 );
    _points.push_back( p1 );
    _points.push_back( p2 );
    _calcRadius();
}


void plTriangle::point0( const plVector3 &point )
{
    _points[0] = point;
    _recalculate();
}


void plTriangle::point1( const plVector3 &point )
{
    _points[1] = point;
    _recalculate();
}


void plTriangle::point2( const plVector3 &point )
{
    _points[2] = point;
    _recalculate();
}


void plTriangle::swapVertices0And1()
{
    plVector3 tempPt ( _points[0] );
    _points[0] = _points[1];
    _points[1] = tempPt;
    _recalculate();
}


void plTriangle::_recalculate()
{
   _normal = ((_points[1] - _points[0]) ^ (_points[2] - _points[0])).normalize();
   _centroid = 0.333333f * (_points[0] + _points[1] + _points[2]);
   _calcRadius();
}


void plTriangle::_calcRadius()
{
    _radius = PL_MAX_OF_3( (_points[0] - _centroid).length(),
                           (_points[1] - _centroid).length(),
                           (_points[2] - _centroid).length() );
}


bool plTriangle::isInside( const plVector3 &point ) const
{
    // Compute barycentric coords
    PLfloat totalAreaDiv = 1 / ( ((_points[1]-_points[0]) ^ (_points[2]-_points[0])) * _normal);
    PLfloat u = (((_points[2]-_points[1]) ^ (point - _points[1])) * _normal) * totalAreaDiv;
    PLfloat v = (((_points[0]-_points[2]) ^ (point - _points[2])) * _normal) * totalAreaDiv;

    // Reject if outside triangle
    if (u < 0 || v < 0 || u + v > 1)
        return false;

    return true;
}


plIntersection plTriangle::rayIntersect( const plVector3 &rayStart, const plVector3 &rayDir, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    // Compute ray/plane intersection
    PLfloat dn = rayDir * _normal;

    if ( dn == 0 || (backFaceCull && dn > 0) )
        return plIntersection(false);   // ray is parallel to plane, or coming from behind

    //PLfloat dist = _points[0] * _normal;

    PLfloat t = ( (_points[0]-rayStart) * _normal ) / dn;

    if (ignoreBehindRay && t < 0)
        return plIntersection(false);   // plane is behind ray

    plVector3 intPoint = rayStart + t * rayDir;

    // check if point is inside the triangle
    if ( !isInside( intPoint ) )
    {
       return plIntersection(false);
    }
    return plIntersection( intPoint, _normal, t );
}


plVector3 plTriangle::barycentricCoords( const plVector3 &testPoint ) const
{
    // we're assuming that testPoint is in-plane with the triangle.

    // Obtained from: http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    //   The more efficient solution proposed below seems to sometimes produce inaccurate
    // results (particularly as the determinant approaches 0), so beware that solution.
    plVector3 bary;

    PLfloat area012 = _normal * ((_points[1]-_points[0])^(_points[2]-_points[0]));
    PLfloat areaT12 = _normal * ((_points[1]-testPoint) ^(_points[2]-testPoint ));
    PLfloat areaT20 = _normal * ((_points[2]-testPoint) ^(_points[0]-testPoint ));

    bary.x = areaT12 / area012;
    bary.y = areaT20 / area012;
    bary.z = 1.0f - bary.x - bary.y;

    return bary;
}


PLbool plTriangle::contains( const plVector3& pt, const PLfloat& epsilon ) const
{
    if (epsilon)
        for ( PLuint i = 0; i < _points.size(); i++ )
            if ( (_points[i] - pt).length() <= epsilon )
                return true;
    else
        for ( PLuint i = 0; i < _points.size(); i++ )
            if ( _points[i] == pt )
                return true;
    return false;
}


PLfloat plTriangle::getArea() const
{
    return 0.5f * ( (_points[1] - _points[0]) ^ (_points[2] - _points[0]) ).length();
}


PLbool plTriangle::operator== ( const plTriangle& other ) const
{
    return (_points[0] == other._points[0] &&
            _points[1] == other._points[1] &&
            _points[2] == other._points[2] &&
            _normal    == other._normal);
}


plVector3 plTriangle::closestPointTo( const plVector3& point ) const
{
    plVector3 e0 = closestPointOnEdge( 0, point );
    plVector3 e1 = closestPointOnEdge( 1, point );
    plVector3 e2 = closestPointOnEdge( 2, point );

    PLfloat d0 = ( e0 - point ).squaredLength();
    PLfloat d1 = ( e1 - point ).squaredLength();
    PLfloat d2 = ( e2 - point ).squaredLength();

    if ( d0 < d1 )
    {
        if ( d0 < d2 )
            return e0;
        else
            return e2;
    }
    else
    {
        if ( d1 < d2 )
            return e1;
        else
            return e2;
    }
}


plVector3 plTriangle::closestPointOnEdge( PLuint edgeIndex, const plVector3& point ) const
{
    return plMath::closestPointOnSegment( point, _points[ edgeIndex ], _points[ ( edgeIndex + 1 ) % 3 ] );
}


// I/O operators
std::ostream& operator << ( std::ostream& stream, const plTriangle &p )
{
    stream << "Normal    = " << p.normal() << "\n"
           << "Vertex 0  = " << p.point0() << "\n"
           << "Vertex 1  = " << p.point1() << "\n"
           << "Vertex 2  = " << p.point2() << "\n";
    return stream;
}


namespace plSTL
{
    PLbool _plCheckTypeSizes();

    PLbool importFile( std::vector<plTriangle> &triangles, const plString &filename, PLbool verbose )
    {
        if ( !filename.compare( ".stl", filename.length()-4, 4) )
        {
            std::cout << "plSTL::importFile() error: Unrecognized suffix on filename '" << filename
                      << "'. STL filenames should have suffix .stl" << std::endl;
            return false;
        }

        if ( verbose )
            std::cout << "Importing " << filename << "...";

        // just in case, clear seq
        triangles.clear();

        std::ifstream infile (filename.c_str(), std::ifstream::binary);
        if (!infile.good())
        {
            std::cerr << "plSTL::importFile() error: STL file could not be opened" << std::endl;
            return false;
        }

        plVector3 n, p0, p1, p2;

        // First line: ASCII or RAW?
        plString line;
        std::getline(infile, line);
        bool isAscii = line.compareCaseInsensitive( "solid", 5);

        if (isAscii)
        {
            PLchar filler[1024];	// for reading filler text

            // Read ASCII STL
            while ( !infile.eof() )
            {

                std::getline( infile, line );

                line.stripPreceedingWhitespace();


                if (line.compare("facet", 5))
                {
                    // normal
                    sscanf(line.c_str(), "%s %s %f %f %f", filler, filler, &n.x, &n.y, &n.z);
                }
                else if ( line.compare("vertex", 6) )
                {
                    // vertex 1
                    sscanf(line.c_str(), "%s %f %f %f", filler, &p0.x, &p0.y, &p0.z);
                    // vertex 2
                    std::getline(infile, line); // read next vertex line
                    sscanf(line.c_str(), "%s %f %f %f", filler, &p1.x, &p1.y, &p1.z);
                    // vertex 3
                    std::getline(infile, line); // read next vertex line
                    sscanf(line.c_str(), "%s %f %f %f", filler, &p2.x, &p2.y, &p2.z);
                }
                else if (line.compare("endfacet", 8))
                {
                    // end of face, build triangle
                    triangles.push_back( plTriangle(n,p0,p1,p2) );
                }
            }
            infile.close();
        }
        else
        {
            if ( !_plCheckTypeSizes() )
                return false;

            // close previous binary file
            infile.close();

            // open new stream, binary format
            std::ifstream binfile( filename.c_str(), std::ifstream::binary );

            // Skip 80-byte header
            PLchar first80[80]; // create a buffer
            binfile.read( &first80[0], sizeof(PLchar)*80 ); // read to buffer

            // get number of faces
            PLuint numTriangles;
            binfile.read( reinterpret_cast<PLchar*>( &numTriangles ), sizeof(PLuint) );
            triangles.reserve( numTriangles );

            // Read the triangles
            for (PLuint i=0; i<numTriangles; i++)
            {
                PLushort nAttr;

                binfile.read( reinterpret_cast<PLchar*>( &n.x ),   sizeof(PLfloat)*3 );
                binfile.read( reinterpret_cast<PLchar*>( &p0.x ),  sizeof(PLfloat)*3 );
                binfile.read( reinterpret_cast<PLchar*>( &p1.x ),  sizeof(PLfloat)*3 );
                binfile.read( reinterpret_cast<PLchar*>( &p2.x ),  sizeof(PLfloat)*3 );
                binfile.read( reinterpret_cast<PLchar*>( &nAttr ), sizeof(PLushort)  );

                triangles.push_back( plTriangle( n, p0, p1, p2 ) );
            }

            binfile.close();
        }

        if ( verbose )
            std::cout << "\t\t\tComplete.\n";
        return true;
    }


    PLbool exportFileASCII( const std::vector<plTriangle> &triangles , const plString &filename )
    {
        std::ofstream outfile ( filename.c_str() );
        if ( !outfile.good() )
        {
            std::cerr << "plSTL::exportFileASCII() error: STL file could not be written \n";
            return false;
        }

        outfile << "solid\n";

        for (PLuint i=0; i<triangles.size(); i++)
        {
            outfile << "  facet normal " << triangles[i].normal().x << " " << triangles[i].normal().y << " " << triangles[i].normal().z << "\n" <<
                       "    outer loop\n" <<
                       "      vertex " << triangles[i].point0().x << " " << triangles[i].point0().y << " " << triangles[i].point0().z << "\n" <<
                       "      vertex " << triangles[i].point1().x << " " << triangles[i].point1().y << " " << triangles[i].point1().z << "\n" <<
                       "      vertex " << triangles[i].point2().x << " " << triangles[i].point2().y << " " << triangles[i].point2().z << "\n" <<
                       "    endloop\n" <<
                       "  endfacet\n";
        }

        outfile <<"endsolid\n";

        outfile.close();
        return true;
    }


    PLbool exportFileBinary( const std::vector<plTriangle> &triangles , const plString &filename )
    {
        if ( !_plCheckTypeSizes() )
            return false;

        std::ofstream outfile ( filename.c_str(), std::ios::trunc | std::ios::out | std::ios::binary );
        if ( !outfile.good() )
        {
            std::cerr << "plSTL::exportFileBinary() error: STL file could not be written \n";
            return false;
        }

        // 80 byte header
        PLchar header[80];
        for (PLuint i=0; i<80; i++)
        {
            header[i] = (PLchar)(0);
        }
        outfile.write( reinterpret_cast<PLchar*>(header), sizeof(PLchar)*80 );

        // 4 byte size
        PLuint size = triangles.size();
        outfile.write( reinterpret_cast<PLchar*>(&size) , sizeof(PLuint) );

        // for each facet, 50 bytes
        PLushort zeroShort(0); // at the end of every facet
        for (PLuint i=0; i<triangles.size(); i++)
        {
            outfile.write( reinterpret_cast<const PLchar*>( &triangles[i].normal().x) , sizeof(PLfloat)*3 );
            outfile.write( reinterpret_cast<const PLchar*>( &triangles[i].point0().x) , sizeof(PLfloat)*3 );
            outfile.write( reinterpret_cast<const PLchar*>( &triangles[i].point1().x) , sizeof(PLfloat)*3 );
            outfile.write( reinterpret_cast<const PLchar*>( &triangles[i].point2().x) , sizeof(PLfloat)*3 );
            outfile.write( reinterpret_cast<const PLchar*>( &zeroShort)               , sizeof(PLushort)  );
        }

        outfile.close();
        return true;
    }


    PLbool _plCheckTypeSizes()
    {
        // check to ensure compiler designates compatible bytes to each type
        if (sizeof(PLuint) != 4)
        {
            std::cerr << "plSTL::_plCheckTypeSizes() error: Expected PLuint to be 4 bytes, but it is "
                      << sizeof( PLuint ) << ".  Fix this." << std::endl;
            return false;
        }
        if (sizeof(PLushort) != 2)
        {
            std::cerr << "plSTL::_plCheckTypeSizes() error: Expected PLushort to be 2 bytes, but it is "
                      << sizeof( PLushort ) << ".  Fix this." << std::endl;
            return false;
        }
        if (sizeof(PLfloat) != 4)
        {
            std::cerr << "plSTL::_plCheckTypeSizes() error: Expected PLfloat to be 4 bytes, but it is "
                      << sizeof( PLfloat ) << ".  Fix this." << std::endl;
            return false;
        }
        return true;
    }

}
