#include "plPlannerStage0.h"

plAnnealingGroup::plAnnealingGroup(float32_t initialEnergy)
{
    // invocation ssbos
    _invoEnergiesSSBO = std::make_shared<plSSBO>(PL_STAGE_0_INVOCATIONS*sizeof(float32_t));
    _invoGraftPositionsSSBO = std::make_shared<plSSBO>(PL_STAGE_0_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4));
    _invoGraftNormalsSSBO = std::make_shared<plSSBO>(PL_STAGE_0_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4));
    _invoGraftRadiiSSBO = std::make_shared<plSSBO>(PL_STAGE_0_INVOCATIONS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(float32_t));
    _invoGraftCountsSSBO = std::make_shared<plSSBO>(PL_STAGE_0_INVOCATIONS*sizeof(uint32_t));
    // group ssbos
    _groupEnergiesSSBO = std::make_shared<plSSBO>(PL_STAGE_0_NUM_GROUPS*sizeof(float32_t));
    _groupGraftPositionsSSBO = std::make_shared<plSSBO>(PL_STAGE_0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4));
    _groupGraftNormalsSSBO = std::make_shared<plSSBO>(PL_STAGE_0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(plVector4));
    _groupGraftRadiiSSBO = std::make_shared<plSSBO>(PL_STAGE_0_NUM_GROUPS*PL_MAX_GRAFTS_PER_SOLUTION*sizeof(float32_t));
    _groupGraftCountsSSBO = std::make_shared<plSSBO>(PL_STAGE_0_NUM_GROUPS*sizeof(uint32_t));
    // group energies
    auto energies = std::vector<float32_t>(PL_STAGE_0_NUM_GROUPS, initialEnergy);
    _groupEnergiesSSBO->set<float32_t>(energies, PL_STAGE_0_NUM_GROUPS);
}


void plAnnealingGroup::bind()
{
    _invoEnergiesSSBO->bind(2);
    _invoGraftPositionsSSBO->bind(3);
    _invoGraftNormalsSSBO->bind(4);
    _invoGraftRadiiSSBO->bind(5);
    _invoGraftCountsSSBO->bind(6);

    _groupEnergiesSSBO->bind(7);
    _groupGraftPositionsSSBO->bind(8);
    _groupGraftNormalsSSBO->bind(9);
    _groupGraftRadiiSSBO->bind(10);
    _groupGraftCountsSSBO->bind(11);
}


void plAnnealingGroup::unbind()
{
    _invoEnergiesSSBO->unbind(2);
    _invoGraftPositionsSSBO->unbind(3);
    _invoGraftNormalsSSBO->unbind(4);
    _invoGraftRadiiSSBO->unbind(5);
    _invoGraftCountsSSBO->unbind(6);

    _groupEnergiesSSBO->unbind(7);
    _groupGraftPositionsSSBO->unbind(8);
    _groupGraftNormalsSSBO->unbind(9);
    _groupGraftRadiiSSBO->unbind(10);
    _groupGraftCountsSSBO->unbind(11);
}


void plAnnealingGroup::getSolution(
    std::shared_ptr<plDefectSolution> solution,
    std::shared_ptr<plPlanningBufferData> planningData)
{
    uint32_t index;
    float32_t energy;
    getLowestGroupInfo(index, energy);

    // read graft count
    _groupGraftCountsSSBO->readBytes<uint32_t>(&solution->graftCount, sizeof(uint32_t), 0, index*sizeof(uint32_t));
    _groupGraftPositionsSSBO->read<plVector4>(solution->graftPositions, solution->graftCount, 0, index*PL_MAX_GRAFTS_PER_SOLUTION);
    _groupGraftNormalsSSBO->read<plVector4>(solution->graftNormals, solution->graftCount, 0, index*PL_MAX_GRAFTS_PER_SOLUTION);
    _groupGraftRadiiSSBO->read<float32_t>(solution->graftRadii, solution->graftCount, 0, index*PL_MAX_GRAFTS_PER_SOLUTION);

    // re-compute positions as perturbations will shift them off the mesh surface!
    for (uint32_t i=0; i < solution->graftCount; i++)
    {
        plVector3 rayOrigin(solution->graftPositions[i].x, solution->graftPositions[i].y, solution->graftPositions[i].z);
        plVector3 rayDirection(solution->graftNormals[i].x, solution->graftNormals[i].y, solution->graftNormals[i].z);

        plIntersection intersection = plMath::rayIntersect(planningData->defectSite->triangles, rayOrigin, rayDirection, true);

        solution->graftPositions[i] = plVector4(intersection.point,  1.0f);
        solution->graftSurfaceNormals.push_back(plVector4(intersection.normal, 1.0f));
    }
}


