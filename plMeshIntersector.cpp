#include "plMeshIntersector.h"

PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataVert::operator==(const plMeshIntersectorConnectivityDataVert &other)
{
    if (vert == other.vert)
        return true;
    return false;
}

PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::intersectsVert(const plMeshIntersectorConnectivityDataVert &vert)
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

PLbool plMeshIntersectorConnectivityData::plMeshIntersectorConnectivityDataEdge::operator ==(const plMeshIntersectorConnectivityDataEdge& other)
{
    if (edge == other.edge)
        return true;
    return false;
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
}

PLbool plMeshIntersectorConnectivityData::splitEdgeOnVect( PLuint edgeIndex, plVector3& vertex )
{
    // two tasks here:
    // split the current edge into two pieces
    // split any attached faces each into two pieces
    return true;
}

// add a vertex somewhere in the middle of the triangle, then divide the triangle into three smaller triangles.
PLbool plMeshIntersectorConnectivityData::splitFaceOnVect( PLuint faceIndex, plVector3& vertex )
{
    // find all existing cells, have them available in case they're needed later
    plMeshIntersectorConnectivityDataFace face = faces[faceIndex]; // this will eventually be removed from the list of faces

    plMeshIntersectorConnectivityDataVert& vert0;
    plMeshIntersectorConnectivityDataVert& vert1;
    plMeshIntersectorConnectivityDataVert& vert2;
    PLuint vert0index;
    PLuint vert1index;
    PLuint vert2index;
    for (PLuint i = 0; i < face.vertIndices.size(); i++)
    {
        if (verts[face.vertIndices[i]].vert == face.face.point0())
        {
            vert0 = verts[face.vertIndices[i]].vert;
            vert0index = face.vertIndices[i];
        }
        if (verts[face.vertIndices[i]].vert == face.face.point1())
        {
            vert1 = verts[face.vertIndices[i]].vert;
            vert1index = face.vertIndices[i];
        }
        if (verts[face.vertIndices[i]].vert == face.face.point2())
        {
            vert2 = verts[face.vertIndices[i]].vert;
            vert2index = face.vertIndices[i];
        }
    }

    plMeshIntersectorConnectivityDataEdge& edge01;
    plMeshIntersectorConnectivityDataEdge& edge12;
    plMeshIntersectorConnectivityDataEdge& edge20;
    PLuint edge01index;
    PLuint edge12index;
    PLuint edge20index;
    for (PLuint i = 0; i < face.edgeIndices.size(); i++)
    {
        if (edges[face.edgeIndices[i]].edge.contains(face.face.point0()) &&
            edges[face.edgeIndices[i]].edge.contains(face.face.point1()) )
        {
            edge01 = edges[face.edgeIndices[i]];
            edge01index = face.edgeIndices[i];
        }
        if (edges[face.edgeIndices[i]].edge.contains(face.face.point1()) &&
            edges[face.edgeIndices[i]].edge.contains(face.face.point2()) )
        {
            edge12 = edges[face.edgeIndices[i]];
            edge12index = face.edgeIndices[i];
        }
        if (edges[face.edgeIndices[i]].edge.contains(face.face.point2()) &&
            edges[face.edgeIndices[i]].edge.contains(face.face.point0()) )
        {
            edge20 = edges[face.edgeIndices[i]];
            edge20index = face.edgeIndices[i];
        }
    }

    // create the new cells, storing their eventual indices
    plMeshIntersectorConnectivityDataVert vertN; // N = New
    PLuint vertNindex = verts.size();

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
    vertN.vert = vertex;
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

    // add the cells to our arrays
    verts.add(vertN);

    faces[faceIndex] = face01N;
    faces.add(face12N);
    faces.add(face20N);

    edges.add(edgeN0);
    edges.add(edgeN1);
    edges.add(edgeN2);

    return true;
}


