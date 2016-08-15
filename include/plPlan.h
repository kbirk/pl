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
        plDefectSite& defectSites(uint32_t index) { return *_defectSites[index]; }
        plDonorSite&  donorSites (uint32_t index) { return *_donorSites[index];  }
        plGraft&      grafts     (uint32_t index) { return *_grafts[index];      }
        plModel&      models     (uint32_t index) { return *_models[index];      }

        // const reference getters
        const plDefectSite& defectSites(uint32_t index) const { return *_defectSites[index]; }
        const plDonorSite&  donorSites (uint32_t index) const { return *_donorSites[index];  }
        const plGraft&      grafts     (uint32_t index) const { return *_grafts[index];      }
        const plModel&      models     (uint32_t index) const { return *_models[index];      }

        // const array getters (gives access to array without allowing control to add, remove, etc)
        const std::vector<plDefectSite*>& defectSites() const { return _defectSites; }
        const std::vector<plDonorSite*>&  donorSites()  const { return _donorSites;  }
        const std::vector<plGraft*>&      grafts()      const { return _grafts;      }
        const std::vector<plModel*>&      models()      const { return _models;      }

        void addDefectSite(uint32_t modelIndex = 0);
        void addDonorSite (uint32_t modelIndex = 0);
        void addGraft     (const plPlug& harvest, const plPlug& recipient, float32_t radius, float32_t length = 15.0f);

        void removeDefectSite(uint32_t index);
        void removeDonorSite (uint32_t index);
        void removeGraft     (uint32_t index);

        void updateGraftMarkerPositions();
        void clear();

        int32_t getModelIndex(const plMeshSpecific& mesh) const;

    private:

        std::vector<plDefectSite*> _defectSites;
        std::vector<plDonorSite*>  _donorSites;
        std::vector<plGraft*>      _grafts;
        std::vector<plModel*>      _models;


        int32_t _getDefectSiteIndex(const plMeshSpecific& mesh) const;

        bool _modelIndexErrorCheck(const std::string& callingFunction, int32_t modelIndex) const;

};
