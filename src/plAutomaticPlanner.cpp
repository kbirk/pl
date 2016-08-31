#include "plAutomaticPlanner.h"

#include "plOpenGLInfo.h"
#include "plProgress.h"

namespace plAutomaticPlanner
{
    // private function prototypes
    void _dispatch(
        std::shared_ptr<plPlan> plan,
        std::shared_ptr<plPlanningBufferData> planningData,
        uint32_t defectSiteIndex);
    void _clearPreviousPlan(std::shared_ptr<plPlan> plan);

    void calculate(std::shared_ptr<plPlan> plan, uint32_t defectSiteIndex)
    {
        if (plOpenGLInfo::majorVersion < 4 ||
            (plOpenGLInfo::majorVersion == 4 && plOpenGLInfo::minorVersion < 3))
        {
            // compute shaders require at least OpenGL 4.3
            LOG_WARN("Automated planning requires OpenGL 4.3, current version: "
                << plOpenGLInfo::majorVersion << "." << plOpenGLInfo::minorVersion);
            return;
        }

        // error checking
        if (plan->defectSites().size() == 0)
        {
            LOG_WARN("No defect sites specified");
            return;
        }
        if (plan->donorSites().size() == 0)
        {
            LOG_WARN("No donor sites specified");
            return;
        }
        if (plan->defectSites(defectSiteIndex)->spline->size() < 4)
        {
            LOG_WARN("No defect spline specified");
            return;
        }
        if (plan->defectSites(defectSiteIndex)->boundary->size() < 3)
        {
            LOG_WARN("No recipient area specified");
            return;
        }

        LOG_INFO("Assembling planning data structures...");
        auto planningData = std::make_shared<plPlanningBufferData>(plan->defectSites(0), plan->donorSites());

        // clear previous plan
        _clearPreviousPlan(plan);

        // if data is good, dispatch planner
        if (!planningData->good())
        {
            LOG_WARN("Could not produce coherent planning data buffers");
            return;
        }

        // proceed with plan
        _dispatch(plan, planningData, defectSiteIndex);
    }


    void _clearPreviousPlan(std::shared_ptr<plPlan> plan)
    {
        // clear previous grafts
        uint32_t previousCount = plan->grafts().size();
        for (uint32_t i=0; i < previousCount; i++)
        {
            plan->removeGraft(0);
        }
    }


    void _dispatch(std::shared_ptr<plPlan> plan, std::shared_ptr<plPlanningBufferData> planningData, uint32_t defectSiteIndex)
    {
        auto defectSolution = std::make_shared<plDefectSolution>();
        auto capIndices = std::make_shared<plCapIndices>();
        auto rmsData = std::make_shared<plRmsData>();
        auto donorSolution = std::make_shared<plDonorSolution>();

        auto start = plTimer::now();

        // stage 0
        LOG_INFO("Stage 0: Optimizing defect site graft surface area coverage");
        plProgress::startProgress();
        plPlannerStage0::run(defectSolution, planningData);
        plProgress::endProgress();

        // stage 1
        LOG_INFO("Stage 1: Preprocessing and caching defect site graft surface indices");
        plProgress::startProgress();
        plPlannerStage1::run(capIndices, planningData, defectSolution);
        plProgress::endProgress();

        // stage 2
        LOG_INFO("Stage 2: Calculating potential donor grafts surface RMS error");
        plProgress::startProgress();
        plPlannerStage2::run(rmsData, planningData, defectSolution, capIndices);
        plProgress::endProgress();

        // stage 3
        LOG_INFO("Stage 3: Optimizing donor cap selection");
        plProgress::startProgress();
        plPlannerStage3::run(donorSolution, planningData, defectSolution, rmsData);
        plProgress::endProgress();

        auto end = plTimer::now();

        LOG_INFO("SUCCESS: Plan computed successfully in " << ((end - start) / 1000.0) << " sec");

        if (donorSolution->graftPositions.size() > 0)
        {
            for (uint32_t i=0; i < defectSolution->graftCount; i++)
            {
                // harvest transform
                plVector3 harvestOrigin(donorSolution->graftPositions[i]);
                plVector3 harvestX(donorSolution->graftXAxes[i]);
                plVector3 harvestY(donorSolution->graftNormals[i]);
                plVector3 harvestSurfaceNormal(donorSolution->graftSurfaceNormals[i]);

                // we have the final transform, we need to seperate it into the surface transform, and rotational offset
                // find rotation from the Y to surface normal
                plMatrix44 harvestRotation;
                harvestRotation.setRotation(harvestY, harvestSurfaceNormal);

                plVector3 harvestSurfaceX = (harvestRotation * harvestX).normalize();
                plVector3 harvestSurfaceY = (harvestRotation * harvestY).normalize();;

                plTransform harvestSurfaceOrientation(harvestSurfaceX, harvestSurfaceY, harvestOrigin);
                plTransform harvestRotationalOffset(harvestRotation.inverse());

                // recipient transform
                plVector3 recipientOrigin(defectSolution->graftPositions[i]);
                plVector3 recipientY(defectSolution->graftNormals[i]);
                plVector3 recipientX((recipientY ^ plVector3(0, 0, 1)).normalize());
                plVector3 recipientSurfaceNormal(defectSolution->graftSurfaceNormals[i]);

                // find rotation from the Y to surface normal
                plMatrix44 recipientRotation;
                recipientRotation.setRotation(recipientY, recipientSurfaceNormal);

                plVector3 recipientSurfaceX = (recipientRotation * recipientX).normalize();
                plVector3 recipientSurfaceY = (recipientRotation * recipientY).normalize();

                plTransform recipientSurfaceOrientation(recipientSurfaceX, recipientSurfaceY, recipientOrigin);
                plTransform recipientRotationalOffset(recipientRotation.inverse());

                // get the model id from the graft site index
                uint32_t modelID = plan->getModelIndex(plan->donorSites(donorSolution->graftSiteIndices[i])->boundary);

                auto harvest = std::make_shared<plPlug>(
                    plan->models(modelID)->mesh(),
                    harvestSurfaceOrientation,
                    harvestRotationalOffset);

                auto recipient = std::make_shared<plPlug>(
                    plan->defectSites(defectSiteIndex)->spline->surfaceMesh(),
                    recipientSurfaceOrientation,
                    recipientRotationalOffset);

                plan->addGraft(harvest, recipient, defectSolution->graftRadii[i]);
            }
        }

        // set marker directions
        plan->updateGraftMarkerPositions();

        // set boundaries invisible
        for (uint32_t i = 0; i < plan->defectSites().size(); i++)
        {
            plan->defectSites(i)->spline->setInvisible();
            plan->defectSites(i)->boundary->setInvisible();
        }
        for (uint32_t i = 0; i < plan->donorSites().size(); i++)
        {
            plan->donorSites(i)->boundary->setInvisible();
        }

    }

}
