#include "plPlannerStage1.h"

namespace plPlannerStage1
{

    void run(
        std::shared_ptr<plCapIndices> capData,
        std::shared_ptr<plPlanningBufferData> planningData,
        std::shared_ptr<plDefectSolution> defectSolution)
    {
        std::vector<std::string> shaderfiles;

        shaderfiles.push_back("./resources/shaders/planning/defines.h");
        shaderfiles.push_back("./resources/shaders/planning/geometry.h");
        shaderfiles.push_back("./resources/shaders/planning/defectSite.h");
        shaderfiles.push_back("./resources/shaders/planning/donorSites.h");
        shaderfiles.push_back("./resources/shaders/planning/defectSolution.h");
        shaderfiles.push_back("./resources/shaders/planning/capIndices.h");
        shaderfiles.push_back("./resources/shaders/planning/stage1.h");

        shaderfiles.push_back("./resources/shaders/planning/geometry.comp");
        shaderfiles.push_back("./resources/shaders/planning/defectSite.comp");
        shaderfiles.push_back("./resources/shaders/planning/donorSites.comp");
        shaderfiles.push_back("./resources/shaders/planning/defectSolution.comp");
        shaderfiles.push_back("./resources/shaders/planning/capIndices.comp");
        shaderfiles.push_back("./resources/shaders/planning/stage1.comp");

        // compile / link stage 1 shader
        plPlannerShader stage1Shader(shaderfiles);

        if (!stage1Shader.good())
            return;

        stage1Shader.bind();

        // set uniforms
        stage1Shader.setDefectSiteUniforms(planningData->defectSite);
        stage1Shader.setDonorSiteUniforms(planningData->donorSites);
        stage1Shader.setDefectSolutionUniforms(defectSolution);

        // create and initialize cap indices SSBOs to 0
        std::vector<uint32_t> defectIndices(defectSolution->graftCount*PL_MAX_CAP_TRIANGLES, 0);
        std::vector<uint32_t> donorIndices(planningData->totalDonorGridPoints()*PL_MAX_CAP_TRIANGLES, 0);

        capData->defectCapIndexSSBO->set(defectIndices, defectIndices.size());
        capData->donorCapIndexSSBO->set(donorIndices, donorIndices.size());

        // bind SSBOs
        planningData->defectSiteSSBO->bind(0);
        planningData->donorSitesSSBO->bind(1);
        capData->defectCapIndexSSBO->bind(2);
        capData->donorCapIndexSSBO->bind(3);

        const uint32_t NUM_WORKGROUPS = ceil(planningData->totalDonorGridPoints() + defectSolution->graftCount / (float32_t) PL_STAGE_1_GROUP_SIZE); // ensure enough workgroups are used

        plUtility::printProgressBar(0.0);

        // call compute shader with 1D workgrouping
        glDispatchCompute(NUM_WORKGROUPS, 1, 1);

        // memory barrier
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        plUtility::printProgressBar(1.0);

        planningData->defectSiteSSBO->unbind(0);
        planningData->donorSitesSSBO->unbind(1);
        capData->defectCapIndexSSBO->unbind(2);
        capData->donorCapIndexSSBO->unbind(3);

    }

}
