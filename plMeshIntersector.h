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

            PLbool operator==(const plMeshIntersectorConnectivityDataEdge&);
        };

        class plMeshIntersectorConnectivityDataFace
        {
          public:

            plTriangle    face;
            plSeq<PLuint> vertIndices; // size should always be 3
            plSeq<PLuint> edgeIndices; // size should always be 3

            plMeshIntersectorConnectivityDataFace () {}

            PLbool operator==(const plMeshIntersectorConnectivityDataFace&);
        };

        plSeq<plMeshIntersectorConnectivityDataVert> verts;
        plSeq<plMeshIntersectorConnectivityDataEdge> edges;
        plSeq<plMeshIntersectorConnectivityDataFace> faces;

        PLbool intersect(const plSeq<plTriangle> &, plSeq<plTriangle> &, PLuint verbose, PLuint depth=0);

      private:

        PLfloat _epsilon;

        // return true if an intersection is detected, false otherwise. a plVector3& is declared to hold an intersection point if it is detected.
        PLbool _intersectionVertEdge(const plMeshIntersectorConnectivityDataVert&, const plMeshIntersectorConnectivityDataEdge&            , PLuint verbose, PLuint depth=0);
        PLbool _intersectionEdgeEdge(const plMeshIntersectorConnectivityDataEdge&, const plMeshIntersectorConnectivityDataEdge&, plVector3&, PLuint verbose, PLuint depth=0);
        PLbool _intersectionVertFace(const plMeshIntersectorConnectivityDataVert&, const plMeshIntersectorConnectivityDataFace&            , PLuint verbose, PLuint depth=0);
        PLbool _intersectionEdgeFace(const plMeshIntersectorConnectivityDataEdge&, const plMeshIntersectorConnectivityDataFace&, plVector3&, PLuint verbose, PLuint depth=0);

        // all the rest of these functions return false if an error is detected. At this point the intersector should abort with a message of some kind.
        PLbool _findVert( const plVector3& vertex, PLint &index, PLuint verbose, PLuint depth=0 );

        PLbool _findAndFixVertEdgeIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
        PLbool _findAndFixVertFaceIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
        PLbool _findAndFixEdgeEdgeIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
        PLbool _findAndFixEdgeFaceIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);

        PLbool _splitEdgeOnVect(PLuint edgeIndex, const plVector3& vertex, PLuint verbose, PLuint depth=0);
        PLbool _splitFaceOnVect(PLuint faceIndex, const plVector3& vertex, PLuint verbose, PLuint depth=0);

        PLbool _importTriSeq(const plSeq<plTriangle>& tris, PLuint verbose, PLuint depth=0);
        PLbool _exportTriSeq(      plSeq<plTriangle>& tris, PLuint verbose, PLuint depth=0);

        PLbool _checkArraySizes       ( PLuint verbose, PLuint depth=0 );
        PLbool _checkNoDuplicates     ( PLuint verbose, PLuint depth=0 );
        PLbool _checkBidirectional    ( PLuint verbose, PLuint depth=0 );
        PLbool _checkNoSliverTriangles( PLuint verbose, PLuint depth=0 );
        PLbool _checkForAllErrors     ( PLuint verbose, PLuint depth=0 );

        void   _reportSizes( PLuint verbose, PLuint depth=0 );

    };

    PLbool intersect(const plSeq<plTriangle>& input, plSeq<plTriangle>& output, PLuint verbose, PLuint depth=0);

}

std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataVert &p );
std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge &p );
std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace &p );

#endif // PLMESHINTERSECTOR_H
