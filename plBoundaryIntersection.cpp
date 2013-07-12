#include "plBoundaryIntersection.h"

// Determine the polygons of a triangle mesh that are inside a boundary.
//
// This is a bit brute force in that it compare each triangle edge to
// each wall of the boundary.  That could eventually be improved by
// finding a single intersection point, then walking along the
// boundary across the triangle mesh.


void plFindInteriorMesh( plSeq<plTriangle> &triangles, plBoundary &boundary, plSeq<plTriangle> &interiorTriangles )
{
  // set all the processed flags to false
  plSeq<PLbool> trianglesProcessedFlag (triangles.size());
  for (PLint i=0; i<triangles.size(); i++)
    trianglesProcessedFlag.add(false);

  // just in case polygons has stuff in it, it should be emptied
  interiorTriangles.clear();

  // allocate a worst-case number of interior points
  plSeq<plVector3> interiorPoints( 3 * triangles.size() );

  // Collect polygons that intersect the boundary
  plSeq<plPolygon> interiorPolygons;
  for (PLint i=0; i<triangles.size(); i++) {
    plTriangleCutsBoundary( triangles[i], trianglesProcessedFlag[i], boundary, interiorPolygons, interiorPoints );
  }

  // Collect other polygons that contain an interior point.  This is very slow.
  for (PLint interiorPointsIndex=0; interiorPointsIndex<interiorPoints.size(); interiorPointsIndex++)
  {
    for (PLint trianglesIndex=0; trianglesIndex<triangles.size(); trianglesIndex++)
    {
      if (!trianglesProcessedFlag[trianglesIndex])
      {
        for (PLint triangleVerticesIndex=0; triangleVerticesIndex<3; triangleVerticesIndex++)
        {
          if (interiorPoints[interiorPointsIndex] == (triangles[trianglesIndex])[triangleVerticesIndex]) {
            plPolygon poly;
            poly.points.add    ( triangles[trianglesIndex].point0() );
            poly.points.add    ( triangles[trianglesIndex].point1() );
            poly.points.add    ( triangles[trianglesIndex].point2() );
            poly.normal        = triangles[trianglesIndex].normal();
            interiorPolygons.add( poly );
            trianglesProcessedFlag[trianglesIndex] = true;
            plUpdateInteriorPoints( triangles[trianglesIndex], interiorPoints);
            break;
          } // end if
        } // end for
      } // end if
    } // end for
  } // end for

  // convert polygons to triangles for the output
  plMath::concavePolysToTris(interiorPolygons,interiorTriangles);

} // end void function


static void plUpdateInteriorPoints( plTriangle &triangle , plSeq<plVector3> &interiorPoints ) {
  for (PLint vertexIndex=0; vertexIndex<3; vertexIndex++)
  {
    PLint interiorPointsIndex;
    for (interiorPointsIndex=0; interiorPointsIndex<interiorPoints.size(); interiorPointsIndex++)
    {
      if (triangle[vertexIndex] == interiorPoints[interiorPointsIndex])
        break;
    } // end for
    if (interiorPointsIndex == interiorPoints.size())
      interiorPoints.add( triangle[vertexIndex] );
  } // end for
}


