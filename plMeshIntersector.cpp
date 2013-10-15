#include "plMeshIntersector.h"

PLbool plMeshIntersector::_intersectionVertEdge(const plMeshConnectivityData::plMeshConnectivityDataVert& vert, const plMeshConnectivityData::plMeshConnectivityDataEdge& edge, PLuint verbose, PLuint depth)
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

        barycentricCoords = _data.faces[edge.faceIndices[i]].face.barycentricCoords(vert.vert);
        if ((vert.vert - plVector3(1.965,1.965,2.0)).length() <= 0.01 && ((edge.edge.pt1-plVector3(2,2,2)).length() <= 0.01 || (edge.edge.pt2-plVector3(2,2,2)).length() <= 0.01) )
        { // this whole thing needs checking
           for (PLuint k=0;k<depth;k++)
                std::cout << "\t";
            std::cout << "Edge being considered is " << edge.edge.pt1 << " - " << edge.edge.pt2 << " being split on " << vert.vert << std::endl;
            for (PLuint k=0;k<depth;k++)
                std::cout << "\t";
            std::cout << "For face " << _data.faces[edge.faceIndices[i]].face.point0() << " | " << _data.faces[edge.faceIndices[i]].face.point1() << " | " << _data.faces[edge.faceIndices[i]].face.point2() << std::endl;

            for (PLuint k=0;k<depth;k++)
                std::cout << "\t";
            std::cout << "Barycentric weights: " << barycentricCoords << std::endl;
            plVector3 v0 = (_data.faces[edge.faceIndices[i]].face.point1() - _data.faces[edge.faceIndices[i]].face.point0()).normalize(),
                      v1 = (_data.faces[edge.faceIndices[i]].face.point2() - _data.faces[edge.faceIndices[i]].face.point0()).normalize(),
                      v2 = (vert.vert          - _data.faces[edge.faceIndices[i]].face.point0()).normalize();

            PLfloat d00 = (v0*v0);
            PLfloat d01 = (v0*v1);
            PLfloat d11 = (v1*v1);
            PLfloat d20 = (v2*v0);
            PLfloat d21 = (v2*v1);
            PLfloat denom = d00 * d11 - d01 * d01;
            for (PLuint k=0;k<depth;k++)
                std::cout << "\t";
            std::cout << "denom: " << denom << std::endl;
            PLfloat v = (d11 * d20 - d01 * d21) / denom;
            PLfloat w = (d00 * d21 - d01 * d20) / denom;
            PLfloat u = 1.0f - v - w;
            for (PLuint k=0;k<depth;k++)
                std::cout << "\t";
            std::cout << "params: " << u << " " << v << " " << w << std::endl;
        }
        if (_data.faces[edge.faceIndices[i]].face.contains(vert.vert,_epsilon))
        {
            //for (PLuint i=0;i<depth;i++)
            //    std::cout << "\t";
            //std::cout << "Exited on face contains, " << vert.vert << " is in " << faces[edge.faceIndices[i]].face.point0() << " | " << faces[edge.faceIndices[i]].face.point1() << " | " << faces[edge.faceIndices[i]].face.point2() << " | " << std::endl;
            return false;
        }

        if (edge.edge.pt1 != _data.faces[edge.faceIndices[i]].face.point0() && edge.edge.pt2 != _data.faces[edge.faceIndices[i]].face.point0())
        {
            if ((barycentricCoords.x > barycentricCoords.y || barycentricCoords.x > barycentricCoords.z) && fabs(barycentricCoords.x) > 0.f)
            {
                //for (PLuint i=0;i<depth;i++)
                //    std::cout << "\t";
                //std::cout << "Exited on bary x conditions" << std::endl;
                return false;
            }
        }
        if (edge.edge.pt1 != _data.faces[edge.faceIndices[i]].face.point1() && edge.edge.pt2 != _data.faces[edge.faceIndices[i]].face.point1())
        {
            if ((barycentricCoords.y > barycentricCoords.x || barycentricCoords.y > barycentricCoords.z) && fabs(barycentricCoords.x) > 0.f)
            {
                //for (PLuint i=0;i<depth;i++)
                //    std::cout << "\t";
                //std::cout << "Exited on bary y conditions" << std::endl;
                return false;
            }
        }
        if (edge.edge.pt1 != _data.faces[edge.faceIndices[i]].face.point2() && edge.edge.pt2 != _data.faces[edge.faceIndices[i]].face.point2())
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

PLbool plMeshIntersector::_intersectionEdgeEdge(const plMeshConnectivityData::plMeshConnectivityDataEdge& e1, const plMeshConnectivityData::plMeshConnectivityDataEdge& e2, plVector3& intersection, PLuint verbose, PLuint depth)
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

PLbool plMeshIntersector::_intersectionVertFace(const plMeshConnectivityData::plMeshConnectivityDataVert& vert, const plMeshConnectivityData::plMeshConnectivityDataFace& face, PLuint verbose, PLuint depth)
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

