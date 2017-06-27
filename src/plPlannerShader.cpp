#include "plPlannerShader.h"

plPlannerShader::plPlannerShader(const std::vector<std::string>& sourceFiles)
{
    // create and compile shader, tests for errors
    GLuint shader = _createAndCompileShader(sourceFiles, GL_COMPUTE_SHADER);

    // create program
    _id = glCreateProgram();
    LOG_OPENGL("glCreateProgram");

    // attach shader
    glAttachShader(_id, shader);
    LOG_OPENGL("glAttachShader");

    // link program, check for error
    _linkProgram();

    // free shader objects
    glDeleteShader(shader);
    LOG_OPENGL("glDeleteShader");

    // get uniform locations
    _getUniformLocations();
}

void plPlannerShader::_getUniformLocations()
{
    _defectSiteGridPointCountID = glGetUniformLocation(_id, "uDefectSiteGridPointCount");
    LOG_OPENGL("glGetUniformLocation");
    _defectSiteBoundaryPointCountID = glGetUniformLocation(_id, "uDefectSiteBoundaryPointCount");
    LOG_OPENGL("glGetUniformLocation");
    _defectSiteTriangleCountID = glGetUniformLocation(_id, "uDefectSiteTriangleCount");
    LOG_OPENGL("glGetUniformLocation");
    _defectSiteAreaID = glGetUniformLocation(_id, "uDefectSiteArea");
    LOG_OPENGL("glGetUniformLocation");
    _defectSiteAvgNormalID = glGetUniformLocation(_id, "uDefectSiteAvgNormal");
    LOG_OPENGL("glGetUniformLocation");

    _donorSiteCountID = glGetUniformLocation(_id, "uDonorSiteCount");
    LOG_OPENGL("glGetUniformLocation");
    _donorSiteTotalGridPointsID = glGetUniformLocation(_id, "uDonorTotalGridPointCount");
    LOG_OPENGL("glGetUniformLocation");
    _donorSiteTriangleCountsID = glGetUniformLocation(_id, "uDonorSiteTriangleCounts");
    LOG_OPENGL("glGetUniformLocation");
    _donorSiteGridPointCountsID = glGetUniformLocation(_id, "uDonorSiteGridPointCounts");
    LOG_OPENGL("glGetUniformLocation");
    _donorSiteBoundaryPointCountsID = glGetUniformLocation(_id, "uDonorSiteBoundaryPointCounts");
    LOG_OPENGL("glGetUniformLocation");
    _donorSiteDataOffsetsID = glGetUniformLocation(_id, "uDonorSiteDataOffsets");
    LOG_OPENGL("glGetUniformLocation");

    _defectSolutionGraftCountID = glGetUniformLocation(_id, "uDefectSolutionGraftCount");
    LOG_OPENGL("glGetUniformLocation");
    _defectSolutionGraftPositionsID = glGetUniformLocation(_id, "uDefectSolutionGraftPositions");
    LOG_OPENGL("glGetUniformLocation");
    _defectSolutionGraftNormalsID = glGetUniformLocation(_id, "uDefectSolutionGraftNormals");
    LOG_OPENGL("glGetUniformLocation");
    _defectSolutionGraftRadiiID = glGetUniformLocation(_id, "uDefectSolutionGraftRadii");
    LOG_OPENGL("glGetUniformLocation");
    _defectSolutionSurfaceNormalsID = glGetUniformLocation(_id, "uDefectSolutionSurfaceNormals");
    LOG_OPENGL("glGetUniformLocation");

    _seedID = glGetUniformLocation(_id, "uSeed");
    LOG_OPENGL("glGetUniformLocation");

    _temperatureID = glGetUniformLocation(_id, "uTemperature");
    LOG_OPENGL("glGetUniformLocation");
    _loadLocalID = glGetUniformLocation(_id, "uLoadLocal");
    LOG_OPENGL("glGetUniformLocation");

    _rotationIndexID = glGetUniformLocation(_id, "uRotationIndex");
    LOG_OPENGL("glGetUniformLocation");
    _rotationAnglesID = glGetUniformLocation(_id, "uRotationAngles");
    LOG_OPENGL("glGetUniformLocation");
}

