#include "plMeshIntersector.h"

namespace plMeshIntersector
{

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataVert::operator==(const plMeshIntersectorConnectivityDataVert &other)
    {
        if (vert == other.vert)
            return true;
        return false;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::operator ==(const plMeshIntersectorConnectivityDataEdge& other)
    {
        if (edge == other.edge)
            return true;
        return false;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::operator ==(const plMeshIntersectorConnectivityDataFace& other)
    {
        if (face == other.face)
            return true;
        return false;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::intersectsVert(const plMeshIntersectorConnectivityDataVert& vert)
    {
        if (edge.contains(vert.vert)) // if it is on the edge, this is not an intersection
            return false;

        // TODO: try the next one with an epsilon comparison, rather than exact
        if ((plMath::closestPointOnSegment(vert.vert,edge.pt1,edge.pt2) - vert.vert).length() > 0.f) // if it's NOT on the line
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::intersectsEdge(const plMeshIntersectorConnectivityDataEdge& other, plVector3& intersection)
    {
        if (*this == other)
            return false;

        plVector3 pointOnThis;
        plVector3 pointOnOther;
        PLfloat   distance;

        if (!plMath::closestPointsBetweenSegments(edge.pt1,edge.pt2,other.edge.pt1,other.edge.pt2,pointOnThis,pointOnOther,distance))
            return false;

        if (distance > 0.f)
            return false;
        intersection = pointOnThis;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::intersectsVert(const plMeshIntersectorConnectivityDataVert &vert)
    {
        // don't consider vertices that are a part of this triangle
        if (face.contains(vert.vert))
            return false;

        // first see if it's even in the plane
        if ((plMath::projectVectorOnPlane(vert.vert,face.normal()) - vert.vert).length() > 0.f)
            return false;

        // now see if the barycentric coordinates are in the right ranges
        plVector3 barycentricCoords = face.barycentricCoords(vert.vert);
        if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
            barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
            barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::intersectsEdge(const plMeshIntersectorConnectivityDataEdge& edge, plVector3& intersection)
    {
        // don't consider edges that are a part of this triangle
        if (face.contains(edge.edge.pt1) && face.contains(edge.edge.pt2))
            return false;

        // first find the line intersection with the plane
        plSeq<plTriangle> triSeq;
        triSeq.add(face);
        plIntersection intersectionData = plMath::rayIntersect(triSeq, edge.edge.pt1, (edge.edge.pt2-edge.edge.pt1), false, false);
        if (!intersectionData.exists)
            return false;
        if (intersectionData.t < 0.f || intersectionData.t > 1.f)
            return false;

        // now see if the barycentric coordinates are in the right ranges
        intersection = intersectionData.point;
        plVector3 barycentricCoords = face.barycentricCoords(intersection);
        if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
            barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
            barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_splitEdgeOnVect( PLuint edgeIndex, const plVector3& vertex )
    {
        // find all existing cells, have them available in case they're needed later
        // split the current edge into two pieces

        // find all existing cells, have them available in case they're needed later
        plMeshIntersectorConnectivityDataEdge edgeAB = edges[edgeIndex];

        PLuint vertAindex = edges[edgeIndex].vertIndices[0];
        PLuint vertBindex = edges[edgeIndex].vertIndices[1];
        plMeshIntersectorConnectivityDataVert& vertA = verts[vertAindex];
        plMeshIntersectorConnectivityDataVert& vertB = verts[vertBindex];

        // create the new cells, storing their eventual indices
        plMeshIntersectorConnectivityDataEdge edgeAN;
        plMeshIntersectorConnectivityDataEdge edgeNB;
        PLuint edgeANindex = edgeIndex;
        PLuint edgeNBindex = edges.size();

        plMeshIntersectorConnectivityDataVert vertNsearch; vertNsearch.vert = vertex;
        PLint  vertNsearchIndex = verts.findIndex(vertNsearch);
        PLuint vertNindex; // we need to determine this
        if (vertNsearchIndex == -1)
        {
            vertNindex = verts.size();
            verts.add(vertNsearch);
        }
        else
            vertNindex = (PLuint)vertNsearchIndex;
        plMeshIntersectorConnectivityDataVert& vertN = verts[vertNindex];

        // fill the cells with data, but faces will be treated separately.
        vertN.edgeIndices.add(edgeANindex);
        vertN.edgeIndices.add(edgeNBindex);

        edgeAN.edge = plEdge(vertA.vert, vertN.vert);
        edgeAN.vertIndices.add(vertAindex);
        edgeAN.vertIndices.add(vertNindex);

        edgeNB.edge = plEdge(vertN.vert, vertB.vert);
        edgeNB.vertIndices.add(vertNindex);
        edgeNB.vertIndices.add(vertBindex);

        // add to cell arrays (vertN is added before because we need a reference)
        edges[edgeIndex] = edgeAN;
        edges.add(edgeNB);

        // split any attached faces each into two pieces... one at a time
        plSeq<plMeshIntersectorConnectivityDataFace> facesToSplit; // can be any multiple of 2 in size
        for (PLuint i = 0; i < edgeAB.faceIndices.size(); i++)
        {
            // find all existing cells, have them available in case they're needed later
            PLuint faceABCindex = edgeAB.faceIndices[i];
            plMeshIntersectorConnectivityDataFace faceABC = faces[faceABCindex];

            PLuint vertCindex;
            for (PLuint j = 0; j < 3; j++)
            {
                if (faceABC.vertIndices[j] != vertAindex && faceABC.vertIndices[j] != vertBindex)
                {
                    vertCindex = faceABC.vertIndices[j];
                    break;
                }
            }
            plMeshIntersectorConnectivityDataVert& vertC = verts[vertCindex];

            PLuint edgeACindex;
            PLuint edgeBCindex;
            for (PLuint j = 0; j < 3; j++)
            {
                PLuint currentEdgeIndex = faceABC.edgeIndices[j];
                if (edges[currentEdgeIndex].edge == plEdge(vertA.vert,vertC.vert))
                    edgeACindex = currentEdgeIndex;
                else if (edges[currentEdgeIndex].edge == plEdge(vertB.vert,vertC.vert))
                    edgeBCindex = currentEdgeIndex;
            }
            plMeshIntersectorConnectivityDataEdge& edgeAC = edges[edgeACindex];
            plMeshIntersectorConnectivityDataEdge& edgeBC = edges[edgeBCindex];

            PLbool faceOrientationABC(false); // either ABC or CBA
            if ((faceABC.face.point0() == vertA.vert && faceABC.face.point1() == vertB.vert) ||
                (faceABC.face.point1() == vertA.vert && faceABC.face.point2() == vertB.vert) ||
                (faceABC.face.point2() == vertA.vert && faceABC.face.point0() == vertB.vert))
                faceOrientationABC = true; // otherwise false, as already been set

            // create the new cells, storing their eventual indices
            plMeshIntersectorConnectivityDataEdge edgeNC;
            PLuint edgeNCindex = edges.size();

            plMeshIntersectorConnectivityDataFace faceANC;
            plMeshIntersectorConnectivityDataFace faceBNC;
            PLuint faceANCindex = faceABCindex;
            PLuint faceBNCindex = faces.size();

            // now fill the cells with stuff!
            edgeNC.edge = plEdge(vertN.vert,vertC.vert);
            edgeNC.vertIndices.add(vertNindex);
            edgeNC.vertIndices.add(vertCindex);
            edgeNC.faceIndices.add(faceANCindex);
            edgeNC.faceIndices.add(faceBNCindex);

            if (faceOrientationABC)
            {
                faceANC.face = plTriangle(vertA.vert,vertN.vert,vertC.vert);
                faceANC.vertIndices.add(vertAindex);
                faceANC.vertIndices.add(vertNindex);
                faceANC.vertIndices.add(vertCindex);
            }
            else
            {
                faceBNC.face = plTriangle(vertB.vert,vertC.vert,vertN.vert);
                faceBNC.vertIndices.add(vertBindex);
                faceBNC.vertIndices.add(vertCindex);
                faceBNC.vertIndices.add(vertNindex);
            }
            faceANC.edgeIndices.add(edgeANindex);
            faceANC.edgeIndices.add(edgeNCindex);
            faceANC.edgeIndices.add(edgeACindex);
            faceBNC.edgeIndices.add(edgeNBindex);
            faceBNC.edgeIndices.add(edgeNCindex);
            faceBNC.edgeIndices.add(edgeBCindex);

            // add to arrays
            faces[faceABCindex] = faceANC;
            faces.add(faceBNC);
            edges.add(edgeNC);

            // update anything else that has been affected by what we just did
            edgeAN.faceIndices.add(faceANCindex);
            edgeNB.faceIndices.add(faceBNCindex);

            edgeBC.faceIndices.remove(edgeBC.faceIndices.findIndex(faceABCindex));
            edgeBC.faceIndices.add(faceBNCindex); // don't need to do this for edgeAC, since ABC == ANC
            vertB.faceIndices.remove(vertB.faceIndices.findIndex(faceABCindex));
            vertB.faceIndices.add(faceBNCindex);

            vertN.faceIndices.add(faceANCindex);
            vertN.faceIndices.add(faceBNCindex);
            vertC.faceIndices.add(faceBNCindex); // already should have faceANCindex, since it was previously faceABCindex

            vertN.edgeIndices.add(edgeNCindex);
            vertC.edgeIndices.add(edgeNCindex);
        }

        return true;
    }

    // add a vertex somewhere in the middle of the triangle, then divide the triangle into three smaller triangles.
    PLbool plMeshIntersectorConnectivityData::_splitFaceOnVect( PLuint faceIndex, const plVector3& vertex )
    {
        // find all existing cells, have them available in case they're needed later
        plMeshIntersectorConnectivityDataFace face = faces[faceIndex]; // this will eventually be removed from the list of faces

        PLuint vert0index;
        PLuint vert1index;
        PLuint vert2index;
        for (PLuint i = 0; i < face.vertIndices.size(); i++) // TODO: Ask myself if this is really needed...
        {
            if (verts[face.vertIndices[i]].vert == face.face.point0())
                vert0index = face.vertIndices[i];
            if (verts[face.vertIndices[i]].vert == face.face.point1())
                vert1index = face.vertIndices[i];
            if (verts[face.vertIndices[i]].vert == face.face.point2())
                vert2index = face.vertIndices[i];
        }
        plMeshIntersectorConnectivityDataVert& vert0 = verts[vert0index];
        plMeshIntersectorConnectivityDataVert& vert1 = verts[vert1index];
        plMeshIntersectorConnectivityDataVert& vert2 = verts[vert2index];

        PLuint edge01index;
        PLuint edge12index;
        PLuint edge20index;
        for (PLuint i = 0; i < face.edgeIndices.size(); i++)
        {
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point0()) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point1()) )
                edge01index = face.edgeIndices[i];
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point1()) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point2()) )
                edge12index = face.edgeIndices[i];
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point2()) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point0()) )
                edge20index = face.edgeIndices[i];
        }
        plMeshIntersectorConnectivityDataEdge& edge01 = edges[edge01index];
        plMeshIntersectorConnectivityDataEdge& edge12 = edges[edge12index];
        plMeshIntersectorConnectivityDataEdge& edge20 = edges[edge20index];

        // create the new cells, storing their eventual indices
        plMeshIntersectorConnectivityDataVert vertNsearch; vertNsearch.vert = vertex;
        PLint  vertNsearchIndex = verts.findIndex(vertNsearch);
        PLuint vertNindex; // we need to determine this
        if (vertNsearchIndex == -1)
        {
            vertNindex = verts.size();
            verts.add(vertNsearch);
        }
        else
            vertNindex = (PLuint)vertNsearchIndex;
        plMeshIntersectorConnectivityDataVert& vertN = verts[vertNindex];

        plMeshIntersectorConnectivityDataEdge edgeN0;
        plMeshIntersectorConnectivityDataEdge edgeN1;
        plMeshIntersectorConnectivityDataEdge edgeN2;
        PLuint edgeN0index(edges.size()  );
        PLuint edgeN1index(edges.size()+1);
        PLuint edgeN2index(edges.size()+2);

        plMeshIntersectorConnectivityDataFace face01N;
        plMeshIntersectorConnectivityDataFace face12N;
        plMeshIntersectorConnectivityDataFace face20N;
        PLuint face01Nindex(faceIndex);
        PLuint face12Nindex(faces.size());
        PLuint face20Nindex(faces.size()+1);

        // fill the cells with data
        vertN.faceIndices.add(face01Nindex);
        vertN.faceIndices.add(face12Nindex);
        vertN.faceIndices.add(face20Nindex);
        vertN.edgeIndices.add(edgeN0index);
        vertN.edgeIndices.add(edgeN1index);
        vertN.edgeIndices.add(edgeN2index);

        face01N.face = plTriangle(face.face.point0(),face.face.point1(),vertex); // this one will replace faces[faceIndex]
        face01N.edgeIndices.add(edge01index);
        face01N.edgeIndices.add(edgeN1index);
        face01N.edgeIndices.add(edgeN0index);
        face01N.vertIndices.add(vert0index);
        face01N.vertIndices.add(vert1index);
        face01N.vertIndices.add(vertNindex);

        face12N.face = plTriangle(face.face.point1(),face.face.point2(),vertex); // this one will become  faces[size]
        face12N.edgeIndices.add(edge12index);
        face12N.edgeIndices.add(edgeN2index);
        face12N.edgeIndices.add(edgeN1index);
        face12N.vertIndices.add(vert1index);
        face12N.vertIndices.add(vert2index);
        face12N.vertIndices.add(vertNindex);

        face20N.face = plTriangle(face.face.point2(),face.face.point0(),vertex); // this one will become  faces[size+1]
        face20N.edgeIndices.add(edge20index);
        face20N.edgeIndices.add(edgeN0index);
        face20N.edgeIndices.add(edgeN2index);
        face20N.vertIndices.add(vert2index);
        face20N.vertIndices.add(vert0index);
        face20N.vertIndices.add(vertNindex);

        edgeN0.edge = plEdge(face.face.point0(),vertex);
        edgeN0.vertIndices.add(vertNindex);
        edgeN0.vertIndices.add(vert0index);
        edgeN0.faceIndices.add(face20Nindex);
        edgeN0.faceIndices.add(face01Nindex);

        edgeN1.edge = plEdge(face.face.point1(),vertex);
        edgeN1.vertIndices.add(vertNindex);
        edgeN1.vertIndices.add(vert1index);
        edgeN1.faceIndices.add(face01Nindex);
        edgeN1.faceIndices.add(face12Nindex);

        edgeN2.edge = plEdge(face.face.point2(),vertex);
        edgeN1.vertIndices.add(vertNindex);
        edgeN1.vertIndices.add(vert2index);
        edgeN1.faceIndices.add(face12Nindex);
        edgeN1.faceIndices.add(face20Nindex);

        // add the cells to our arrays (vertN is added before because we need a reference)
        faces[faceIndex] = face01N;
        faces.add(face12N);
        faces.add(face20N);

        edges.add(edgeN0);
        edges.add(edgeN1);
        edges.add(edgeN2);

        // update anything else that has been affected by what we just did
        vert0.faceIndices.add(face20Nindex); // keeping in mind faceIndex is already there, and is now face01Nindex
        vert1.faceIndices.add(face12Nindex); // see above comment
        vert2.faceIndices.remove(vert2.faceIndices.findIndex(faceIndex)); // face01N is no longer connected, so remove it.
        vert2.faceIndices.add(face20Nindex);
        vert2.faceIndices.add(face12Nindex);

        vert0.edgeIndices.add(edgeN0index);
        vert1.edgeIndices.add(edgeN1index);
        vert2.edgeIndices.add(edgeN2index);

        edge12.faceIndices.remove(edge12.faceIndices.findIndex(faceIndex));
        edge12.faceIndices.add(face12Nindex);
        edge20.faceIndices.remove(edge20.faceIndices.findIndex(faceIndex));
        edge20.faceIndices.add(face20Nindex); // edge01 doesn't need this because faceIndex == face01Nindex

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_importTriSeq(const plSeq<plTriangle> &tris)
    {
        verts.clear();
        edges.clear();
        faces.clear();
        for (PLuint i = 0; i < tris.size(); i++)
        {
            // add cells as necessary
            plTriangle& currentTriangle = tris[i];

            PLint vert0index,vert1index,vert2index;
            plMeshIntersectorConnectivityDataVert vert0;
            vert0.vert = currentTriangle[0];
            vert0index = verts.findIndex(vert0);
            if (vert0index == -1)
            {
                vert0index = verts.size();
                verts.add(vert0);
            }
            plMeshIntersectorConnectivityDataVert vert1;
            vert1.vert = currentTriangle[1];
            vert1index = verts.findIndex(vert1);
            if (vert1index == -1)
            {
                vert1index = verts.size();
                verts.add(vert1);
            }
            plMeshIntersectorConnectivityDataVert vert2;
            vert2.vert = currentTriangle[2];
            vert2index = verts.findIndex(vert2);
            if (vert2index == -1)
            {
                vert2index = verts.size();
                verts.add(vert2);
            }

            PLint edge01index, edge12index, edge20index;
            plMeshIntersectorConnectivityDataEdge edge01;
            edge01.edge = plEdge(vert0.vert,vert1.vert);
            edge01index = edges.findIndex(edge01);
            if (edge01index == -1)
            {
                edge01index = edges.size();
                edges.add(edge01);
            }
            plMeshIntersectorConnectivityDataEdge edge12;
            edge12.edge = plEdge(vert0.vert,vert1.vert);
            edge12index = edges.findIndex(edge12);
            if (edge12index == -1)
            {
                edge12index = edges.size();
                edges.add(edge12);
            }
            plMeshIntersectorConnectivityDataEdge edge20;
            edge20.edge = plEdge(vert0.vert,vert1.vert);
            edge20index = edges.findIndex(edge20);
            if (edge20index == -1)
            {
                edge20index = edges.size();
                edges.add(edge20);
            }

            plMeshIntersectorConnectivityDataFace face012;
            face012.face = currentTriangle;
            PLint face012index = faces.size();
            faces.add(face012);

            // now update connectivity information
            verts[vert0index].edgeIndices.add((PLuint)edge01index);
            verts[vert0index].edgeIndices.add((PLuint)edge20index);
            verts[vert0index].faceIndices.add((PLuint)face012index);
            verts[vert1index].edgeIndices.add((PLuint)edge01index);
            verts[vert1index].edgeIndices.add((PLuint)edge12index);
            verts[vert1index].faceIndices.add((PLuint)face012index);
            verts[vert2index].edgeIndices.add((PLuint)edge12index);
            verts[vert2index].edgeIndices.add((PLuint)edge20index);
            verts[vert2index].faceIndices.add((PLuint)face012index);
            edges[edge01index].vertIndices.add((PLuint)vert0index);
            edges[edge01index].vertIndices.add((PLuint)vert1index);
            edges[edge01index].faceIndices.add((PLuint)face012index);
            edges[edge12index].vertIndices.add((PLuint)vert1index);
            edges[edge12index].vertIndices.add((PLuint)vert2index);
            edges[edge12index].faceIndices.add((PLuint)face012index);
            edges[edge20index].vertIndices.add((PLuint)vert2index);
            edges[edge20index].vertIndices.add((PLuint)vert0index);
            edges[edge20index].faceIndices.add((PLuint)face012index);
            faces[face012index].vertIndices.add((PLuint)vert0index);
            faces[face012index].vertIndices.add((PLuint)vert1index);
            faces[face012index].vertIndices.add((PLuint)vert2index);
            faces[face012index].edgeIndices.add((PLuint)edge01index);
            faces[face012index].edgeIndices.add((PLuint)edge12index);
            faces[face012index].edgeIndices.add((PLuint)edge20index);
        }
        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_exportTriSeq(plSeq<plTriangle> &tris)
    {
        if (tris.size() != 0)
        {
            std::cout << "Warning in plMeshIntersectorConnectivityData::exportTriSeq(): tris array provided already contains data. Clearing contents." << std::endl;
        }
        tris.clear();
        for (PLuint i = 0; i < faces.size(); i++)
        {
            tris.add(faces[i].face);
        }
        return true;
    }

    PLbool plMeshIntersectorConnectivityData::intersect(const plSeq<plTriangle> &inputTris, plSeq<plTriangle> &outputTris)
    {
        _importTriSeq(inputTris);

        // intersectionPoint is going to be reused as necessary
        plVector3 intersectionPoint;

        // vertex-edge intersections
        for (PLuint i = 0; i < edges.size(); i++)
        {
            for (PLuint j = 0; j < verts.size(); j++)
            {
                if (edges[i].intersectsVert(verts[j]))
                {
                    _splitEdgeOnVect(i,verts[j].vert);
                    i--;
                    break; // restart edge iteration, since it may need to be split again! you never know!
                }
            }
        }

        // edge-edge intersections
        for (PLuint i = 0; i < edges.size(); i++)
        {
            for (PLuint j = i+1; j < edges.size(); j++)
            {
                if (edges[i].intersectsEdge(edges[j],intersectionPoint))
                {
                    _splitEdgeOnVect(i,intersectionPoint);
                    i--;
                    break; // restart edge iteration, since it may need to be split again! you never know!
                }
            }
        }

        // vertex-face intersections
        for (PLuint i = 0; i < faces.size(); i++)
        {
            for (PLuint j = 0; j < verts.size(); j++)
            {
                if (faces[i].intersectsVert(verts[j]))
                {
                    _splitFaceOnVect(i,verts[j].vert);
                    i--;
                    break;
                }
            }
        }

        // edge-face intersections
        for (PLuint i = 0; i < faces.size(); i++)
        {
            for (PLuint j = 0; j < edges.size(); j++)
            {
                if (faces[i].intersectsEdge(edges[j],intersectionPoint))
                {
                    _splitFaceOnVect(i,intersectionPoint);
                    i--;
                    break;
                }
            }
        }

        _exportTriSeq(outputTris);

        return true;
    }

    PLbool intersect(const plSeq<plTriangle> &input, plSeq<plTriangle> &output)
    {
        plMeshIntersectorConnectivityData intersector;
        intersector.intersect(input,output);
    }

}
