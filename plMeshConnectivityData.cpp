#include "plMeshConnectivityData.h"

plMeshConnectivityDataVert::plMeshConnectivityDataVert( const plMeshConnectivityDataVert &other )
{
    *this = other;
}

plMeshConnectivityDataEdge::plMeshConnectivityDataEdge( const plMeshConnectivityDataEdge &other )
{
    *this = other;
}

plMeshConnectivityDataFace::plMeshConnectivityDataFace( const plMeshConnectivityDataFace &other )
{
    *this = other;
}

PLbool plMeshConnectivityDataVert::operator==(const plMeshConnectivityDataVert &other) const
{
    if (originatingMesh == other.originatingMesh && vert == other.vert)
        return true;
    return false;
}

PLbool plMeshConnectivityDataEdge::operator==(const plMeshConnectivityDataEdge& other) const
{
    if (originatingMesh == other.originatingMesh && edge == other.edge)
        return true;
    return false;
}

PLbool plMeshConnectivityDataFace::operator==(const plMeshConnectivityDataFace& other) const
{
    if (originatingMesh == other.originatingMesh && face == other.face)
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
        orderedVertsOther[0] = other.verts[0];
        orderedVertsOther[1] = other.verts[1];
    }
    else if ( *(other.verts[1]) < *(other.verts[0]) )
    {
        orderedVertsOther[0] = other.verts[1];
        orderedVertsOther[1] = other.verts[0];
    }
    else // shouldn't happen
        return false;

    // compare least to least, if undecided, then try greater to greater
    if ( *(orderedVertsThis[0]) < *(orderedVertsOther[0]) )
        return true;
    else if ( *(orderedVertsOther[0]) < *(orderedVertsThis[0]) )
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

    if ( *(other.verts[0]) < *(other.verts[1]) )
    {
        if ( *(other.verts[0]) < *(other.verts[2]) )
        {
            if ( *(other.verts[1]) < *(other.verts[2]) )
            {
                orderedVertsOther[0] = other.verts[0];
                orderedVertsOther[1] = other.verts[1];
                orderedVertsOther[2] = other.verts[2];
            }
            else if ( *(other.verts[2]) < *(other.verts[1]) )
            {
                orderedVertsOther[0] = other.verts[0];
                orderedVertsOther[1] = other.verts[2];
                orderedVertsOther[2] = other.verts[1];
            }
        }
        else if ( *(other.verts[2]) < *(other.verts[0]) )
        {
            orderedVertsOther[0] = other.verts[2];
            orderedVertsOther[1] = other.verts[0];
            orderedVertsOther[2] = other.verts[1];
        }
    }
    else if ( *(other.verts[1]) < *(other.verts[0]) )
    {
        if ( *(other.verts[2]) < *(other.verts[0]) )
        {
            if ( *(other.verts[1]) < *(other.verts[2]) )
            {
                orderedVertsOther[0] = other.verts[1];
                orderedVertsOther[1] = other.verts[2];
                orderedVertsOther[2] = other.verts[0];
            }
            else if ( *(other.verts[2]) < *(other.verts[1]) )
            {
                orderedVertsOther[0] = other.verts[2];
                orderedVertsOther[1] = other.verts[1];
                orderedVertsOther[2] = other.verts[0];
            }
        }
        else if ( *(other.verts[0]) < *(other.verts[2]) )
        {
            orderedVertsOther[0] = other.verts[1];
            orderedVertsOther[1] = other.verts[0];
            orderedVertsOther[2] = other.verts[2];
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
    else if (face.normal().y > other.face.normal().y)
        return false;

    if (face.normal().z < other.face.normal().z)
        return true;
    //else if (face.normal().z > other.face.normal().z)
    //    return false;

    return false;
}

plMeshConnectivityDataVert& plMeshConnectivityDataVert::operator=(const plMeshConnectivityDataVert& other)
{
    vert = other.vert;
    originatingMesh = other.originatingMesh;
    edges.clear();
    for (PLuint i = 0; i < other.edges.size(); i++)
        edges.add(other.edges[i]);
    faces.clear();
    for (PLuint i = 0; i < other.faces.size(); i++)
        faces.add(other.faces[i]);
    dataset = other.dataset;
    return *this;
}

plMeshConnectivityDataEdge& plMeshConnectivityDataEdge::operator=(const plMeshConnectivityDataEdge& other)
{
    edge = other.edge;
    originatingMesh = other.originatingMesh;
    verts.clear();
    for (PLuint i = 0; i < other.verts.size(); i++)
        verts.add(other.verts[i]);
    faces.clear();
    for (PLuint i = 0; i < other.faces.size(); i++)
        faces.add(other.faces[i]);
    dataset = other.dataset;
    return *this;
}

plMeshConnectivityDataFace& plMeshConnectivityDataFace::operator=(const plMeshConnectivityDataFace& other)
{
    face = other.face;
    originatingMesh = other.originatingMesh;
    verts.clear();
    for (PLuint i = 0; i < other.verts.size(); i++)
        verts.add(other.verts[i]);
    edges.clear();
    for (PLuint i = 0; i < other.edges.size(); i++)
        edges.add(other.edges[i]);
    dataset = other.dataset;
    return *this;
}

const plMeshConnectivityDataVert* plMeshConnectivityData::addVert( const plVector3 &vert , PLuint originatingMesh )
{
    plMeshConnectivityDataVert vertToAdd;
    vertToAdd.vert = vert;
    vertToAdd.originatingMesh = originatingMesh;
    vertToAdd.dataset = this;
    const plMeshConnectivityDataVert* vertPtr;
    if (verts.find(vertToAdd) == verts.end())
    {
        verts.insert(vertToAdd);
    }
    else
    {
        std::cout << "Warning in plMeshConnectivityData::addVert(): Vertex " << vert << " already exists. Returning it." << std::endl;
    }
    vertPtr = &(*(verts.find(vertToAdd)));
    return vertPtr;
}

const plMeshConnectivityDataEdge* plMeshConnectivityData::addEdge(const plMeshConnectivityDataVert* vert0, const plMeshConnectivityDataVert* vert1, PLuint originatingMesh)
{
    // sort the vertices. They should be in sorted order such that pt1 < pt2
    const plMeshConnectivityDataVert* orderedVerts[2] = { NULL, NULL };
    if ( *vert0 < *vert1 )
    {
        orderedVerts[0] = vert0;
        orderedVerts[1] = vert1;
    }
    else if ( *vert1 < *vert0 )
    {
        orderedVerts[0] = vert1;
        orderedVerts[1] = vert0;
    }
    else // shouldn't happen
    {
        std::cout << "Error in plMeshConnectivityData::addEdge(): vert0 and vert1 appear to be the same. Returning NULL." << std::endl;
        return NULL;
    }

    // now set up the new element
    plMeshConnectivityDataEdge edgeToAdd;
    edgeToAdd.edge = plEdge(orderedVerts[0]->vert,orderedVerts[1]->vert);
    edgeToAdd.verts.add(orderedVerts[0]);
    edgeToAdd.verts.add(orderedVerts[1]);
    edgeToAdd.originatingMesh = originatingMesh;
    edgeToAdd.dataset = this;
    const plMeshConnectivityDataEdge* edgePtr;
    if (edges.find(edgeToAdd) == edges.end())
    {
        edges.insert(edgeToAdd);
        edgePtr = &(*(edges.find(edgeToAdd)));
        // update the verts that were provided as input.
        vert0->edges.add(edgePtr);
        vert1->edges.add(edgePtr);
    }
    else
    {
        std::cout << "Warning in plMeshConnectivityData::addEdge(): Edge between verts " << vert0 << " and " << vert1 << " already exists. Returning it without updating other cells." << std::endl;
        edgePtr = &(*(edges.find(edgeToAdd)));
    }

    return edgePtr;
}

const plMeshConnectivityDataFace* plMeshConnectivityData::addFace(const plMeshConnectivityDataVert* vert0, const plMeshConnectivityDataVert* vert1, const plMeshConnectivityDataVert* vert2, const plMeshConnectivityDataEdge* edge01, const plMeshConnectivityDataEdge* edge12, const plMeshConnectivityDataEdge* edge20, PLuint originatingMesh)
{
    // sort the vertices, they should be sorted in order such that point0 < point1 and point0 < point2
    // points should maintain the same counter-clockwise orientation
    const plMeshConnectivityDataVert* orderedVerts[3] = { NULL, NULL, NULL };
    const plMeshConnectivityDataEdge* orderedEdges[3] = { NULL, NULL, NULL };
    if ( *vert0 < *vert1 && *vert0 < *vert2 )
    {
        orderedVerts[0] = vert0;
        orderedVerts[1] = vert1;
        orderedVerts[2] = vert2;
        orderedEdges[0] = edge01;
        orderedEdges[1] = edge12;
        orderedEdges[2] = edge20;
    }
    else if ( *vert1 < *vert0 && *vert1 < *vert2 )
    {
        orderedVerts[0] = vert1;
        orderedVerts[1] = vert2;
        orderedVerts[2] = vert0;
        orderedEdges[0] = edge12;
        orderedEdges[1] = edge20;
        orderedEdges[2] = edge01;
    }
    else if ( *vert2 < *vert0 && *vert2 < *vert1 )
    {
        orderedVerts[0] = vert2;
        orderedVerts[1] = vert0;
        orderedVerts[2] = vert1;
        orderedEdges[0] = edge20;
        orderedEdges[1] = edge01;
        orderedEdges[2] = edge12;
    }
    else // shouldn't happen
    {
        std::cout << "Error in plMeshConnectivityData::addFace(): the two 'smallest' verts appear to be the same. Returning NULL." << std::endl;
        return NULL;
    }

    // now set up the new element
    plMeshConnectivityDataFace faceToAdd;
    faceToAdd.face = plTriangle(orderedVerts[0]->vert,orderedVerts[1]->vert,orderedVerts[2]->vert);
    faceToAdd.verts.add(orderedVerts[0]);
    faceToAdd.verts.add(orderedVerts[1]);
    faceToAdd.verts.add(orderedVerts[2]);
    faceToAdd.edges.add(orderedEdges[0]);
    faceToAdd.edges.add(orderedEdges[1]);
    faceToAdd.edges.add(orderedEdges[2]);
    faceToAdd.originatingMesh = originatingMesh;
    faceToAdd.dataset = this;
    const plMeshConnectivityDataFace* facePtr;
    if (faces.find(faceToAdd) == faces.end())
    {
        faces.insert(faceToAdd);
        facePtr = &(*(faces.find(faceToAdd)));
        // update the verts that were provided as input.
        vert0->faces.add(facePtr);
        vert1->faces.add(facePtr);
        vert2->faces.add(facePtr);
        edge01->faces.add(facePtr);
        edge12->faces.add(facePtr);
        edge20->faces.add(facePtr);
    }
    else
    {
        std::cout << "Warning in plMeshConnectivityData::addFace(): Face of verts " << vert0 << ", " << vert1 << ", and " << vert2 << "already exists. Returning it without updating other cells." << std::endl;
        facePtr = &(*(faces.find(faceToAdd)));
    }

    return facePtr;
}

void plMeshConnectivityData::removeVert(const plMeshConnectivityDataVert* vert)
{
    // iterate through edges, remove references to this vert
    for (PLuint i = 0; i < vert->edges.size(); i++)
    {
        PLint indexOfVert = vert->edges[i]->verts.findIndex(vert);
        if (indexOfVert != -1)
            vert->edges.remove(indexOfVert);
        else
            std::cout << "Warning in plMeshConnectivityData::removeVert(): Could not find the vert provided as input " << vert << " in edge " << vert->edges[i] << ". Ignoring " << std::endl;
    }
    // iterate through faces, remove references to this vert
    for (PLuint i = 0; i < vert->faces.size(); i++)
    {
        PLint indexOfVert = vert->faces[i]->verts.findIndex(vert);
        if (indexOfVert != -1)
            vert->faces.remove(indexOfVert);
        else
            std::cout << "Warning in plMeshConnectivityData::removeVert(): Could not find the vert provided as input " << vert << " in face " << vert->faces[i] << ". Ignoring " << std::endl;
    }
    // remove this vert
    verts.erase(*vert);
}

void plMeshConnectivityData::removeEdge(const plMeshConnectivityDataEdge* edge)
{
    // iterate through verts, remove references to this edge
    for (PLuint i = 0; i < edge->verts.size(); i++)
    {
        PLint indexOfEdge = edge->verts[i]->edges.findIndex(edge);
        if (indexOfEdge != -1)
            edge->verts.remove(indexOfEdge);
        else
            std::cout << "Warning in plMeshConnectivityData::removeEdge(): Could not find the edge provided as input " << edge << " in vert " << edge->verts[i] << ". Ignoring " << std::endl;
    }
    // iterate through faces, remove references to this edge
    for (PLuint i = 0; i < edge->faces.size(); i++)
    {
        PLint indexOfEdge = edge->faces[i]->edges.findIndex(edge);
        if (indexOfEdge != -1)
            edge->faces.remove(indexOfEdge);
        else
            std::cout << "Warning in plMeshConnectivityData::removeEdge(): Could not find the edge provided as input " << edge << " in face " << edge->faces[i] << ". Ignoring " << std::endl;
    }
    // remove this edge
    edges.erase(*edge);
}

void plMeshConnectivityData::removeFace(const plMeshConnectivityDataFace* face)
{
    // iterate through verts, remove references to this face
    for (PLuint i = 0; i < face->verts.size(); i++)
    {
        PLint indexOfFace = face->verts[i]->faces.findIndex(face);
        if (indexOfFace != -1)
            face->verts.remove(indexOfFace);
        else
            std::cout << "Warning in plMeshConnectivityData::removeFace(): Could not find the face provided as input " << face << " in vert " << face->verts[i] << ". Ignoring " << std::endl;
    }
    // iterate through edge, remove references to this face
    for (PLuint i = 0; i < face->edges.size(); i++)
    {
        PLint indexOfFace = face->edges[i]->faces.findIndex(face);
        if (indexOfFace != -1)
            face->edges.remove(indexOfFace);
        else
            std::cout << "Warning in plMeshConnectivityData::removeFace(): Could not find the face provided as input " << face << " in vert " << face->edges[i] << ". Ignoring " << std::endl;
    }
    // remove this face
    faces.erase(*face);
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

