#ifndef PLMESHCONNECTIVITY_H
#define PLMESHCONNECTIVITY_H

#include "plSeq.h"
#include "plTriangle.h"
#include "plPolygon.h"
#include "plVector3.h"

class plMeshConnectivityData
{
  public:

    class plMeshConnectivityDataVert
    {
      public:

        plVector3     vert;
        plSeq<PLuint> edgeIndices; // size can be any positive number
        plSeq<PLuint> faceIndices; // size can be any positive number

        plMeshConnectivityDataVert () {}

        PLbool operator==(const plMeshConnectivityDataVert&);
    };

    class plMeshConnectivityDataEdge
    {
      public:

        plEdge        edge;
        plSeq<PLuint> vertIndices; // size should always be 2
        plSeq<PLuint> faceIndices; // size should always be an even number

        plMeshConnectivityDataEdge () {}

        PLbool operator==(const plMeshConnectivityDataEdge&);
    };

    class plMeshConnectivityDataFace
    {
      public:

        plTriangle    face;
        plSeq<PLuint> vertIndices; // size should always be 3
        plSeq<PLuint> edgeIndices; // size should always be 3

        plMeshConnectivityDataFace () {}

        PLbool operator==(const plMeshConnectivityDataFace&);
    };

    plSeq<plMeshConnectivityDataVert> verts;
    plSeq<plMeshConnectivityDataEdge> edges;
    plSeq<plMeshConnectivityDataFace> faces;

};

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityData::plMeshConnectivityDataVert &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityData::plMeshConnectivityDataEdge &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityData::plMeshConnectivityDataFace &p );

#endif // PLMESHCONNECTIVITY_H
