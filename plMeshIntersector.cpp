#include "plMeshIntersector.h"

namespace plMeshIntersector
{

    plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityData()
    {
    }

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
        if ((plMath::closestPointOnSegment(vert.vert,edge.pt1,edge.pt2) - vert.vert).length() > 0.f ) // if it's NOT on the line
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::intersectsEdge(const plMeshIntersectorConnectivityDataEdge& other, plVector3& intersection)
    {
        if (this->edge.contains(other.edge.pt1) || this->edge.contains(other.edge.pt2))
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
        if ((plMath::plClosestPointOnPlane(face.normal(),vert.vert,face.normal(),face.point0()) - vert.vert).length() > 0.f )
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
        if (face.contains(edge.edge.pt1) || face.contains(edge.edge.pt2))
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

    PLbool plMeshIntersectorConnectivityData::_splitEdgeOnVect( PLuint edgeABindex, const plVector3& vertex )
    {
        // split the current edge into two pieces

        // find all existing cells, have them available in case they're needed later
        plMeshIntersectorConnectivityDataEdge edgeAB = edges[edgeABindex];
        plSeq<PLuint> faceIndicesToSplit (edges[edgeABindex].faceIndices);

        PLuint vertAindex = edges[edgeABindex].vertIndices[0];
        PLuint vertBindex = edges[edgeABindex].vertIndices[1];
        plMeshIntersectorConnectivityDataVert& vertA = verts[vertAindex];
        plMeshIntersectorConnectivityDataVert& vertB = verts[vertBindex];

        // create the new cells, storing their eventual indices
        plMeshIntersectorConnectivityDataEdge edgeAN;
        plMeshIntersectorConnectivityDataEdge edgeNB;
        PLuint edgeANindex = edgeABindex;
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
        verts[vertNindex].edgeIndices.add(edgeANindex);
        verts[vertNindex].edgeIndices.add(edgeNBindex);

        edgeAN.edge = plEdge(verts[vertAindex].vert, verts[vertNindex].vert);
        edgeAN.vertIndices.add(vertAindex);
        edgeAN.vertIndices.add(vertNindex);

        edgeNB.edge = plEdge(verts[vertNindex].vert, verts[vertBindex].vert);
        edgeNB.vertIndices.add(vertNindex);
        edgeNB.vertIndices.add(vertBindex);

        // add to cell arrays (vertN is added before because we need a reference)
        edges[edgeABindex] = edgeAN;
        edges.add(edgeNB);

        // split any attached faces each into two pieces... one at a time
        for (PLuint i = 0; i < faceIndicesToSplit.size(); i++)
        {
            // find all existing cells, have them available in case they're needed later
            PLuint faceABCindex = faceIndicesToSplit[i];
            plMeshIntersectorConnectivityDataFace faceABC = faces[faceABCindex];

            PLuint vertCindex;
            for (PLuint j = 0; j < 3; j++)
            {
                if (faces[faceABCindex].vertIndices[j] != vertAindex && faces[faceABCindex].vertIndices[j] != vertBindex)
                {
                    vertCindex = faces[faceABCindex].vertIndices[j];
                    break;
                }
            }
            plMeshIntersectorConnectivityDataVert& vertC = verts[vertCindex];

            PLuint edgeACindex;
            PLuint edgeBCindex;
            for (PLuint j = 0; j < 3; j++)
            {
                PLuint currentEdgeIndex = faces[faceABCindex].edgeIndices[j];
                if (edges[currentEdgeIndex].edge == plEdge(verts[vertAindex].vert,verts[vertCindex].vert))
                    edgeACindex = currentEdgeIndex;
                else if (edges[currentEdgeIndex].edge == plEdge(verts[vertBindex].vert,verts[vertCindex].vert))
                    edgeBCindex = currentEdgeIndex;
            }
            plMeshIntersectorConnectivityDataEdge& edgeAC = edges[edgeACindex];
            plMeshIntersectorConnectivityDataEdge& edgeBC = edges[edgeBCindex];

            PLbool faceOrientationABC(false); // either ABC or CBA
            if ((faces[faceABCindex].face.point0() == verts[vertAindex].vert && faces[faceABCindex].face.point1() == verts[vertBindex].vert) ||
                (faces[faceABCindex].face.point1() == verts[vertAindex].vert && faces[faceABCindex].face.point2() == verts[vertBindex].vert) ||
                (faces[faceABCindex].face.point2() == verts[vertAindex].vert && faces[faceABCindex].face.point0() == verts[vertBindex].vert))
                faceOrientationABC = true; // otherwise false, as already been set

            // create the new cells, storing their eventual indices
            plMeshIntersectorConnectivityDataEdge edgeNC;
            PLuint edgeNCindex = edges.size();

            plMeshIntersectorConnectivityDataFace faceANC;
            plMeshIntersectorConnectivityDataFace faceBNC;
            PLuint faceANCindex = faceABCindex;
            PLuint faceBNCindex = faces.size();

            // now fill the cells with stuff!
            edgeNC.edge = plEdge(verts[vertNindex].vert,verts[vertCindex].vert);
            edgeNC.vertIndices.add(vertNindex);
            edgeNC.vertIndices.add(vertCindex);
            edgeNC.faceIndices.add(faceANCindex);
            edgeNC.faceIndices.add(faceBNCindex);

            if (faceOrientationABC)
            {
                faceANC.face = plTriangle(verts[vertAindex].vert,verts[vertNindex].vert,verts[vertCindex].vert);
                faceANC.vertIndices.add(vertAindex);
                faceANC.vertIndices.add(vertNindex);
                faceANC.vertIndices.add(vertCindex);
                faceBNC.face = plTriangle(verts[vertBindex].vert,verts[vertCindex].vert,verts[vertNindex].vert);
                faceBNC.vertIndices.add(vertBindex);
                faceBNC.vertIndices.add(vertCindex);
                faceBNC.vertIndices.add(vertNindex);
            }
            else
            {
                faceANC.face = plTriangle(verts[vertCindex].vert,verts[vertNindex].vert,verts[vertAindex].vert);
                faceANC.vertIndices.add(vertCindex);
                faceANC.vertIndices.add(vertNindex);
                faceANC.vertIndices.add(vertAindex);
                faceBNC.face = plTriangle(verts[vertNindex].vert,verts[vertCindex].vert,verts[vertBindex].vert);
                faceBNC.vertIndices.add(vertNindex);
                faceBNC.vertIndices.add(vertCindex);
                faceBNC.vertIndices.add(vertBindex);
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
            edges[edgeANindex].faceIndices.add(faceANCindex);
            edges[edgeNBindex].faceIndices.add(faceBNCindex);

            edges[edgeBCindex].faceIndices.remove(edges[edgeBCindex].faceIndices.findIndex(faceABCindex));
            edges[edgeBCindex].faceIndices.add(faceBNCindex); // don't need to do this for edgeAC, since ABC == ANC
            verts[vertBindex].faceIndices.remove(verts[vertBindex].faceIndices.findIndex(faceABCindex));
            verts[vertBindex].faceIndices.add(faceBNCindex);

            verts[vertNindex].faceIndices.add(faceANCindex);
            verts[vertNindex].faceIndices.add(faceBNCindex);
            verts[vertCindex].faceIndices.add(faceBNCindex); // already should have faceANCindex, since it was previously faceABCindex

            verts[vertNindex].edgeIndices.add(edgeNCindex);
            verts[vertCindex].edgeIndices.add(edgeNCindex);
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
        edgeN2.vertIndices.add(vertNindex);
        edgeN2.vertIndices.add(vert2index);
        edgeN2.faceIndices.add(face12Nindex);
        edgeN2.faceIndices.add(face20Nindex);

        // add the cells to our arrays (vertN is added before because we need a reference)
        faces[faceIndex] = face01N;
        faces.add(face12N);
        faces.add(face20N);

        edges.add(edgeN0);
        edges.add(edgeN1);
        edges.add(edgeN2);

        // update anything else that has been affected by what we just did
        verts[vert0index].faceIndices.add(face20Nindex); // keeping in mind faceIndex is already there, and is now face01Nindex
        verts[vert1index].faceIndices.add(face12Nindex); // see above comment
        verts[vert2index].faceIndices.remove(verts[vert2index].faceIndices.findIndex(faceIndex)); // face01N is no longer connected, so remove it.
        verts[vert2index].faceIndices.add(face20Nindex);
        verts[vert2index].faceIndices.add(face12Nindex);

        verts[vert0index].edgeIndices.add(edgeN0index);
        verts[vert1index].edgeIndices.add(edgeN1index);
        verts[vert2index].edgeIndices.add(edgeN2index);

        edges[edge12index].faceIndices.remove(edges[edge12index].faceIndices.findIndex(faceIndex));
        edges[edge12index].faceIndices.add(face12Nindex);
        edges[edge20index].faceIndices.remove(edges[edge20index].faceIndices.findIndex(faceIndex));
        edges[edge20index].faceIndices.add(face20Nindex); // edge01 doesn't need this because faceIndex == face01Nindex

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_checkArraySizes()
    {
        bool good(true);
        for (PLuint i = 0; i < edges.size(); i++)
        {
            if (edges[i].faceIndices.size() % 2 != 0 || edges[i].faceIndices.size() == 0)
            {
                std::cout << "Warning in plMeshIntersectorConnectivityData::_checkArraySizes(): faceIndices for edge " << i << " is of non positive even size " <<edges[i].faceIndices.size()  << ". Should be even." << std::endl;
                std::cout << edges[i] << std::endl;
                good = false;
            }
            if (edges[i].vertIndices.size() != 2)
            {
                std::cout << "Warning in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for edge " << i << " is of size " << edges[i].vertIndices.size() << ". Should be 2." << std::endl;
                good = false;
                std::cout << edges[i] << std::endl;
            }
        }
        for (PLuint i = 0; i < faces.size(); i++)
        {
            if (faces[i].vertIndices.size() != 3)
            {
                std::cout << "Warning in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for face " << i << " is of size " << faces[i].vertIndices.size() << ". Should be 3." << std::endl;
                good = false;
                std::cout << faces[i] << std::endl;
            }
            if (faces[i].edgeIndices.size() != 3)
            {
                std::cout << "Warning in plMeshIntersectorConnectivityData::_checkArraySizes(): edgeIndices for face " << i << " is of size " << faces[i].edgeIndices.size() << ". Should be 3." << std::endl;
                good = false;
                std::cout << faces[i] << std::endl;
            }
        }
        if (!good)
        {
            std::cout << "Summary of faulty model: " << verts.size() << " vertices, " << edges.size() << " edges, and " << faces.size() << " faces." << std::endl;
        }
        return good;
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
            PLbool vert0existed(true);
            if (vert0index == -1)
            {
                vert0index = verts.size();
                verts.add(vert0);
                vert0existed = false;
            }
            plMeshIntersectorConnectivityDataVert vert1;
            vert1.vert = currentTriangle[1];
            vert1index = verts.findIndex(vert1);
            PLbool vert1existed(true);
            if (vert1index == -1)
            {
                vert1index = verts.size();
                verts.add(vert1);
                vert1existed = false;
            }
            plMeshIntersectorConnectivityDataVert vert2;
            vert2.vert = currentTriangle[2];
            vert2index = verts.findIndex(vert2);
            PLbool vert2existed(true);
            if (vert2index == -1)
            {
                vert2index = verts.size();
                verts.add(vert2);
                vert2existed = false;
            }

            PLint edge01index, edge12index, edge20index;
            plMeshIntersectorConnectivityDataEdge edge01;
            edge01.edge = plEdge(vert0.vert,vert1.vert);
            edge01index = edges.findIndex(edge01);
            if (edge01index == -1)
            {
                edge01index = edges.size();
                edges.add(edge01);
                edges[edge01index].vertIndices.add((PLuint)vert0index);
                edges[edge01index].vertIndices.add((PLuint)vert1index);
            }
            plMeshIntersectorConnectivityDataEdge edge12;
            edge12.edge = plEdge(vert1.vert,vert2.vert);
            edge12index = edges.findIndex(edge12);
            if (edge12index == -1)
            {
                edge12index = edges.size();
                edges.add(edge12);
                edges[edge12index].vertIndices.add((PLuint)vert1index);
                edges[edge12index].vertIndices.add((PLuint)vert2index);
            }
            plMeshIntersectorConnectivityDataEdge edge20;
            edge20.edge = plEdge(vert2.vert,vert0.vert);
            edge20index = edges.findIndex(edge20);
            if (edge20index == -1)
            {
                edge20index = edges.size();
                edges.add(edge20);
                edges[edge20index].vertIndices.add((PLuint)vert2index);
                edges[edge20index].vertIndices.add((PLuint)vert0index);
            }

            plMeshIntersectorConnectivityDataFace face012;
            face012.face = currentTriangle;
            PLint face012index = faces.size();
            faces.add(face012);

            // this should only be done if the vertex did not exist. Didn't do earlier since we didn't know the edge indices yet
            if (!vert0existed)
            {
                verts[vert0index].edgeIndices.add((PLuint)edge01index);
                verts[vert0index].edgeIndices.add((PLuint)edge20index);
            }
            if (!vert1existed)
            {
                verts[vert1index].edgeIndices.add((PLuint)edge01index);
                verts[vert1index].edgeIndices.add((PLuint)edge12index);
            }
            if (!vert2existed)
            {
                verts[vert2index].edgeIndices.add((PLuint)edge12index);
                verts[vert2index].edgeIndices.add((PLuint)edge20index);
            }

            // now update connectivity information
            verts[vert0index].faceIndices.add((PLuint)face012index);
            verts[vert1index].faceIndices.add((PLuint)face012index);
            verts[vert2index].faceIndices.add((PLuint)face012index);
            edges[edge01index].faceIndices.add((PLuint)face012index);
            edges[edge12index].faceIndices.add((PLuint)face012index);
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
                    _checkArraySizes();
                    _splitEdgeOnVect(i,verts[j].vert);
                    _checkArraySizes();
                    i--;
                    break; // restart edge iteration, since it may need to be split again! you never know!
                }
            }
        }

        _checkArraySizes();
        // edge-edge intersections
        for (PLuint i = 0; i < edges.size(); i++)
        {
            for (PLuint j = i+1; j < edges.size(); j++)
            {
                if (edges[i].intersectsEdge(edges[j],intersectionPoint))
                {
                    _checkArraySizes();
                    _splitEdgeOnVect(i,intersectionPoint);
                    _checkArraySizes();
                    _splitEdgeOnVect(j,intersectionPoint);
                    _checkArraySizes();
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
                    _checkArraySizes();
                    _splitFaceOnVect(i,verts[j].vert);
                    _checkArraySizes();
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
                    _checkArraySizes();
                    _splitFaceOnVect(i,intersectionPoint);
                    _checkArraySizes();
                    _splitEdgeOnVect(j,intersectionPoint);
                    _checkArraySizes();
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

std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataVert &vert )
{
    stream << vert.vert << "\n";
    stream << "Edge Indices:";
    for (PLuint i = 0; i < vert.edgeIndices.size(); i++)
        stream << "\n" << vert.edgeIndices[i];
    stream << "\nFace Indices:";
    for (PLuint i = 0; i < vert.faceIndices.size(); i++)
        stream << "\n" << vert.faceIndices[i];
}

std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge &edge )
{
    stream << edge.edge.pt1 << " - " << edge.edge.pt2 << "\n";
    stream << "Vert Indices:";
    for (PLuint i = 0; i < edge.vertIndices.size(); i++)
        stream << "\n" << edge.vertIndices[i];
    stream << "\nFace Indices:";
    for (PLuint i = 0; i < edge.faceIndices.size(); i++)
        stream << "\n" << edge.faceIndices[i];
}

std::ostream& operator << ( std::ostream &stream, const plMeshIntersector::plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace &face )
{
    stream << face.face;
    stream << "Vert Indices:";
    for (PLuint i = 0; i < face.vertIndices.size(); i++)
        stream << "\n" << face.vertIndices[i];
    stream << "\nEdge Indices:";
    for (PLuint i = 0; i < face.edgeIndices.size(); i++)
        stream << "\n" << face.edgeIndices[i];
}
