#include "plPlanningSite.h"

plPlanningSite::plPlanningSite(
    const std::vector<plTriangle>& tris,
    std::shared_ptr<plBoundary> boundary,
    bool fineGrain)
{
    // generate interior triangles
    plMeshCutter::findInteriorMesh(triangles, tris, boundary);
    if (fineGrain)
    {
        _generateFineGridPoints();
    }
    else
    {
        _generateCoarseGridPoints();
    }
    _generateBoundaryPoints(boundary);
    _calcArea();
    _calcNormal();

    LOG_INFO("    " <<  triangles.size()  << " triangles calculated");
    LOG_INFO("    " <<  gridPoints.size() << " grid points calculated");

    if (triangles.size() == 0 || gridPoints.size() == 0)
    {
        LOG_WARN("Planning data buffer empty, please REMOVE all empty boundaries before proceeding with planner");
    }
}


bool plPlanningSite::good() const
{
    return !(gridPoints.size() == 0  ||
        gridNormals.size() == 0 ||
        triangles.size() == 0   ||
        boundaryPoints.size() == 0 ||
        boundaryNormals.size() == 0 ||
        area == 0.0f ||
        avgNormal.length() == 0.0f);
}


void plPlanningSite::_bufferGridData(std::vector<plVector4>& data) const
{
    for (uint32_t i=0; i < gridPoints.size(); i++)
    {
        data.push_back(gridPoints[i]);
    }

    for (uint32_t i=0; i < gridNormals.size(); i++)
    {
        data.push_back(gridNormals[i]);
    }
}


void plPlanningSite::_bufferMeshData(std::vector<plVector4>& data) const
{
    for (uint32_t i=0; i < triangles.size(); i++)
    {
        data.push_back(plVector4(triangles[i].point0(), 1.0));
        data.push_back(plVector4(triangles[i].point1(), 1.0));
        data.push_back(plVector4(triangles[i].point2(), 1.0));
        data.push_back(plVector4(triangles[i].normal(), 1.0));
    }
}


void plPlanningSite::_bufferBoundaryData(std::vector<plVector4>& data) const
{
    for (uint32_t i=0; i < boundaryPoints.size(); i++)
        data.push_back(boundaryPoints[i]);

    for (uint32_t i=0; i < boundaryNormals.size(); i++)
        data.push_back(boundaryNormals[i]);
}


std::shared_ptr<plSSBO> plPlanningSite::getSSBO() const
{
    std::vector<plVector4> data;
    data.reserve(totalSize());

    _bufferGridData(data);
    _bufferMeshData(data);
    _bufferBoundaryData(data);

    uint32_t numBytes = totalSize() * sizeof(plVector4);

    LOG_INFO("    Total buffer size: " << numBytes << " bytes");

    return std::make_shared<plSSBO>(numBytes, (void*)(&data[0]));
}


void plPlanningSite::_generateCoarseGridPoints()
{
    std::set<plPointAndNormal> pointsAndNormals;
    for (const plTriangle& triangle : triangles)
    {
        plVector3 smoothNormal0 = plMath::getAverageNormal(triangles, PL_NORMAL_SMOOTHING_RADIUS, triangle.point0(), triangle.normal());
        plVector3 smoothNormal1 = plMath::getAverageNormal(triangles, PL_NORMAL_SMOOTHING_RADIUS, triangle.point1(), triangle.normal());
        plVector3 smoothNormal2 = plMath::getAverageNormal(triangles, PL_NORMAL_SMOOTHING_RADIUS, triangle.point2(), triangle.normal());
        pointsAndNormals.insert(plPointAndNormal(triangle.point0(), smoothNormal0));
        pointsAndNormals.insert(plPointAndNormal(triangle.point1(), smoothNormal1));
        pointsAndNormals.insert(plPointAndNormal(triangle.point2(), smoothNormal2));
    }

    for (const plPointAndNormal& pointNormal : pointsAndNormals)
    {
        gridPoints.push_back(plVector4(pointNormal.point,  1));
        gridNormals.push_back(plVector4(pointNormal.normal, 1));
    }
}


