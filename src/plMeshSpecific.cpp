#include "plMeshSpecific.h"

plMeshSpecific::plMeshSpecific()
    : _mesh(nullptr)
{
}

plMeshSpecific::plMeshSpecific(std::shared_ptr<plMesh> mesh)
    : _mesh(mesh)
{
}
