#include "plMeshIntersector.h"

PLbool plMeshIntersector::_intersectionVertEdge(const plMeshConnectivityDataVert* vert, const plMeshConnectivityDataEdge* edge, PLuint verbose, PLuint depth)
{
    //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::_intersectsVert()" << std::endl; }

    if ( plUtility::exists( edge->verts, vert ) ) // if it is on the edge, this is not an intersection
        return false;

    if ( ( plMath::closestPointOnSegment(vert->vert,edge->edge.pt1,edge->edge.pt2) - vert->vert ).length() > _data.epsilon() ) // if it's NOT on the line
        return false;

    // now need to make sure that this vertex doesn't intersect any other edges in the same triangle.
    // We need to ensure that only one edge will ever be intersected, and that this one won't be intersected
    // on the next iteration UNLESS this edge actually passes through the vertex.
    // We can use barycentric coordinates of the adjacent faces. We should only ever consider an intersection if
    // the weight for the opposite vertex is smaller than the other two in the same face
    plVector3 barycentricCoords;
    for (PLuint i = 0; i < edge->faces.size();i++)
    {

        barycentricCoords = edge->faces[i]->face.barycentricCoords(vert->vert);
        // vertex of triangle
        if ( plUtility::exists( edge->faces[i]->verts, vert ) )
        {
            return false;
        }
        // edge of triangle
        if (edge->verts[0] != edge->faces[i]->verts[0] && edge->verts[1] != edge->faces[i]->verts[0])
        {
            if ( ( (barycentricCoords.y < barycentricCoords.x && barycentricCoords.y < barycentricCoords.z) ||
                   (barycentricCoords.z < barycentricCoords.x && barycentricCoords.z < barycentricCoords.y) ) &&
                 ( barycentricCoords.x > 0.f ) )
            {
                return false;
            }
        }
        if (edge->verts[0] != edge->faces[i]->verts[1] && edge->verts[1] != edge->faces[i]->verts[1])
        {
            if ( ( (barycentricCoords.z < barycentricCoords.x && barycentricCoords.z < barycentricCoords.y) ||
                   (barycentricCoords.x < barycentricCoords.y && barycentricCoords.x < barycentricCoords.z) ) &&
                 ( barycentricCoords.y > 0.f ) )
            {
                return false;
            }
        }
        if (edge->verts[0] != edge->faces[i]->verts[2] && edge->verts[1] != edge->faces[i]->verts[2])
        {
            if ( ( (barycentricCoords.y < barycentricCoords.x && barycentricCoords.y < barycentricCoords.z) ||
                   (barycentricCoords.x < barycentricCoords.y && barycentricCoords.x < barycentricCoords.z) ) &&
                 ( barycentricCoords.z > 0.f ) )
            {
                return false;
            }
        }
    }

    return true;
}

PLbool plMeshIntersector::_intersectionEdgeEdge(const plMeshConnectivityDataEdge* e1, const plMeshConnectivityDataEdge* e2, plVector3& intersection, PLuint verbose, PLuint depth)
{
    //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::_intersectsEdge()" << std::endl; }

    if ( plUtility::exists( e2->verts, e1->verts[1] ) || plUtility::exists( e2->verts, e1->verts[1] ) )
        return false;


    plVector3 pointOnThis;
    plVector3 pointOnOther;
    PLfloat   distance;

    if ( !plMath::closestPointsBetweenSegments(e2->edge.pt1,e2->edge.pt2,e1->edge.pt1,e1->edge.pt2,pointOnThis,pointOnOther,distance) )
        return false;

    if ( distance >  _data.epsilon() )
        return false;
    intersection = pointOnThis;

    // check just to make sure the intersection isn't in fact already a vertex...
    if (e2->edge.contains(intersection, _data.epsilon()) || e1->edge.contains(intersection, _data.epsilon()))
        return false;

    return true;
}