static void plTriangleCutsBoundary( plTriangle &triangle, PLbool &triangleProcessed, plBoundary &boundary, plSeq<plPolygon> &interiorPolygons, plSeq<plVector3> &interiorPoints )
{
  plSeq<plCut> edgeCuts;

  for (PLint boundaryPointIndex=0; boundaryPointIndex<boundary.size(); boundaryPointIndex++) {

    // For each triangle edge, see if it crosses the line starting at boundary.points(index)

    plVector3 intersectionPoint;
    PLfloat  edgeParameter, boundaryParameter;
    PLint    intersectionDirection;

    if (triangle.normal() * boundary.normals(boundaryPointIndex) > 0) // only consider triangles on same side of mesh as boundary walls
    {
      for (PLint edgeIndex=0; edgeIndex<3; edgeIndex++)
      {
        if (plEdgeCutsBoundary( triangle[edgeIndex], triangle[(edgeIndex+1)%3], boundary, boundaryPointIndex, intersectionPoint, edgeParameter, boundaryParameter, intersectionDirection ))
          edgeCuts.add( plCut( intersectionPoint, edgeIndex, edgeParameter, boundaryPointIndex, boundaryParameter, intersectionDirection ) );
      } // end for
    } // end if
  } // end for

  // Build the polygons that remain inside the boundary
  PLint numCutsLeft = edgeCuts.size();

  if (numCutsLeft == 0)
    return;

  plSeq<plCut> boundaryCuts = edgeCuts;

  // Sort the cuts
  qsort( &edgeCuts[0],     edgeCuts.size(),     sizeof(plCut), plCompareEdgeCuts     ); // sort by increasing edge index, then by increasing parameter on each edge
  qsort( &boundaryCuts[0], boundaryCuts.size(), sizeof(plCut), plCompareBoundaryCuts ); // sort by increasing boundary index, then by increasing parameter on each boundary wall

  // Bookkeeping to know when to stop

  for (PLint i=0; i<edgeCuts.size(); i++)
    edgeCuts[i].processed = false;

  while (numCutsLeft > 0)
  {
    // Build one polygon
    plPolygon poly;

    poly.normal = triangle.normal();

    // Find an initial edge cut at which the triangle edge is going
    // outward through boundary wall.

    PLint edgeCutIndex;
    for (edgeCutIndex=0; edgeCutIndex<edgeCuts.size(); edgeCutIndex++)
    {
      if (!edgeCuts[ edgeCutIndex ].processed && edgeCuts[ edgeCutIndex ].direction == +1)
        break;
    } // end for

    if (edgeCutIndex == edgeCuts.size()) {
      std::cerr << "Error: Couldn't find an initial outgoing triangle edge cut." << std::endl;
      exit(1);
    }

    do
    {
      // Add this int point

      poly.points.add( edgeCuts[ edgeCutIndex ].point );
      edgeCuts[ edgeCutIndex ].processed = true;
      numCutsLeft--;

      // Find this intersection point in the list of boundary cuts.

      PLint boundaryCutIndex;
      for (boundaryCutIndex=0; boundaryCutIndex<boundaryCuts.size(); boundaryCutIndex++)
        if (boundaryCuts[ boundaryCutIndex ].point == edgeCuts[ edgeCutIndex ].point)
      break;

      if (boundaryCutIndex == boundaryCuts.size()) {
        std::cerr << "Error: An intersection point is missing from the boundaryCuts list." << std::endl;
        exit(1);
      }

      // Move along the boundary from this intersection point to the next intersection point.

      PLint thisBoundaryIndex = boundaryCuts[ boundaryCutIndex ].boundaryIndex;
      PLint nextBoundaryIndex = boundaryCuts[ (boundaryCutIndex+1)%boundaryCuts.size() ].boundaryIndex;

      while (thisBoundaryIndex != nextBoundaryIndex)
      {
        // We're not yet re-entering on the same boundary wall, so we need to
        // include some of the boundary vertices.
        thisBoundaryIndex = (thisBoundaryIndex+1) % boundary.size();

        poly.points.add( boundary.points(thisBoundaryIndex) );
      } // end while

      // Advance to the boundary cut at which the triangle edge re-enters the boundary.

      boundaryCutIndex = (boundaryCutIndex+1)%boundaryCuts.size();

      // Find this intersection point in the list of edge cuts.

      for (edgeCutIndex=0; edgeCutIndex<edgeCuts.size(); edgeCutIndex++)
      {
        if (edgeCuts[ edgeCutIndex ].point == boundaryCuts[ boundaryCutIndex ].point)
          break;
      } // end for

      // error checking
      if (edgeCutIndex == edgeCuts.size())
      {
        std::cerr << "Error: An intersection point is missing from the edgeCuts list." << std::endl;
        exit(1);
      }
      else if (edgeCuts[ edgeCutIndex ].direction != -1)
      {
        std::cerr << "Error: Expected to have an incoming triangle edge but an outgoing edge was found." << std::endl;
        exit(1);
      } // end if

      // Add this int point

      poly.points.add( edgeCuts[ edgeCutIndex ].point );
      edgeCuts[ edgeCutIndex ].processed = true;
      numCutsLeft--;

      // Move along the triangle edges from this intersection point to the next.

      PLint thisEdgeIndex = edgeCuts[ edgeCutIndex ].edgeIndex;
      PLint nextEdgeIndex = edgeCuts[ (edgeCutIndex+1)%edgeCuts.size() ].edgeIndex;

      while (thisEdgeIndex != nextEdgeIndex) {

        // We're not intersecting the next boundary wall on the same triangle
        // edge, so walk around the triangle edges.

        thisEdgeIndex = (thisEdgeIndex+1) % 3; // (3 edges per triangle)
        poly.points.add   ( triangle[ thisEdgeIndex ] );
        interiorPoints.add( triangle[ thisEdgeIndex ] );
      }

      // Advance to the edge cut at which the triangle edge exits the boundary wall.

      edgeCutIndex = (edgeCutIndex+1)%edgeCuts.size();
    } // end do
    while (edgeCuts[ edgeCutIndex ].point != poly.points[0]); // Stop if we've reached the starting point.

    interiorPolygons.add( poly );
  }

  triangleProcessed = true;
}



