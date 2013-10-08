#include "plMeshConnectivityData.h"

PLbool plMeshConnectivityDataVert::operator==(const plMeshConnectivityDataVert &other) const
{
    if (vert == other.vert)
        return true;
    return false;
}

PLbool plMeshConnectivityDataEdge::operator==(const plMeshConnectivityDataEdge& other) const
{
    if (edge == other.edge)
        return true;
    return false;
}

PLbool plMeshConnectivityDataFace::operator==(const plMeshConnectivityDataFace& other) const
{
    if (face == other.face)
        return true;
    return false;
}

PLbool plMeshConnectivityDataVert::operator<(const plMeshConnectivityDataVert &other) const
{
    if (originatingMesh < other.originatingMesh)
        return true;
    if (originatingMesh > other.originatingMesh)
        return false;

    if (vert.x < other.vert.x)
        return true;
    if (vert.x > other.vert.x)
        return false;

    if (vert.y < other.vert.y)
        return true;
    if (vert.y > other.vert.y)
        return false;

    if (vert.z < other.vert.z)
        return true;
    //if (vert.z > other.vert.z) // implied
        //return false;

    return false;
}

PLbool plMeshConnectivityDataVert::operator>(const plMeshConnectivityDataVert &other) const
{
    if (originatingMesh > other.originatingMesh)
        return true;
    if (originatingMesh < other.originatingMesh)
        return false;

    if (vert.x > other.vert.x)
        return true;
    if (vert.x < other.vert.x)
        return false;

    if (vert.y > other.vert.y)
        return true;
    if (vert.y < other.vert.y)
        return false;

    if (vert.z > other.vert.z)
        return true;
    //if (vert.z < other.vert.z) // implied
        //return false;

    return false;
}

PLbool plMeshConnectivityDataEdge::operator<(const plMeshConnectivityDataEdge& other) const
{
    if (originatingMesh < other.originatingMesh)
        return true;
    if (originatingMesh > other.originatingMesh)
        return false;

    const plMeshConnectivityDataVert* orderedVertsThis [2] = { NULL, NULL };
    const plMeshConnectivityDataVert* orderedVertsOther[2] = { NULL, NULL };

    // now detect which vertex is least
    if ( *(verts[0]) < *(verts[1]) )
    {
        orderedVertsThis[0] = verts[0];
        orderedVertsThis[1] = verts[1];
    }
    else if ( *(verts[1]) < *(verts[0]) )
    {
        orderedVertsThis[0] = verts[1];
        orderedVertsThis[1] = verts[0];
    }
    else // shouldn't happen
        return false;

    if ( *(other.verts[0]) < *(other.verts[1]) )
    {
        orderedVertsThis[0] = other.verts[0];
        orderedVertsThis[1] = other.verts[1];
    }
    else if ( *(other.verts[1]) < *(other.verts[0]) )
    {
        orderedVertsThis[0] = other.verts[1];
        orderedVertsThis[1] = other.verts[0];
    }
    else // shouldn't happen
        return false;


    // compare least to least, if undecided, then try greater to greater
    if ( *(orderedVertsThis[0]) < *(orderedVertsOther[0]) )
        return true;
    else if ( *(orderedVertsThis[0]) > *(orderedVertsOther[0]) )
        return false;

    if ( *(orderedVertsThis[1]) < *(orderedVertsOther[1]) )
        return true;
    // other case for > is implied

    return false;
}