PLbool plMeshIntersector::_intersectionVertFace(const plMeshConnectivityDataVert* vert, const plMeshConnectivityDataFace* face, PLuint verbose, PLuint depth)
{
    //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::_intersectsVert()" << std::endl; }

    // don't consider vertices that are a part of this triangle
    if ( plUtility::exists( face->verts, vert ) )
        return false;

    // see if it's even in the plane
    if ((plMath::closestPointOnPlane(face->face.normal(),vert->vert,face->face.normal(),face->face.point0()) - vert->vert).length() >  _data.epsilon() )
        return false;

    // now see if the barycentric coordinates are in the right ranges
    plVector3 barycentricCoords = face->face.barycentricCoords(vert->vert);
    if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
        barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
        barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
        return false;

    /*for (PLuint i=0;i<depth;i++)
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

    std::cout << "d00: " << d00 << std::endl;
    std::cout << "d01: " << d01 << std::endl;
    std::cout << "d11: " << d11 << std::endl;
    std::cout << "d20: " << d20 << std::endl;
    std::cout << "d21: " << d21 << std::endl;

    PLfloat denomF = d00 * d11 - d01 * d01;
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "float denom: " << denomF << std::endl;
    PLfloat vF = (d11 * d20 - d01 * d21) / denomF;
    PLfloat wF = (d00 * d21 - d01 * d20) / denomF;
    PLfloat uF = 1.0f - vF - wF;
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "float params: " << uF << " " << vF << " " << wF << std::endl;

    PLdouble denomD = d00 * d11 - d01 * d01;
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "double denom: " << denomD << std::endl;
    PLfloat vD = (d11 * d20 - d01 * d21) / denomD;
    PLfloat wD = (d00 * d21 - d01 * d20) / denomD;
    PLfloat uD = 1.0f - vD - wD;
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "double params: " << uD << " " << vD << " " << wD << std::endl;*/

    // on the edge, don't consider as an intersection
    if ((plMath::closestPointOnSegment(vert->vert,face->face.point0(),face->face.point1()) - vert->vert).length() <= _data.epsilon() ||
        (plMath::closestPointOnSegment(vert->vert,face->face.point1(),face->face.point2()) - vert->vert).length() <= _data.epsilon() ||
        (plMath::closestPointOnSegment(vert->vert,face->face.point2(),face->face.point0()) - vert->vert).length() <= _data.epsilon() )
        return false;

    return true;
}

PLbool plMeshIntersector::_intersectionEdgeFace(const plMeshConnectivityDataEdge* edge, const plMeshConnectivityDataFace* face, plVector3& intersection, PLuint verbose, PLuint depth)
{
    //if (verbose >= PL_LOGGER_LEVEL_DEBUG) { for (PLuint i=0;i<depth;i++) std::cout << "\t"; std::cout << "Debug: Entering plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataFace::_intersectsEdge()" << std::endl; }

    // don't consider edges that are a part of this triangle
    if ( plUtility::exists( face->verts, edge->verts[0] ) || plUtility::exists( face->verts, edge->verts[1] ) )
        return false;

    // first find the line intersection with the plane
    std::vector<plTriangle> triSeq;
    triSeq.push_back(face->face);
    plIntersection intersectionData = plMath::rayIntersect(triSeq, edge->edge.pt1, (edge->edge.pt2-edge->edge.pt1), false, false);
    if (!intersectionData.exists)
        return false;
    if (intersectionData.t < 0.f || intersectionData.t > 1.f)
        return false;

    // now see if the barycentric coordinates are in the right ranges
    intersection = intersectionData.point;
    plVector3 barycentricCoords = face->face.barycentricCoords(intersection);
    if (barycentricCoords.x < 0.f || barycentricCoords.x > 1.f ||
        barycentricCoords.y < 0.f || barycentricCoords.y > 1.f ||
        barycentricCoords.z < 0.f || barycentricCoords.z > 1.f)
        return false;

    // if on one of the edge vertices, don't consider as an intersection
    if (edge->edge.contains(intersection,_data.epsilon()))
        return false;

    // if on one of the face vertices, don't consider as an intersection
    if (face->face.contains(intersection,_data.epsilon()))
        return false;

    // on a face edge, don't consider as an intersection
    if ((plMath::closestPointOnSegment(intersection,face->face.point0(),face->face.point1()) - intersection).length() <= _data.epsilon() ||
        (plMath::closestPointOnSegment(intersection,face->face.point1(),face->face.point2()) - intersection).length() <= _data.epsilon() ||
        (plMath::closestPointOnSegment(intersection,face->face.point2(),face->face.point0()) - intersection).length() <= _data.epsilon() )
        return false;

    return true;
}


PLbool plMeshIntersector::_findAndFixVertEdgeIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixVertEdgeIntersections()" << std::endl;
    }

    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); )
    {
        const plMeshConnectivityDataVert* currentV = &(*vit);
        vit++;
        for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); )
        {
            const plMeshConnectivityDataEdge* currentE = &(*eit);
            eit++;
            if (_intersectionVertEdge(currentV,currentE,verbose,depth+1))
            {
                _splitEdgeOnVect(currentE,currentV,verbose,depth+1);
                if (!_checkForAllErrors(verbose,depth+1)) { return false;}
            }
        }
    }
    return true;
}

