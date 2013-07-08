#include "plModelBase.h"

////////////////////////////////////////////////////////
// plModelBase
////////////////////////////////////////////////////////

plModelBase::plModelBase( std::string filename )
{
    if (filename.compare(filename.length()-4, 4, ".stl") != 0)
    {
        std::cout << "plModel error: Unrecognized suffix on filename '"
                  << "'. plModel filenames should have suffix .stl" << std::endl;
        return;
    }

    plSTLImportFile( _triangles, filename );
    _filename = filename;
}


void plModelBase::getMinMax(plVector3 &min, plVector3 &max) const
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


plVector3 plModelBase::getAverageNormal( PLfloat radius, const plVector3 &origin, const plVector3 &up ) const
{
    plVector3 normal(0,0,0);
    PLint count = 0;
    float radiusSquared = radius * radius;  // increasing by factor of two to fruther smooth normal

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


plString plModelBase::getFilenameWithoutPath()
{
    plString filenameOnly( _filename );
    plStringStripPreceedingFilepath( filenameOnly );
    return filenameOnly;
}


std::ostream& operator << ( std::ostream& out, const plModelBase &m )
{
    out << m._filename;
    return out;
}




////////////////////////////////////////////////////////
// plBoneAndCartilage
////////////////////////////////////////////////////////


plBoneAndCartilageBase::plBoneAndCartilageBase( plString bone_file, plString cartilage_file)
    :   bone(bone_file), cartilage(cartilage_file)
{
}


void plBoneAndCartilageBase::getMinMax(plVector3 &min, plVector3 &max) const
{
    plVector3 bmin, bmax, cmin, cmax;
    bone.getMinMax(bmin, bmax);
    cartilage.getMinMax(cmin, cmax);

    min.x = (bmin.x < cmin.x) ? bmin.x : cmin.x;
    min.y = (bmin.y < cmin.y) ? bmin.y : cmin.y;
    min.z = (bmin.z < cmin.z) ? bmin.z : cmin.z;

    max.x = (bmax.x > cmax.x) ? bmax.x : cmax.x;
    max.y = (bmax.y > cmax.y) ? bmax.y : cmax.y;
    max.z = (bmax.z > cmax.z) ? bmax.z : cmax.z;
}


void plBoneAndCartilageBase::readFromCSV( const plSeq<plString> &row, const plString &directory )
{
    // fill in the field
    plString subfield = row[2];

    if (plStringCompareCaseInsensitive(subfield, "bone file") )
    {
        bone = plModelBase( plStringConcat(directory, row[3])  );       // combine directory and filename
    }
    else if (plStringCompareCaseInsensitive(subfield, "cartilage file") )
    {
        cartilage = plModelBase( plStringConcat(directory, row[3])  );  // combine directory and filename
    }
    else
    {
        std::cerr << "Error importing plan, 'model': Unrecognized word '" << subfield << "' in third column." << std::endl;
    }
}
