#include "plTriangle.h"

plTriangle::plTriangle()
    : _points(3, plVector3(0,0,0)),
      _normal(0,0,0),
      _centroid(0,0,0)
{
    _calcRadius();
}


plTriangle::plTriangle(const plVector3 &n, const plVector3 &p0, const plVector3 &p1, const plVector3 &p2)
    :   _centroid(0.333333f * (p0 + p1 + p2))
{
    _points.push_back(p0);
    _points.push_back(p1);
    _points.push_back(p2);

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


plTriangle::plTriangle(const plVector3 &p0, const plVector3 &p1, const plVector3 &p2)
    : _normal(((p1 - p0) ^ (p2 - p0)).normalize()),
      _centroid(0.333333f * (p0 + p1 + p2))
{
    _points.push_back(p0);
    _points.push_back(p1);
    _points.push_back(p2);
    _calcRadius();
}


void plTriangle::point0(const plVector3 &point)
{
    _points[0] = point;
    _recalculate();
}


void plTriangle::point1(const plVector3 &point)
{
    _points[1] = point;
    _recalculate();
}


void plTriangle::point2(const plVector3 &point)
{
    _points[2] = point;
    _recalculate();
}


void plTriangle::swapVertices0And1()
{
    plVector3 tempPt (_points[0]);
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
    _radius = PL_MAX_OF_3((_points[0] - _centroid).length(),
                           (_points[1] - _centroid).length(),
                           (_points[2] - _centroid).length());
}


bool plTriangle::isInside(const plVector3 &point) const
{
    // Compute barycentric coords
    float32_t totalAreaDiv = 1 / (((_points[1]-_points[0]) ^ (_points[2]-_points[0])) * _normal);
    float32_t u = (((_points[2]-_points[1]) ^ (point - _points[1])) * _normal) * totalAreaDiv;
    float32_t v = (((_points[0]-_points[2]) ^ (point - _points[2])) * _normal) * totalAreaDiv;

    // Reject if outside triangle
    if (u < 0 || v < 0 || u + v > 1)
        return false;

    return true;
}


plIntersection plTriangle::rayIntersect(const plVector3 &rayStart, const plVector3 &rayDir, bool ignoreBehindRay, bool backFaceCull) const
{
    // Compute ray/plane intersection
    float32_t dn = rayDir * _normal;

    if (dn == 0 || (backFaceCull && dn > 0))
        return plIntersection(false);   // ray is parallel to plane, or coming from behind

    //float32_t dist = _points[0] * _normal;

    float32_t t = ((_points[0]-rayStart) * _normal) / dn;

    if (ignoreBehindRay && t < 0)
        return plIntersection(false);   // plane is behind ray

    plVector3 intPoint = rayStart + t * rayDir;

    // check if point is inside the triangle
    if (!isInside(intPoint))
    {
       return plIntersection(false);
    }
    return plIntersection(intPoint, _normal, t);
}


plVector3 plTriangle::barycentricCoords(const plVector3 &testPoint) const
{
    // we're assuming that testPoint is in-plane with the triangle.

    // Obtained from: http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    //   The more efficient solution proposed below seems to sometimes produce inaccurate
    // results (particularly as the determinant approaches 0), so beware that solution.
    plVector3 bary;

    float32_t area012 = _normal * ((_points[1]-_points[0])^(_points[2]-_points[0]));
    float32_t areaT12 = _normal * ((_points[1]-testPoint) ^(_points[2]-testPoint));
    float32_t areaT20 = _normal * ((_points[2]-testPoint) ^(_points[0]-testPoint));

    bary.x = areaT12 / area012;
    bary.y = areaT20 / area012;
    bary.z = 1.0f - bary.x - bary.y;

    return bary;
}


bool plTriangle::contains(const plVector3& pt, const float32_t& epsilon) const
{
    if (epsilon)
        for (uint32_t i = 0; i < _points.size(); i++)
            if ((_points[i] - pt).length() <= epsilon)
                return true;
    else
        for (uint32_t i = 0; i < _points.size(); i++)
            if (_points[i] == pt)
                return true;
    return false;
}


float32_t plTriangle::getArea() const
{
    return 0.5f * ((_points[1] - _points[0]) ^ (_points[2] - _points[0])).length();
}


bool plTriangle::operator== (const plTriangle& other) const
{
    return (_points[0] == other._points[0] &&
            _points[1] == other._points[1] &&
            _points[2] == other._points[2] &&
            _normal    == other._normal);
}


plVector3 plTriangle::closestPointTo(const plVector3& point) const
{
    plVector3 e0 = closestPointOnEdge(0, point);
    plVector3 e1 = closestPointOnEdge(1, point);
    plVector3 e2 = closestPointOnEdge(2, point);

    float32_t d0 = (e0 - point).squaredLength();
    float32_t d1 = (e1 - point).squaredLength();
    float32_t d2 = (e2 - point).squaredLength();

    if (d0 < d1)
    {
        if (d0 < d2)
            return e0;
        else
            return e2;
    }
    else
    {
        if (d1 < d2)
            return e1;
        else
            return e2;
    }
}


plVector3 plTriangle::closestPointOnEdge(uint32_t edgeIndex, const plVector3& point) const
{
    return plMath::closestPointOnSegment(point, _points[edgeIndex], _points[(edgeIndex + 1) % 3]);
}


// I/O operators
std::ostream& operator << (std::ostream& stream, const plTriangle &p)
{
    stream << "Normal    = " << p.normal() << "\n"
           << "Vertex 0  = " << p.point0() << "\n"
           << "Vertex 1  = " << p.point1() << "\n"
           << "Vertex 2  = " << p.point2() << "\n";
    return stream;
}


namespace plSTL
{
    bool _plCheckTypeSizes();

    bool importFile(std::vector<plTriangle> &triangles, const plString &filename, bool verbose)
    {
        if (!filename.compare(".stl", filename.length()-4, 4))
        {
            std::cout << "plSTL::importFile() error: Unrecognized suffix on filename '" << filename
                      << "'. STL filenames should have suffix .stl" << std::endl;
            return false;
        }

        std::cout << "Importing " << filename << "..." << std::endl;

        // just in case, clear
        triangles.clear();

        std::ifstream infile(filename.c_str(), std::ifstream::binary);
        if (!infile.good())
        {
            std::cerr << "plSTL::importFile() error: STL file could not be opened" << std::endl;
            return false;
        }

        std::cout << "Opened " << filename << " successfully..." << std::endl;

        plVector3 n, p0, p1, p2;

        // First line: ASCII or RAW?
        plString line;
        std::getline(infile, line);
        bool isAscii = line.compareCaseInsensitive("solid", 5);

        if (isAscii)
        {
            std::cout << "ASCII STL format detected..." << std::endl;
            uint8_t filler[1024];    // for reading filler text

            // Read ASCII STL
            while (!infile.eof())
            {

                std::getline(infile, line);

                line.stripPreceedingWhitespace();

                if (line.compare("facet", 5))
                {
                    // normal
                    sscanf(line.c_str(), "%s %s %f %f %f", filler, filler, &n.x, &n.y, &n.z);
                }
                else if (line.compare("vertex", 6))
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
                    triangles.push_back(plTriangle(n,p0,p1,p2));
                }
            }
            infile.close();
        }
        else
        {
            std::cout << "Binary STL format detected..." << std::endl;
            if (!_plCheckTypeSizes())
            {
                return false;
            }

            // close previous binary file
            //infile.close();

            // reset get pointer
            infile.seekg(0);

            // Skip 80-byte header
            char first80[80]; // create a buffer
            infile.read(&first80[0], sizeof(char)*80); // read to buffer

            // get number of faces
            uint32_t numTriangles;
            infile.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));
            triangles.reserve(numTriangles);

            // Read the triangles
            for (uint32_t i=0; i<numTriangles; i++)
            {
                uint16_t nAttr;

                infile.read(reinterpret_cast<char*>(&n.x),   sizeof(float32_t)*3);
                infile.read(reinterpret_cast<char*>(&p0.x),  sizeof(float32_t)*3);
                infile.read(reinterpret_cast<char*>(&p1.x),  sizeof(float32_t)*3);
                infile.read(reinterpret_cast<char*>(&p2.x),  sizeof(float32_t)*3);
                infile.read(reinterpret_cast<char*>(&nAttr), sizeof(uint16_t));

                triangles.push_back(plTriangle(n, p0, p1, p2));
            }

            infile.close();
        }

        std::cout << "Successfully loaded " << filename << "..." << std::endl;
        return true;
    }


    bool exportFileASCII(const std::vector<plTriangle> &triangles , const plString &filename)
    {
        std::ofstream outfile (filename.c_str());
        if (!outfile.good())
        {
            std::cerr << "plSTL::exportFileASCII() error: STL file could not be written \n";
            return false;
        }

        outfile << "solid\n";

        for (uint32_t i=0; i<triangles.size(); i++)
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


    bool exportFileBinary(const std::vector<plTriangle> &triangles , const plString &filename)
    {
        if (!_plCheckTypeSizes())
            return false;

        std::ofstream outfile (filename.c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
        if (!outfile.good())
        {
            std::cerr << "plSTL::exportFileBinary() error: STL file could not be written \n";
            return false;
        }

        // 80 byte header
        char header[80];
        for (uint32_t i=0; i<80; i++)
        {
            header[i] = (char)(0);
        }
        outfile.write(reinterpret_cast<char*>(header), sizeof(char)*80);

        // 4 byte size
        uint32_t size = triangles.size();
        outfile.write(reinterpret_cast<char*>(&size) , sizeof(uint32_t));

        // for each facet, 50 bytes
        uint16_t zeroShort(0); // at the end of every facet
        for (uint32_t i=0; i<triangles.size(); i++)
        {
            outfile.write(reinterpret_cast<const char*>(&triangles[i].normal().x) , sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&triangles[i].point0().x) , sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&triangles[i].point1().x) , sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&triangles[i].point2().x) , sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&zeroShort)               , sizeof(uint16_t));
        }

        outfile.close();
        return true;
    }


    bool _plCheckTypeSizes()
    {
        // check to ensure compiler designates compatible bytes to each type
        if (sizeof(uint32_t) != 4)
        {
            std::cerr << "plSTL::_plCheckTypeSizes() error: Expected uint32_t to be 4 bytes, but it is "
                      << sizeof(uint32_t) << ".  Fix this." << std::endl;
            return false;
        }
        if (sizeof(uint16_t) != 2)
        {
            std::cerr << "plSTL::_plCheckTypeSizes() error: Expected uint16_t to be 2 bytes, but it is "
                      << sizeof(uint16_t) << ".  Fix this." << std::endl;
            return false;
        }
        if (sizeof(float32_t) != 4)
        {
            std::cerr << "plSTL::_plCheckTypeSizes() error: Expected float32_t to be 4 bytes, but it is "
                      << sizeof(float32_t) << ".  Fix this." << std::endl;
            return false;
        }
        return true;
    }

}
