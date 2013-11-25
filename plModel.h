#ifndef PL_MODEL_H
#define PL_MODEL_H

#include "plCommon.h"

#include "plOctreeMesh.h"
#include "plTriangle.h"
#include "plTransparentRenderable.h"
#include "plPicking.h"
#include "plRenderingPipeline.h"
#include "plVAO.h"
#include "plRenderer.h"

#define PL_MODEL_DEFAULT_OCTREE_DEPTH   6

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

        void extractRenderComponents( plRenderMap& renderMap ) const;

        void draw( const plVector3 &colour ) const;

	private:
	
	    plOctreeMesh _mesh;
        std::shared_ptr< plVAO > _vao;
        
        void _generateVAO();

};


std::ostream& operator << ( std::ostream& out, const plModel &m );


#endif
