#ifndef PLMESHINTERSECTOR_H
#define PLMESHINTERSECTOR_H

#include "plVector3.h"
#include "plPolygon.h"
#include "plTriangle.h"
#include "plMath.h"

namespace plMeshIntersector
{
    class plMeshIntersectorConnectivityData
    {

      public:

        plMeshIntersectorConnectivityData();

        class plMeshIntersectorConnectivityDataVert
        {
          public:

            plVector3     vert;
            plSeq<PLuint> edgeIndices; // size can be any positive number
            plSeq<PLuint> faceIndices; // size can be any positive number

            plMeshIntersectorConnectivityDataVert () {}

            PLbool operator==(const plMeshIntersectorConnectivityDataVert&);
        };

        class plMeshIntersectorConnectivityDataEdge
        {
          public:

            plEdge        edge;
            plSeq<PLuint> vertIndices; // size should always be 2
            plSeq<PLuint> faceIndices; // size should always be an even number

            plMeshIntersectorConnectivityDataEdge () {}

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

            plMeshIntersectorConnectivityDataFace () {}

            PLbool intersectsVert(const plMeshIntersectorConnectivityDataVert&);
            PLbool intersectsEdge(const plMeshIntersectorConnectivityDataEdge&, plVector3&);

            PLbool operator==(const plMeshIntersectorConnectivityDataFace&);
        };

        plSeq<plMeshIntersectorConnectivityDataVert> verts;
        plSeq<plMeshIntersectorConnectivityDataEdge> edges;
        plSeq<plMeshIntersectorConnectivityDataFace> faces;

        PLbool intersect(const plSeq<plTriangle> &, plSeq<plTriangle> &);

      private:

        PLbool _findVert( const plVector3& vertex, PLint &index );

        PLbool _splitEdgeOnVect(PLuint edgeIndex, const plVector3& vertex);
        PLbool _splitFaceOnVect(PLuint faceIndex, const plVector3& vertex);

        PLbool _importTriSeq(const plSeq<plTriangle>&);
        PLbool _exportTriSeq(      plSeq<plTriangle>&);

        PLbool _checkArraySizes();
        PLbool _checkNoDuplicates();
        PLbool _checkBidirectionalConnections();
        PLbool _checkForAllErrors();

        void   _reportSizes();

    };

    PLbool intersect(const plSeq<plTriangle> &, plSeq<plTriangle> &);

}

std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataVert &p );
std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge &p );
std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace &p );

#endif // PLMESHINTERSECTOR_H
