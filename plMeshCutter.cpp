#include "plMeshCutter.h"

// Determine the polygons of a triangle mesh that are inside a boundary.
//
// This is a bit brute force in that it compare each triangle edge to
// each wall of the boundary.  That could eventually be improved by
// finding a single intersection point, then walking along the
// boundary across the triangle mesh.
plCut::plCut( plVector3 pt, PLint ei, PLfloat ep, PLint bi, PLfloat bp, PLint dir )
    : point(pt), edgeIndex(ei), edgeParam(ep), boundaryIndex(bi), boundaryParam(bp), direction(dir)
{
}

namespace plMeshCutter
{

    void    _updateInteriorPoints  ( const plTriangle &triangle , std::vector<plVector3> &interiorPoints );
    PLbool  _edgeCutsBoundary      ( const plVector3 &edgeVert0, const plVector3 &edgeVert1, const plBoundary &wall, PLuint index, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &wallParam, PLint &intDir );
    PLbool  _triangleCutsBoundary  ( const plTriangle &tri, int &triProcessed, const plBoundary &walls, std::vector<plPolygon> &polys, std::vector<plVector3> &interiorPoints );
    PLint   _compareEdgeCuts       ( const void* a, const void* b );
    PLint   _compareBoundaryCuts   ( const void* a, const void* b );

    PLbool findInteriorMesh( std::vector<plTriangle> &interiorTriangles, const std::vector<plTriangle> &triangles, const plBoundary &boundary )
    {
        // set all the processed flags to false
        std::vector<int> trianglesProcessedFlag( triangles.size(), false );

        // just in case polygons has stuff in it, it should be emptied
        interiorTriangles.clear();

        // allocate a worst-case number of interior points
        std::vector<plVector3> interiorPoints;  interiorPoints.reserve( 3 * triangles.size() );

        // Collect polygons that intersect the boundary
        std::vector<plPolygon> interiorPolygons;
        for (PLuint i=0; i<triangles.size(); i++) 
        {
            if ( !_triangleCutsBoundary( triangles[i], trianglesProcessedFlag[i], boundary, interiorPolygons, interiorPoints ) )
            {
                return false;
            }
        }

        // Collect other polygons that contain an interior point.  This is very slow.
        for (PLuint interiorPointsIndex=0; interiorPointsIndex<interiorPoints.size(); interiorPointsIndex++)
        {
            for (PLuint trianglesIndex=0; trianglesIndex<triangles.size(); trianglesIndex++)
            {
                if ( !trianglesProcessedFlag[trianglesIndex] )
                {
                    for (PLuint triangleVerticesIndex=0; triangleVerticesIndex<3; triangleVerticesIndex++)
                    {
                        if (interiorPoints[interiorPointsIndex] == (triangles[trianglesIndex])[triangleVerticesIndex]) 
                        {
                            plPolygon poly;
                            poly.points.push_back( triangles[trianglesIndex].point0() );
                            poly.points.push_back( triangles[trianglesIndex].point1() );
                            poly.points.push_back( triangles[trianglesIndex].point2() );
                            poly.normal    = triangles[trianglesIndex].normal();
                            interiorPolygons.push_back( poly );
                            trianglesProcessedFlag[trianglesIndex] = 1;
                            _updateInteriorPoints( triangles[trianglesIndex], interiorPoints);
                            break;
                        } 
                    } 
                }
            }
        }

        // convert polygons to triangles for the output
        plMath::concavePolysToTris( interiorTriangles, interiorPolygons );
		
        return true;
    } 


    void _updateInteriorPoints( const plTriangle &triangle , std::vector<plVector3> &interiorPoints )
    {
        for (PLint vertexIndex=0; vertexIndex<3; vertexIndex++)
        {
            PLuint interiorPointsIndex;
            for (interiorPointsIndex=0; interiorPointsIndex<interiorPoints.size(); interiorPointsIndex++)
            {
                if (triangle[vertexIndex] == interiorPoints[interiorPointsIndex])
                    break;
            }
            if (interiorPointsIndex == interiorPoints.size())
            {
                interiorPoints.push_back( triangle[vertexIndex] );
            }
        }
    }


