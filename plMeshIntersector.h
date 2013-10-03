#ifndef PLMESHINTERSECTOR_H
#define PLMESHINTERSECTOR_H

#include "plMeshAlgorithm.h"
#include "plMath.h"

class plMeshIntersector : public plMeshAlgorithm
{

  public:

    PLbool intersect(const plSeq<plTriangle> &, plSeq<plTriangle> &, PLuint verbose, PLuint depth=0);

  private:

    // return true if an intersection is detected, false otherwise. a plVector3& is declared to hold an intersection point if it is detected.
    PLbool _intersectionVertEdge(const plMeshConnectivityData::plMeshConnectivityDataVert&, const plMeshConnectivityData::plMeshConnectivityDataEdge&            , PLuint verbose, PLuint depth=0);
    PLbool _intersectionEdgeEdge(const plMeshConnectivityData::plMeshConnectivityDataEdge&, const plMeshConnectivityData::plMeshConnectivityDataEdge&, plVector3&, PLuint verbose, PLuint depth=0);
    PLbool _intersectionVertFace(const plMeshConnectivityData::plMeshConnectivityDataVert&, const plMeshConnectivityData::plMeshConnectivityDataFace&            , PLuint verbose, PLuint depth=0);
    PLbool _intersectionEdgeFace(const plMeshConnectivityData::plMeshConnectivityDataEdge&, const plMeshConnectivityData::plMeshConnectivityDataFace&, plVector3&, PLuint verbose, PLuint depth=0);

    PLbool _findAndFixVertEdgeIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
    PLbool _findAndFixVertFaceIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
    PLbool _findAndFixEdgeEdgeIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
    PLbool _findAndFixEdgeFaceIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);

};

#endif // PLMESHINTERSECTOR_H
