#pragma once

#include "plCommon.h"
#include "plMesh.h"

class plMeshSpecific {
public:
    plMeshSpecific();
    plMeshSpecific(std::shared_ptr<plMesh> mesh);

    virtual std::shared_ptr<plMesh> mesh() const { return _mesh; }

protected:
    std::shared_ptr<plMesh> _mesh;
};
