#include "plPlannerShader.h"

plPlannerShader::plPlannerShader(const std::vector<std::string > &sourceFiles)
{
    // create and compile shader, tests for errors
    GLuint shader = _createAndCompileShader(sourceFiles, GL_COMPUTE_SHADER);

    // create program
    _id = glCreateProgram();

    // attach shader
    glAttachShader(_id, shader);

    // link program, check for error
    _linkProgram();

    // free shader objects
    glDeleteShader(shader);

    // get uniform locations
    _getUniformLocations();
}


void plPlannerShader::_getUniformLocations()
{
    _defectSiteGridPointCountID     = glGetUniformLocation(_id, "uDefectSiteGridPointCount");
    _defectSiteBoundaryPointCountID = glGetUniformLocation(_id, "uDefectSiteBoundaryPointCount");
    _defectSiteTriangleCountID      = glGetUniformLocation(_id, "uDefectSiteTriangleCount");
    _defectSiteAreaID               = glGetUniformLocation(_id, "uDefectSiteArea");
    _defectSiteAvgNormalID          = glGetUniformLocation(_id, "uDefectSiteAvgNormal");

    _donorSiteCountID               = glGetUniformLocation(_id, "uDonorSiteCount");
    _donorSiteTotalGridPointsID     = glGetUniformLocation(_id, "uDonorTotalGridPointCount");
    _donorSiteTriangleCountsID      = glGetUniformLocation(_id, "uDonorSiteTriangleCounts");
    _donorSiteGridPointCountsID     = glGetUniformLocation(_id, "uDonorSiteGridPointCounts");
    _donorSiteBoundaryPointCountsID = glGetUniformLocation(_id, "uDonorSiteBoundaryPointCounts");
    _donorSiteDataOffsetsID         = glGetUniformLocation(_id, "uDonorSiteDataOffsets");

    _defectSolutionGraftCountID      = glGetUniformLocation(_id, "uDefectSolutionGraftCount");
    _defectSolutionGraftPositionsID  = glGetUniformLocation(_id, "uDefectSolutionGraftPositions");
    _defectSolutionGraftNormalsID    = glGetUniformLocation(_id, "uDefectSolutionGraftNormals");
    _defectSolutionGraftRadiiID      = glGetUniformLocation(_id, "uDefectSolutionGraftRadii");
    _defectSolutionSurfaceNormalsID  = glGetUniformLocation(_id, "uDefectSolutionSurfaceNormals");

    _seedID = glGetUniformLocation(_id, "uSeed");

    _temperatureID = glGetUniformLocation(_id, "uTemperature");
    _loadLocalID   = glGetUniformLocation(_id, "uLoadLocal");

    _rotationIndexID  = glGetUniformLocation(_id, "uRotationIndex");
    _rotationAnglesID = glGetUniformLocation(_id, "uRotationAngles");
}


void plPlannerShader::setDefectSiteUniforms(const plPlanningSite &defectSite) const
{
    glUniform1ui  (_defectSiteTriangleCountID,      defectSite.triangles.size());
    glUniform1f   (_defectSiteAreaID,               defectSite.area);
    glUniform1ui  (_defectSiteGridPointCountID,     defectSite.gridPoints.size());
    glUniform1ui  (_defectSiteBoundaryPointCountID, defectSite.boundaryPoints.size());
    glUniform4fv  (_defectSiteAvgNormalID, 1,      &defectSite.avgNormal.x);
}


void plPlannerShader::setDonorSiteUniforms(const std::vector<plPlanningSite> &donorSites) const
{
    uint32_t totalGridPoints = 0;
    uint32_t dataIndexOffset = 0;
    std::vector<uint32_t> gridPointCounts;
    std::vector<uint32_t> triangleCounts;
    std::vector<uint32_t> boundaryPointCounts;
    std::vector<uint32_t> dataOffsets;

    for (const plPlanningSite& donorSite : donorSites)
    {
        totalGridPoints +=  donorSite.gridPoints.size();
        gridPointCounts.push_back     (donorSite.gridPoints.size());
        triangleCounts.push_back      (donorSite.triangles.size());
        boundaryPointCounts.push_back (donorSite.boundaryPoints.size());
        dataOffsets.push_back(dataIndexOffset);
        dataIndexOffset += donorSite.totalSize();
    }

    glUniform1ui  (_donorSiteCountID,               donorSites.size());
    glUniform1ui  (_donorSiteTotalGridPointsID,     totalGridPoints);
    glUniform1uiv (_donorSiteGridPointCountsID,     gridPointCounts.size(),     &gridPointCounts[0]);
    glUniform1uiv (_donorSiteTriangleCountsID,      triangleCounts.size(),      &triangleCounts[0]);
    glUniform1uiv (_donorSiteBoundaryPointCountsID, boundaryPointCounts.size(), &boundaryPointCounts[0]);
    glUniform1uiv (_donorSiteDataOffsetsID,         dataOffsets.size(),         &dataOffsets[0]);
}


void plPlannerShader::setDefectSolutionUniforms (const plDefectSolution &solution) const
{
    glUniform1ui  (_defectSolutionGraftCountID,     solution.graftCount);
    glUniform4fv  (_defectSolutionGraftPositionsID, solution.graftPositions.size(),      &solution.graftPositions[0].x);
    glUniform4fv  (_defectSolutionGraftNormalsID,   solution.graftNormals.size(),        &solution.graftNormals[0].x);
    glUniform1fv  (_defectSolutionGraftRadiiID,     solution.graftRadii.size(),          &solution.graftRadii[0]);
    glUniform4fv  (_defectSolutionSurfaceNormalsID, solution.graftSurfaceNormals.size(), &solution.graftSurfaceNormals[0].x);
}


void plPlannerShader::setSeedUniform() const
{
    glUniform1ui  (_seedID, rand());
}


void plPlannerShader::setTemperatureUniform(float32_t temperature) const
{
    glUniform1f   (_temperatureID, temperature);
}


void plPlannerShader::setLocalLoadUniform(uint32_t loadLocal) const
{
    glUniform1ui (_loadLocalID, loadLocal);
}


void plPlannerShader::setRotationAngleUniforms(uint32_t numDirections) const
{
    // get rotation angles
    std::vector<float32_t> rotationAngles;
    float32_t da = 360.0f / (float32_t)(numDirections);
    for (uint32_t i=0; i < numDirections; i++)
    {
            rotationAngles.push_back(da * i);
    }
    glUniform1fv (_rotationAnglesID, rotationAngles.size(), &rotationAngles[0]);
}


void plPlannerShader::setRotationIndexUniform(uint32_t rotationIndex) const
{
    glUniform1ui (_rotationIndexID, rotationIndex);

}
