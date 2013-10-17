#ifndef PL_MESH_CONNECTIVITY_H
#define PL_MESH_CONNECTIVITY_H

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
    
        PLfloat _epsilon;

    public:
    
        plSet<plMeshConnectivityDataVert> verts;
        plSet<plMeshConnectivityDataEdge> edges;
        plSet<plMeshConnectivityDataFace> faces;
        plMeshConnectivityData() { _epsilon = PL_EPSILON; }

        PLfloat epsilon() const               { return _epsilon; }
        void    epsilon( PLfloat newEpsilon ) { _epsilon = newEpsilon; }

        const plMeshConnectivityDataVert* addVert( const plVector3& vert , PLuint originatingMesh, PLuint verbose=PL_LOGGER_LEVEL_INFO );
        const plMeshConnectivityDataEdge* addEdge( const plMeshConnectivityDataVert* v0, const plMeshConnectivityDataVert* v1, PLuint originatingMesh, PLuint verbose=PL_LOGGER_LEVEL_INFO );
        const plMeshConnectivityDataFace* addFace( const plMeshConnectivityDataVert* v0, const plMeshConnectivityDataVert* v1, const plMeshConnectivityDataVert* v2, const plMeshConnectivityDataEdge* e01, const plMeshConnectivityDataEdge* e12, const plMeshConnectivityDataEdge* e20, PLuint originatingMesh, PLuint verbose=PL_LOGGER_LEVEL_INFO );

        void removeVert( const plMeshConnectivityDataVert* );
        void removeEdge( const plMeshConnectivityDataEdge* );
        void removeFace( const plMeshConnectivityDataFace* );

        PLbool findVertWithinEpsilon( const plVector3& vertex, const plMeshConnectivityDataVert*& vertPointer );
        PLbool importTriSeq(const plSeq<plTriangle> &tris, PLuint originatingMesh, PLuint verbose );
        PLbool exportTriSeq(      plSeq<plTriangle> &tris, PLuint verbose );
        void   reportSizes();
};

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataVert &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataEdge &p );
std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataFace &p );

#endif // PLMESHCONNECTIVITY_H
