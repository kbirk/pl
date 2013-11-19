#ifndef PL_MODEL_H
#define PL_MODEL_H

#include "plCommon.h"

#include "plMesh.h"
#include "plTriangle.h"
#include "plTransparentRenderable.h"
#include "plPickingTexture.h"
#include "plRenderingPipeline.h"
#include "plOctreeMesh.h"
#include "plVAO.h"

#define PL_MODEL_DEFAULT_OCTREE_DEPTH   7

// used to order triangles for drawing transparent meshes
class plOrderPair
{
    public:
    
        int   index;
        float distance;

        plOrderPair (int i, float d)
            : index(i), distance(d)
        {
        }
        
        PLbool operator< ( const plOrderPair& orderPair ) const { return distance > orderPair.distance; } // greater distance is considered "less"
    
};


class plModel : public plTransparentRenderable
{
    public:

        plString filename;

        plModel( const plString &file, PLuint octreeDepth = PL_MODEL_DEFAULT_OCTREE_DEPTH );
        plModel( const std::vector<plTriangle> &triangles, const plString &file, PLuint octreeDepth = PL_MODEL_DEFAULT_OCTREE_DEPTH );
               
        const plOctreeMesh& mesh() const { return _mesh; }

        void extractRenderComponents( std::set<plRenderComponent>& renderComponents ) const;

        void draw( const plVector3 &colour ) const;

	private:
	
	    plOctreeMesh _mesh;
        plVAO        _vao;
        
        void _generateVAO();

};


std::ostream& operator << ( std::ostream& out, const plModel &m );


#endif
