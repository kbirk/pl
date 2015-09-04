#ifndef PL_MESH_ALGORITHM_H
#define PL_MESH_ALGORITHM_H

#include "plCommon.h"
#include "plVector3.h"
#include "plPolygon.h"
#include "plTriangle.h"
#include "plMeshConnectivityData.h"

class plMeshAlgorithm
{

  public:

    plMeshAlgorithm();

    PLbool importTriSeq( const std::vector<plTriangle>& tris, PLuint originatingMesh, PLuint verbose, PLuint depth=0 );
    PLbool exportTriSeq(       std::vector<plTriangle>& tris, PLuint verbose, PLuint depth=0 );

  protected:

    PLfloat _epsilon;

    // the connectivity data
    plMeshConnectivityData _data;

    // all the rest of these functions return false if an error is detected. At this point the intersector should abort with a message of some kind.
    PLbool _findVertWithinEpsilon( const plVector3& vertex, const plMeshConnectivityDataVert*& vertPointer, PLuint verbose, PLuint depth=0 );

    PLbool _splitEdgeOnVect( const plMeshConnectivityDataEdge* edgeAB , const plMeshConnectivityDataVert* vertN, PLuint verbose, PLuint depth=0 );
    PLbool _splitFaceOnVect( const plMeshConnectivityDataFace* face012, const plMeshConnectivityDataVert* vertN, PLuint verbose, PLuint depth=0 );

    // these return true when the mesh is good, false when it is bad
    PLbool _checkArraySizes       ( PLuint verbose, PLuint depth=0 );
    PLbool _checkNoDuplicates     ( PLuint verbose, PLuint depth=0 );
    PLbool _checkBidirectional    ( PLuint verbose, PLuint depth=0 );
    PLbool _checkNoSliverTriangles( PLuint verbose, PLuint depth=0 );
    PLbool _checkForAllErrors     ( PLuint verbose, PLuint depth=0 );

    template< class Type1, class Type2 >
    PLbool _addressExists( const std::vector<Type1> &vector, const Type2 *t );

};


template< class Type1, class Type2 >
PLbool plMeshAlgorithm::_addressExists( const std::vector<Type1> &vector, const Type2 *t )
{
    return std::find( vector.begin(), vector.end(), t ) != vector.end();
}

#endif // PLMESHCONNECTIVITY_H
