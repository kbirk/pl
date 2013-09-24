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
        if (edge.contains(vert.vert, PL_EPSILON)) // if it is on the edge, this is not an intersection
            return false;

        if ((plMath::closestPointOnSegment(vert.vert,edge.pt1,edge.pt2) - vert.vert).length() > PL_EPSILON ) // if it's NOT on the line
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::intersectsEdge(const plMeshIntersectorConnectivityDataEdge& other, plVector3& intersection)
    {
        if (this->edge.contains(other.edge.pt1, PL_EPSILON) || this->edge.contains(other.edge.pt2, PL_EPSILON))
            return false;

        plVector3 pointOnThis;
        plVector3 pointOnOther;
        PLfloat   distance;

        if (!plMath::closestPointsBetweenSegments(edge.pt1,edge.pt2,other.edge.pt1,other.edge.pt2,pointOnThis,pointOnOther,distance))
            return false;

        if (distance >  PL_EPSILON)
            return false;
        intersection = pointOnThis;

        // check just to make sure the intersection isn't in fact already a vertex...
        std::cout << intersection << std::endl;
        std::cout << edge << std::endl;
        std::cout << other.edge << std::endl;
        std::cout << "---------------\n";
        if (edge.contains(intersection, PL_EPSILON) || other.edge.contains(intersection, PL_EPSILON))
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::intersectsVert(const plMeshIntersectorConnectivityDataVert &vert)
    {
        // don't consider vertices that are a part of this triangle
        if (face.contains(vert.vert, PL_EPSILON))
            return false;

        // first see if it's even in the plane
        if ((plMath::closestPointOnPlane(face.normal(),vert.vert,face.normal(),face.point0()) - vert.vert).length() >  PL_EPSILON )
            return false;

        // now see if the barycentric coordinates are in the right ranges
        plVector3 barycentricCoords = face.barycentricCoords(vert.vert);
        if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
            barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
            barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
            return false;

        // on the edge, don't consider as an intersection
        if ((plMath::closestPointOnSegment(vert.vert,face.point0(),face.point1()) - vert.vert).length() <= PL_EPSILON ||
            (plMath::closestPointOnSegment(vert.vert,face.point1(),face.point2()) - vert.vert).length() <= PL_EPSILON ||
            (plMath::closestPointOnSegment(vert.vert,face.point2(),face.point0()) - vert.vert).length() <= PL_EPSILON )
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::intersectsEdge(const plMeshIntersectorConnectivityDataEdge& edge, plVector3& intersection)
    {
        // don't consider edges that are a part of this triangle
        if (face.contains(edge.edge.pt1, PL_EPSILON) || face.contains(edge.edge.pt2, PL_EPSILON))
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

        // on the edge, don't consider as an intersection
        if ((plMath::closestPointOnSegment(intersection,face.point0(),face.point1()) - intersection).length() <= PL_EPSILON ||
            (plMath::closestPointOnSegment(intersection,face.point1(),face.point2()) - intersection).length() <= PL_EPSILON ||
            (plMath::closestPointOnSegment(intersection,face.point2(),face.point0()) - intersection).length() <= PL_EPSILON )
            return false;

        // if on one of the face edges, don't consider as an intersection
        if (edge.edge.contains(intersection,PL_EPSILON))
            return false;

        // if on one of the face vertices, don't consider as an intersection
        if (face.contains(intersection,PL_EPSILON))
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_findVert( const plVector3& vertex, PLint &index )
    {
        for (PLuint i = 0; i < verts.size(); i++)
        {
            if ((verts[i].vert - vertex).length() <= PL_EPSILON)
            {
                index = i;
                return true;
            }
        }
        index = -1;
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
        PLint  vertNsearchIndex;
        _findVert(vertex, vertNsearchIndex);
        PLuint vertNindex; // we need to determine this
        if ((PLuint)vertNsearchIndex == vertAindex)
        {
            std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): N vertex is A vertex. This is possibly due to epsilon being too large. Aborting this particular edge split, but beware of future errors." << std::endl;
            return false;
        }
        if ((PLuint)vertNsearchIndex == vertBindex)
        {
            std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): N vertex is B vertex. This is possibly due to epsilon being too large. Aborting this particular edge split, but beware of future errors." << std::endl;
            return false;
        }
        if (vertNsearchIndex == -1)
        {
            vertNindex = verts.size();
            verts.add(vertNsearch);
        }
        else
            vertNindex = (PLuint)vertNsearchIndex;
        plMeshIntersectorConnectivityDataVert& vertN = verts[vertNindex];

        // fill the cells with data, but faces will be treated separately.
        //if (vertNsearchIndex == -1)
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

        //std::cout << "vertAindex: " <<vertAindex << std::endl;
        //std::cout << "vertBindex: " <<vertBindex << std::endl;
        //std::cout << "vertNindex: " <<vertNindex << std::endl;
        //std::cout << "edgeANindex: "<<edgeANindex << std::endl;
        //std::cout << "edgeNBindex: "<<edgeNBindex << std::endl;
        //std::cout << "-----------" << std::endl;

        // update anything else that might be affected:
        verts[vertBindex].edgeIndices.remove(verts[vertBindex].edgeIndices.findIndex(edgeANindex));
        verts[vertBindex].edgeIndices.add(edgeNBindex);

        // split any attached faces each into two pieces... one at a time
        plSeq<PLuint> allCverts; // used for error checking
        for (PLuint i = 0; i < faceIndicesToSplit.size(); i++)
        {
            // find all existing cells, have them available in case they're needed later
            PLuint faceABCindex = faceIndicesToSplit[i];
            plMeshIntersectorConnectivityDataFace faceABC = faces[faceABCindex];

            // TODO: Consider adding a check to make sure that the face doesn't contain one of the edges we're adding...
            PLint vertCsearchIndex(-1);
            for (PLuint j = 0; j < 3; j++)
            {
                if (faces[faceABCindex].vertIndices[j] != vertAindex && faces[faceABCindex].vertIndices[j] != vertBindex)
                {
                    vertCsearchIndex = faces[faceABCindex].vertIndices[j];
                    break;
                }
            }
            if (vertCsearchIndex == -1)
            {
                std::cerr << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the C vertex. Aborting operation." << std::endl;
                std::cerr << edges[edgeANindex] << std::endl;
                std::cerr << edges[edgeNBindex] << std::endl;
                std::cerr << faces[faceABCindex] << std::endl;
                return false;
            }
            PLuint vertCindex((PLuint)vertCsearchIndex);
            if (allCverts.findIndex(vertCindex) != -1)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is being added more than once. This is very bad, possibly some rare situation that hasn't been considered? Aborting." << std::endl;
                return false;
            }
            allCverts.add(vertCindex);
            if (vertCindex == vertNindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is N vertex. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (vertCindex == vertAindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is A vertex. This should never happen, and is indicitave of a programming logic error. Aborting." << std::endl;
                continue;
            }
            if (vertCindex == vertBindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is B vertex. This should never happen, and is indicitave of a programming logic error. Aborting." << std::endl;
                continue;
            }
            plMeshIntersectorConnectivityDataVert& vertC = verts[vertCindex];

            PLint edgeACsearchIndex(-1);
            PLint edgeBCsearchIndex(-1);
            for (PLuint j = 0; j < 3; j++)
            {
                PLuint currentEdgeIndex = faces[faceABCindex].edgeIndices[j];
                if (edges[currentEdgeIndex].edge == plEdge(verts[vertAindex].vert,verts[vertCindex].vert))
                    edgeACsearchIndex = currentEdgeIndex;
                else if (edges[currentEdgeIndex].edge == plEdge(verts[vertBindex].vert,verts[vertCindex].vert))
                    edgeBCsearchIndex = currentEdgeIndex;
            }
            if (edgeACsearchIndex == -1)
            {
                std::cerr << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the AC edge. Aborting operation." << std::endl;
                std::cerr << verts[vertAindex] << std::endl;
                std::cerr << verts[vertCindex] << std::endl;
                std::cerr << faces[faceABCindex] << std::endl;
                return false;
            }
            if (edgeBCsearchIndex == -1)
            {
                std::cerr << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the BC edge. Aborting operation." << std::endl;
                std::cerr << verts[vertBindex] << std::endl;
                std::cerr << verts[vertCindex] << std::endl;
                std::cerr << faces[faceABCindex] << std::endl;
                return false;
            }
            PLuint edgeACindex((PLuint)edgeACsearchIndex);
            PLuint edgeBCindex((PLuint)edgeBCsearchIndex);
            if (edgeBCindex == edgeANindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): BC edge is AN edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (edgeACindex == edgeANindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): AC edge is AN edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (edgeBCindex == edgeNBindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): BC edge is NB edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (edgeACindex == edgeNBindex)
            {
                std::cerr << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): AC edge is NB edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
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

            //if (vertNsearchIndex == -1) // only if the vertex is new should this next thing be added
            verts[vertNindex].faceIndices.add(faceANCindex);
            verts[vertNindex].faceIndices.add(faceBNCindex);
            verts[vertCindex].faceIndices.add(faceBNCindex); // already should have faceANCindex, since it was previously faceABCindex

            verts[vertNindex].edgeIndices.add(edgeNCindex);
            verts[vertCindex].edgeIndices.add(edgeNCindex);

            //std::cout << "vertAindex: " <<vertAindex << std::endl;
            //std::cout << "vertBindex: " <<vertBindex << std::endl;
            //std::cout << "vertCindex: " <<vertCindex << std::endl;
            //std::cout << "vertNindex: " <<vertNindex << std::endl;
            //std::cout << "edgeANindex: "<<edgeANindex << std::endl;
            //std::cout << "edgeNBindex: "<<edgeNBindex << std::endl;
            //std::cout << "edgeNCindex: "<<edgeNCindex << std::endl;
            //std::cout << "-----------" << std::endl;
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

        PLint edge01searchIndex(-1);
        PLint edge12searchIndex(-1);
        PLint edge20searchIndex(-1);
        for (PLuint i = 0; i < face.edgeIndices.size(); i++)
        {
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point0(), PL_EPSILON) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point1(), PL_EPSILON) )
                edge01searchIndex = face.edgeIndices[i];
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point1(), PL_EPSILON) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point2(), PL_EPSILON) )
                edge12searchIndex = face.edgeIndices[i];
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point2(), PL_EPSILON) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point0(), PL_EPSILON) )
                edge20searchIndex = face.edgeIndices[i];
        }
        if (edge01searchIndex == -1 || edge12searchIndex == -1 || edge20searchIndex == -1)
        {
            std::cerr << "Error in plMeshIntersectorConnectivityData::_splitFaceOnVect(): Could not find one of the edges in the face. Aborting operation." << std::endl;
            std::cerr << faces[faceIndex] << std::endl;
            return false;
        }
        PLuint edge01index((PLuint)edge01searchIndex);
        PLuint edge12index((PLuint)edge12searchIndex);
        PLuint edge20index((PLuint)edge20searchIndex);
        plMeshIntersectorConnectivityDataEdge& edge01 = edges[edge01index];
        plMeshIntersectorConnectivityDataEdge& edge12 = edges[edge12index];
        plMeshIntersectorConnectivityDataEdge& edge20 = edges[edge20index];

        // create the new cells, storing their eventual indices
        plMeshIntersectorConnectivityDataVert vertNsearch; vertNsearch.vert = vertex;
        PLint  vertNsearchIndex;
        _findVert(vertex, vertNsearchIndex);
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

        //std::cout << "vertNindex: " << vertNindex << std::endl;
        //std::cout << "vert0index: " << vert0index << std::endl;
        //std::cout << "vert1index: " << vert1index << std::endl;
        //std::cout << "vert2index: " << vert2index << std::endl;
        //std::cout << "edge01index: " << edge01index << std::endl;
        //std::cout << "edge12index: " << edge12index << std::endl;
        //std::cout << "edge20index: " << edge20index << std::endl;
        //std::cout << "face012index: " << faceIndex << std::endl;
        //std::cout << "-----------" << std::endl;

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

    PLbool plMeshIntersectorConnectivityData::_checkNoDuplicates()
    {
        bool good(true);
        plSeq<PLuint> indices;
        for (PLuint i=0; i < verts.size(); i++)
        {
            // check edges
            indices.clear();
            for (PLuint j = 0; j < verts[i].edgeIndices.size(); j++)
            {
                if (indices.findIndex(verts[i].edgeIndices[j]) > -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << i << " contains a duplicate edge: " << verts[i].edgeIndices[j] << "." << std::endl;
                    good = false;
                    std::cout << verts[i] << std::endl;
                }
                else
                {
                    indices.add(verts[i].edgeIndices[j]);
                }
            }
            // check faces
            indices.clear();
            for (PLuint j = 0; j < verts[i].faceIndices.size(); j++)
            {
                if (indices.findIndex(verts[i].faceIndices[j]) > -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << i << " contains a duplicate face: " << verts[i].faceIndices[j] << "." << std::endl;
                    good = false;
                    std::cout << verts[i] << std::endl;
                }
                else
                {
                    indices.add(verts[i].faceIndices[j]);
                }
            }
        }
        for (PLuint i=0; i < edges.size(); i++)
        {
            // check vertices
            indices.clear();
            for (PLuint j = 0; j < edges[i].vertIndices.size(); j++)
            {
                if (indices.findIndex(edges[i].vertIndices[j]) > -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << i << " contains a duplicate vertex: " << edges[i].vertIndices[j] << "." << std::endl;
                    good = false;
                    std::cout << edges[i] << std::endl;
                }
                else
                {
                    indices.add(edges[i].vertIndices[j]);
                }
            }
            // check faces
            indices.clear();
            for (PLuint j = 0; j < edges[i].faceIndices.size(); j++)
            {
                if (indices.findIndex(edges[i].faceIndices[j]) > -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << i << " contains a duplicate face: " << edges[i].faceIndices[j] << "." << std::endl;
                    good = false;
                    std::cout << edges[i] << std::endl;
                }
                else
                {
                    indices.add(edges[i].faceIndices[j]);
                }
            }
        }
        for (PLuint i=0; i < faces.size(); i++)
        {
            // check vertices
            indices.clear();
            for (PLuint j = 0; j < faces[i].vertIndices.size(); j++)
            {
                if (indices.findIndex(faces[i].vertIndices[j]) > -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << i << " contains a duplicate vertex: " << faces[i].vertIndices[j] << "." << std::endl;
                    good = false;
                    std::cout << faces[i] << std::endl;
                }
                else
                {
                    indices.add(faces[i].vertIndices[j]);
                }
            }
            // check edges
            indices.clear();
            for (PLuint j = 0; j < faces[i].edgeIndices.size(); j++)
            {
                if (indices.findIndex(faces[i].edgeIndices[j]) > -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << i << " contains a duplicate edge: " << faces[i].edgeIndices[j] << "." << std::endl;
                    good = false;
                    std::cout << faces[i] << std::endl;
                }
                else
                {
                    indices.add(faces[i].edgeIndices[j]);
                }
            }
        }
        return good;
    }

    PLbool plMeshIntersectorConnectivityData::_checkBidirectionalConnections()
    {
        bool good(true);
        plSeq<PLuint> indices;
        for (PLuint i=0; i < verts.size(); i++)
        {
            // check edges
            indices.clear();
            for (PLuint j = 0; j < verts[i].edgeIndices.size(); j++)
            {
                if (edges[verts[i].edgeIndices[j]].vertIndices.findIndex(i) == -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << i << " references edge " << verts[i].edgeIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << verts[i] << std::endl;
                    std::cout << edges[verts[i].edgeIndices[j]] << std::endl;
                }
            }
            // check faces
            indices.clear();
            for (PLuint j = 0; j < verts[i].faceIndices.size(); j++)
            {
                if (faces[verts[i].faceIndices[j]].vertIndices.findIndex(i) == -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << i << " references face " << verts[i].faceIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << verts[i] << std::endl;
                    std::cout << faces[verts[i].faceIndices[j]] << std::endl;
                }
            }
        }
        for (PLuint i=0; i < edges.size(); i++)
        {
            // check vertices
            indices.clear();
            for (PLuint j = 0; j < edges[i].vertIndices.size(); j++)
            {
                if (verts[edges[i].vertIndices[j]].edgeIndices.findIndex(i) == -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << i << " references vert " << edges[i].vertIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << edges[i] << std::endl;
                    std::cout << verts[edges[i].vertIndices[j]] << std::endl;
                }
            }
            // check faces
            indices.clear();
            for (PLuint j = 0; j < edges[i].faceIndices.size(); j++)
            {
                if (faces[edges[i].faceIndices[j]].edgeIndices.findIndex(i) == -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << i << " references face " << edges[i].faceIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << edges[i] << std::endl;
                    std::cout << faces[edges[i].faceIndices[j]] << std::endl;
                }
            }
        }
        for (PLuint i=0; i < faces.size(); i++)
        {
            // check vertices
            indices.clear();
            for (PLuint j = 0; j < faces[i].vertIndices.size(); j++)
            {
                if (verts[faces[i].vertIndices[j]].faceIndices.findIndex(i) == -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << i << " references vert " << faces[i].vertIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << faces[i] << std::endl;
                    std::cout << verts[faces[i].vertIndices[j]] << std::endl;
                }
            }
            // check edges
            indices.clear();
            for (PLuint j = 0; j < faces[i].edgeIndices.size(); j++)
            {
                if (edges[faces[i].edgeIndices[j]].faceIndices.findIndex(i) == -1)
                {
                    std::cout << "Warning in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << i << " references edge " << faces[i].edgeIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << faces[i] << std::endl;
                    std::cout << edges[faces[i].edgeIndices[j]] << std::endl;
                }
            }
        }
        return good;
    }

    PLbool plMeshIntersectorConnectivityData::_checkNoSliverTriangles()
    {
        PLbool good(true);
        for (PLuint i = 0; i < faces.size(); i++)
        {
            if (( ((faces[i].face.point1())-(faces[i].face.point0())).normalize() ^
                  ((faces[i].face.point2())-(faces[i].face.point0())).normalize()).length() <= PL_EPSILON )
            {
                std::cout << "Warning in plMeshIntersectorConnectivityData::_checkNoSliverTriangles(): face " << i << " appears to be ultra thin. This is bad." << std::endl;
                good = false;
                std::cout << faces[i] << std::endl;
            }
        }
        return good;
    }

    PLbool plMeshIntersectorConnectivityData::_checkForAllErrors()
    {
        return _checkNoDuplicates() && _checkArraySizes() && _checkBidirectionalConnections() && _checkNoSliverTriangles();
    }

    void plMeshIntersectorConnectivityData::_reportSizes()
    {
        std::cout << "Size of verts: " << verts.size() << "\nSize of edges: " << edges.size() << "\nSize of faces: " << faces.size() << "\n";
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
            _findVert(vert0.vert,vert0index);
            PLbool vert0existed(true);
            if (vert0index == -1)
            {
                vert0index = verts.size();
                verts.add(vert0);
                vert0existed = false;
            }
            plMeshIntersectorConnectivityDataVert vert1;
            vert1.vert = currentTriangle[1];
            _findVert(vert1.vert,vert1index);
            PLbool vert1existed(true);
            if (vert1index == -1)
            {
                vert1index = verts.size();
                verts.add(vert1);
                vert1existed = false;
            }
            plMeshIntersectorConnectivityDataVert vert2;
            vert2.vert = currentTriangle[2];
            _findVert(vert2.vert,vert2index);
            PLbool vert2existed(true);
            if (vert2index == -1)
            {
                vert2index = verts.size();
                verts.add(vert2);
                vert2existed = false;
            }

            PLint edge01index, edge12index, edge20index;
            plMeshIntersectorConnectivityDataEdge edge01;
            edge01.edge = plEdge(verts[vert0index].vert,verts[vert1index].vert);
            edge01index = edges.findIndex(edge01);
            if (edge01index == -1)
            {
                edge01index = edges.size();
                edges.add(edge01);
                edges[edge01index].vertIndices.add((PLuint)vert0index);
                edges[edge01index].vertIndices.add((PLuint)vert1index);
            }
            plMeshIntersectorConnectivityDataEdge edge12;
            edge12.edge = plEdge(verts[vert1index].vert,verts[vert2index].vert);
            edge12index = edges.findIndex(edge12);
            if (edge12index == -1)
            {
                edge12index = edges.size();
                edges.add(edge12);
                edges[edge12index].vertIndices.add((PLuint)vert1index);
                edges[edge12index].vertIndices.add((PLuint)vert2index);
            }
            plMeshIntersectorConnectivityDataEdge edge20;
            edge20.edge = plEdge(verts[vert2index].vert,verts[vert0index].vert);
            edge20index = edges.findIndex(edge20);
            if (edge20index == -1)
            {
                edge20index = edges.size();
                edges.add(edge20);
                edges[edge20index].vertIndices.add((PLuint)vert2index);
                edges[edge20index].vertIndices.add((PLuint)vert0index);
            }

            plMeshIntersectorConnectivityDataFace face012;
            face012.face = plTriangle(verts[vert0index].vert,verts[vert1index].vert,verts[vert2index].vert);
            PLint face012index = faces.size();
            faces.add(face012);

            // this should only be done if the vertex did not exist. Didn't do earlier since we didn't know the edge indices yet
            if (verts[vert0index].edgeIndices.findIndex(edge01index) == -1)  verts[vert0index].edgeIndices.add((PLuint)edge01index);
            if (verts[vert0index].edgeIndices.findIndex(edge20index) == -1)  verts[vert0index].edgeIndices.add((PLuint)edge20index);
            if (verts[vert1index].edgeIndices.findIndex(edge01index) == -1)  verts[vert1index].edgeIndices.add((PLuint)edge01index);
            if (verts[vert1index].edgeIndices.findIndex(edge12index) == -1)  verts[vert1index].edgeIndices.add((PLuint)edge12index);
            if (verts[vert2index].edgeIndices.findIndex(edge12index) == -1)  verts[vert2index].edgeIndices.add((PLuint)edge12index);
            if (verts[vert2index].edgeIndices.findIndex(edge20index) == -1)  verts[vert2index].edgeIndices.add((PLuint)edge20index);

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
            if (!_checkBidirectionalConnections()) { std::cout << "Error: Aborting due to bad import." << std::endl; exit(1); }
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

        if (!_checkForAllErrors()) return false;

        PLuint vertsSize(0);
        PLuint edgesSize(0);
        PLuint facesSize(0);
        _reportSizes();

        while (vertsSize != verts.size() || edgesSize != edges.size() || facesSize != faces.size())
        {
            vertsSize = verts.size();
            edgesSize = edges.size();
            facesSize = faces.size();

            std::cout << "Detecting vert-edge intersections" << std::endl;
            // vertex-edge intersections
            for (PLuint i = 0; i < edges.size(); i++)
            {
                for (PLuint j = 0; j < verts.size(); j++)
                {
                    if (edges[i].intersectsVert(verts[j]))
                    {
                        _splitEdgeOnVect(i,verts[j].vert);
                        if (!_checkForAllErrors()) { return false;}
                        i--;
                        break; // restart edge iteration, since it may need to be split again! you never know!
                    }
                }
            }
            _reportSizes();

            std::cout << "Detecting vert-face intersections" << std::endl;
            // vertex-face intersections
            for (PLuint i = 0; i < faces.size(); i++)
            {
                for (PLuint j = 0; j < verts.size(); j++)
                {
                    if (faces[i].intersectsVert(verts[j]))
                    {
                        _splitFaceOnVect(i,verts[j].vert);
                        if (!_checkForAllErrors()) return false;
                        i--;
                        break;
                    }
                }
            }
            _reportSizes();

            std::cout << "Detecting edge-edge intersections" << std::endl;
            // edge-edge intersections
            for (PLuint i = 0; i < edges.size(); i++)
            {
                for (PLuint j = i+1; j < edges.size(); j++)
                {
                    if (edges[i].intersectsEdge(edges[j],intersectionPoint))
                    {
                        _splitEdgeOnVect(i,intersectionPoint);
                        if (!_checkForAllErrors()) return false;
                        _splitEdgeOnVect(j,intersectionPoint);
                        if (!_checkForAllErrors()) return false;
                        i--;
                        break; // restart edge iteration, since it may need to be split again! you never know!
                    }
                }
            }
            _reportSizes();

            std::cout << "Detecting edge-face intersections" << std::endl;
            // edge-face intersections
            for (PLuint i = 0; i < faces.size(); i++)
            {
                for (PLuint j = 0; j < edges.size(); j++)
                {
                    if (faces[i].intersectsEdge(edges[j],intersectionPoint))
                    {
                        _splitFaceOnVect(i,intersectionPoint);
                        if (!_checkForAllErrors()) { std::cout << faces[i] << std::endl; std::cout << edges[j] << std::endl; _exportTriSeq(outputTris); return false;}
                        _splitEdgeOnVect(j,intersectionPoint);
                        if (!_checkForAllErrors()) { std::cout << faces[i] << std::endl; std::cout << edges[j] << std::endl; _exportTriSeq(outputTris); return false;}
                        i--;
                        break;
                    }
                }
            }
            _reportSizes();

            _exportTriSeq(outputTris); // TODO: This is temporary.
        }

        // should be good, but for sanity's sake...
        if (!_checkForAllErrors()) return false;

        std::cout << "Done." << std::endl;
        _reportSizes();
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