void plPlanningSite::_generateFineGridPoints()
{
    const float32_t GRID_SPACING = 0.6f;

    std::set<plPointAndNormal> pointsAndNormals;

    // select points in each triangle at approx spacing
    for (uint32_t i=0; i < triangles.size(); i++)
    {
        plVector3 e01 = triangles[i].point1() - triangles[i].point0();
        plVector3 e12 = triangles[i].point2() - triangles[i].point1();
        plVector3 e20 = triangles[i].point0() - triangles[i].point2();

        float32_t eLength01 = e01.squaredLength();
        float32_t eLength12 = e12.squaredLength();
        float32_t eLength20 = e20.squaredLength();

        float32_t longest = PL_MAX_OF_3(eLength01, eLength12, eLength20);

        plVector3 u, v, origin;
        float32_t uMax, vMax = 0;

        if (longest == eLength01)
        {
            plVector3 tangent = (e01 ^ triangles[i].normal()).normalize();
            origin = triangles[i].point0();
            v = e01.normalize();
            vMax = e01.length();
            u = tangent.normalize();
            uMax = e20 * tangent;
        }
        else if (longest == eLength12)
        {
            plVector3 tangent = (e12 ^ triangles[i].normal()).normalize();
            origin = triangles[i].point1();
            v = e12.normalize();
            vMax = e12.length();
            u = tangent.normalize();
            uMax = e01 * tangent;
        }
        else // if (longest == eLength20)
        {
            plVector3 tangent = (e20 ^ triangles[i].normal()).normalize();
            origin = triangles[i].point2();
            v = e20.normalize();
            vMax = e20.length();
            u = tangent.normalize();
            uMax = e12 * tangent;
        }

        // build grid, row by row, right to left (more efficient this way)
        for (float32_t j=0; j<vMax; j+= GRID_SPACING)
        {
            for (float32_t k=0; k<uMax; k+= GRID_SPACING)
            {
                plVector3 point = origin + k*-u + j*v;
                plVector3 bCoord = triangles[i].barycentricCoords(point);

                if (bCoord.x < -0.001 || bCoord.y < -0.001 || bCoord.z < -0.001)
                {
                    break;  // outside of triangle edge, go to next row
                }
                plVector3 smoothNormal = plMath::getAverageNormal(triangles, PL_NORMAL_SMOOTHING_RADIUS, point, triangles[i].normal());
                pointsAndNormals.insert(plPointAndNormal(point, smoothNormal));
            }
        }

    }

    for (const plPointAndNormal& pointNormal : pointsAndNormals)
    {
        gridPoints.push_back(plVector4(pointNormal.point, 1));
        gridNormals.push_back(plVector4(pointNormal.normal, 1));
    }

}


void plPlanningSite::_generateBoundaryPoints(std::shared_ptr<plBoundary> boundary)
{
    uint32_t size = boundary->size();

    for (uint32_t i=0; i < size; i++)
    {
        boundaryPoints.push_back(plVector4(boundary->points(i), 1));
    }

    for (uint32_t i=0; i < size; i++)
    {
        plVector3 d = boundary->points((i+1)%size) - boundary->points(i);
        plVector3 n = 0.5f * (boundary->normals((i+1)%size) + boundary->normals(i));
        boundaryNormals.push_back(plVector4((d ^ n).normalize(), 1.0f));
    }
}


void plPlanningSite::_calcArea()
{
    area = 0;
    for (uint32_t i=0; i< triangles.size(); i++)
    {
        area += triangles[i].getArea();
    }
    LOG_INFO("    Site area: " << area << "");
}


void plPlanningSite::_calcNormal()
{
    avgNormal = plVector3(0, 0, 0);
    for (uint32_t i=0; i< triangles.size(); i++)
    {
        avgNormal = avgNormal + triangles[i].normal();
    }
    avgNormal = (1/(float32_t)triangles.size() * avgNormal).normalize();
}
