#include "plMeshIntersector.h"

PLbool plMeshIntersector::_intersectionVertEdge(const plMeshConnectivityDataVert& vert, const plMeshConnectivityDataEdge& edge, PLuint verbose, PLuint depth)
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
    for (PLuint i = 0; i < edge.faces.size();i++)
    {

        barycentricCoords = edge.faces[i]->face.barycentricCoords(vert.vert);
        if ((vert.vert - plVector3(1.965,1.965,2.0)).length() <= 0.01 && ((edge.edge.pt1-plVector3(2,2,2)).length() <= 0.01 || (edge.edge.pt2-plVector3(2,2,2)).length() <= 0.01) )
        { // this whole thing needs checking
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Edge being considered is " << edge.edge.pt1 << " - " << edge.edge.pt2 << " being split on " << vert.vert << std::endl;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "For face " << edge.faces[i]->face.point0() << " | " << edge.faces[i]->face.point1() << " | " << edge.faces[i]->face.point2() << std::endl;

            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Barycentric weights: " << barycentricCoords << std::endl;
            plVector3 v0 = (edge.faces[i]->face.point1() - edge.faces[i]->face.point0()).normalize(),
                      v1 = (edge.faces[i]->face.point2() - edge.faces[i]->face.point0()).normalize(),
                      v2 = (vert.vert                          - edge.faces[i]->face.point0()).normalize();

            PLfloat d00 = (v0*v0);
            PLfloat d01 = (v0*v1);
            PLfloat d11 = (v1*v1);
            PLfloat d20 = (v2*v0);
            PLfloat d21 = (v2*v1);
            PLfloat denom = d00 * d11 - d01 * d01;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "denom: " << denom << std::endl;
            PLfloat v = (d11 * d20 - d01 * d21) / denom;
            PLfloat w = (d00 * d21 - d01 * d20) / denom;
            PLfloat u = 1.0f - v - w;
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "params: " << u << " " << v << " " << w << std::endl;
        }
        if (edge.faces[i]->face.contains(vert.vert,_epsilon))
        {
            //for (PLuint i=0;i<depth;i++)
            //    std::cout << "\t";
            //std::cout << "Exited on face contains, " << vert.vert << " is in " << faces[edge.faceIndices[i]].face.point0() << " | " << faces[edge.faceIndices[i]].face.point1() << " | " << faces[edge.faceIndices[i]].face.point2() << " | " << std::endl;
            return false;
        }

        if (edge.edge.pt1 != edge.faces[i]->face.point0() && edge.edge.pt2 != edge.faces[i]->face.point0())
        {
            if ((barycentricCoords.x > barycentricCoords.y || barycentricCoords.x > barycentricCoords.z) && fabs(barycentricCoords.x) > 0.f)
            {
                //for (PLuint i=0;i<depth;i++)
                //    std::cout << "\t";
                //std::cout << "Exited on bary x conditions" << std::endl;
                return false;
            }
        }
        if (edge.edge.pt1 != edge.faces[i]->face.point1() && edge.edge.pt2 != edge.faces[i]->face.point1())
        {
            if ((barycentricCoords.y > barycentricCoords.x || barycentricCoords.y > barycentricCoords.z) && fabs(barycentricCoords.x) > 0.f)
            {
                //for (PLuint i=0;i<depth;i++)
                //    std::cout << "\t";
                //std::cout << "Exited on bary y conditions" << std::endl;
                return false;
            }
        }
        if (edge.edge.pt1 != edge.faces[i]->face.point2() && edge.edge.pt2 != edge.faces[i]->face.point2())
        {
            if ((barycentricCoords.z > barycentricCoords.x || barycentricCoords.z > barycentricCoords.y) && fabs(barycentricCoords.x) > 0.f)
            {
                //for (PLuint i=0;i<depth;i++)
                //    std::cout << "\t";
                //std::cout << "Exited on bary z conditions" << std::endl;
                return false;
            }
        }
    }

    return true;
}

PLbool plMeshIntersector::_intersectionEdgeEdge(const plMeshConnectivityDataEdge& e1, const plMeshConnectivityDataEdge& e2, plVector3& intersection, PLuint verbose, PLuint depth)
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

