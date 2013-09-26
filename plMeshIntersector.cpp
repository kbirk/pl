#include "plMeshIntersector.h"

namespace plMeshIntersector
{

    plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityData()
    {
        _epsilon = PL_EPSILON;
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

    PLbool plMeshIntersectorConnectivityData::_intersectionVertEdge(const plMeshIntersectorConnectivityDataVert& vert, const plMeshIntersectorConnectivityDataEdge& edge, PLuint verbose, PLuint depth)
    {
        //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::_intersectsVert()" << std::endl; }

        if (edge.edge.contains(vert.vert, _epsilon)) // if it is on the edge, this is not an intersection
            return false;

        if ((plMath::closestPointOnSegment(vert.vert,edge.edge.pt1,edge.edge.pt2) - vert.vert).length() > _epsilon ) // if it's NOT on the line
            return false;

        // now need to make sure that this vertex doesn't intersect any other edges in the same triangle.
        // We need to ensure that only one edge will ever be intersected, and that this one won't be intersected
        // on the next iteration UNLESS this edge actually passes through the vertex.
        // We can use barycentric coordinates of the adjacent faces. We should only ever consider an intersection if
        // the weight for the opposite vertex is smaller than the other two in the same face
        plVector3 barycentricCoords;
        for (PLuint i = 0; i < edge.faceIndices.size();i++)
        {
            //if (faces[edge.faceIndices[i]].face.contains(vert.vert),_epsilon)
            //    return false;

            barycentricCoords = faces[edge.faceIndices[i]].face.barycentricCoords(vert.vert);
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Barycentric weights: " << barycentricCoords << std::endl;
            plVector3 v0 = (faces[edge.faceIndices[i]].face.point1() - faces[edge.faceIndices[i]].face.point0()).normalize(),
                      v1 = (faces[edge.faceIndices[i]].face.point2() - faces[edge.faceIndices[i]].face.point0()).normalize(),
                      v2 = (vert.vert          - faces[edge.faceIndices[i]].face.point0()).normalize();

            PLfloat d00 = (v0*v0);
            PLfloat d01 = (v0*v1);
            PLfloat d11 = (v1*v1);
            PLfloat d20 = (v2*v0);
            PLfloat d21 = (v2*v1);
            PLdouble denom = d00 * d11 - d01 * d01;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "denom: " << denom << std::endl;
            PLfloat v = (d11 * d20 - d01 * d21) / denom;
            PLfloat w = (d00 * d21 - d01 * d20) / denom;
            PLfloat u = 1.0f - v - w;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "params: " << v << " " << w << " " << u << std::endl;

            if (edge.edge.pt1 != faces[edge.faceIndices[i]].face.point0() && edge.edge.pt2 != faces[edge.faceIndices[i]].face.point0())
            {
                if ((barycentricCoords.x > barycentricCoords.y || barycentricCoords.x > barycentricCoords.z) && fabs(barycentricCoords.x) > 0.f)
                    return false;
            }
            if (edge.edge.pt1 != faces[edge.faceIndices[i]].face.point1() && edge.edge.pt2 != faces[edge.faceIndices[i]].face.point1())
            {
                if ((barycentricCoords.y > barycentricCoords.x || barycentricCoords.y > barycentricCoords.z) && fabs(barycentricCoords.x) > 0.f)
                    return false;
            }
            if (edge.edge.pt1 != faces[edge.faceIndices[i]].face.point2() && edge.edge.pt2 != faces[edge.faceIndices[i]].face.point2())
            {
                if ((barycentricCoords.z > barycentricCoords.x || barycentricCoords.z > barycentricCoords.y) && fabs(barycentricCoords.x) > 0.f)
                    return false;
            }
        }

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_intersectionEdgeEdge(const plMeshIntersectorConnectivityDataEdge& e1, const plMeshIntersectorConnectivityDataEdge& e2, plVector3& intersection, PLuint verbose, PLuint depth)
    {
        //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::_intersectsEdge()" << std::endl; }

        if (e2.edge.contains(e1.edge.pt1, 0.f) || e2.edge.contains(e1.edge.pt2, 0.f))
            return false;

        plVector3 pointOnThis;
        plVector3 pointOnOther;
        PLfloat   distance;

        if (!plMath::closestPointsBetweenSegments(e2.edge.pt1,e2.edge.pt2,e1.edge.pt1,e1.edge.pt2,pointOnThis,pointOnOther,distance))
            return false;

        if (distance >  _epsilon)
            return false;
        intersection = pointOnThis;

        // check just to make sure the intersection isn't in fact already a vertex...
        if (e2.edge.contains(intersection, _epsilon) || e1.edge.contains(intersection, _epsilon))
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_intersectionVertFace(const plMeshIntersectorConnectivityDataVert& vert, const plMeshIntersectorConnectivityDataFace& face, PLuint verbose, PLuint depth)
    {
        //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::_intersectsVert()" << std::endl; }

        // don't consider vertices that are a part of this triangle
        if (face.face.contains(vert.vert, _epsilon))
            return false;

        // see if it's even in the plane
        if ((plMath::closestPointOnPlane(face.face.normal(),vert.vert,face.face.normal(),face.face.point0()) - vert.vert).length() >  _epsilon )
            return false;

        // now see if the barycentric coordinates are in the right ranges
        plVector3 barycentricCoords = face.face.barycentricCoords(vert.vert);
        if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
            barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
            barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
            return false;

        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Barycentric weights: " << barycentricCoords << std::endl;
        plVector3 v0 = (face.face.point1() - face.face.point0()).normalize(),
                  v1 = (face.face.point2() - face.face.point0()).normalize(),
                  v2 = (vert.vert          - face.face.point0()).normalize();

        PLfloat d00 = (v0*v0);
        PLfloat d01 = (v0*v1);
        PLfloat d11 = (v1*v1);
        PLfloat d20 = (v2*v0);
        PLfloat d21 = (v2*v1);
        PLdouble denom = d00 * d11 - d01 * d01;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "denom: " << denom << std::endl;
        PLfloat v = (d11 * d20 - d01 * d21) / denom;
        PLfloat w = (d00 * d21 - d01 * d20) / denom;
        PLfloat u = 1.0f - v - w;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "params: " << v << " " << w << " " << u << std::endl;

        // on the edge, don't consider as an intersection
        if ((plMath::closestPointOnSegment(vert.vert,face.face.point0(),face.face.point1()) - vert.vert).length() <= _epsilon ||
            (plMath::closestPointOnSegment(vert.vert,face.face.point1(),face.face.point2()) - vert.vert).length() <= _epsilon ||
            (plMath::closestPointOnSegment(vert.vert,face.face.point2(),face.face.point0()) - vert.vert).length() <= _epsilon )
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_intersectionEdgeFace(const plMeshIntersectorConnectivityDataEdge& edge, const plMeshIntersectorConnectivityDataFace& face, plVector3& intersection, PLuint verbose, PLuint depth)
    {
        //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::_intersectsEdge()" << std::endl; }

        // don't consider edges that are a part of this triangle
        if (face.face.contains(edge.edge.pt1, _epsilon) || face.face.contains(edge.edge.pt2, _epsilon))
            return false;

        // first find the line intersection with the plane
        plSeq<plTriangle> triSeq;
        triSeq.add(face.face);
        plIntersection intersectionData = plMath::rayIntersect(triSeq, edge.edge.pt1, (edge.edge.pt2-edge.edge.pt1), false, false);
        if (!intersectionData.exists)
            return false;
        if (intersectionData.t < 0.f || intersectionData.t > 1.f)
            return false;

        // now see if the barycentric coordinates are in the right ranges
        intersection = intersectionData.point;
        plVector3 barycentricCoords = face.face.barycentricCoords(intersection);
        if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
            barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
            barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
            return false;

        // on a face edge, don't consider as an intersection
        if ((plMath::closestPointOnSegment(intersection,face.face.point0(),face.face.point1()) - intersection).length() <= _epsilon ||
            (plMath::closestPointOnSegment(intersection,face.face.point1(),face.face.point2()) - intersection).length() <= _epsilon ||
            (plMath::closestPointOnSegment(intersection,face.face.point2(),face.face.point0()) - intersection).length() <= _epsilon )
            return false;

        // if on one of the edge vertices, don't consider as an intersection
        if (edge.edge.contains(intersection,_epsilon))
            return false;

        // if on one of the face vertices, don't consider as an intersection
        if (face.face.contains(intersection,_epsilon))
            return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_findVert( const plVector3& vertex, PLint &index, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findVert()" << std::endl;
        }

        index = -1;
        for (PLuint i = 0; i < verts.size(); i++)
        {
            if ((verts[i].vert - vertex).length() <= _epsilon)
            {
                if (index == -1)
                    index = i;
                else
                {
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_findVert(): More than one candidate for vertex " << vertex << ". This could mean that epsilon is set too large. Aborting operation." << std::endl;
                    return false;
                }
            }
        }
        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_splitEdgeOnVect( PLuint edgeABindex, const plVector3& vertex, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_splitEdgeOnVect()" << std::endl;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Splitting on vertex " << vertex << std::endl;
        }

        // split the current edge into two pieces

        // find all existing cells, have them available in case they're needed later
        plMeshIntersectorConnectivityDataEdge edgeAB = edges[edgeABindex];
        plSeq<PLuint> faceIndicesToSplit (edges[edgeABindex].faceIndices);
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Edge being split is " << edges[edgeABindex].edge.pt1 << " - " << edges[edgeABindex].edge.pt2 << std::endl;
            for (PLuint j=0;j<edges[edgeABindex].faceIndices.size();j++)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Debug: Attached face: " << faces[edges[edgeABindex].faceIndices[j]].face.point0() << " | " << faces[edges[edgeABindex].faceIndices[j]].face.point1() << " | " << faces[edges[edgeABindex].faceIndices[j]].face.point2() << std::endl;
            }
        }

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
        vertNsearchIndex = verts.findIndex(vertNsearch);
        PLuint vertNindex; // we need to determine this
        if ((PLuint)vertNsearchIndex == vertAindex)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): N vertex is A vertex. This is possibly due to epsilon being too large. Aborting this particular edge split, but beware of future errors." << std::endl;
            return false;
        }
        if ((PLuint)vertNsearchIndex == vertBindex)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): N vertex is B vertex. This is possibly due to epsilon being too large. Aborting this particular edge split, but beware of future errors." << std::endl;
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

