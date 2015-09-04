#ifndef PL_MODEL_H
#define PL_MODEL_H

#include "plCommon.h"

#include "plOctreeMesh.h"
#include "plTriangle.h"
#include "plTransparentRenderable.h"
#include "plArthroViewable.h"
#include "plPicking.h"
#include "plRenderingPipeline.h"
#include "plVAO.h"
#include "plRenderer.h"
#include "plEditable.h"

// used to order triangles for drawing transparent meshes
class plOrderPair
{
    public:

        int   index;
        float distance;

        plOrderPair (int i, float d)
            : index( i ), distance( d )
        {
        }

        PLbool operator< ( const plOrderPair& orderPair ) const { return distance > orderPair.distance; } // greater distance is considered "less"

};


#define PL_MODEL_COLOUR                          0.80f, 0.80f, 0.80f
#define PL_MODEL_BONE_COLOUR                     0.80f, 0.80f, 0.80f
#define PL_MODEL_CARTILAGE_COLOUR                0.50f, 0.20f, 0.20f


class plModel : public plTransparentRenderable,
                public plArthroViewable,
                public plEditable
{
    public:

        plString filename;

        plModel( const plString &file, PLuint octreeDepth = 0 );
        plModel( const std::vector<plTriangle> &triangles, const plString &file, PLuint octreeDepth = 0 );

        const plMesh& mesh() const { return *_mesh; }

        void toggleOctreeVisibility();

        void extractRenderComponents( plRenderMap& renderMap ) const;
        void extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const;

        plVector3 getCentroid() const;

	private:

		std::shared_ptr< plMesh > _mesh;
	    std::shared_ptr< plVAO >  _vao;

        void _generateVAO();

};

#endif
