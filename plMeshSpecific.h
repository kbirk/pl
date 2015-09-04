#ifndef PL_MESH_SPECIFIC_H
#define PL_MESH_SPECIFIC_H

#include "plCommon.h"
#include "plMesh.h"


class plMeshSpecific
{
    public:

        plMeshSpecific();
        plMeshSpecific( const plMesh& mesh );

        virtual const plMesh& mesh() const { return *_mesh; }

    protected:

        const plMesh* _mesh;
};


#endif