void plAnnealingGroup::getLowestGroupInfo(uint32_t& index, float32_t& energy)
{
    // read energies
    std::vector<float32_t> energies;
    _groupEnergiesSSBO->read<float32_t>(energies, PL_STAGE_0_NUM_GROUPS);

    // find best group
    float32_t lowestEnergy = FLT_MAX;
    uint32_t lowestGroup  = 0;

    for (uint32_t i=0; i < PL_STAGE_0_NUM_GROUPS; i++)
    {
        if (energies[i] < lowestEnergy)
        {
            lowestEnergy = energies[i];
            lowestGroup = i;
        }
    }

    energy = lowestEnergy;
    index = lowestGroup;
}


uint32_t logBN(float32_t base, float32_t num)
{
    return log10(num) / log10(base);
}


namespace plPlannerStage0
{

    void run(
        std::shared_ptr<plDefectSolution> defectSolution,
        std::shared_ptr<plPlanningBufferData> planningData)
    {
        std::vector<std::string> shaderfiles;

        shaderfiles.push_back("./resources/shaders/planning/defines.h");
        shaderfiles.push_back("./resources/shaders/planning/geometry.h");
        shaderfiles.push_back("./resources/shaders/planning/defectSite.h");
        shaderfiles.push_back("./resources/shaders/planning/state.h");
        shaderfiles.push_back("./resources/shaders/planning/annealing.h");
        shaderfiles.push_back("./resources/shaders/planning/rand.h");
        shaderfiles.push_back("./resources/shaders/planning/stage0.h");

        shaderfiles.push_back("./resources/shaders/planning/geometry.comp");
        shaderfiles.push_back("./resources/shaders/planning/defectSite.comp");
        shaderfiles.push_back("./resources/shaders/planning/state.comp");
        shaderfiles.push_back("./resources/shaders/planning/annealing.comp");
        shaderfiles.push_back("./resources/shaders/planning/rand.comp");
        shaderfiles.push_back("./resources/shaders/planning/stage0.comp");

        // compile / link stage 0 shader
        plPlannerShader stage0Shader(shaderfiles);

        if (!stage0Shader.good())
            return;

        stage0Shader.bind(); // bind shader
        stage0Shader.setDefectSiteUniforms(planningData->defectSite);

        auto triangleAreaSSBO = std::make_shared<plSSBO>(planningData->defectSite->triangles.size()*PL_STAGE_0_INVOCATIONS*sizeof(float32_t));
        auto annealingBuffers = std::make_shared<plAnnealingGroup>(planningData->defectSite->area);

        planningData->defectSiteSSBO->bind(0);
        triangleAreaSSBO->bind(1);
        annealingBuffers->bind(); // 2, 3, 4, 5, 6, 7, 8, 9, 10, 11

        float32_t temperature = PL_STAGE_0_INITIAL_TEMPERATURE;

        uint32_t TOTAL_ITERATIONS = logBN(1.0 - PL_STAGE_0_COOLING_RATE, PL_STAGE_0_STOPPING_TEMPERATURE);
        uint32_t iterationNum = 0;

        // simulated annealing
        while (temperature > PL_STAGE_0_STOPPING_TEMPERATURE)
        {
            // group iteration
            stage0Shader.setLocalLoadUniform(0);
            stage0Shader.setTemperatureUniform(temperature);

            uint32_t its = uint32_t(PL_STAGE_0_ITERATIONS * temperature) + 1;

            for (int32_t i=its; i>=0; i--)
            {
                // local iteration
                // call compute shader with 1D workgrouping
                glDispatchCompute(PL_STAGE_0_NUM_GROUPS, 1, 1);
                LOG_OPENGL("glDispatchCompute");

                // memory barrier
                glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
                LOG_OPENGL("glMemoryBarrier");

                stage0Shader.setLocalLoadUniform(i);
                stage0Shader.setSeedUniform();
            }

            uint32_t bestGroup;
            float32_t energy;
            // get best group info
            annealingBuffers->getLowestGroupInfo(bestGroup, energy);

            // cool temperature
            temperature *= 1 - PL_STAGE_0_COOLING_RATE;

            plUtility::printProgressBar(iterationNum++ / (float32_t)TOTAL_ITERATIONS);
        }
        plUtility::printProgressBar(1.0);

        // load global solution from annealing state to defect state
        annealingBuffers->getSolution(defectSolution, planningData);

        // unbind and delete site and temporary buffers
        planningData->defectSiteSSBO->unbind(0);
        triangleAreaSSBO->unbind(1);
        annealingBuffers->unbind();
    }

}