        // split any attached faces each into two pieces...
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
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the C vertex. Aborting operation." << std::endl;
                std::cout << edges[edgeANindex] << std::endl;
                std::cout << edges[edgeNBindex] << std::endl;
                std::cout << faces[faceABCindex] << std::endl;
                return false;
            }
            PLuint vertCindex((PLuint)vertCsearchIndex);
            if (vertCindex == vertNindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is N vertex. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (vertCindex == vertAindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is A vertex. This should never happen, and is indicitave of a programming logic error. Aborting." << std::endl;
                continue;
            }
            if (vertCindex == vertBindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is B vertex. This should never happen, and is indicitave of a programming logic error. Aborting." << std::endl;
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
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the AC edge. Aborting operation." << std::endl;
                std::cout << verts[vertAindex] << std::endl;
                std::cout << verts[vertCindex] << std::endl;
                std::cout << faces[faceABCindex] << std::endl;
                return false;
            }
            if (edgeBCsearchIndex == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the BC edge. Aborting operation." << std::endl;
                std::cout << verts[vertBindex] << std::endl;
                std::cout << verts[vertCindex] << std::endl;
                std::cout << faces[faceABCindex] << std::endl;
                return false;
            }
            PLuint edgeACindex((PLuint)edgeACsearchIndex);
            PLuint edgeBCindex((PLuint)edgeBCsearchIndex);
            if (edgeBCindex == edgeANindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): BC edge is AN edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (edgeACindex == edgeANindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): AC edge is AN edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (edgeBCindex == edgeNBindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): BC edge is NB edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
                continue;
            }
            if (edgeACindex == edgeNBindex)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): AC edge is NB edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
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
    PLbool plMeshIntersectorConnectivityData::_splitFaceOnVect( PLuint faceIndex, const plVector3& vertex, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_splitFaceOnVect()" << std::endl;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Splitting on vertex " << vertex << std::endl;
        }

        // find all existing cells, have them available in case they're needed later
        plMeshIntersectorConnectivityDataFace face = faces[faceIndex]; // this will eventually be removed from the list of faces
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Face being split is " << faces[faceIndex].face.point0() << " | "
                                                       << faces[faceIndex].face.point1() << " | "
                                                       << faces[faceIndex].face.point2() << std::endl;
        }

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
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Vertices detected are " << verts[vert0index].vert << " | "
                                                         << verts[vert1index].vert << " | "
                                                         << verts[vert2index].vert << std::endl;
        }

        PLint edge01searchIndex(-1);
        PLint edge12searchIndex(-1);
        PLint edge20searchIndex(-1);
        for (PLuint i = 0; i < face.edgeIndices.size(); i++)
        {
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point0(), _epsilon) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point1(), _epsilon) )
                edge01searchIndex = face.edgeIndices[i];
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point1(), _epsilon) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point2(), _epsilon) )
                edge12searchIndex = face.edgeIndices[i];
            if (edges[face.edgeIndices[i]].edge.contains(face.face.point2(), _epsilon) &&
                edges[face.edgeIndices[i]].edge.contains(face.face.point0(), _epsilon) )
                edge20searchIndex = face.edgeIndices[i];
        }
        if (edge01searchIndex == -1 || edge12searchIndex == -1 || edge20searchIndex == -1)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_splitFaceOnVect(): Could not find one of the edges in the face. Aborting operation." << std::endl;
            std::cout << faces[faceIndex] << std::endl;
            return false;
        }
        PLuint edge01index((PLuint)edge01searchIndex);
        PLuint edge12index((PLuint)edge12searchIndex);
        PLuint edge20index((PLuint)edge20searchIndex);
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Edge01 detected is " << edges[edge01index].edge.pt1 << " - " << edges[edge01index].edge.pt2 << std::endl;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Edge12 detected is " << edges[edge12index].edge.pt1 << " - " << edges[edge12index].edge.pt2 << std::endl;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Edge20 detected is " << edges[edge20index].edge.pt1 << " - " << edges[edge20index].edge.pt2 << std::endl;
        }
        plMeshIntersectorConnectivityDataEdge& edge01 = edges[edge01index];
        plMeshIntersectorConnectivityDataEdge& edge12 = edges[edge12index];
        plMeshIntersectorConnectivityDataEdge& edge20 = edges[edge20index];

        // create the new cells, storing their eventual indices
        plMeshIntersectorConnectivityDataVert vertNsearch; vertNsearch.vert = vertex;
        PLint  vertNsearchIndex;
        vertNsearchIndex = verts.findIndex(vertNsearch);
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

    PLbool plMeshIntersectorConnectivityData::_checkArraySizes( PLuint verbose, PLuint depth )
    {
        bool good(true);
        for (PLuint i = 0; i < edges.size(); i++)
        {
            if (edges[i].faceIndices.size() % 2 != 0 || edges[i].faceIndices.size() == 0)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): faceIndices for edge " << i << " is of non positive even size " <<edges[i].faceIndices.size()  << ". Should be even." << std::endl;
                std::cout << edges[i] << std::endl;
                good = false;
            }
            if (edges[i].vertIndices.size() != 2)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for edge " << i << " is of size " << edges[i].vertIndices.size() << ". Should be 2." << std::endl;
                good = false;
                std::cout << edges[i] << std::endl;
            }
        }
        for (PLuint i = 0; i < faces.size(); i++)
        {
            if (faces[i].vertIndices.size() != 3)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for face " << i << " is of size " << faces[i].vertIndices.size() << ". Should be 3." << std::endl;
                good = false;
                std::cout << faces[i] << std::endl;
            }
            if (faces[i].edgeIndices.size() != 3)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): edgeIndices for face " << i << " is of size " << faces[i].edgeIndices.size() << ". Should be 3." << std::endl;
                good = false;
                std::cout << faces[i] << std::endl;
            }
        }
        return good;
    }

    PLbool plMeshIntersectorConnectivityData::_checkNoDuplicates( PLuint verbose, PLuint depth )
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << i << " contains a duplicate edge: " << verts[i].edgeIndices[j] << "." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << i << " contains a duplicate face: " << verts[i].faceIndices[j] << "." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << i << " contains a duplicate vertex: " << edges[i].vertIndices[j] << "." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << i << " contains a duplicate face: " << edges[i].faceIndices[j] << "." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << i << " contains a duplicate vertex: " << faces[i].vertIndices[j] << "." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << i << " contains a duplicate edge: " << faces[i].edgeIndices[j] << "." << std::endl;
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

    PLbool plMeshIntersectorConnectivityData::_checkBidirectional( PLuint verbose, PLuint depth )
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << i << " references edge " << verts[i].edgeIndices[j] << ", but not vice versa." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << i << " references face " << verts[i].faceIndices[j] << ", but not vice versa." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << i << " references vert " << edges[i].vertIndices[j] << ", but not vice versa." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << i << " references face " << edges[i].faceIndices[j] << ", but not vice versa." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << i << " references vert " << faces[i].vertIndices[j] << ", but not vice versa." << std::endl;
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
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << i << " references edge " << faces[i].edgeIndices[j] << ", but not vice versa." << std::endl;
                    good = false;
                    std::cout << faces[i] << std::endl;
                    std::cout << edges[faces[i].edgeIndices[j]] << std::endl;
                }
            }
        }
        return good;
    }

    PLbool plMeshIntersectorConnectivityData::_checkNoSliverTriangles( PLuint verbose, PLuint depth )
    {
        PLbool good(true);
        for (PLuint i = 0; i < faces.size(); i++)
        {
            if (( ((faces[i].face.point1())-(faces[i].face.point0())).normalize() ^
                  ((faces[i].face.point2())-(faces[i].face.point0())).normalize()).length() == 0.f )
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoSliverTriangles(): face " << i << " appears to be ultra thin. This is bad." << std::endl;
                good = false;
                std::cout << faces[i] << std::endl;
            }
        }
        return good;
    }

    PLbool plMeshIntersectorConnectivityData::_checkForAllErrors( PLuint verbose, PLuint depth )
    {
        return _checkNoDuplicates      (verbose,depth+1) &&
               _checkArraySizes        (verbose,depth+1) &&
               _checkBidirectional     (verbose,depth+1) &&
               _checkNoSliverTriangles (verbose,depth+1) ;
    }

    void plMeshIntersectorConnectivityData::_reportSizes( PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_reportSizes()" << std::endl;
        }
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Size of verts: " << verts.size() << "\n";
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Size of edges: " << edges.size() << "\n";
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Size of faces: " << faces.size() << "\n";
    }

    PLbool plMeshIntersectorConnectivityData::_importTriSeq(const plSeq<plTriangle> &tris, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_importTriSeq()" << std::endl;
        }

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
            if (!_findVert(vert0.vert,vert0index,verbose,depth+1)) return false;
            PLbool vert0existed(true);
            if (vert0index == -1)
            {
                vert0index = verts.size();
                verts.add(vert0);
                vert0existed = false;
            }
            plMeshIntersectorConnectivityDataVert vert1;
            vert1.vert = currentTriangle[1];
            if (!_findVert(vert1.vert,vert1index,verbose,depth+1)) return false;
            PLbool vert1existed(true);
            if (vert1index == -1)
            {
                vert1index = verts.size();
                verts.add(vert1);
                vert1existed = false;
            }
            plMeshIntersectorConnectivityDataVert vert2;
            vert2.vert = currentTriangle[2];
            if (!_findVert(vert2.vert,vert2index,verbose,depth+1)) return false;
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
        }
        if (!_checkForAllErrors( verbose,depth+1 )) return false;
        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_exportTriSeq(plSeq<plTriangle> &tris, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_exportTriSeq()" << std::endl;
        }

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

    PLbool plMeshIntersectorConnectivityData::_findAndFixVertEdgeIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixVertEdgeIntersections()" << std::endl;
        }

        if (endIndex == 0) endIndex = verts.size()-1;
        for (PLuint i = startIndex; i <= endIndex; i++)
        {
            for (PLuint j = 0; j < edges.size(); j++)
            {
                if (_intersectionVertEdge(verts[i],edges[j],verbose,depth+1))
                {
                    // first make sure that no other edges in the same face will be split!
                    /*plSeq<PLuint> edgesToCheck;
                    PLuint k; // reused index
                    for (k = 0; k < edges[j].faceIndices.size(); k++)
                    {
                        for (PLuint l = 0; l < 3; l++)
                        {
                            if (faces[edges[j].faceIndices[k]].edgeIndices[l] != j)
                            {
                                edgesToCheck.add(faces[edges[j].faceIndices[k]].edgeIndices[l]);
                            }
                        }
                    }
                    std::cout << edgesToCheck.size() << std::endl;
                    for (k = 0; k < edgesToCheck.size(); k++)
                    {
                        if ((plMath::closestPointOnSegment(verts[i].vert,              edges[j].edge.pt1,              edges[j].edge.pt2) - verts[i].vert).length() >
                            (plMath::closestPointOnSegment(verts[i].vert,edges[edgesToCheck[k]].edge.pt1,edges[edgesToCheck[k]].edge.pt2) - verts[i].vert).length() )
                            break;
                    }
                    if (k != edgesToCheck.size()) continue;*/
                    _splitEdgeOnVect(j,verts[i].vert,verbose,depth+1);
                    if (!_checkForAllErrors(verbose,depth+1)) { return false;}
                    i--;
                    break; // restart edge iteration, since it may need to be split again! you never know!
                }
            }
        }
        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_findAndFixVertFaceIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixVertFaceIntersections()" << std::endl;
        }

        if (endIndex == 0) endIndex = verts.size()-1;
        for (PLuint i = startIndex; i <= endIndex; i++)
        {
            for (PLuint j = 0; j < faces.size(); j++)
            {
                if (_intersectionVertFace(verts[i],faces[j],verbose,depth+1))
                {
                    _splitFaceOnVect(j,verts[i].vert,verbose,depth+1);
                    if (!_checkForAllErrors(verbose,depth+1)) { return false;}
                    i--;
                    break;
                }
            }
        }
        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_findAndFixEdgeEdgeIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixEdgeEdgeIntersections()" << std::endl;
        }

        PLuint vertsSizeBeforeOps(verts.size());
        plVector3 intersectionPoint;
        plSeq<plVector3> intersectionPoints;
        if (endIndex == 0) endIndex = edges.size()-1;
        for (PLuint i = startIndex; i <= endIndex; i++)
        {
            for (PLuint j = 0; j < edges.size(); j++)
            {
                if (_intersectionEdgeEdge(edges[j],edges[i],intersectionPoint,verbose,depth+1))
                {
                    intersectionPoints.add(intersectionPoint);
                }
            }
        }

        plSeq<PLuint> extraIndicesToCheck;
        for (PLuint i = 0; i < intersectionPoints.size(); i++)
        {
            plMeshIntersectorConnectivityDataVert newVert;
            newVert.vert = intersectionPoints[i];
            PLint searchIndex;// = (verts.findIndex(newVert));
            if (!_findVert(newVert.vert,searchIndex,verbose,depth+1)) return false;
            if (searchIndex == -1)
            {
                if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Debug: Creating new vertex: " << intersectionPoints[i] << std::endl;
                }
                verts.add(newVert);
            }
            else
                extraIndicesToCheck.add( (PLuint)searchIndex );
        }

        for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
        {
            if (!_findAndFixVertEdgeIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
        }
        if (!_findAndFixVertEdgeIntersections(vertsSizeBeforeOps,verts.size()-1,verbose,depth+1)) return false;
        for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
        {
            if (!_findAndFixVertFaceIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
        }
        if (!_findAndFixVertFaceIntersections(vertsSizeBeforeOps,verts.size()-1,verbose,depth+1)) return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::_findAndFixEdgeFaceIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixEdgeFaceIntersections()" << std::endl;
        }

        PLuint vertsSizeBeforeOps(verts.size());
        PLuint edgesSizeBeforeOps(edges.size());
        plVector3 intersectionPoint;
        plSeq<plVector3> intersectionPoints;
        if (endIndex == 0) endIndex = edges.size()-1;
        for (PLuint i = startIndex; i <= endIndex; i++)
        {
            for (PLuint j = 0; j < faces.size(); j++)
            {
                if (_intersectionEdgeFace(edges[i],faces[j],intersectionPoint,verbose,depth+1))
                {
                    intersectionPoints.add(intersectionPoint);
                }
            }
        }

        plSeq<PLuint> extraIndicesToCheck;
        for (PLuint i = 0; i < intersectionPoints.size(); i++)
        {
            plMeshIntersectorConnectivityDataVert newVert;
            newVert.vert = intersectionPoints[i];
            PLint searchIndex;// = (verts.findIndex(newVert));
            if (!_findVert(newVert.vert,searchIndex,verbose,depth+1)) return false;
            if (searchIndex == -1)
            {
                if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
                    for (PLuint i=0;i<depth;i++)
                        std::cout << "\t";
                    std::cout << "Debug: Creating new vertex: " << intersectionPoints[i] << std::endl;
                }
                verts.add(newVert);
            }
            else
                extraIndicesToCheck.add( (PLuint)searchIndex );
        }

        for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
        {
            if (!_findAndFixVertEdgeIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
        }
        if(!_findAndFixVertEdgeIntersections(vertsSizeBeforeOps,verts.size()-1,verbose,depth+1)) return false;
        for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
        {
            if (!_findAndFixVertFaceIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
        }
        if (!_findAndFixVertFaceIntersections(vertsSizeBeforeOps,verts.size()-1,verbose,depth+1)) return false;

        if (!_findAndFixEdgeEdgeIntersections(edgesSizeBeforeOps,(edges.size()-1),verbose,depth+1)) return false;

        return true;
    }

    PLbool plMeshIntersectorConnectivityData::intersect(const plSeq<plTriangle> &inputTris, plSeq<plTriangle> &outputTris, PLuint verbose, PLuint depth )
    {
        if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Entering plMeshIntersectorConnectivityData::intersect()" << std::endl;
        }

        if (!_importTriSeq(inputTris,verbose,depth+1)) return false;

        //exit(1);

        if (!_checkForAllErrors(verbose,depth+1)) return false;

        _reportSizes(verbose,depth+1);

        PLuint vertsSize(0);
        PLuint edgesSize(0);
        PLuint facesSize(0);

        while (vertsSize != verts.size() || edgesSize != edges.size() || facesSize != faces.size())
        {
            vertsSize = verts.size();
            edgesSize = edges.size();
            facesSize = faces.size();
            if (!_findAndFixVertEdgeIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
            if (!_findAndFixVertFaceIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
            if (!_findAndFixEdgeEdgeIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
            if (!_findAndFixEdgeFaceIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
            //break;
        }

        // should be good, but for sanity's sake...
        if (!_checkForAllErrors(verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}

        _reportSizes(verbose,depth+1);
        _exportTriSeq(outputTris,verbose,depth+1);

        return true;
    }

    PLbool intersect(const plSeq<plTriangle> &input, plSeq<plTriangle> &output, PLuint verbose, PLuint depth)
    {
        plMeshIntersectorConnectivityData intersector;
        return intersector.intersect(input,output,verbose,depth+1);
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
