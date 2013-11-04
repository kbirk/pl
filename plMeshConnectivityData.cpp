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
        edges.push_back(other.edges[i]);
    faces.clear();
    for (PLuint i = 0; i < other.faces.size(); i++)
        faces.push_back(other.faces[i]);
    dataset = other.dataset;
    return *this;
}

plMeshConnectivityDataEdge& plMeshConnectivityDataEdge::operator=(const plMeshConnectivityDataEdge& other)
{
    edge = other.edge;
    originatingMesh = other.originatingMesh;
    verts.clear();
    for (PLuint i = 0; i < other.verts.size(); i++)
        verts.push_back(other.verts[i]);
    faces.clear();
    for (PLuint i = 0; i < other.faces.size(); i++)
        faces.push_back(other.faces[i]);
    dataset = other.dataset;
    return *this;
}

plMeshConnectivityDataFace& plMeshConnectivityDataFace::operator=(const plMeshConnectivityDataFace& other)
{
    face = other.face;
    originatingMesh = other.originatingMesh;
    verts.clear();
    for (PLuint i = 0; i < other.verts.size(); i++)
        verts.push_back(other.verts[i]);
    edges.clear();
    for (PLuint i = 0; i < other.edges.size(); i++)
        edges.push_back(other.edges[i]);
    dataset = other.dataset;
    return *this;
}

const plMeshConnectivityDataVert* plMeshConnectivityData::addVert( const plVector3 &vert , PLuint originatingMesh , PLuint verbose )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) std::cout << "Debug: Entering plMeshConnectivityData::addVert()" << std::endl;
    plMeshConnectivityDataVert vertToAdd;
    vertToAdd.vert = vert;
    vertToAdd.originatingMesh = originatingMesh;
    vertToAdd.dataset = this;
    const plMeshConnectivityDataVert* vertPtr (NULL);
    if (verts.find(vertToAdd) == verts.end()) // first check for the same vertex
    {
        findVertWithinEpsilon(vert,vertPtr); // now search for something close within epsilon
        if (vertPtr == NULL)
        {
            verts.insert(vertToAdd);
            vertPtr = &(*(verts.find(vertToAdd)));
        }
    }
    else
    {
        vertPtr = &(*(verts.find(vertToAdd)));
        if (verbose >= PL_LOGGER_LEVEL_WARNING) std::cout << "Warning in plMeshConnectivityData::addVert(): Vertex " << vert << " already exists. Returning it." << std::endl;
    }
    return vertPtr;
}

const plMeshConnectivityDataEdge* plMeshConnectivityData::addEdge(const plMeshConnectivityDataVert* vert0, const plMeshConnectivityDataVert* vert1, PLuint originatingMesh, PLuint verbose)
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) std::cout << "Debug: Entering plMeshConnectivityData::addEdge()" << std::endl;
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
        if (verbose >= PL_LOGGER_LEVEL_ERROR) std::cout << "Error in plMeshConnectivityData::addEdge(): vert0 and vert1 appear to be the same. Returning NULL." << std::endl;
        return NULL;
    }

    // now set up the new element
    plMeshConnectivityDataEdge edgeToAdd;
    edgeToAdd.edge = plEdge(orderedVerts[0]->vert,orderedVerts[1]->vert);
    edgeToAdd.verts.push_back(orderedVerts[0]);
    edgeToAdd.verts.push_back(orderedVerts[1]);
    edgeToAdd.originatingMesh = originatingMesh;
    edgeToAdd.dataset = this;
    const plMeshConnectivityDataEdge* edgePtr;
    if (edges.find(edgeToAdd) == edges.end())
    {
        edges.insert(edgeToAdd);
        edgePtr = &(*(edges.find(edgeToAdd)));
        // update the verts that were provided as input.
        vert0->edges.push_back(edgePtr);
        vert1->edges.push_back(edgePtr);
    }
    else
    {
        if (verbose >= PL_LOGGER_LEVEL_WARNING) std::cout << "Warning in plMeshConnectivityData::addEdge(): Edge between verts " << vert0 << " and " << vert1 << " already exists. Returning it without updating other cells." << std::endl;
        edgePtr = &(*(edges.find(edgeToAdd)));
    }

    return edgePtr;
}

