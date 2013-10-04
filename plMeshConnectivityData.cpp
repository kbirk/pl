#include "plMeshConnectivityData.h"

PLbool plMeshConnectivityData::plMeshConnectivityDataVert::operator ==(const plMeshConnectivityDataVert &other)
{
    if (vert == other.vert)
        return true;
    return false;
}

PLbool plMeshConnectivityData::plMeshConnectivityDataEdge::operator ==(const plMeshConnectivityDataEdge& other)
{
    if (edge == other.edge)
        return true;
    return false;
}

PLbool plMeshConnectivityData::plMeshConnectivityDataFace::operator ==(const plMeshConnectivityDataFace& other)
{
    if (face == other.face)
        return true;
    return false;
}

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityData::plMeshConnectivityDataVert &vert )
{
    stream << vert.vert << "\n";
    stream << "Edge Indices:";
    for (PLuint i = 0; i < vert.edgeIndices.size(); i++)
        stream << "\n" << vert.edgeIndices[i];
    stream << "\nFace Indices:";
    for (PLuint i = 0; i < vert.faceIndices.size(); i++)
        stream << "\n" << vert.faceIndices[i];
}

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityData::plMeshConnectivityDataEdge &edge )
{
    stream << edge.edge.pt1 << " - " << edge.edge.pt2 << "\n";
    stream << "Vert Indices:";
    for (PLuint i = 0; i < edge.vertIndices.size(); i++)
        stream << "\n" << edge.vertIndices[i];
    stream << "\nFace Indices:";
    for (PLuint i = 0; i < edge.faceIndices.size(); i++)
        stream << "\n" << edge.faceIndices[i];
}

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityData::plMeshConnectivityDataFace &face )
{
    stream << face.face;
    stream << "Vert Indices:";
    for (PLuint i = 0; i < face.vertIndices.size(); i++)
        stream << "\n" << face.vertIndices[i];
    stream << "\nEdge Indices:";
    for (PLuint i = 0; i < face.edgeIndices.size(); i++)
        stream << "\n" << face.edgeIndices[i];
}

