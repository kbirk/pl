#pragma once

#include "plCommon.h"
#include "plString.h"

#include "plVector3.h"
#include "plCSV.h"
#include "plRenderable.h"
#include "plGraft.h"
#include "plModel.h"
#include "plDefectSite.h"
#include "plDonorSite.h"

#define PL_MODEL_DEFAULT_OCTREE_DEPTH 6

class plPlan : public plRenderable
{
    public:

        plPlan(plString filename);
        plPlan(int32_t argc, char **argv);

        ~plPlan();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        void importFile(const plString &filename);
        void exportFile(const plString &filename);

        // non-const reference getters
        std::shared_ptr<plDefectSite> defectSites(uint32_t index) { return _defectSites[index]; }
        std::shared_ptr<plDonorSite> donorSites (uint32_t index) { return _donorSites[index]; }
        std::shared_ptr<plGraft> grafts(uint32_t index) { return _grafts[index]; }
        std::shared_ptr<plModel> models(uint32_t index) { return _models[index]; }

        // const array getters (gives access to array without allowing control to add, remove, etc)
        const std::vector<std::shared_ptr<plDefectSite>>& defectSites() const { return _defectSites; }
        const std::vector<std::shared_ptr<plDonorSite>>& donorSites() const { return _donorSites; }
        const std::vector<std::shared_ptr<plGraft>>& grafts() const { return _grafts; }
        const std::vector<std::shared_ptr<plModel>>& models() const { return _models; }

        void addDefectSite(uint32_t modelIndex = 0);
        void addDonorSite(uint32_t modelIndex = 0);
        void addGraft(
            std::shared_ptr<plPlug>harvest, 
            std::shared_ptr<plPlug>recipient,
            float32_t radius,
            float32_t length = 15.0f);

        void removeDefectSite(uint32_t index);
        void removeDonorSite(uint32_t index);
        void removeGraft(uint32_t index);

        void updateGraftMarkerPositions();
        void clear();

        int32_t getModelIndex(std::shared_ptr<plMeshSpecific> mesh) const;

    private:

        std::vector<std::shared_ptr<plDefectSite>> _defectSites;
        std::vector<std::shared_ptr<plDonorSite>> _donorSites;
        std::vector<std::shared_ptr<plGraft>> _grafts;
        std::vector<std::shared_ptr<plModel>> _models;

        int32_t _getDefectSiteIndex(std::shared_ptr<plMeshSpecific> mesh) const;

        bool _modelIndexErrorCheck(const std::string& callingFunction, int32_t modelIndex) const;

};
