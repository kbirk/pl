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
    mutable plSeq<const plMeshConnectivityDataEdge*> edges; // size can be any positive number
    mutable plSeq<const plMeshConnectivityDataFace*> faces; // size can be any positive number
    plMeshConnectivityData*                          dataset;

    plMeshConnectivityDataVert () { originatingMesh = 0; }
    plMeshConnectivityDataVert ( const plMeshConnectivityDataVert& );

    plMeshConnectivityDataVert& operator=(const plMeshConnectivityDataVert&);
    PLbool operator==(const plMeshConnectivityDataVert&) const;
    PLbool operator< (const plMeshConnectivityDataVert&) const;
    PLbool operator> (const plMeshConnectivityDataVert&) const;
};

class plMeshConnectivityDataEdge
{
  public:

    plEdge                                           edge;
    PLuint                                           originatingMesh;
    mutable plSeq<const plMeshConnectivityDataVert*> verts; // size should always be 2
    mutable plSeq<const plMeshConnectivityDataFace*> faces; // size should always be an even number
    plMeshConnectivityData*                          dataset;

    plMeshConnectivityDataEdge () { originatingMesh = 0; }
    plMeshConnectivityDataEdge ( const plMeshConnectivityDataEdge& );

    plMeshConnectivityDataEdge& operator=(const plMeshConnectivityDataEdge&);
    PLbool operator==(const plMeshConnectivityDataEdge&) const;
    PLbool operator< (const plMeshConnectivityDataEdge&) const;
};

class plMeshConnectivityDataFace
{
  public:

    plTriangle                                       face;
    PLuint                                           originatingMesh;
    mutable plSeq<const plMeshConnectivityDataVert*> verts; // size should always be 3
    mutable plSeq<const plMeshConnectivityDataEdge*> edges; // size should always be 3
    plMeshConnectivityData*                          dataset;

    plMeshConnectivityDataFace () { originatingMesh = 0; }
    plMeshConnectivityDataFace ( const plMeshConnectivityDataFace& );

    plMeshConnectivityDataFace& operator=(const plMeshConnectivityDataFace&);
    PLbool operator==(const plMeshConnectivityDataFace&) const;
    PLbool operator< (const plMeshConnectivityDataFace&) const;
};

typedef std::set<plMeshConnectivityDataVert>::iterator plMeshConnectivityDataVertIterator;
typedef std::set<plMeshConnectivityDataEdge>::iterator plMeshConnectivityDataEdgeIterator;
typedef std::set<plMeshConnectivityDataFace>::iterator plMeshConnectivityDataFaceIterator;

// container class for all types of cell

class plMeshConnectivityData
{
  protected:

  public:
    plSet<plMeshConnectivityDataVert> verts;
    plSet<plMeshConnectivityDataEdge> edges;
    plSet<plMeshConnectivityDataFace> faces;

    const plMeshConnectivityDataVert* addVert( const plVector3& vert , PLuint originatingMesh );
    const plMeshConnectivityDataEdge* addEdge( const plMeshConnectivityDataVert* , const plMeshConnectivityDataVert* , PLuint originatingMesh );
    const plMeshConnectivityDataFace* addFace( const plMeshConnectivityDataVert* , const plMeshConnectivityDataVert* , const plMeshConnectivityDataVert* , const plMeshConnectivityDataEdge* , const plMeshConnectivityDataEdge*  , const plMeshConnectivityDataEdge* , PLuint originatingMesh );
    void removeVert( const plMeshConnectivityDataVert* );
    void removeEdge( const plMeshConnectivityDataEdge* );
    void removeFace( const plMeshConnectivityDataFace* );
};

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataVert &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataEdge &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataFace &p );

#endif // PLMESHCONNECTIVITY_H
