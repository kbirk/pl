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


void plSTLImportFile( plSeq<plTriangle>  &triangles, plString filename)
{
    // just in case, clear seq
    triangles.clear();

    std::ifstream infile (filename.c_str());
    if (!infile.good())
    {
        std::cerr << "STL file could not be opened \n";
        exit(1);
    }
    
    plVector3 n,p1,p2,p3;
    
    // First line: ASCII or RAW?
    plString line;
    std::getline(infile, line);
    bool isAscii = plStringCompareCaseInsensitive(line, "solid", 5);

    if (isAscii) 
    {      
        PLchar filler[1024];	// for reading filler text

        // Read ASCII STL
        while (!infile.eof()) 
        {
            
            std::getline(infile, line);    
            
            plStringStripPreceedingWhitespace( line );
       
            if (plStringCompareCaseInsensitive(line, "facet", 5)) 
            {   
                // normal   
                sscanf(line.c_str(), "%s %s %f %f %f", filler, filler, &n.x, &n.y, &n.z);
            } 
            else if (plStringCompareCaseInsensitive(line, "vertex", 6)) 
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
            else if (plStringCompareCaseInsensitive(line, "endfacet", 8))
            {
                _plCheckAndFixNormal( n, p1, p2, p3 );

                // end of face, build triangle
                triangles.add( plTriangle(n,p1,p2,p3) );
                
            }
        }

    } 
    else 
    {
        _plCheckIOTypeSizes();

        // reset file position to beginning
        infile.seekg(0);

        // Skip 80-byte header       
        PLchar first80[80]; // create a buffer
        infile.read( &first80[0], sizeof(PLchar)*80 ); // read to buffer
        
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

            _plCheckAndFixNormal( n, p1, p2, p3 );
           
            triangles.add( plTriangle( n, p1, p2, p3) );
        }
    }

}


void plSTLExportFileASCII( const plSeq<plTriangle> &triangles , plString filename )
{
    std::ofstream outfile ( filename.c_str() );
    if ( !outfile.good() )
    {
        std::cerr << "STL file could not be written \n";
        exit(1);
    }

    outfile << "solid\n";

    for (PLuint i=0; i<triangles.size(); i++) 
    {
        outfile << "  facet normal " << triangles[i].normal.x << " " << triangles[i].normal.y << " " << triangles[i].normal.z << "\n" <<
                   "    outer loop\n" <<
                   "      vertex " << triangles[i].point1.x << " " << triangles[i].point1.y << " " << triangles[i].point1.z << "\n" <<
                   "      vertex " << triangles[i].point2.x << " " << triangles[i].point2.y << " " << triangles[i].point2.z << "\n" <<
                   "      vertex " << triangles[i].point3.x << " " << triangles[i].point3.y << " " << triangles[i].point3.z << "\n" <<
                   "    endloop\n" <<
                   "  endfacet\n";
    }

    outfile <<"endsolid\n";

    outfile.close();
}


void plSTLExportFileBinary( const plSeq<plTriangle> &triangles , plString filename )
{
    _plCheckIOTypeSizes();

    std::ofstream outfile ( filename.c_str(), std::ios::trunc | std::ios::out | std::ios::binary );
    if ( !outfile.good() )
    {
        std::cerr << "STL file could not be written \n";
        exit(1);
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
    PLushort zeroPLushort(0); // at the end of every facet
    for (PLuint i=0; i<triangles.size(); i++) 
    {
        outfile.write( reinterpret_cast<PLchar*>(&triangles[i].normal.x) , sizeof(PLfloat)*3 );
        outfile.write( reinterpret_cast<PLchar*>(&triangles[i].point1.x) , sizeof(PLfloat)*3 );
        outfile.write( reinterpret_cast<PLchar*>(&triangles[i].point2.x) , sizeof(PLfloat)*3 );
        outfile.write( reinterpret_cast<PLchar*>(&triangles[i].point3.x) , sizeof(PLfloat)*3 );
        outfile.write( reinterpret_cast<PLchar*>(&zeroPLushort)          , sizeof(PLushort)  );
    }

    outfile.close();
}


void _plCheckIOTypeSizes()
{
    // check to ensure compiler designates compatible bytes to each type
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
}


// if the normal is zero, compute it from the three points
void _plCheckAndFixNormal(plVector3 &n, const plVector3 &p1, const plVector3 &p2, const plVector3 &p3)
{
    if (n.x == 0.0f && n.y == 0.0f && n.z == 0.0f) 
    {
        n = ((p2-p1)^(p3-p1)).normalize();
    }
}
