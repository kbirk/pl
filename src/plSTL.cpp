#include "plSTL.h"

namespace plSTL
{
    bool _plCheckTypeSizes();

    bool importFile(std::vector<plTriangle> &triangles, const plString &filename, bool verbose)
    {
        if (!filename.compare(".stl", filename.length()-4, 4))
        {
            std::cout << "plSTL::importFile() error: Unrecognized suffix on filename '"
                << filename
                << "'. STL filenames should have suffix .stl"
                << std::endl;
            return false;
        }

        // just in case, clear
        triangles.clear();

        std::ifstream infile(filename.c_str(), std::ifstream::binary);
        if (!infile.good())
        {
            std::cout << " Failed." << std::endl;
            std::cerr << "plSTL::importFile() error: STL file could not be opened" << std::endl;
            infile.close();
            return false;
        }

        plVector3 n, p0, p1, p2;

        // first line: ASCII or RAW?
        plString line;
        std::getline(infile, line);
        bool isAscii = line.compareCaseInsensitive("solid", 5);

        if (isAscii)
        {
            // for reading filler text
            uint8_t filler[1024];

            // read ASCII STL
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
            if (!_plCheckTypeSizes())
            {
                return false;
            }

            // reset get pointer
            infile.seekg(0);

            // skip 80-byte header
            char first80[80]; // create a buffer
            infile.read(&first80[0], sizeof(char)*80); // read to buffer

            // get number of faces
            uint32_t numTriangles;
            infile.read(reinterpret_cast<char*>(&numTriangles), sizeof(uint32_t));
            triangles.reserve(numTriangles);

            // read the triangles
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

        return true;
    }


    bool exportFileASCII(const std::vector<plTriangle> &triangles , const plString &filename)
    {
        std::ofstream outfile (filename.c_str());
        if (!outfile.good())
        {
            std::cerr << "plSTL::exportFileASCII() error: STL file could not be written " << std::endl;
            return false;
        }

        outfile << "solid" << std::endl;

        for (uint32_t i=0; i<triangles.size(); i++)
        {
            outfile << "  facet normal " << triangles[i].normal().x << " " << triangles[i].normal().y << " " << triangles[i].normal().z << "\n" <<
                       "    outer loop\n" <<
                       "      vertex " << triangles[i].point0().x << " " << triangles[i].point0().y << " " << triangles[i].point0().z << "\n" <<
                       "      vertex " << triangles[i].point1().x << " " << triangles[i].point1().y << " " << triangles[i].point1().z << "\n" <<
                       "      vertex " << triangles[i].point2().x << " " << triangles[i].point2().y << " " << triangles[i].point2().z << "\n" <<
                       "    endloop\n" <<
                       "  endfacet" << std::endl;
        }

        outfile <<"endsolid" << std::endl;

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
            std::cerr << "plSTL::exportFileBinary() error: STL file could not be written " << std::endl;
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
            outfile.write(reinterpret_cast<const char*>(&triangles[i].normal().x), sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&triangles[i].point0().x), sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&triangles[i].point1().x), sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&triangles[i].point2().x), sizeof(float32_t)*3);
            outfile.write(reinterpret_cast<const char*>(&zeroShort), sizeof(uint16_t));
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
