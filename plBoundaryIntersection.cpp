// offset.cpp


#include "plBoundaryIntersection.h"


// Determine the polygons of a triangle mesh that are inside a boundary.
//
// This is a bit brute force in that it compare each triangle edge to
// each wall of the boundary.  That could eventually be improved by
// finding a single intersection point, then walking along the
// boundary across the triangle mesh.


void findInteriorMesh( plSeq<plTriangle> &triangles, plSeq<plWall> &walls, plSeq<plPolygon> &interiorPolygons )
{
  // set all the processed flags to false
  plSeq<PLbool> trianglesProcessedFlag (triangles.size());
  for (PLint i=0; i<triangles.size(); i++)
    trianglesProcessedFlag.add(false);

  // just in case polygons has stuff in it, it should be emptied
  interiorPolygons.clear();

  // allocate a worst-case number of interior points
  plSeq<plVector3> interiorPoints( 3 * triangles.size() );

  // Collect polygons that intersect the boundary
  for (PLint i=0; i<triangles.size(); i++) {
    triangleCutsBoundary( triangles[i], trianglesProcessedFlag[i], walls, interiorPolygons, interiorPoints );
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
            for (PLint l=0; l<3; l++)
            {
              PLint m;
              for (m=0; m<interiorPoints.size(); m++)
              {
                if (triangles[trianglesIndex][l] == interiorPoints[m])
                  break;
              } // end for
              if (m == interiorPoints.size())
                interiorPoints.add( triangles[trianglesIndex][l] );
            } // end for
            break;
          } // end if
        } // end for
      } // end if
    } // end for
  } // end for
} // end void function


void triangleCutsBoundary( plTriangle &tri, PLbool &triProcessed, plSeq<plWall> &walls, plSeq<plPolygon> &interiorPolygons, plSeq<plVector3> &interiorPoints )

{
  plSeq<plCut> edgeCuts;

  for (PLint wallIndex=0; wallIndex<walls.size(); wallIndex++) {

    // For each triangle edge, see if it crosses walls[i]

    plVector3 intersectionPoint;
    PLfloat  edgeParameter, wallParameter;
    PLint    intDir;

    if (tri.normal() * walls[wallIndex].n0 > 0) // only consider triangles on same side of mesh as walls
    {
      for (PLint edgeIndex=0; edgeIndex<3; edgeIndex++)
      {
        if (edgeCutsWall( tri[edgeIndex], tri[(edgeIndex+1)%3], walls[wallIndex], intersectionPoint, edgeParameter, wallParameter, intDir ))
          edgeCuts.add( plCut( intersectionPoint, edgeIndex, edgeParameter, wallIndex, wallParameter, intDir ) );
      } // end for
    } // end if
  } // end for







  // Build the polygons that remain inside the wall
  PLint numCutsLeft = edgeCuts.size();

  if (numCutsLeft == 0)
    return;

  plSeq<plCut> wallCuts = edgeCuts;

  // Sort the cuts
  qsort( &edgeCuts[0], edgeCuts.size(), sizeof(plCut), compareEdgeCuts ); // sort by increasing edge index, then by increasing parameter on each edge
  qsort( &wallCuts[0], wallCuts.size(), sizeof(plCut), compareWallCuts ); // sort by increasing wall index, then by increasing parameter on each wall

  // Bookkeeping to know when to stop

  for (PLint i=0; i<edgeCuts.size(); i++)
    edgeCuts[i].processed = false;

  while (numCutsLeft > 0)
  {
    // Build one polygon
    plPolygon poly;

    poly.normal = tri.normal();

    // Find an initial edge cut at which the triangle edge is going
    // outward through wall.

    PLint edgeCutIndex;
    for (edgeCutIndex=0; edgeCutIndex<edgeCuts.size(); edgeCutIndex++)
    {
      if (!edgeCuts[ edgeCutIndex ].processed && edgeCuts[ edgeCutIndex ].dir == +1)
        break;
    } // end for

    if (edgeCutIndex == edgeCuts.size()) {
      std::cerr << "Error: Couldn't find an initial outgoing triangle edge cut." << std::endl;
      exit(1);
    }

    do
    {
      // Add this int point

      poly.points.add( edgeCuts[ edgeCutIndex ].p );
      edgeCuts[ edgeCutIndex ].processed = true;
      numCutsLeft--;

      // Find this intersection point in the list of wall cuts.

      PLint wallCutIndex;
      for (wallCutIndex=0; wallCutIndex<wallCuts.size(); wallCutIndex++)
        if (wallCuts[ wallCutIndex ].p == edgeCuts[ edgeCutIndex ].p)
      break;

      if (wallCutIndex == wallCuts.size()) {
        std::cerr << "Error: An intersection point is missing from the wallCuts list." << std::endl;
        exit(1);
      }

      // Move along the walls from this intersection point to the next intersection point.

      PLint thisWallIndex = wallCuts[ wallCutIndex ].wallIndex;
      PLint nextWallIndex = wallCuts[ (wallCutIndex+1)%wallCuts.size() ].wallIndex;

      while (thisWallIndex != nextWallIndex)
      {
        // We're not yet re-entering on the same wall, so we need to
        // include some of the wall vertices.
        thisWallIndex = (thisWallIndex+1) % walls.size();

        poly.points.add( walls[ thisWallIndex ].p0 );
      } // end while

      // Advance to the wall cut at which the triangle edge re-enters the wall.

      wallCutIndex = (wallCutIndex+1)%wallCuts.size();

      // Find this intersection point in the list of edge cuts.

      for (edgeCutIndex=0; edgeCutIndex<edgeCuts.size(); edgeCutIndex++)
      {
        if (edgeCuts[ edgeCutIndex ].p == wallCuts[ wallCutIndex ].p)
          break;
      } // end for

      // error checking
      if (edgeCutIndex == edgeCuts.size())
      {
        std::cerr << "Error: An intersection point is missing from the edgeCuts list." << std::endl;
        exit(1);
      }
      else if (edgeCuts[ edgeCutIndex ].dir != -1)
      {
        std::cerr << "Error: Expected to have an incoming triangle edge but an outgoing edge was found." << std::endl;
        exit(1);
      } // end if

      // Add this int point

      poly.points.add( edgeCuts[ edgeCutIndex ].p );
      edgeCuts[ edgeCutIndex ].processed = true;
      numCutsLeft--;

      // Move along the triangle edges from this intersection point to the next.

      PLint thisEdgeIndex = edgeCuts[ edgeCutIndex ].edgeIndex;
      PLint nextEdgeIndex = edgeCuts[ (edgeCutIndex+1)%edgeCuts.size() ].edgeIndex;

      while (thisEdgeIndex != nextEdgeIndex) {

        // We're not intersecting the next wall on the same triangle
        // edge, so walk around the triangle edges.

        thisEdgeIndex = (thisEdgeIndex+1) % 3; // (3 edges per triangle)
        poly.points.add   ( tri[ thisEdgeIndex ] );
        interiorPoints.add( tri[ thisEdgeIndex ] );
      }

      // Advance to the edge cut at which the triangle edge exits the wall.

      edgeCutIndex = (edgeCutIndex+1)%edgeCuts.size();

    } // end do
    while (edgeCuts[ edgeCutIndex ].p != poly.points[0]); // Stop if we've reached the starting point.

    interiorPolygons.add( poly );
  }

  triProcessed = true;
}