const plMeshConnectivityDataFace* plMeshConnectivityData::addFace(const plMeshConnectivityDataVert* vert0, const plMeshConnectivityDataVert* vert1, const plMeshConnectivityDataVert* vert2, const plMeshConnectivityDataEdge* edge01, const plMeshConnectivityDataEdge* edge12, const plMeshConnectivityDataEdge* edge20, PLuint originatingMesh, PLuint verbose)
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) std::cout << "Debug: Entering plMeshConnectivityData::addFace()" << std::endl;
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
        if (verbose >= PL_LOGGER_LEVEL_ERROR) std::cout << "Error in plMeshConnectivityData::addFace(): the two 'smallest' verts appear to be the same. Returning NULL." << std::endl;
        return NULL;
    }

    // now set up the new element
    plMeshConnectivityDataFace faceToAdd;
    faceToAdd.face = plTriangle(orderedVerts[0]->vert,orderedVerts[1]->vert,orderedVerts[2]->vert);
    faceToAdd.verts.push_back(orderedVerts[0]);
    faceToAdd.verts.push_back(orderedVerts[1]);
    faceToAdd.verts.push_back(orderedVerts[2]);
    faceToAdd.edges.push_back(orderedEdges[0]);
    faceToAdd.edges.push_back(orderedEdges[1]);
    faceToAdd.edges.push_back(orderedEdges[2]);
    faceToAdd.originatingMesh = originatingMesh;
    faceToAdd.dataset = this;
    const plMeshConnectivityDataFace* facePtr;
    if (faces.find(faceToAdd) == faces.end())
    {
        faces.insert(faceToAdd);
        facePtr = &(*(faces.find(faceToAdd)));
        // update the verts that were provided as input.
        vert0->faces.push_back(facePtr);
        vert1->faces.push_back(facePtr);
        vert2->faces.push_back(facePtr);
        edge01->faces.push_back(facePtr);
        edge12->faces.push_back(facePtr);
        edge20->faces.push_back(facePtr);
    }
    else
    {
        if (verbose >= PL_LOGGER_LEVEL_WARNING) std::cout << "Warning in plMeshConnectivityData::addFace(): Face of verts " << vert0 << ", " << vert1 << ", and " << vert2 << "already exists. Returning it without updating other cells." << std::endl;
        facePtr = &(*(faces.find(faceToAdd)));
    }

    return facePtr;
}

void plMeshConnectivityData::removeVert(const plMeshConnectivityDataVert* vert)
{
    // iterate through edges, remove references to this vert
    for (PLuint i = 0; i < vert->edges.size(); i++)
    {
        if ( !plUtility::removeIfExists( vert->edges[i]->verts, vert ) )
            std::cout << "Warning in plMeshConnectivityData::removeVert(): Could not find the vert provided as input " << vert << " in edge " << vert->edges[i] << ". Ignoring " << std::endl;
    }
    // iterate through faces, remove references to this vert
    for (PLuint i = 0; i < vert->faces.size(); i++)
    {
        if ( !plUtility::removeIfExists( vert->faces[i]->verts, vert ) )
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
        if ( !plUtility::removeIfExists( edge->verts[i]->edges, edge ) )
            std::cout << "Warning in plMeshConnectivityData::removeEdge(): Could not find the edge provided as input " << edge << " in vert " << edge->verts[i] << ". Ignoring " << std::endl;
    }
    // iterate through faces, remove references to this edge
    for (PLuint i = 0; i < edge->faces.size(); i++)
    {
        if ( !plUtility::removeIfExists( edge->faces[i]->edges, edge ) )
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
        if ( !plUtility::removeIfExists( face->verts[i]->faces, face ) )
            std::cout << "Warning in plMeshConnectivityData::removeFace(): Could not find the face provided as input " << face << " in vert " << face->verts[i] << ". Ignoring " << std::endl;
    }
    // iterate through edge, remove references to this face
    for (PLuint i = 0; i < face->edges.size(); i++)
    {
        if ( !plUtility::removeIfExists( face->edges[i]->faces, face ) )
            std::cout << "Warning in plMeshConnectivityData::removeFace(): Could not find the face provided as input " << face << " in vert " << face->edges[i] << ". Ignoring " << std::endl;
    }
    // remove this face
    faces.erase(*face);
}

