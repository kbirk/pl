#include "plScan.h"


// constructors

plScan::plScan()
{
    _field = NULL;
}

plScan::plScan( const plScanPointData& point, const PLfloat& resolutionW )
{
    _field = NULL;
    if (!initializeVolume( point.point, point.radius, resolutionW ))
        std::cerr << "Error in plScan::plScan(): Cannot initialize volume." << std::endl;
    else if (!addPoint( point ))
        std::cerr << "Error in plScan::plScan(): Cannot add the point." << std::endl;
}

plScan::plScan( const plString& filename, const PLfloat& resolutionW )
{
    std::cout << "entering constructor" << std::endl;
    _field = NULL;
    plSeq<plScanPointData> pointsRead(plScanPointReader::readPoints(filename));
    if ( pointsRead.size() == 0 )
        std::cerr << "Error in plScan::plScan(): No points were found in the file " << filename << ". Cannot initialize volume." << std::endl;
    else if (!initializeVolume( pointsRead[0].point, pointsRead[0].radius, resolutionW ))
        std::cerr << "Error in plScan::plScan(): Cannot initialize volume." << std::endl;
    else
        for ( PLuint i = 0; i < pointsRead.size(); i++ )
            if (!addPoint( pointsRead[i] ))
                std::cerr << "Error in plScan::plScan(): Cannot add point " << i << "." << std::endl;
    std::cout << "reached end of constructor" << std::endl;
}

plScan::~plScan()
{
    std::cout << "entering destructor" << std::endl;
    clearAll();
    std::cout << "reached end of destructor" << std::endl;
}


// controlling the volume nad points

PLbool plScan::addPoint(const plScanPointData& point)
{
    if ( !_field->carveSphere( point.point, point.radius ) )
        return false;
    _points.add( point );
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
    if ( _field != NULL )
    {
        std::cout << "Warning in plScan::initializeVolume(): field is not null. Deleting contents. Advise using clearAll() before calling initializeVolume()." << std::endl;
        delete _field;
        _field = NULL;
    }

    plVector3 paddingW    ( radiusW*10, radiusW*10, radiusW*10 );
    plVector3 originW     ( centerW-paddingW );
    plVector3 dimsW       ( 2.f*paddingW );

    _field = new plScanField( originW, dimsW, resolutionW );
    return true;
}

PLbool plScan::clearAll()
{
    if ( _field != NULL )
        delete _field;
    _field = NULL;
    _points.clear();
    return true;
}


// output/visualization

PLbool plScan::printPoints() const
{
    for ( PLuint i = 0; i < _points.size(); i++ )
        std::cout << _points[i] << std::endl;
    return true;
}

PLbool plScan::printField() const
{
    if (_field == NULL)
        std::cout << "Empty" << std::endl;
    else
        std::cout << *_field << std::endl;
    return true;
}

PLbool plScan::draw() const
{
    _field->draw();
    return true;
}