    PLbool _triangleCutsBoundary( const plTriangle &triangle, int &triangleProcessed, const plBoundary &boundary, std::vector<plPolygon> &interiorPolygons, std::vector<plVector3> &interiorPoints )
    {
        std::vector<plCut> edgeCuts;

        for (PLuint boundaryPointIndex=0; boundaryPointIndex<boundary.size(); boundaryPointIndex++) 
        {

            // For each triangle edge, see if it crosses the line starting at boundary.points(index)

            plVector3 intersectionPoint;
            PLfloat  edgeParameter, boundaryParameter;
            PLint    intersectionDirection;

            if (triangle.normal() * boundary.normals(boundaryPointIndex) > 0) // only consider triangles on same side of mesh as boundary walls
            {
                for (PLint edgeIndex=0; edgeIndex<3; edgeIndex++)
                {
                    if (_edgeCutsBoundary( triangle[edgeIndex], 
                                           triangle[(edgeIndex+1)%3], 
                                           boundary, 
                                           boundaryPointIndex, 
                                           intersectionPoint, 
                                           edgeParameter, 
                                           boundaryParameter, 
                                           intersectionDirection ))
                    {                      
                        edgeCuts.push_back( plCut( intersectionPoint, edgeIndex, edgeParameter, boundaryPointIndex, boundaryParameter, intersectionDirection ) );
                    }
                }
            } 
        }

        // Build the polygons that remain inside the boundary
        PLint numCutsLeft = edgeCuts.size();

        if (numCutsLeft == 0)
            return true;

        std::vector<plCut> boundaryCuts = edgeCuts;

        // Sort the cuts
        qsort( &edgeCuts[0],     edgeCuts.size(),     sizeof(plCut), _compareEdgeCuts     ); // sort by increasing edge index, then by increasing parameter on each edge
        qsort( &boundaryCuts[0], boundaryCuts.size(), sizeof(plCut), _compareBoundaryCuts ); // sort by increasing boundary index, then by increasing parameter on each boundary wall

        // Bookkeeping to know when to stop

        for (PLuint i=0; i<edgeCuts.size(); i++)
            edgeCuts[i].processed = false;

        while (numCutsLeft > 0)
        {
            // Build one polygon
            plPolygon poly;

            poly.normal = triangle.normal();

            // Find an initial edge cut at which the triangle edge is going
            // outward through boundary wall.

            PLuint edgeCutIndex;
            for (edgeCutIndex=0; edgeCutIndex<edgeCuts.size(); edgeCutIndex++)
            {
                if (!edgeCuts[ edgeCutIndex ].processed && edgeCuts[ edgeCutIndex ].direction == +1)
                    break;
            }

            if (edgeCutIndex == edgeCuts.size()) 
            {
                std::cerr << "Error: Couldn't find an initial outgoing triangle edge cut." << std::endl;
                return false;
            }

            do
            {
                // Add this int point

                poly.points.push_back( edgeCuts[ edgeCutIndex ].point );
                edgeCuts[ edgeCutIndex ].processed = true;
                numCutsLeft--;

                // Find this intersection point in the list of boundary cuts.

                PLuint boundaryCutIndex;
                for (boundaryCutIndex=0; boundaryCutIndex < boundaryCuts.size(); boundaryCutIndex++)
                    if (boundaryCuts[ boundaryCutIndex ].point == edgeCuts[ edgeCutIndex ].point)
                        break;

                if (boundaryCutIndex == boundaryCuts.size())
                {
                    std::cerr << "Error: An intersection point is missing from the boundaryCuts list." << std::endl;
                    return false;
                }

                // Move along the boundary from this intersection point to the next intersection point.

                PLint thisBoundaryIndex = boundaryCuts[ boundaryCutIndex ].boundaryIndex;
                PLint nextBoundaryIndex = boundaryCuts[ (boundaryCutIndex+1)%boundaryCuts.size() ].boundaryIndex;

                while (thisBoundaryIndex != nextBoundaryIndex)
                {
                    // We're not yet re-entering on the same boundary wall, so we need to
                    // include some of the boundary vertices.
                    thisBoundaryIndex = (thisBoundaryIndex+1) % boundary.size();

                    poly.points.push_back( boundary.points(thisBoundaryIndex) );
                } // end while

                // Advance to the boundary cut at which the triangle edge re-enters the boundary.

                boundaryCutIndex = (boundaryCutIndex+1)%boundaryCuts.size();

                // Find this intersection point in the list of edge cuts.

                for (edgeCutIndex=0; edgeCutIndex<edgeCuts.size(); edgeCutIndex++)
                {
                    if (edgeCuts[ edgeCutIndex ].point == boundaryCuts[ boundaryCutIndex ].point)
                        break;
                }

                // error checking
                if (edgeCutIndex == edgeCuts.size())
                {
                    std::cerr << "Error: An intersection point is missing from the edgeCuts list." << std::endl;
                    return false;
                }
                else if (edgeCuts[ edgeCutIndex ].direction != -1)
                {
                    std::cerr << "Error: Expected to have an incoming triangle edge but an outgoing edge was found." << std::endl;
                    return false;
                } 

                // Add this int point

                poly.points.push_back( edgeCuts[ edgeCutIndex ].point );
                edgeCuts[ edgeCutIndex ].processed = true;
                numCutsLeft--;
        
                // Move along the triangle edges from this intersection point to the next.

                PLint thisEdgeIndex = edgeCuts[ edgeCutIndex ].edgeIndex;
                PLint nextEdgeIndex = edgeCuts[ (edgeCutIndex+1)%edgeCuts.size() ].edgeIndex;

                while (thisEdgeIndex != nextEdgeIndex) 
                {

                    // We're not intersecting the next boundary wall on the same triangle
                    // edge, so walk around the triangle edges.

                    thisEdgeIndex = (thisEdgeIndex+1) % 3; // (3 edges per triangle)
                    poly.points.push_back   ( triangle[ thisEdgeIndex ] );
                    interiorPoints.push_back( triangle[ thisEdgeIndex ] );
                }

                // Advance to the edge cut at which the triangle edge exits the boundary wall.

                edgeCutIndex = (edgeCutIndex+1)%edgeCuts.size();
            } // end do
            while (edgeCuts[ edgeCutIndex ].point != poly.points[0]); // Stop if we've reached the starting point.

            interiorPolygons.push_back( poly );
        }

        triangleProcessed = 1;
        return true;
    }


