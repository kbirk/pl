#include "plTriangle.h"

// Compute plane/ray intersection, and then the local coordinates to
// see whether the intersection point is inside.
bool plTriangle::rayIntersect( plVector3 &intPoint, plVector3 &intNorm, PLfloat &t, const plVector3 &rayStart, const plVector3 &rayDir, PLbool ignoreBehindRay, PLbool backFaceCull ) const
{
    // Compute ray/plane intersection
    PLfloat dn = rayDir * normal;

    if (dn == 0 || (backFaceCull &&  dn > 0) )
        return false;		// ray is parallel to plane    

    PLfloat dist = point1 * normal;

    t = (dist - rayStart*normal) / dn;
    
    if (ignoreBehindRay && t < 0) 
        return false;       // plane is behind ray

    intPoint = rayStart + t * rayDir;

    // Compute barycentric coords
    PLfloat totalArea = ((point2-point1) ^ (point3-point1)) * normal;
    PLfloat u = (((point3-point2) ^ (intPoint - point2)) * normal) / totalArea;
    PLfloat v = (((point1-point3) ^ (intPoint - point3)) * normal) / totalArea;

    // Reject if outside triangle
    if (u < 0 || v < 0 || u + v > 1)
        return false;

    // Return int point and normal and parameter
    intNorm = normal;

    return true;
}


// I/O operators
std::ostream& operator << ( std::ostream& stream, const plTriangle &p )
{
    stream << "Normal    = " << p.normal << "\n"
           << "Vertex 1  = " << p.point1 << "\n"
           << "Vertex 2  = " << p.point2 << "\n"
           << "Vertex 3  = " << p.point3 << "\n";
    return stream;
}

void plReadSTLFile( plSeq<plTriangle>  &triangles, plString filename)
{
    // just in case, clear seq
    triangles.clear();

    std::ifstream infile (filename);
    if (!infile.good())
    {
        std::cerr << "STL file could not be opened \n";
        exit(1);
    }
    
    plVector3 n,p1,p2,p3;
    
    // First line: ASCII or RAW?
    plString line;
    std::getline(infile, line);
    bool isAscii = compareCaseInsensitive(line, "solid", 5);

    if (isAscii) 
    {      
        PLchar filler[1024];	// for reading filler text
   
        // Read ASCII STL
        while (!infile.eof()) 
        {
            
            std::getline(infile, line);            
            if (compareCaseInsensitive(line, "facet", 5)) 
            {   
                // normal   
                sscanf(line.c_str(), "%s %s %f %f %f", filler, filler, &n.x, &n.y, &n.z);
            } 
            else if (compareCaseInsensitive(line, "vertex", 6)) 
            {
                // vertex 1
                sscanf(line.c_str(), "%s %f %f %f", filler, &p1.x, &p1.y, &p1.z);
                // vertex 2
                std::getline(infile, line); // read next vertex line
                sscanf(line.c_str(), "%s %f %f %f", filler, &p2.x, &p2.y, &p2.z);
                // vertex 3
                std::getline(infile, line); // read next vertex line
                sscanf(line.c_str(), "%s %f %f %f", filler, &p3.x, &p3.y, &p3.z);
            } 
            else if (compareCaseInsensitive(line, "endfacet", 8))
            {
                // end of face, build triangle
                triangles.add( plTriangle(n,p1,p2,p3) );
            }
        }

    } 
    else 
    {
        // Read RAW STL
        if (sizeof(PLuint) != 4) 
        {
          std::cerr << "Expected PLuint to be 4 bytes, but it is " 
                    << sizeof( PLuint ) << ".  Fix this." << std::endl;
          exit(1);
        }
        if (sizeof(PLushort) != 2) 
        {
          std::cerr << "Expected PLushort to be 2 bytes, but it is " 
                    << sizeof( PLushort ) << ".  Fix this." << std::endl;
          exit(1);
        }
        if (sizeof(PLfloat) != 4) 
        {
          std::cerr << "Expected PLfloat to be 4 bytes, but it is " 
                    << sizeof( PLfloat ) << ".  Fix this." << std::endl;
          exit(1);
        }

        // Skip 80-byte header       
        plSeq<PLchar> first80(80); // create a buffer
        infile.read( &first80[0], sizeof(PLchar)*80); // read to buffer
        
        // get number of faces
        PLuint numTriangles;
        infile.read( reinterpret_cast<PLchar*>(&numTriangles), sizeof(PLuint));
        triangles.reserve(numTriangles);

        // Read the triangles
        for (PLuint i=0; i<numTriangles; i++) 
        {
            PLushort nAttr;
            
            infile.read(reinterpret_cast<PLchar*>(&n.x),  sizeof(PLfloat)*3);
            infile.read(reinterpret_cast<PLchar*>(&p1.x), sizeof(PLfloat)*3);
            infile.read(reinterpret_cast<PLchar*>(&p2.x), sizeof(PLfloat)*3);
            infile.read(reinterpret_cast<PLchar*>(&p3.x), sizeof(PLfloat)*3);
            infile.read(reinterpret_cast<PLchar*>(&nAttr), sizeof(PLushort));
           
            triangles.add( plTriangle( n, p1, p2, p3) ); 
            
        }

    }
}






