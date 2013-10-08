#ifndef PLMESHCONNECTIVITY_H
#define PLMESHCONNECTIVITY_H

#include "plSeq.h"
#include "plTriangle.h"
#include "plPolygon.h"
#include "plVector3.h"

// cells

class plMeshConnectivityData;
class plMeshConnectivityDataVert;
class plMeshConnectivityDataEdge;
class plMeshConnectivityDataFace;

class plMeshConnectivityDataVert
{
  public:

    plVector3                                        vert;
    PLuint                                           originatingMesh;
    mutable plSeq<const plMeshConnectivityDataEdge*> edgeIndices; // size can be any positive number
    mutable plSeq<const plMeshConnectivityDataFace*> faceIndices; // size can be any positive number
    plMeshConnectivityData*                          dataset;

    plMeshConnectivityDataVert () {}

    PLbool operator==(const plMeshConnectivityDataVert&) const;
    PLbool operator< (const plMeshConnectivityDataVert&) const;
    PLbool operator> (const plMeshConnectivityDataVert&) const;
};

class plMeshConnectivityDataEdge
{
  public:

    plEdge                                           edge;
    PLuint                                           originatingMesh;
    mutable plSeq<const plMeshConnectivityDataVert*> vertIndices; // size should always be 2
    mutable plSeq<const plMeshConnectivityDataFace*> faceIndices; // size should always be an even number
    plMeshConnectivityData*                          dataset;

    plMeshConnectivityDataEdge () {}

    PLbool operator==(const plMeshConnectivityDataEdge&) const;
    PLbool operator< (const plMeshConnectivityDataEdge&) const;
};

class plMeshConnectivityDataFace
{
  public:

    plTriangle                                       face;
    PLuint                                           originatingMesh;
    mutable plSeq<const plMeshConnectivityDataVert*> vertIndices; // size should always be 3
    mutable plSeq<const plMeshConnectivityDataEdge*> edgeIndices; // size should always be 3
    plMeshConnectivityData*                          dataset;

    plMeshConnectivityDataFace () {}

    PLbool operator==(const plMeshConnectivityDataFace&) const;
    PLbool operator< (const plMeshConnectivityDataFace&) const;
};

typedef std::set<plMeshConnectivityDataVert>::iterator plMeshConnectivityDataVertIterator;
typedef std::set<plMeshConnectivityDataEdge>::iterator plMeshConnectivityDataEdgeIterator;
typedef std::set<plMeshConnectivityDataFace>::iterator plMeshConnectivityDataFaceIterator;

// container class for all types of cell

class plMeshConnectivityData
{
  public:

    plSet<plMeshConnectivityDataVert> verts;
    plSet<plMeshConnectivityDataEdge> edges;
    plSet<plMeshConnectivityDataFace> faces;

    PLfloat epsilon;

};

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataVert &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataEdge &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataFace &p );

#endif // PLMESHCONNECTIVITY_H