void plPlannerShader::setDefectSiteUniforms(std::shared_ptr<plPlanningSite> defectSite) const
{
    glUniform1ui(_defectSiteTriangleCountID, defectSite->triangles.size());
    LOG_OPENGL("glUniform1ui");
    glUniform1f(_defectSiteAreaID, defectSite->area);
    LOG_OPENGL("glUniform1f");
    glUniform1ui(_defectSiteGridPointCountID, defectSite->gridPoints.size());
    LOG_OPENGL("glUniform1ui");
    glUniform1ui(_defectSiteBoundaryPointCountID, defectSite->boundaryPoints.size());
    LOG_OPENGL("glUniform1ui");
    glUniform4fv(_defectSiteAvgNormalID, 1, &defectSite->avgNormal.x);
    LOG_OPENGL("glUniform4fv");
}

void plPlannerShader::setDonorSiteUniforms(const std::vector<std::shared_ptr<plPlanningSite> >& donorSites) const
{
    uint32_t totalGridPoints = 0;
    uint32_t dataIndexOffset = 0;
    std::vector<uint32_t> gridPointCounts;
    std::vector<uint32_t> triangleCounts;
    std::vector<uint32_t> boundaryPointCounts;
    std::vector<uint32_t> dataOffsets;

    for (auto donorSite : donorSites) {
        totalGridPoints += donorSite->gridPoints.size();
        gridPointCounts.push_back(donorSite->gridPoints.size());
        triangleCounts.push_back(donorSite->triangles.size());
        boundaryPointCounts.push_back(donorSite->boundaryPoints.size());
        dataOffsets.push_back(dataIndexOffset);
        dataIndexOffset += donorSite->totalSize();
    }

    glUniform1ui(_donorSiteCountID, donorSites.size());
    LOG_OPENGL("glUniform1ui");
    glUniform1ui(_donorSiteTotalGridPointsID, totalGridPoints);
    LOG_OPENGL("glUniform1ui");
    glUniform1uiv(_donorSiteGridPointCountsID, gridPointCounts.size(), &gridPointCounts[0]);
    LOG_OPENGL("glUniform1uiv");
    glUniform1uiv(_donorSiteTriangleCountsID, triangleCounts.size(), &triangleCounts[0]);
    LOG_OPENGL("glUniform1uiv");
    glUniform1uiv(_donorSiteBoundaryPointCountsID, boundaryPointCounts.size(), &boundaryPointCounts[0]);
    LOG_OPENGL("glUniform1uiv");
    glUniform1uiv(_donorSiteDataOffsetsID, dataOffsets.size(), &dataOffsets[0]);
    LOG_OPENGL("glUniform1uiv");
}

void plPlannerShader::setDefectSolutionUniforms(std::shared_ptr<plDefectSolution> solution) const
{
    glUniform1ui(_defectSolutionGraftCountID, solution->graftCount);
    LOG_OPENGL("glUniform1ui");
    glUniform4fv(_defectSolutionGraftPositionsID, solution->graftPositions.size(), &solution->graftPositions[0].x);
    LOG_OPENGL("glUniform4fv");
    glUniform4fv(_defectSolutionGraftNormalsID, solution->graftNormals.size(), &solution->graftNormals[0].x);
    LOG_OPENGL("glUniform4fv");
    glUniform1fv(_defectSolutionGraftRadiiID, solution->graftRadii.size(), &solution->graftRadii[0]);
    LOG_OPENGL("glUniform1fv");
    glUniform4fv(_defectSolutionSurfaceNormalsID, solution->graftSurfaceNormals.size(), &solution->graftSurfaceNormals[0].x);
    LOG_OPENGL("glUniform4fv");
}

void plPlannerShader::setSeedUniform() const
{
    glUniform1ui(_seedID, rand());
    LOG_OPENGL("glUniform1ui");
}

void plPlannerShader::setTemperatureUniform(float32_t temperature) const
{
    glUniform1f(_temperatureID, temperature);
    LOG_OPENGL("glUniform1f");
}

void plPlannerShader::setLocalLoadUniform(uint32_t loadLocal) const
{
    glUniform1ui(_loadLocalID, loadLocal);
    LOG_OPENGL("glUniform1ui");
}

void plPlannerShader::setRotationAngleUniforms(uint32_t numDirections) const
{
    // get rotation angles
    std::vector<float32_t> rotationAngles;
    float32_t da = 360.0f / (float32_t)(numDirections);
    for (uint32_t i = 0; i < numDirections; i++) {
        rotationAngles.push_back(da * i);
    }
    glUniform1fv(_rotationAnglesID, rotationAngles.size(), &rotationAngles[0]);
    LOG_OPENGL("glUniform1fv");
}

void plPlannerShader::setRotationIndexUniform(uint32_t rotationIndex) const
{
    glUniform1ui(_rotationIndexID, rotationIndex);
    LOG_OPENGL("glUniform1ui");
}