static PLbool plEdgeCutsBoundary( const plVector3 &v0, const plVector3 &v1, plBoundary &boundary, PLuint boundaryPointIndex, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &boundaryParam, PLint &intDir )

{
    plVector3 point0(boundary.points  (   boundaryPointIndex  )                     );
    plVector3 point1(boundary.points  ( ( boundaryPointIndex+1) % boundary.size() ) );
    plVector3 normal0(boundary.normals(   boundaryPointIndex  )                     );
    plVector3 normal1(boundary.normals( ( boundaryPointIndex+1) % boundary.size() ) );

    // outward pointing normal:
    plVector3 avgNormal     (  (normal0+normal1)         .normalize() );
    plVector3 outwardNormal ( ((point1-point0)^avgNormal).normalize() );

    //parameter for plane equation, n*x = d
    PLfloat d = point0 * outwardNormal;

    // Find the intersection point
    PLfloat dot0 = v0 * outwardNormal;
    PLfloat dot1 = v1 * outwardNormal;

    PLfloat denom = dot1 - dot0;

    if (fabs(denom) > 1e-6) {	// edge is not parallel to boundary wall

      PLfloat t = (d - dot0) / denom;

      if (0 <= t && t <= 1) {	// edge endpoints are on opposite sides of boundary wall

        // Project intersection point onto line in boundary between points

        plVector3 x = v0 + t * (v1-v0);

        PLfloat s = ((x-point0) * (point1-point0)) / ((point1-point0)*(point1-point0));

        if (0 <= s && s <= 1) {

          plVector3 y = point0 + s * (point1-point0);

          if ((x-y).length() < 0.5*(point1-point0).length()) {

            intPoint = x;
            intDir = (denom > 0 ? +1 : -1);
            edgeParam = t;
            boundaryParam = s;	// We're assuming that the projections are monotonically increasing
                                  // as we walk across the mesh from one boundary wall extreme to the other.
            return true;
          }
        }
      }
    }

  return false;
}


static PLint plCompareEdgeCuts( const void* a, const void* b )

{
  plCut &pa = * (plCut*) a;
  plCut &pb = * (plCut*) b;

  if (pa.edgeIndex < pb.edgeIndex)
    return -1;
  else if (pa.edgeIndex > pb.edgeIndex)
    return +1;
  else if (pa.edgeParam < pb.edgeParam)
    return -1;
  else if (pa.edgeParam > pb.edgeParam)
    return +1;
  else
    return 0;
}



static PLint plCompareBoundaryCuts( const void* a, const void* b )

{
  plCut &pa = * (plCut*) a;
  plCut &pb = * (plCut*) b;

  if (pa.boundaryIndex < pb.boundaryIndex)
    return -1;
  else if (pa.boundaryIndex > pb.boundaryIndex)
    return +1;
  else if (pa.boundaryParam < pb.boundaryParam)
    return -1;
  else if (pa.boundaryParam > pb.boundaryParam)
    return +1;
  else
    return 0;
}



