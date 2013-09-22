#ifndef PLMESHINTERSECTOR_H
#define PLMESHINTERSECTOR_H

#include "plVector3.h"
#include "plPolygon.h"
#include "plTriangle.h"
#include "plMath.h"

class plMeshIntersectorConnectivityData
{
  private:

    class plMeshIntersectorConnectivityDataVert
    {
      public:
        plVector3     vert;
        plSeq<PLuint> edgeIndices; // size can be any positive number
        plSeq<PLuint> faceIndices; // size can be any positive number

        PLbool operator==(const plMeshIntersectorConnectivityDataVert&);
    };

    class plMeshIntersectorConnectivityDataEdge
    {
      public:
        plEdge        edge;
        plSeq<PLuint> vertIndices; // size should always be 2
        plSeq<PLuint> faceIndices; // size should always be an even number

        PLbool intersectsVert(const plMeshIntersectorConnectivityDataVert&);
        PLbool intersectsEdge(const plMeshIntersectorConnectivityDataEdge&, plVector3&);

        PLbool operator==(const plMeshIntersectorConnectivityDataEdge&);
    };

    class plMeshIntersectorConnectivityDataFace
    {
      public:
        plTriangle    face;
        plSeq<PLuint> vertIndices; // size should always be 3
        plSeq<PLuint> edgeIndices; // size should always be 3

        PLbool intersectsVert(const plMeshIntersectorConnectivityDataVert&);
        PLbool intersectsEdge(const plMeshIntersectorConnectivityDataEdge&, plVector3&);

        PLbool operator==(const plMeshIntersectorConnectivityDataFace&);
    };

    PLbool _splitEdgeOnVect(PLuint edgeIndex, PLuint vertIndex);
    PLbool _splitFaceOnVect(PLuint faceIndex, PLuint vertIndex);

    PLbool _importTriSeq(const plSeq<plTriangle>&);
    PLbool _exportTriSeq(      plSeq<plTriangle>&);

  public:
    plSeq<plMeshIntersectorConnectivityDataVert> verts;
    plSeq<plMeshIntersectorConnectivityDataEdge> edges;
    plSeq<plMeshIntersectorConnectivityDataFace> faces;

    intersect(const plSeq<plTriangle> &);
};

#endif // PLMESHINTERSECTOR_H