PLbool edgeCutsWall( const plVector3 &v0, const plVector3 &v1, plWall &wall, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &wallParam, PLint &intDir )

{
    plVector3 avgNormal ((wall.n0+wall.n1).normalize());

    // outward pointing normal:
    plVector3 outwardNormal = ((wall.p1-wall.p0)^avgNormal).normalize();

    //parameter for plane equation, n*x = d

    PLfloat d = wall.p0 * outwardNormal;

    // Find the intersection point

    PLfloat dot0 = v0 * outwardNormal;
    PLfloat dot1 = v1 * outwardNormal;

    PLfloat denom = dot1 - dot0;

    if (fabs(denom) > 1e-6) {	// edge is not parallel to wall

      PLfloat t = (d - dot0) / denom;

      if (0 <= t && t <= 1) {	// edge endpoints are on opposite sides of wall

        // Project intersection point onto line in wall from wall.p0 to wall.p1

        plVector3 x = v0 + t * (v1-v0);

        PLfloat s = ((x-wall.p0) * (wall.p1-wall.p0)) / ((wall.p1-wall.p0)*(wall.p1-wall.p0));

        if (0 <= s && s <= 1) {

          plVector3 y = wall.p0 + s * (wall.p1-wall.p0);

          if ((x-y).length() < 0.5*(wall.p1-wall.p0).length()) {

            intPoint = x;
            intDir = (denom > 0 ? +1 : -1);
            edgeParam = t;
            wallParam = s;	// We're assuming that the projections are monotonically increasing
                                  // as we walk across the mesh from one wall extreme to the other.
            return true;
          }
        }
      }
    }

    /*plVector3 avgNormal ((wall.n0+wall.n1).normalize());

    // outward pointing normal:
    wall.n = ((wall.p1-wall.p0)^avgNormal).normalize();

    //parameter for plane equation, n*x = d
    wall.d = wall.p0 * wall.n;

  // Find the intersection point

  PLfloat dot0 = v0 * wall.n;
  PLfloat dot1 = v1 * wall.n;

  PLfloat denom = dot1 - dot0;

  if (fabs(denom) > 1e-6) {	// edge is not parallel to wall

    PLfloat t = (wall.d - dot0) / denom;

    if (0 <= t && t <= 1) {	// edge endpoints are on opposite sides of wall

      // Project intersection point onto line in wall from wall.p0 to wall.p1

      plVector3 x = v0 + t * (v1-v0);

      PLfloat s = ((x-wall.p0) * (wall.p1-wall.p0)) / ((wall.p1-wall.p0)*(wall.p1-wall.p0));

      if (0 <= s && s <= 1) {

        plVector3 y = wall.p0 + s * (wall.p1-wall.p0);

        if ((x-y).length() < 0.5*(wall.p1-wall.p0).length()) {

          intPoint = x;
          intDir = (denom > 0 ? +1 : -1);
          edgeParam = t;
          wallParam = s;	// We're assuming that the projections are monotonically increasing
                                // as we walk across the mesh from one wall extreme to the other.
          return true;
        }
      }
    }
  }*/

  return false;
}



PLint compareEdgeCuts( const void* a, const void* b )

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



PLint compareWallCuts( const void* a, const void* b )

{
  plCut &pa = * (plCut*) a;
  plCut &pb = * (plCut*) b;

  if (pa.wallIndex < pb.wallIndex)
    return -1;
  else if (pa.wallIndex > pb.wallIndex)
    return +1;
  else if (pa.wallParam < pb.wallParam)
    return -1;
  else if (pa.wallParam > pb.wallParam)
    return +1;
  else
    return 0;
}