PLbool plMeshIntersector::_intersectionEdgeFace(const plMeshConnectivityData::plMeshConnectivityDataEdge& edge, const plMeshConnectivityData::plMeshConnectivityDataFace& face, plVector3& intersection, PLuint verbose, PLuint depth)
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

    if (endIndex == 0) endIndex = _data.verts.size()-1;
    for (PLuint i = startIndex; i <= endIndex; i++)
    {
        for (PLuint j = 0; j < _data.edges.size(); j++)
        {
            if (_intersectionVertEdge(_data.verts[i],_data.edges[j],verbose,depth+1))
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
                _splitEdgeOnVect(j,_data.verts[i].vert,verbose,depth+1);
                if (!_checkForAllErrors(verbose,depth+1)) { return false;}
                i--;
                break; // restart edge iteration, since it may need to be split again! you never know!
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
    for (PLuint i = startIndex; i <= endIndex; i++)
    {
        for (PLuint j = 0; j < _data.faces.size(); j++)
        {
            if (_intersectionVertFace(_data.verts[i],_data.faces[j],verbose,depth+1))
            {
                _splitFaceOnVect(j,_data.verts[i].vert,verbose,depth+1);
                if (!_checkForAllErrors(verbose,depth+1)) { return false;}
                i--;
                break;
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

    PLuint vertsSizeBeforeOps(_data.verts.size());
    plVector3 intersectionPoint;
    plSeq<plVector3> intersectionPoints;
    if (endIndex == 0) endIndex = _data.edges.size()-1;
    for (PLuint i = startIndex; i <= endIndex; i++)
    {
        for (PLuint j = 0; j < _data.edges.size(); j++)
        {
            if (_intersectionEdgeEdge(_data.edges[j],_data.edges[i],intersectionPoint,verbose,depth+1))
            {
                intersectionPoints.add(intersectionPoint);
            }
        }
    }

    plSeq<PLuint> extraIndicesToCheck;
    for (PLuint i = 0; i < intersectionPoints.size(); i++)
    {
        plMeshConnectivityData::plMeshConnectivityDataVert newVert;
        newVert.vert = intersectionPoints[i];
        PLint searchIndex = (_data.verts.findIndex(newVert));
        //if (!_findVert(newVert.vert,searchIndex,verbose,depth+1)) return false;
        if (searchIndex == -1)
        {
            if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
                for (PLuint k=0;k<depth;k++)
                    std::cout << "\t";
                std::cout << "Debug: Creating new vertex: " << intersectionPoints[i] << std::endl;
            }
            _data.verts.add(newVert);
        }
        else
            extraIndicesToCheck.add( (PLuint)searchIndex );
    }

    for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
    {
        if (!_findAndFixVertEdgeIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
    }
    if (!_findAndFixVertEdgeIntersections(vertsSizeBeforeOps,_data.verts.size()-1,verbose,depth+1)) return false;
    for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
    {
        if (!_findAndFixVertFaceIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
    }
    if (!_findAndFixVertFaceIntersections(vertsSizeBeforeOps,_data.verts.size()-1,verbose,depth+1)) return false;

    return true;
}

PLbool plMeshIntersector::_findAndFixEdgeFaceIntersections(PLuint startIndex, PLuint endIndex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findAndFixEdgeFaceIntersections()" << std::endl;
    }

    PLuint vertsSizeBeforeOps(_data.verts.size());
    PLuint edgesSizeBeforeOps(_data.edges.size());
    plVector3 intersectionPoint;
    plSeq<plVector3> intersectionPoints;
    if (endIndex == 0) endIndex = _data.edges.size()-1;
    for (PLuint i = startIndex; i <= endIndex; i++)
    {
        for (PLuint j = 0; j < _data.faces.size(); j++)
        {
            if (_intersectionEdgeFace(_data.edges[i],_data.faces[j],intersectionPoint,verbose,depth+1))
            {
                intersectionPoints.add(intersectionPoint);
            }
        }
    }

    plSeq<PLuint> extraIndicesToCheck;
    for (PLuint i = 0; i < intersectionPoints.size(); i++)
    {
        plMeshConnectivityData::plMeshConnectivityDataVert newVert;
        newVert.vert = intersectionPoints[i];
        PLint searchIndex = (_data.verts.findIndex(newVert));
        //if (!_findVert(newVert.vert,searchIndex,verbose,depth+1)) return false;
        if (searchIndex == -1)
        {
            if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
                for (PLuint k=0;k<depth;k++)
                    std::cout << "\t";
                std::cout << "Debug: Creating new vertex: " << intersectionPoints[i] << std::endl;
            }
            _data.verts.add(newVert);
        }
        else
            extraIndicesToCheck.add( (PLuint)searchIndex );
    }

    for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
    {
        if (!_findAndFixVertEdgeIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
    }
    if(!_findAndFixVertEdgeIntersections(vertsSizeBeforeOps,_data.verts.size()-1,verbose,depth+1)) return false;
    for (PLuint i = 0; i < extraIndicesToCheck.size(); i++)
    {
        if (!_findAndFixVertFaceIntersections(extraIndicesToCheck[i],extraIndicesToCheck[i],verbose,depth+1)) return false;
    }
    if (!_findAndFixVertFaceIntersections(vertsSizeBeforeOps,_data.verts.size()-1,verbose,depth+1)) return false;
    return true;

    if (!_findAndFixEdgeEdgeIntersections(edgesSizeBeforeOps,(_data.edges.size()-1),verbose,depth+1)) return false;

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
