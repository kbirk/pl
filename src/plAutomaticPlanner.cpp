#include "plAutomaticPlanner.h"

namespace plAutomaticPlanner
{
    // private function prototypes
    void _dispatch          (plPlan &plan, const plPlanningBufferData &planningData, PLuint defectSiteIndex);
    void _clearPreviousPlan (plPlan &plan);

    void calculate(plPlan &plan, PLuint defectSiteIndex)
    {
        // error checking
        if (plan.defectSites().size() == 0)                        { std::cerr << "plAutomaticPlanner::calculate() error: No defect sites specified\n";   return; }
        if (plan.donorSites().size()  == 0)                        { std::cerr << "plAutomaticPlanner::calculate() error: No donor sites specified\n";    return; }
        if (plan.defectSites(defectSiteIndex).spline.size() < 4)   { std::cerr << "plAutomaticPlanner::calculate() error: No defect spline specified\n";  return; }
        if (plan.defectSites(defectSiteIndex).boundary.size() < 3) { std::cerr << "plAutomaticPlanner::calculate() error: No recipient area specified\n"; return; }

        std::cout << "Assembling planning data structures ... \n";
        plPlanningBufferData planningData(plan.defectSites(0), plan.donorSites());

        // clear previous plan
        _clearPreviousPlan(plan);

        // if data is good, dispatch planner
        if (!planningData.good())
        {
            std::cerr << "plAutomaticPlanner::calculate() error: could not produce coherent planning data buffers" << std::endl;
            return;
        }

        // proceed with plan
        std::cout << std::endl << std::endl << std::endl << std::endl;
        _dispatch(plan, planningData, defectSiteIndex);
    }


    void _clearPreviousPlan(plPlan &plan)
    {
        // clear previous grafts
        PLuint previousCount = plan.grafts().size();
        for (PLuint i=0; i < previousCount; i++)
        {
            plan.removeGraft(0);
        }
    }


    void _dispatch(plPlan &plan, const plPlanningBufferData &planningData, PLuint defectSiteIndex)
    {
        PLtime t0, t1;

        plDefectSolution defectSolution;
        plCapIndices     capIndices;
        plRmsData        rmsData;
        plDonorSolution  donorSolution;

        // stage 0
        std::cout << std::endl << "Stage 0:   Optimizing defect site graft surface area coverage" << std::endl << std::endl;
        t0 = plTimer::now();
        plPlannerStage0::run(defectSolution, planningData, plan);
        t1 = plTimer::now();
        std::cout << ", elapsed time: " << (t1 - t0) / 1000.0f << " sec" << std::endl;

        // stage 1
        std::cout << std::endl << "Stage 1:   Preprocessing and caching defect site graft surface indices" << std::endl << std::endl;
        t0 = plTimer::now();
        plPlannerStage1::run(capIndices, planningData, defectSolution);
        t1 = plTimer::now();
        std::cout << ", elapsed time: " << (t1 - t0) / 1000.0f << " sec" << std::endl;

        // stage 2
        std::cout << std::endl << "Stage 2:   Calculating potential donor grafts surface RMS error" << std::endl << std::endl;
        t0 = plTimer::now();
        plPlannerStage2::run(rmsData, planningData, defectSolution, capIndices);
        t1 = plTimer::now();
        std::cout << ", elapsed time: " << (t1 - t0) / 1000.0f << " sec" << std::endl;

        // stage 3
        std::cout << std::endl << "Stage 3:   Optimizing donor cap selection" << std::endl << std::endl;;
        t0 = plTimer::now();
        plPlannerStage3::run(donorSolution, planningData, defectSolution, rmsData);
        t1 = plTimer::now();
        std::cout << ", elapsed time: " << (t1 - t0) / 1000.0f << " sec" << std::endl;

        if (donorSolution.graftPositions.size() > 0)
        {
            for (PLuint i=0; i < defectSolution.graftCount; i++)
            {
                // harvest transform
                plVector3 harvestOrigin(donorSolution.graftPositions[i]);
                plVector3 harvestX     (donorSolution.graftXAxes[i]);
                plVector3 harvestY     (donorSolution.graftNormals[i]);
                plVector3 harvestSurfaceNormal(donorSolution.graftSurfaceNormals[i]);

                // we have the final transform, we need to seperate it into the surface transform, and rotational offset
                // find rotation from the Y to surface normal
                plMatrix44 harvestRotation;     harvestRotation.setRotation(harvestY, harvestSurfaceNormal);

                plVector3 harvestSurfaceX = (harvestRotation * harvestX).normalize();
                plVector3 harvestSurfaceY = (harvestRotation * harvestY).normalize();;

                plTransform harvestSurfaceOrientation(harvestSurfaceX, harvestSurfaceY, harvestOrigin);
                plTransform harvestRotationalOffset(harvestRotation.inverse());

                // recipient transform
                plVector3 recipientOrigin(defectSolution.graftPositions[i]);
                plVector3 recipientY     (defectSolution.graftNormals[i]);
                plVector3 recipientX     ((recipientY ^ plVector3(0, 0, 1)).normalize());
                plVector3 recipientSurfaceNormal(defectSolution.graftSurfaceNormals[i]);

                // find rotation from the Y to surface normal
                plMatrix44 recipientRotation;    recipientRotation.setRotation(recipientY, recipientSurfaceNormal);

                plVector3 recipientSurfaceX = (recipientRotation * recipientX).normalize();
                plVector3 recipientSurfaceY = (recipientRotation * recipientY).normalize();

                plTransform recipientSurfaceOrientation(recipientSurfaceX, recipientSurfaceY, recipientOrigin);
                plTransform recipientRotationalOffset(recipientRotation.inverse());

                // get the model id from the graft site index
                PLuint modelID = plan.getModelIndex(plan.donorSites(donorSolution.graftSiteIndices[i]).boundary);

                plPlug harvest  (plan.models(modelID).mesh(),
                                  PL_PICKING_INDEX_GRAFT_DONOR,
                                  harvestSurfaceOrientation,
                                  harvestRotationalOffset);

                plPlug recipient(plan.defectSites(defectSiteIndex).spline.surfaceMesh(),
                                  PL_PICKING_INDEX_GRAFT_DEFECT,
                                  recipientSurfaceOrientation,
                                  recipientRotationalOffset);

                plan.addGraft(harvest, recipient, defectSolution.graftRadii[i]);
            }
        }

        // set marker directions
        plan.updateGraftMarkerPositions();

        // set boundaries invisible
        for (PLuint i = 0; i < plan.iGuideSites().size(); i++)
        {
            plan.iGuideSites(i).boundary.setInvisible();
        }

        for (PLuint i = 0; i < plan.defectSites().size(); i++)
        {
            plan.defectSites(i).spline.setInvisible();
            plan.defectSites(i).boundary.setInvisible();
        }

        for (PLuint i = 0; i < plan.donorSites().size(); i++)
        {
            plan.donorSites(i).boundary.setInvisible();
        }

    }

}
