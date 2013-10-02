#include "plScan.h"

plScan::plScan()
{
    field = NULL;
}

plScan::plScan( const plScanPointData& point, const PLfloat& resolutionW )
{
    field = NULL;
    if (!initializeVolume( point.point, point.radius, resolutionW ))
        std::cerr << "Error in plScan::plScan(): Cannot initialize volume." << std::endl;
    else if (!addPoint( point ))
        std::cerr << "Error in plScan::plScan(): Cannot add the point." << std::endl;
}

plScan::plScan( const plString& filename, const PLfloat& resolutionW )
{
    field = NULL;
    plSeq<plScanPointData> pointsRead(plScanPointReader::readPoints(filename));
    std::cout << pointsRead[0] << std::endl;
    if ( pointsRead.size() == 0 )
        std::cerr << "Error in plScan::plScan(): No points were found in the file " << filename << ". Cannot initialize volume." << std::endl;
    else if (!initializeVolume( pointsRead[0].point, pointsRead[0].radius, resolutionW ))
        std::cerr << "Error in plScan::plScan(): Cannot initialize volume." << std::endl;
    else
        for ( PLuint i = 0; i < pointsRead.size(); i++ )
            if (!addPoint( pointsRead[i] ))
                std::cerr << "Error in plScan::plScan(): Cannot add point " << i << "." << std::endl;
}

PLbool plScan::addPoint(const plScanPointData& point)
{
    if ( !field->carveSphere( point.point, point.radius ) )
        return false;
    points.add( point );
    return true;
}

PLbool plScan::initializeVolume( const plVector3& centerW, PLfloat radiusW, PLfloat resolutionW )
{
    if ( radiusW <= 0.f )
    {
        std::cerr << "Error in plScan::initializeVolume(): radiusW needs to be positive. Aborting." << std::endl;
        return false;
    }
    if ( resolutionW <= 0.f )
    {
        std::cerr << "Error in plScan::initializeVolume(): resolutionW needs to be positive. Aborting." << std::endl;
        return false;
    }
    if ( field != NULL )
    {
        std::cout << "Warning in plScan::initializeVolume(): field is not null. Deleting contents. Advise using clearAll() before calling initializeVolume()." << std::endl;
        delete field;
        field = NULL;
    }

    plVector3 paddingW    ( radiusW*10, radiusW*10, radiusW*10 );
    plVector3 originW     ( centerW-paddingW );
    plVector3 dimsW       ( 2.f*paddingW );

    field = new plScanField( originW, dimsW, resolutionW );
    return true;
}

PLbool plScan::clearAll()
{
    if ( field != NULL )
        delete field;
    field = NULL;
    points.clear();
    return true;
}

PLbool plScan::printField()
{
    if (field == NULL)
        std::cout << "Empty" << std::endl;
    else
        std::cout << *field << std::endl;
    return true;
}

