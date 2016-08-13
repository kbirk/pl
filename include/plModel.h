#pragma once

#include "plCommon.h"

#include "plOctreeMesh.h"
#include "plTriangle.h"
#include "plTransparentRenderable.h"
#include "plPicking.h"
#include "plRenderingPipeline.h"
#include "plVAO.h"
#include "plRenderer.h"
#include "plEditable.h"

// used to order triangles for drawing transparent meshes
class plOrderPair
{
    public:

        int32_t   index;
        float32_t distance;

        plOrderPair (int32_t i, float32_t d)
            : index(i), distance(d)
        {
        }

        bool operator< (const plOrderPair& orderPair) const { return distance > orderPair.distance; } // greater distance is considered "less"

};


#define PL_MODEL_COLOUR                          0.80f, 0.80f, 0.80f
#define PL_MODEL_BONE_COLOUR                     0.80f, 0.80f, 0.80f
#define PL_MODEL_CARTILAGE_COLOUR                0.50f, 0.20f, 0.20f


class plModel : public plTransparentRenderable, public plEditable
{
    public:

        plString filename;

        plModel(const plString &file, uint32_t octreeDepth = 0);
        plModel(const std::vector<plTriangle> &triangles, const plString &file, uint32_t octreeDepth = 0);

        virtual ~plModel();

        const plMesh& mesh() const { return *_mesh; }

        void toggleOctreeVisibility();

        void extractRenderComponents(plRenderMap& renderMap) const;
        void extractRenderComponents(plRenderMap& renderMap, uint32_t technique) const;

        plVector3 getCentroid() const;

    private:

        std::shared_ptr<plMesh> _mesh;
        std::shared_ptr<plVAO >  _vao;

        void _generateVAO();

};
