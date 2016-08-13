#include "plPlannerStage3.h"

plGreedyGroup::plGreedyGroup()
    :   _lowestRMS       (FLT_MAX),
        _lowestPositions (PL_MAX_GRAFTS_PER_SOLUTION, plVector4()),
        _lowestNormals   (PL_MAX_GRAFTS_PER_SOLUTION, plVector4()),
        _lowestXAxes     (PL_MAX_GRAFTS_PER_SOLUTION, plVector4()),
        _donorSolutionPositionsSSBO  (PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4)),
        _donorSolutionNormalsSSBO    (PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4)),
        _donorSolutionXAxesSSBO      (PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4)),
        _donorSolutionSiteIndicesSSBO(PL_STAGE_3_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(uint32_t)),
        _totalRmsSSBO                (PL_STAGE_3_INVOCATIONS*sizeof(float32_t))
{
    // initialize all rms to -1
    std::vector<float32_t> totalRMS(PL_STAGE_3_INVOCATIONS, -1.0f);
    _totalRmsSSBO.set(totalRMS, totalRMS.size());
}


void plGreedyGroup::bind()
{
    _donorSolutionPositionsSSBO.bind(3);
    _donorSolutionNormalsSSBO.bind(4);
    _donorSolutionXAxesSSBO.bind(5);
    _donorSolutionSiteIndicesSSBO.bind(6);
    _totalRmsSSBO.bind(7);
}


void plGreedyGroup::unbind()
{
    _donorSolutionPositionsSSBO.unbind(3);
    _donorSolutionNormalsSSBO.unbind(4);
    _donorSolutionXAxesSSBO.unbind(5);
    _donorSolutionSiteIndicesSSBO.bind(6);
    _totalRmsSSBO.unbind(7);
}


void plGreedyGroup::update()
{
    std::vector<float32_t> totalRMS(PL_STAGE_3_INVOCATIONS, -1.0f);
    _totalRmsSSBO.read(totalRMS, totalRMS.size());

    // find invocation with lowest RMS
    float32_t minRMS   = FLT_MAX;
    int32_t minIndex = -1;

    // get lowest rms index
    for (uint32_t i=0; i < PL_STAGE_3_INVOCATIONS; i++)
    {
        if (totalRMS[i] > 0 && totalRMS[i] < minRMS)
        {
            minRMS   = totalRMS[i];
            minIndex = i;
        }
    }

    // is it better than global solution?
    if (minIndex != -1 && minRMS < _lowestRMS)
    {
        _lowestRMS = minRMS;
        _donorSolutionPositionsSSBO.read  (_lowestPositions,   PL_MAX_GRAFTS_PER_SOLUTION, 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION);
        _donorSolutionNormalsSSBO.read    (_lowestNormals,     PL_MAX_GRAFTS_PER_SOLUTION, 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION);
        _donorSolutionXAxesSSBO.read      (_lowestXAxes,       PL_MAX_GRAFTS_PER_SOLUTION, 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION);
        _donorSolutionSiteIndicesSSBO.read(_lowestSiteIndices, PL_MAX_GRAFTS_PER_SOLUTION, 0, minIndex*PL_MAX_GRAFTS_PER_SOLUTION);
    }

}


void plGreedyGroup::getSolution(plDonorSolution &solution, const plPlanningBufferData &planningData)
{
    solution.graftPositions = _lowestPositions;
    solution.graftNormals   = _lowestNormals;
    solution.graftXAxes     = _lowestXAxes;
    solution.graftSiteIndices = _lowestSiteIndices;
    solution.rms = _lowestRMS;

    for (uint32_t i=0; i < solution.graftPositions.size(); i++)
    {
        // intersect surface
        plIntersection intersection = plMath::rayIntersect(planningData.donorSites[solution.graftSiteIndices[i]].triangles,
                                                            solution.graftPositions[i],
                                                            -solution.graftNormals[i],
                                                            true);
        // set surface normal for graft
        solution.graftSurfaceNormals.push_back(plVector4(intersection.normal, 1.0f));

    }
}


namespace plPlannerStage3
{

    void run(plDonorSolution &donorSolution, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plRmsData &rmsData)
    {
        std::vector<std::string > shaderfiles;

        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defines.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/geometry.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSite.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/donorSites.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSolution.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/donorSolution.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/rms.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/rand.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/stage3.hcmp");

        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/geometry.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSite.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/donorSites.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSolution.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/donorSolution.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/rms.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/rand.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/stage3.cmp");

        // compile / link stage 2 shader
        plPlannerShader stage3Shader(shaderfiles);

        if (!stage3Shader.good())
            return;

        stage3Shader.bind();

        // set uniforms
        stage3Shader.setDefectSiteUniforms(planningData.defectSite);
        stage3Shader.setDonorSiteUniforms(planningData.donorSites);
        stage3Shader.setDefectSolutionUniforms(defectSolution);
        stage3Shader.setRotationAngleUniforms(PL_NUM_COMPARISION_DIRECTIONS);

        // create and initialize cap indices SSBOs to 0
        plGreedyGroup greedyBuffers;

        // bind SSBOs
        planningData.defectSiteSSBO.bind(0);
        planningData.donorSitesSSBO.bind(1);
        rmsData.rmsSSBO.bind(2);
        greedyBuffers.bind(); // 3, 4, 5, 6, 7

        for (uint32_t i=0; i<PL_STAGE_3_ITERATIONS; i++)
        {
            // update seed uniform
            stage3Shader.setSeedUniform();

            // call compute shader with 1D workgrouping
            glDispatchCompute(PL_STAGE_3_NUM_GROUPS, 1, 1);

            // memory barrier
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            // check latest solution
            greedyBuffers.update();

            plUtility::printProgressBar(i / (float32_t)PL_STAGE_3_ITERATIONS);
        }
        plUtility::printProgressBar(1.0);

        greedyBuffers.getSolution(donorSolution, planningData);

        // no state found
        if (donorSolution.rms == FLT_MAX)
        {
            std::cerr << "plPlannerStage3::run() error: Unable to find suitable harvest locations \n";
        }

        // unbind and delete site and rms buffers
        planningData.defectSiteSSBO.unbind(0);
        planningData.donorSitesSSBO.unbind(1);
        rmsData.rmsSSBO.unbind(2);
        greedyBuffers.unbind();

        // DEBUG
        /*
        std::cout << std::endl << "DEBUG: " << std::endl;
        for (uint32_t i=0; i<defectSolution.graftCount; i++)
        {
            std::cout << "Graft " << i << ",    Position: " << donorSolution.graftPositions[i]
                                       << ",    Normal: "   << donorSolution.graftNormals[i] << std::endl;
        }
        std::cout << std::endl;
        //
        */
    }


}
