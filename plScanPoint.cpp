#include "plScanPoint.h"

plScanPointData::plScanPointData() {}

plScanPointData::plScanPointData(const plVector3& p, const plVector3& n, PLfloat r)
{
    point  = p;
    normal = n;
    radius = r;
}

plSeq<plScanPointData> plScanPointReader::readPoints(const plString& filename)
{
    std::ifstream inFileStream( filename.c_str(), std::ios::in );
    plSeq<plScanPointData> output;

    if (!inFileStream) {
        std::cerr << "Error in plScanPointReader::readPoints(): Could not open '" << filename << "'." << std::endl;
        return output;
    }

    while (true) {
        PLfloat px,py,pz,  nx,ny,nz,  r;

        inFileStream >> px >> py >> pz >> nx >> ny >> nz >> r;

        if (!inFileStream)
            break;

        plVector3 point (px,py,pz);
        plVector3 normal = plVector3(-nx,-ny,-nz).normalize();

        output.add( plScanPointData( point, normal, r ) ); // ignore radius in g
    }
    return output;
}

std::ostream& operator << ( std::ostream &stream, const plScanPointData &p )
{
    stream << p.point << " " << p.normal << " " << p.radius << std::endl;
    return stream;
}
