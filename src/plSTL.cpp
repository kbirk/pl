#include "plSTL.h"

namespace plSTL
{
    bool importFile(std::vector<plTriangle>& triangles, const plString& filename)
    {
        if (!filename.compare(".stl", filename.length()-4, 4))
        {
            LOG_WARN("Unrecognized suffix on filename `" << filename
                << "`. STL filenames should have suffix .stl");
            return false;
        }

        // just in case, clear
        triangles.clear();

        std::ifstream infile(filename.c_str(), std::ifstream::binary);
        if (!infile.good())
        {
            LOG_WARN("STL file could not be opened");
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

}
