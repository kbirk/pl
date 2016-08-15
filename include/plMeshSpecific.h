#pragma once

#include "plCommon.h"
#include "plMesh.h"

class plMeshSpecific
{
    public:

        plMeshSpecific();
        plMeshSpecific(const plMesh& mesh);

        virtual const plMesh& mesh() const { return *_mesh; }

    protected:

        const plMesh* _mesh;
};