PLbool plMeshIntersector::_findAndFixVertFaceIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixVertFaceIntersections()" << std::endl;
    }

    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); )
    {
        const plMeshConnectivityDataVert* currentV = &(*vit);
        vit++;
        for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); )
        {
            const plMeshConnectivityDataFace* currentF = &(*fit);
            fit++;
            if (_intersectionVertFace(currentV,currentF,verbose,depth+1))
            {
                _splitFaceOnVect(currentF,currentV,verbose,depth+1);
                if (!_checkForAllErrors(verbose,depth+1)) { return false;}
            }
        }
    }
    return true;
}

PLbool plMeshIntersector::_findAndFixEdgeEdgeIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixEdgeEdgeIntersections()" << std::endl;
    }

    plVector3 intersectionPoint;
    //std::vector<const plMeshConnectivityDataVert*> newVertsToCheck;

    for (plMeshConnectivityDataEdgeIterator eit1 = _data.edges.begin(); eit1 != _data.edges.end(); eit1++)
    {
        for (plMeshConnectivityDataEdgeIterator eit2 = _data.edges.begin(); eit2 != _data.edges.end(); eit2++)
        {
            const plMeshConnectivityDataEdge* edgePtr1 = &(*eit1);
            const plMeshConnectivityDataEdge* edgePtr2 = &(*eit2);
            if (_intersectionEdgeEdge(edgePtr1,edgePtr2,intersectionPoint,verbose,depth+1))
            {
                _data.addVert(intersectionPoint,0,verbose);
            }
        }
    }

    if (!_findAndFixVertEdgeIntersections(0,0,verbose,depth+1)) return false;
    if (!_findAndFixVertFaceIntersections(0,0,verbose,depth+1)) return false;

    return true;
}

PLbool plMeshIntersector::_findAndFixEdgeFaceIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixEdgeFaceIntersections()" << std::endl;
    }

    plVector3 intersectionPoint;

    for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
    {
        for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
        {
            const plMeshConnectivityDataEdge* edgePtr = &(*eit);
            const plMeshConnectivityDataFace* facePtr = &(*fit);
            if (_intersectionEdgeFace(edgePtr,facePtr,intersectionPoint,verbose,depth+1))
            {
                _data.addVert(intersectionPoint,0,verbose);
            }
        }
    }

    if (!_findAndFixVertEdgeIntersections(0,0,verbose,depth+1)) return false;
    if (!_findAndFixVertFaceIntersections(0,0,verbose,depth+1)) return false;
    if (!_findAndFixEdgeEdgeIntersections(0,0,verbose,depth+1)) return false;

    return true;
}

PLbool plMeshIntersector::intersect( std::vector<plTriangle> &outputTris, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::intersect()" << std::endl;
    }

    if (!_checkForAllErrors(verbose,depth+1)) return false;

    _data.reportSizes();

    PLuint vertsSize(0);
    PLuint edgesSize(0);
    PLuint facesSize(0);

    while (vertsSize != _data.verts.size() || edgesSize != _data.edges.size() || facesSize != _data.faces.size())
    {
        vertsSize = _data.verts.size();
        edgesSize = _data.edges.size();
        facesSize = _data.faces.size();
        if (!_findAndFixVertEdgeIntersections(0,0,verbose,depth+1)) { exportTriSeq(outputTris,verbose,depth+1); return false;}
        if (!_findAndFixVertFaceIntersections(0,0,verbose,depth+1)) { exportTriSeq(outputTris,verbose,depth+1); return false;}
        if (!_findAndFixEdgeEdgeIntersections(0,0,verbose,depth+1)) { exportTriSeq(outputTris,verbose,depth+1); return false;}
        if (!_findAndFixEdgeFaceIntersections(0,0,verbose,depth+1)) { exportTriSeq(outputTris,verbose,depth+1); return false;}
        break;
    }

    // should be good, but for sanity's sake...
    if (!_checkForAllErrors(verbose,depth+1)) { exportTriSeq(outputTris,verbose,depth+1); return false;}

    _data.reportSizes();
    exportTriSeq(outputTris,verbose,depth+1);

    return true;
}
