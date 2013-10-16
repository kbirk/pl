#ifndef PLMESHINTERSECTOR_H
#define PLMESHINTERSECTOR_H

#include "plMeshAlgorithm.h"
#include "plMath.h"

class plMeshIntersector : public plMeshAlgorithm
{

  public:

    PLbool intersect(plSeq<plTriangle> &, PLuint verbose=PL_LOGGER_LEVEL_INFO, PLuint depth=0);

  private:

    // return true if an intersection is detected, false otherwise. a plVector3& is declared to hold an intersection point if it is detected.
    PLbool _intersectionVertEdge(const plMeshConnectivityDataVert*, const plMeshConnectivityDataEdge*            , PLuint verbose, PLuint depth=0);
    PLbool _intersectionEdgeEdge(const plMeshConnectivityDataEdge*, const plMeshConnectivityDataEdge*, plVector3&, PLuint verbose, PLuint depth=0);
    PLbool _intersectionVertFace(const plMeshConnectivityDataVert*, const plMeshConnectivityDataFace*            , PLuint verbose, PLuint depth=0);
    PLbool _intersectionEdgeFace(const plMeshConnectivityDataEdge*, const plMeshConnectivityDataFace*, plVector3&, PLuint verbose, PLuint depth=0);

    PLbool _findAndFixVertEdgeIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
    PLbool _findAndFixVertFaceIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
    PLbool _findAndFixEdgeEdgeIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);
    PLbool _findAndFixEdgeFaceIntersections(PLuint start, PLuint end, PLuint verbose, PLuint depth=0);

};

#endif // PLMESHINTERSECTOR_H