PLbool plMeshConnectivityDataFace::operator<(const plMeshConnectivityDataFace& other) const
{
    if (originatingMesh < other.originatingMesh)
        return true;
    if (originatingMesh > other.originatingMesh)
        return false;

    const plMeshConnectivityDataVert* orderedVertsThis [3] = { NULL, NULL, NULL };
    const plMeshConnectivityDataVert* orderedVertsOther[3] = { NULL, NULL, NULL };

    // now detect which vertex is least
    if ( *(verts[0]) < *(verts[1]) )
    {
        if ( *(verts[0]) < *(verts[2]) )
        {
            if ( *(verts[1]) < *(verts[2]) )
            {
                orderedVertsThis[0] = verts[0];
                orderedVertsThis[1] = verts[1];
                orderedVertsThis[2] = verts[2];
            }
            else if ( *(verts[2]) < *(verts[1]) )
            {
                orderedVertsThis[0] = verts[0];
                orderedVertsThis[1] = verts[2];
                orderedVertsThis[2] = verts[1];
            }
        }
        else if ( *(verts[2]) < *(verts[0]) )
        {
            orderedVertsThis[0] = verts[2];
            orderedVertsThis[1] = verts[0];
            orderedVertsThis[2] = verts[1];
        }
    }
    else if ( *(verts[1]) < *(verts[0]) )
    {
        if ( *(verts[2]) < *(verts[0]) )
        {
            if ( *(verts[1]) < *(verts[2]) )
            {
                orderedVertsThis[0] = verts[1];
                orderedVertsThis[1] = verts[2];
                orderedVertsThis[2] = verts[0];
            }
            else if ( *(verts[2]) < *(verts[1]) )
            {
                orderedVertsThis[0] = verts[2];
                orderedVertsThis[1] = verts[1];
                orderedVertsThis[2] = verts[0];
            }
        }
        else if ( *(verts[0]) < *(verts[2]) )
        {
            orderedVertsThis[0] = verts[1];
            orderedVertsThis[1] = verts[0];
            orderedVertsThis[2] = verts[2];
        }
    }

    if ( *(verts[0]) < *(verts[1]) )
    {
        if ( *(verts[0]) < *(verts[2]) )
        {
            if ( *(verts[1]) < *(verts[2]) )
            {
                orderedVertsOther[0] = verts[0];
                orderedVertsOther[1] = verts[1];
                orderedVertsOther[2] = verts[2];
            }
            else if ( *(verts[2]) < *(verts[1]) )
            {
                orderedVertsOther[0] = verts[0];
                orderedVertsOther[1] = verts[2];
                orderedVertsOther[2] = verts[1];
            }
        }
        else if ( *(verts[2]) < *(verts[0]) )
        {
            orderedVertsOther[0] = verts[2];
            orderedVertsOther[1] = verts[0];
            orderedVertsOther[2] = verts[1];
        }
    }
    else if ( *(verts[1]) < *(verts[0]) )
    {
        if ( *(verts[2]) < *(verts[0]) )
        {
            if ( *(verts[1]) < *(verts[2]) )
            {
                orderedVertsOther[0] = verts[1];
                orderedVertsOther[1] = verts[2];
                orderedVertsOther[2] = verts[0];
            }
            else if ( *(verts[2]) < *(verts[1]) )
            {
                orderedVertsOther[0] = verts[2];
                orderedVertsOther[1] = verts[1];
                orderedVertsOther[2] = verts[0];
            }
        }
        else if ( *(verts[0]) < *(verts[2]) )
        {
            orderedVertsOther[0] = verts[1];
            orderedVertsOther[1] = verts[0];
            orderedVertsOther[2] = verts[2];
        }
    }

    // compare least to least, if undecided, then try greater to greater
    if ( *(orderedVertsThis[0]) < *(orderedVertsOther[0]) )
        return true;
    else if ( *(orderedVertsThis[0]) > *(orderedVertsOther[0]) )
        return false;

    if ( *(orderedVertsThis[1]) < *(orderedVertsOther[1]) )
        return true;
    else if ( *(orderedVertsThis[1]) > *(orderedVertsOther[1]) )
        return false;

    if ( *(orderedVertsThis[2]) < *(orderedVertsOther[2]) )
        return true;
    else if ( *(orderedVertsThis[2]) > *(orderedVertsOther[2]) )
        return false;

    // if it really gets this far, then we have to use normals... :-(
    if (face.normal().x < other.face.normal().x)
        return true;
    else if (face.normal().x > other.face.normal().x)
        return false;

    if (face.normal().y < other.face.normal().y)
        return true;
    else if (face.normal().z > other.face.normal().z)
        return false;

    if (face.normal().z < other.face.normal().z)
        return true;
    //else if (face.normal().z > other.face.normal().z)
    //    return false;

    return false;
}

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataVert &vert )
{
    stream << vert.vert << "\n";
    stream << "Edge Indices:";
    for (PLuint i = 0; i < vert.edges.size(); i++)
        stream << "\n" << vert.edges[i];
    stream << "\nFace Indices:";
    for (PLuint i = 0; i < vert.faces.size(); i++)
        stream << "\n" << vert.faces[i];
    return stream;
}

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataEdge &edge )
{
    stream << edge.edge.pt1 << " - " << edge.edge.pt2 << "\n";
    stream << "Vert Indices:";
    for (PLuint i = 0; i < edge.verts.size(); i++)
        stream << "\n" << edge.verts[i];
    stream << "\nFace Indices:";
    for (PLuint i = 0; i < edge.faces.size(); i++)
        stream << "\n" << edge.faces[i];
    return stream;
}

std::ostream& operator << ( std::ostream &stream, const plMeshConnectivityDataFace &face )
{
    stream << face.face;
    stream << "Vert Indices:";
    for (PLuint i = 0; i < face.verts.size(); i++)
        stream << "\n" << face.verts[i];
    stream << "\nEdge Indices:";
    for (PLuint i = 0; i < face.edges.size(); i++)
        stream << "\n" << face.edges[i];
    return stream;
}