PLbool plMeshIntersector::_intersectionVertFace(const plMeshConnectivityDataVert& vert, const plMeshConnectivityDataFace& face, PLuint verbose, PLuint depth)
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
    PLdouble denom = d00 * d11 - d01 * d01;
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "denom: " << denom << std::endl;
    PLfloat v = (d11 * d20 - d01 * d21) / denom;
    PLfloat w = (d00 * d21 - d01 * d20) / denom;
    PLfloat u = 1.0f - v - w;
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "params: " << u << " " << v << " " << w << std::endl;*/

    // on the edge, don't consider as an intersection
    if ((plMath::closestPointOnSegment(vert.vert,face.face.point0(),face.face.point1()) - vert.vert).length() <= _epsilon ||
        (plMath::closestPointOnSegment(vert.vert,face.face.point1(),face.face.point2()) - vert.vert).length() <= _epsilon ||
        (plMath::closestPointOnSegment(vert.vert,face.face.point2(),face.face.point0()) - vert.vert).length() <= _epsilon )
        return false;

    return true;
}

PLbool plMeshIntersector::_intersectionEdgeFace(const plMeshConnectivityDataEdge& edge, const plMeshConnectivityDataFace& face, plVector3& intersection, PLuint verbose, PLuint depth)
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


PLbool plMeshIntersector::_findAndFixVertEdgeIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixVertEdgeIntersections()" << std::endl;
    }

    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); vit++)
    {
        for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
        {
            if (_intersectionVertEdge((*vit),(*eit),verbose,depth+1))
            {
                _splitEdgeOnVect(&(*eit),&(*vit),verbose,depth+1);
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

    if (endIndex == 0) endIndex = _data.verts.size()-1;
    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); vit++)
    {
        for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
        {
            if (_intersectionVertFace((*vit),(*fit),verbose,depth+1))
            {
                _splitFaceOnVect(&(*fit),&(*vit),verbose,depth+1);
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
    //plSeq<const plMeshConnectivityDataVert*> newVertsToCheck;

    for (plMeshConnectivityDataEdgeIterator eit1 = _data.edges.begin(); eit1 != _data.edges.end(); eit1++)
    {
        for (plMeshConnectivityDataEdgeIterator eit2 = _data.edges.begin(); eit2 != _data.edges.end(); eit2++)
        {
            if (_intersectionEdgeEdge((*eit1),(*eit2),intersectionPoint,verbose,depth+1))
            {
                plMeshConnectivityDataVert newVert;
                newVert.vert = intersectionPoint;
                _data.verts.insert(newVert);
                //newVertsToCheck.add(&(*(_data.verts.find(newVert))));
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
            if (_intersectionEdgeFace((*eit),(*fit),intersectionPoint,verbose,depth+1))
            {
                plMeshConnectivityDataVert newVert;
                newVert.vert = intersectionPoint;
                _data.verts.insert(newVert);
            }
        }
    }

    if (!_findAndFixVertEdgeIntersections(0,0,verbose,depth+1)) return false;
    if (!_findAndFixVertFaceIntersections(0,0,verbose,depth+1)) return false;
    if (!_findAndFixEdgeEdgeIntersections(0,0,verbose,depth+1)) return false;

    return true;
}

PLbool plMeshIntersector::intersect(const plSeq<plTriangle> &inputTris, plSeq<plTriangle> &outputTris, PLuint verbose, PLuint depth )
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

    while (vertsSize != _data.verts.size() || edgesSize != _data.edges.size() || facesSize != _data.faces.size())
    {
        vertsSize = _data.verts.size();
        edgesSize = _data.edges.size();
        facesSize = _data.faces.size();
        if (!_findAndFixVertEdgeIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
        if (!_findAndFixVertFaceIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
        if (!_findAndFixEdgeEdgeIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
        if (!_findAndFixEdgeFaceIntersections(0,0,verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}
        break;
    }

    // should be good, but for sanity's sake...
    if (!_checkForAllErrors(verbose,depth+1)) { _exportTriSeq(outputTris,verbose,depth+1); return false;}

    _reportSizes(verbose,depth+1);
    _exportTriSeq(outputTris,verbose,depth+1);

    return true;
}