PLbool plMeshConnectivityData::findVertWithinEpsilon( const plVector3& vertex, const plMeshConnectivityDataVert*& vertPointer )
{
    vertPointer = (NULL);
    for (plMeshConnectivityDataVertIterator vit = verts.begin(); vit != verts.end(); vit++)
    {
        if (( (*vit).vert - vertex).length() <= _epsilon)
        {
            if (vertPointer == NULL)
                vertPointer = &(*vit);
            else
            {
                std::cout << "Error in plMeshIntersectorConnectivityData::_findVert(): More than one candidate for vertex " << vertex << ". This could mean that epsilon is set too large. Setting pointer to NULL and aborting operation." << std::endl;
                vertPointer = NULL;
                return false;
            }
        }
    }

    return true;
}

PLbool plMeshConnectivityData::importTriSeq(const std::vector<plTriangle> &tris, PLuint originatingMesh, PLuint verbose )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_importTriSeq()" << std::endl;
    for (PLuint i = 0; i < tris.size(); i++)
    {
        // add cells as necessary
        const plTriangle& currentTriangle = tris[i];
        const plMeshConnectivityDataVert* vert0 = addVert(currentTriangle.point0(),0,PL_LOGGER_LEVEL_ERROR); // we ignore non-error output, since during the import stage we expect for there to be (for example) duplicate vertices, which are actually already handled appropriately by the function.
        const plMeshConnectivityDataVert* vert1 = addVert(currentTriangle.point1(),0,PL_LOGGER_LEVEL_ERROR);
        const plMeshConnectivityDataVert* vert2 = addVert(currentTriangle.point2(),0,PL_LOGGER_LEVEL_ERROR);
        if (vert0 == NULL || vert1 == NULL || vert2 == NULL)
        {
            if (verbose >= PL_LOGGER_LEVEL_ERROR) std::cout << "Error in plMeshConnectivityData::importTriSeq(): A vert is a null pointer. This indicates a major problem with the mesh. There should be more details in a previous error message. Aborting input." << std::endl;
            return false;
        }
        const plMeshConnectivityDataEdge* edge01 = addEdge(vert0,vert1,originatingMesh,PL_LOGGER_LEVEL_ERROR);
        const plMeshConnectivityDataEdge* edge12 = addEdge(vert1,vert2,originatingMesh,PL_LOGGER_LEVEL_ERROR);
        const plMeshConnectivityDataEdge* edge20 = addEdge(vert2,vert0,originatingMesh,PL_LOGGER_LEVEL_ERROR);
        if (edge01 == NULL || edge12 == NULL || edge20 == NULL)
        {
            if (verbose >= PL_LOGGER_LEVEL_ERROR) std::cout << "Error in plMeshConnectivityData::importTriSeq(): An edge is a null pointer. This indicates a major problem with the mesh. There should be more details in a previous error message. Aborting input." << std::endl;
            return false;
        }
        const plMeshConnectivityDataFace* face012 = addFace(vert0,vert1,vert2,edge01,edge12,edge20,originatingMesh,PL_LOGGER_LEVEL_ERROR);
        if (face012 == NULL)
        {
            if (verbose >= PL_LOGGER_LEVEL_ERROR) std::cout << "Error in plMeshConnectivityData::importTriSeq(): A face is a null pointer. This indicates a major problem with the mesh. There should be more details in a previous error message. Aborting input." << std::endl;
            return false;
        }
    }
    return true;
}

PLbool plMeshConnectivityData::exportTriSeq(std::vector<plTriangle> &tris, PLuint verbose )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_exportTriSeq()" << std::endl;

    if (tris.size() != 0)
    {
        if (verbose >= PL_LOGGER_LEVEL_WARNING) std::cout << "Warning in plMeshIntersectorConnectivityData::exportTriSeq(): tris array provided already contains data. Clearing contents." << std::endl;
        tris.clear();
    }

    for (plMeshConnectivityDataFaceIterator fit = faces.begin(); fit != faces.end(); fit++)
    {
        tris.push_back((*fit).face);
    }

    return true;
}

void plMeshConnectivityData::reportSizes()
{
    std::cout << "Size of verts: " << verts.size() << "\n";
    std::cout << "Size of edges: " << edges.size() << "\n";
    std::cout << "Size of faces: " << faces.size() << "\n";
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

