#include "plMeshExtruder.h"

namespace plMeshExtruder
{

    // helper to collectOutsideEdges
    plSeq<plEdgeIndices> _collectOutsideEdgesUnsorted(const plSeq<plTriangle>& inputTriangles)
    {
        // all edges that are on the border will be unique -
        // that is to say, it will occur exactly once within the set of triangles.
        // any edge that occurs twice must be between two faces, and is therefore not on the border

        plSeq<plEdgeIndices> unsortedEdges;  // this stores the indices for bookkeeping and future manipulation
        for (PLuint triangleIndex = 0; triangleIndex < inputTriangles.size(); triangleIndex++)
        {
            // examine each edge
            for (PLuint vertexIndex = 0; vertexIndex < 3; vertexIndex++)
            {
                // vertex pair
                PLuint vertexAIndex = vertexIndex;
                PLuint vertexBIndex = (vertexIndex+1) % 3;

                // try to find the edge in question
                PLuint unsortedEdgeIndex;
                for (unsortedEdgeIndex = 0; unsortedEdgeIndex < unsortedEdges.size(); unsortedEdgeIndex++)
                {
                    // for readability - these are the indices to get to the vertices that we'll actually be looking at
                    PLuint searchTriangleIndex (unsortedEdges[unsortedEdgeIndex].triangleIndex);
                    PLuint searchVertexAIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexA);
                    PLuint searchVertexBIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexB);

                    // any edge that is being examined will be oriented opposite to what was already found
                    if (inputTriangles[searchTriangleIndex][searchVertexAIndex] == inputTriangles[triangleIndex][vertexBIndex] &&
                        inputTriangles[searchTriangleIndex][searchVertexBIndex] == inputTriangles[triangleIndex][vertexAIndex])
                    {
                        break;
                    }
                } // end for

                if (unsortedEdgeIndex == unsortedEdges.size()) // if the edge does not already exist, add it to the list
                {
                    unsortedEdges.add( plEdgeIndices(triangleIndex, vertexIndex, ((vertexIndex+1)%3) ) );
                }
                else // if the edge DOES exist, it must be between two faces and therefore can't be on the border... so remove it
                {
                    unsortedEdges.remove(unsortedEdgeIndex);
                } // end if
            } // end for
        } // end for
        return unsortedEdges;
    }

    // helper to collectOutsideEdges
    plSeq<plEdgeIndices> _collectOutsideEdgesSort( plSeq<plEdgeIndices> unsortedEdges, const plSeq<plTriangle>& inputTriangles)
    {
        // traverse along the edges and return to the first vertex
        plSeq<plEdgeIndices> sortedEdges;  // this stores the indices for bookkeeping and future manipulation

        plVector3 finalVertex   = inputTriangles[unsortedEdges[0].triangleIndex][unsortedEdges[0].vertexIndexA]; // the final vertex should be this, this should be checked
        plVector3 currentVertex = inputTriangles[unsortedEdges[0].triangleIndex][unsortedEdges[0].vertexIndexB]; // the current vertex, we'll look to 'hop' to another edge with this vertex

        sortedEdges.add(unsortedEdges[0]); // first edge, arbitrarily chosen to be here
        unsortedEdges.remove(0);           // don't need to search here, so remove it from the list of unsorted edges

        while ( unsortedEdges.size() )
        {
            // search for the current vertex in the list of edges. It will appear as the first vertex in an ordered pair, since all edges are counter-clockwise
            PLuint unsortedEdgeIndex;
            for (unsortedEdgeIndex = 0; unsortedEdgeIndex < unsortedEdges.size(); unsortedEdgeIndex++)
            {
                PLuint triangleIndex (unsortedEdges[unsortedEdgeIndex].triangleIndex);
                PLuint vertexAIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexA);
                if (inputTriangles[triangleIndex][vertexAIndex] == currentVertex)
                {
                    break;
                }
            } // end for

            if (unsortedEdgeIndex == unsortedEdges.size() || (unsortedEdges.size() == 0 && currentVertex != finalVertex)) // should not happen if the edges contain a closed surface
            {
                std::cerr << "Error in plIGuide::collectOutsideEdges: the edges don't contain a closed surface! Aborting." << std::endl;
                exit(1);
            }
            else
            {
                // update the current vertex
                PLuint triangleIndex (unsortedEdges[unsortedEdgeIndex].triangleIndex);
                PLuint vertexBIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexB);
                currentVertex = (inputTriangles[triangleIndex][vertexBIndex]);

                // move this entry into the sorted list
                sortedEdges.add(unsortedEdges[unsortedEdgeIndex]);
                unsortedEdges.remove(unsortedEdgeIndex);
            } // end if
        } // end while

        return sortedEdges;
    }


    plSeq<plEdgeIndices> _collectOutsideEdges(const plSeq<plTriangle>& inputTriangles)
    {
        plSeq<plEdgeIndices> edges;
        edges = _collectOutsideEdgesUnsorted( inputTriangles );
        edges = _collectOutsideEdgesSort( edges, inputTriangles );
        return edges;
    }


    plSeq<plTriangle> extrudeMesh(const plSeq<plTriangle>& inputTriangles, const PLfloat magnitude, const plVector3& direction)
    {
        plSeq<plTriangle>    outputTriangles; // create a copy of existing triangles, because they will essentially become part of the offset
        outputTriangles.add(inputTriangles);
        plSeq<plEdgeIndices> outsideEdges = _collectOutsideEdges(inputTriangles);

        // store these for bookkeeping. This is the index where upper (extruded) edges/triangles will start
        PLuint indexOfUpperEdges     = outsideEdges.size();
        PLuint indexOfUpperTriangles = outputTriangles.size();

        // we need to extrude a bunch of these triangles in the direction of the average normal
        plVector3 offset ( magnitude * direction.normalize() );
        for (PLuint i = 0; i < indexOfUpperTriangles; i++)
        {
            plVector3 point0( outputTriangles[i].point0() + offset );
            plVector3 point1( outputTriangles[i].point1() + offset );
            plVector3 point2( outputTriangles[i].point2() + offset );
            plVector3 normal( outputTriangles[i].normal() );
            outputTriangles.add( plTriangle(normal,point0,point1,point2) );
        }

        // update edgeIndices
        for (PLuint i = 0; i < indexOfUpperEdges; i++)
        {
            outsideEdges.add(outsideEdges[i]);
            outsideEdges[i+indexOfUpperEdges].triangleIndex = outsideEdges[i+indexOfUpperEdges].triangleIndex + indexOfUpperTriangles;
        }

        // we need to reverse the normals of the triangles that lie on top of the knee surface
        for (PLuint i = 0; i < indexOfUpperTriangles; i++)
        {
            outputTriangles[i].swapVertices0And1(); // reverses the normal
        }

        // update edgeIndices
        for (PLuint i = 0; i < indexOfUpperEdges; i++)
        {
            outsideEdges[i].vertexIndexA = (outsideEdges[i].vertexIndexA==2?2:!outsideEdges[i].vertexIndexA); // results in mapping: 2 => 2, 1 => 0, 0 => 1. Since vertices 0 and 1 were flipped.
            outsideEdges[i].vertexIndexB = (outsideEdges[i].vertexIndexB==2?2:!outsideEdges[i].vertexIndexB);
        }

        // now fill in the sides of the template with triangles
        for (PLuint i = 0; i < indexOfUpperEdges; i++)
        {
            PLuint lowerTriangleIndex (outsideEdges[i].triangleIndex);
            PLuint lowerVertexIndexA  (outsideEdges[i].vertexIndexA);
            PLuint lowerVertexIndexB  (outsideEdges[i].vertexIndexB);
            PLuint upperTriangleIndex (outsideEdges[i+indexOfUpperEdges].triangleIndex);
            PLuint upperVertexIndexA  (outsideEdges[i+indexOfUpperEdges].vertexIndexA);
            PLuint upperVertexIndexB  (outsideEdges[i+indexOfUpperEdges].vertexIndexB);
            outputTriangles.add(plTriangle(outputTriangles[lowerTriangleIndex][lowerVertexIndexA],
                                           outputTriangles[lowerTriangleIndex][lowerVertexIndexB],
                                           outputTriangles[upperTriangleIndex][upperVertexIndexA] ) );
            outputTriangles.add(plTriangle(outputTriangles[lowerTriangleIndex][lowerVertexIndexB],
                                           outputTriangles[upperTriangleIndex][upperVertexIndexB],
                                           outputTriangles[upperTriangleIndex][upperVertexIndexA] ) );
        }

        return outputTriangles;

    }

}