    PLbool _edgeCutsBoundary( const plVector3 &edgeVert0, const plVector3 &edgeVert1, const plBoundary &boundary, PLuint boundaryPointIndex, plVector3 &intPoint, PLfloat &edgeParam, PLfloat &boundaryParam, PLint &intDir )
    {
        plVector3 point0 ( boundary.points (   boundaryPointIndex  )                     );
        plVector3 point1 ( boundary.points ( ( boundaryPointIndex+1) % boundary.size() ) );
        plVector3 normal0( boundary.normals(   boundaryPointIndex  )                     );
        plVector3 normal1( boundary.normals( ( boundaryPointIndex+1) % boundary.size() ) );

        plVector3 edgeVertex0 = edgeVert0;
        plVector3 edgeVertex1 = edgeVert1;

        // Order v0 and v1 lexicographically, so that different calls to
        // this function always compute the intersection in the same way,
        // resulting in exactly the same intersection point.
        bool reverseEdge = (edgeVertex0.x > edgeVertex1.x || (edgeVertex0.x == edgeVertex1.x &&
                                                             (edgeVertex0.y > edgeVertex1.y || (edgeVertex0.y == edgeVertex1.y && edgeVertex0.z > edgeVertex1.z) ) ) );
        if (reverseEdge) 
        {
            plVector3 temp = edgeVertex0;
            edgeVertex0 = edgeVertex1;
            edgeVertex1 = temp;
        }

        // outward pointing normal:
        plVector3 avgNormal     (  (normal0+normal1)         .normalize() );
        plVector3 outwardNormal ( ((point1-point0)^avgNormal).normalize() );

        //parameter for plane equation, n*x = d
        PLfloat d = point0 * outwardNormal;

        // Find the intersection point
        PLfloat dot0 = edgeVertex0 * outwardNormal;
        PLfloat dot1 = edgeVertex1 * outwardNormal;

        PLfloat denom = dot1 - dot0;

        if (fabs(denom) > 1e-6) 
        {	
            // edge is not parallel to boundary wall
            PLfloat t = (d - dot0) / denom;

            if (0 <= t && t <= 1) 
            {	
                // edge endpoints are on opposite sides of boundary wall

                // Project intersection point onto line in boundary between points
                plVector3 x = edgeVertex0 + t * (edgeVertex1-edgeVertex0);

                PLfloat s = ((x-point0) * (point1-point0)) / ((point1-point0)*(point1-point0));

                if (0 <= s && s <= 1) 
                {
                    plVector3 y = point0 + s * (point1-point0);

                    if ((x-y).length() < 0.5*(point1-point0).length()) 
                    {
                        intPoint = x;
                        intDir = (denom > 0 ? +1 : -1);
                        edgeParam = t;
                        boundaryParam = s;	// We're assuming that the projections are monotonically increasing
                                            // as we walk across the mesh from one boundary wall extreme to the other.
                        if (reverseEdge) 
                        {    
                            // undo the reverse (since edgeVertex0 & edgeVertex1 are passed by reference)
                            plVector3 temp = edgeVertex0;
                            edgeVertex0 = edgeVertex1;
                            edgeVertex1 = temp;

                            // since edge was reversed, fix up edgeParam and intDir
                            edgeParam = 1 - edgeParam;
                            intDir = -1 * intDir;
                        }

                        return true;
                    }
                }
            }
        }

        if (reverseEdge) 
        {    
            // undo the reverse (since edgeVertex0 & edgeVertex1 are passed by reference)
            plVector3 temp = edgeVertex0;
            edgeVertex0 = edgeVertex1;
            edgeVertex1 = temp;
        }

        return false;
    }


    PLint _compareEdgeCuts( const void* a, const void* b )
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


    PLint _compareBoundaryCuts( const void* a, const void* b )
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

}




