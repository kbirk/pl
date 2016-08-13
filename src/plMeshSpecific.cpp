#include "plMeshSpecific.h" 

plMeshSpecific::plMeshSpecific()
    : _mesh( nullptr )
{
}


plMeshSpecific::plMeshSpecific( const plMesh& mesh )
    : _mesh( &mesh )
{
}
