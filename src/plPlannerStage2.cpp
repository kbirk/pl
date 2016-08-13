#include "plPlannerStage2.h"

namespace plPlannerStage2
{

    void run(plRmsData &rmsData, const plPlanningBufferData &planningData, const plDefectSolution &defectSolution, const plCapIndices &capData)
    {
        std::vector<std::string > shaderfiles;

        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defines.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/geometry.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSite.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/donorSites.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSolution.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/capIndices.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/rms.hcmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/stage2.hcmp");

        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/geometry.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSite.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/donorSites.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/defectSolution.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/capIndices.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/rms.cmp");
        shaderfiles.push_back(PL_FILE_PREPATH"shaders/planning/stage2.cmp");

        // compile / link stage 2 shader
        plPlannerShader stage2Shader(shaderfiles);

        if (!stage2Shader.good())
            return;

        stage2Shader.bind();

        // set uniforms
        stage2Shader.setDefectSiteUniforms(planningData.defectSite);
        stage2Shader.setDonorSiteUniforms(planningData.donorSites);
        stage2Shader.setDefectSolutionUniforms(defectSolution);
        stage2Shader.setRotationAngleUniforms(PL_NUM_COMPARISION_DIRECTIONS);

        // create and initialize cap indices SSBOs to 0
        std::vector<float32_t> rmsBuffer(planningData.totalDonorGridPoints()*PL_MAX_GRAFTS_PER_SOLUTION*PL_NUM_COMPARISION_DIRECTIONS, -1.0f);
        rmsData.rmsSSBO.set(rmsBuffer, rmsBuffer.size());

        // bind SSBOs
        planningData.defectSiteSSBO.bind(0);
        planningData.donorSitesSSBO.bind(1);
        capData.defectCapIndexSSBO.bind(2);
        capData.donorCapIndexSSBO.bind(3);
        rmsData.rmsSSBO.bind(4);

        const uint32_t NUM_WORKGROUPS = ceil(planningData.totalDonorGridPoints() / (float32_t) PL_STAGE_2_GROUP_SIZE); // ensure enough workgroups are used

        for (uint32_t i=0; i < PL_NUM_COMPARISION_DIRECTIONS; i++)
        {
            stage2Shader.setRotationIndexUniform(i);

            // call compute shader with 1D workgrouping
            glDispatchCompute(NUM_WORKGROUPS, 1, 1);

            // memory barrier
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

            //uint32_t size = planningData.totalDonorGridPoints()*PL_MAX_GRAFTS_PER_SOLUTION*PL_NUM_COMPARISION_DIRECTIONS;
            //std::vector<float32_t> tempRms(size, -1.0f);
            //rmsData.rmsSSBO.read(tempRms, size);
            rmsData.rmsSSBO.read(rmsBuffer, rmsBuffer.size());

            plUtility::printProgressBar(i / (float32_t)PL_NUM_COMPARISION_DIRECTIONS);
        }

        plUtility::printProgressBar(1.0);
        /*
        rmsData.rmsSSBO.read(rmsBuffer, rmsBuffer.size());
        for (uint32_t i=0; i < rmsBuffer.size(); i+=planningData.totalDonorGridPoints()*PL_MAX_GRAFTS_PER_SOLUTION)
        {
            std::cout << rmsBuffer[i] << "  ";
        }
        */

        planningData.defectSiteSSBO.unbind(0);
        planningData.donorSitesSSBO.unbind(1);
        capData.defectCapIndexSSBO.unbind(2);
        capData.donorCapIndexSSBO.unbind(3);
        rmsData.rmsSSBO.unbind(4);
    }

}
