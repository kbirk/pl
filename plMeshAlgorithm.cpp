#include "plMeshAlgorithm.h"

plMeshAlgorithm::plMeshAlgorithm()
{
    _epsilon = PL_EPSILON;
}

const plMeshConnectivityDataVert* plMeshAlgorithm::_findVert( const plVector3& vertex, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findVert()" << std::endl;
    }

    const plMeshConnectivityDataVert* found(NULL);
    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); vit++)
    {
        if (( (*vit).vert - vertex).length() <= _epsilon)
        {
            if (found == NULL)
                found = &(*vit);
            else
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_findVert(): More than one candidate for vertex " << vertex << ". This could mean that epsilon is set too large. Aborting operation." << std::endl;
                return NULL;
            }
        }
    }

    return found;
}

PLbool plMeshAlgorithm::_splitEdgeOnVect( const plMeshConnectivityDataEdge* edgeAB, const plMeshConnectivityDataVert* vertN, PLuint verbose, PLuint depth )
{
    /*if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_splitEdgeOnVect()" << std::endl;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Splitting on vertex " << vertex << std::endl;
    }*/

    // split the current edge into two pieces

    // find all existing cells, have them available in case they're needed later

    /*if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge being split is " << _data.edges[edgeABindex].edge.pt1 << " - " << _data.edges[edgeABindex].edge.pt2 << std::endl;
        for (PLuint j=0;j<_data.edges[edgeABindex].faceIndices.size();j++)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Attached face: " << _data.faces[_data.edges[edgeABindex].faceIndices[j]].face.point0() << " | " << _data.faces[_data.edges[edgeABindex].faceIndices[j]].face.point1() << " | " << _data.faces[_data.edges[edgeABindex].faceIndices[j]].face.point2() << std::endl;
        }
    }*/

    const plMeshConnectivityDataVert* vertA = edgeAB->vertIndices[0];
    const plMeshConnectivityDataVert* vertB = edgeAB->vertIndices[1];

    // create the new cells, storing their eventual indices

    plMeshConnectivityDataEdge edgeANInsert;
    edgeANInsert.edge = plEdge(vertA->vert, vertN->vert);
    _data.edges.insert(edgeANInsert);

    plMeshConnectivityDataEdge edgeNBInsert;
    edgeNBInsert.edge = plEdge(vertN->vert, vertB->vert);
    _data.edges.insert(edgeNBInsert);

    const plMeshConnectivityDataEdge* edgeAN = &(*(_data.edges.find(edgeANInsert)));
    const plMeshConnectivityDataEdge* edgeNB = &(*(_data.edges.find(edgeNBInsert)));

    // fill the cells with data, but faces will be treated separately.
    vertN->edgeIndices.add(edgeAN);
    vertN->edgeIndices.add(edgeNB);

    edgeAN->vertIndices.add(vertA);
    edgeAN->vertIndices.add(vertN);

    edgeNB->vertIndices.add(vertN);
    edgeNB->vertIndices.add(vertB);

    // split any attached faces each into two pieces...
    plSeq<const plMeshConnectivityDataFace*> faceIndicesToSplit (edgeAB->faceIndices);
    for (PLuint i = 0; i < faceIndicesToSplit.size(); i++)
    {
        // find all existing cells, have them available in case they're needed later
        const plMeshConnectivityDataFace* faceABC = faceIndicesToSplit[i];

        // TODO: Consider adding a check to make sure that the face doesn't contain one of the edges we're adding...
        const plMeshConnectivityDataVert* vertC(NULL);
        for (PLuint j = 0; j < 3; j++)
        {
            if (faceABC->vertIndices[j] != vertA && faceABC->vertIndices[j] != vertB)
            {
                vertC = faceABC->vertIndices[j];
                break;
            }
        }
        if (vertC == NULL)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the C vertex. Aborting operation." << std::endl;
            //std::cout << _data.edges[edgeANindex] << std::endl;
            //std::cout << _data.edges[edgeNBindex] << std::endl;
            //std::cout << _data.faces[faceABCindex] << std::endl;
            return false;
        }
        if (vertC == vertN)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is N vertex. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
            continue;
        }
        if (vertC == vertA)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is A vertex. This should never happen, and is indicitave of a programming logic error. Aborting." << std::endl;
            continue;
        }
        if (vertC == vertB)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): C vertex is B vertex. This should never happen, and is indicitave of a programming logic error. Aborting." << std::endl;
            continue;
        }

        const plMeshConnectivityDataEdge* edgeAC = NULL;
        const plMeshConnectivityDataEdge* edgeBC = NULL;

        for (PLuint j = 0; j < 3; j++)
        {
            const plMeshConnectivityDataEdge* currentEdge = faceABC->edgeIndices[j];
            if (currentEdge->edge == plEdge(vertA->vert,vertC->vert))
                edgeAC = &(*currentEdge);
            else if (currentEdge->edge == plEdge(vertB->vert,vertC->vert))
                edgeBC = &(*currentEdge);
        }
        if (edgeAC == NULL)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the AC edge. Aborting operation." << std::endl;
            //std::cout << _data.verts[vertAindex] << std::endl;
            //std::cout << _data.verts[vertCindex] << std::endl;
            //std::cout << _data.faces[faceABCindex] << std::endl;
            return false;
        }
        if (edgeBC == NULL)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): Could not find the BC edge. Aborting operation." << std::endl;
            //std::cout << _data.verts[vertBindex] << std::endl;
            //std::cout << _data.verts[vertCindex] << std::endl;
            //std::cout << _data.faces[faceABCindex] << std::endl;
            return false;
        }
        if (edgeBC == edgeAN)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): BC edge is AN edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
            continue;
        }
        if (edgeAC == edgeAN)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): AC edge is AN edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
            continue;
        }
        if (edgeBC == edgeNB)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): BC edge is NB edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
            continue;
        }
        if (edgeAC == edgeNB)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Warning in plMeshIntersectorConnectivityData::_splitEdgeOnVect(): AC edge is NB edge. This is possibly due to epsilon being too large. Aborting this particular face split, but beware of future errors." << std::endl;
            continue;
        }

        PLbool faceOrientationABC(false); // either ABC or CBA
        if ((faceABC->face.point0() == vertA->vert && faceABC->face.point1() == vertB->vert) ||
            (faceABC->face.point1() == vertA->vert && faceABC->face.point2() == vertB->vert) ||
            (faceABC->face.point2() == vertA->vert && faceABC->face.point0() == vertB->vert) )
            faceOrientationABC = true; // otherwise false, as already been set

        // create the new cells
        plMeshConnectivityDataEdge edgeNCInsert;
        edgeNCInsert.edge = plEdge(vertN->vert,vertC->vert);

        plMeshConnectivityDataFace faceANCInsert;
        plMeshConnectivityDataFace faceBNCInsert;
        if (faceOrientationABC)
        {
            faceANCInsert.face = plTriangle(vertA->vert,vertN->vert,vertC->vert);
            faceBNCInsert.face = plTriangle(vertB->vert,vertC->vert,vertN->vert);
        }
        else
        {
            faceANCInsert.face = plTriangle(vertC->vert,vertN->vert,vertA->vert);
            faceBNCInsert.face = plTriangle(vertN->vert,vertC->vert,vertB->vert);
        }

        _data.edges.insert(edgeNCInsert);
        _data.faces.insert(faceANCInsert);
        _data.faces.insert(faceBNCInsert);

        const plMeshConnectivityDataEdge* edgeNC  = &(*(_data.edges.find(edgeNCInsert )));
        const plMeshConnectivityDataFace* faceANC = &(*(_data.faces.find(faceANCInsert)));
        const plMeshConnectivityDataFace* faceBNC = &(*(_data.faces.find(faceBNCInsert)));

        // now fill the cells with stuff!
        edgeNC->vertIndices.add(vertN);
        edgeNC->vertIndices.add(vertC);
        edgeNC->faceIndices.add(faceANC);
        edgeNC->faceIndices.add(faceBNC);

        if (faceOrientationABC)
        {
            faceANC->vertIndices.add(vertA);
            faceANC->vertIndices.add(vertN);
            faceANC->vertIndices.add(vertC);
            faceBNC->vertIndices.add(vertB);
            faceBNC->vertIndices.add(vertC);
            faceBNC->vertIndices.add(vertN);
        }
        else
        {
            faceANC->vertIndices.add(vertC);
            faceANC->vertIndices.add(vertN);
            faceANC->vertIndices.add(vertA);
            faceBNC->vertIndices.add(vertN);
            faceBNC->vertIndices.add(vertC);
            faceBNC->vertIndices.add(vertB);
        }
        faceANC->edgeIndices.add(edgeAN);
        faceANC->edgeIndices.add(edgeNC);
        faceANC->edgeIndices.add(edgeAC);
        faceBNC->edgeIndices.add(edgeNB);
        faceBNC->edgeIndices.add(edgeNC);
        faceBNC->edgeIndices.add(edgeBC);

        // update anything else that has been affected by what we just did
        edgeAN->faceIndices.add(faceANC);
        edgeNB->faceIndices.add(faceBNC);

        edgeAC->faceIndices.remove(edgeAC->faceIndices.findIndex(faceABC));
        edgeAC->faceIndices.add(faceANC);
        edgeBC->faceIndices.remove(edgeBC->faceIndices.findIndex(faceABC));
        edgeBC->faceIndices.add(faceBNC);
        vertA->faceIndices.remove(vertA->faceIndices.findIndex(faceABC));
        vertA->faceIndices.add(faceANC);
        vertB->faceIndices.remove(vertB->faceIndices.findIndex(faceABC));
        vertB->faceIndices.add(faceBNC);

        //if (vertNsearchIndex == -1) // only if the vertex is new should this next thing be added
        vertN->faceIndices.add(faceANC);
        vertN->faceIndices.add(faceBNC);
        vertC->faceIndices.add(faceANC);
        vertC->faceIndices.add(faceBNC);

        vertN->edgeIndices.add(edgeNC);
        vertC->edgeIndices.add(edgeNC);

        //std::cout << "vertAindex: " <<vertAindex << std::endl;
        //std::cout << "vertBindex: " <<vertBindex << std::endl;
        //std::cout << "vertCindex: " <<vertCindex << std::endl;
        //std::cout << "vertNindex: " <<vertNindex << std::endl;
        //std::cout << "edgeANindex: "<<edgeANindex << std::endl;
        //std::cout << "edgeNBindex: "<<edgeNBindex << std::endl;
        //std::cout << "edgeNCindex: "<<edgeNCindex << std::endl;
        //std::cout << "-----------" << std::endl;

        _data.faces.erase(_data.faces.find(*faceABC));
    }

    // update anything else that might be affected:
    vertA->edgeIndices.remove(vertA->edgeIndices.findIndex(edgeAB));
    vertB->edgeIndices.remove(vertB->edgeIndices.findIndex(edgeAB));
    _data.edges.erase(edgeAB);

    return true;
}

// add a vertex somewhere in the middle of the triangle, then divide the triangle into three smaller triangles.
PLbool plMeshAlgorithm::_splitFaceOnVect( PLuint faceIndex, const plVector3& vertex, PLuint verbose, PLuint depth )
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
    plMeshConnectivityDataFace face = _data.faces[faceIndex]; // this will eventually be removed from the list of faces
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Face being split is " << _data.faces[faceIndex].face.point0() << " | "
                                                   << _data.faces[faceIndex].face.point1() << " | "
                                                   << _data.faces[faceIndex].face.point2() << std::endl;
    }

    PLuint vert0index;
    PLuint vert1index;
    PLuint vert2index;
    for (PLuint i = 0; i < face.vertIndices.size(); i++) // TODO: Ask myself if this is really needed...
    {
        if (_data.verts[face.vertIndices[i]].vert == face.face.point0())
            vert0index = face.vertIndices[i];
        if (_data.verts[face.vertIndices[i]].vert == face.face.point1())
            vert1index = face.vertIndices[i];
        if (_data.verts[face.vertIndices[i]].vert == face.face.point2())
            vert2index = face.vertIndices[i];
    }
    plMeshConnectivityDataVert& vert0 = _data.verts[vert0index];
    plMeshConnectivityDataVert& vert1 = _data.verts[vert1index];
    plMeshConnectivityDataVert& vert2 = _data.verts[vert2index];
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Vertices detected are " << _data.verts[vert0index].vert << " | "
                                                     << _data.verts[vert1index].vert << " | "
                                                     << _data.verts[vert2index].vert << std::endl;
    }

    PLint edge01searchIndex(-1);
    PLint edge12searchIndex(-1);
    PLint edge20searchIndex(-1);
    for (PLuint i = 0; i < face.edgeIndices.size(); i++)
    {
        if (_data.edges[face.edgeIndices[i]].edge.contains(face.face.point0(), _epsilon) &&
            _data.edges[face.edgeIndices[i]].edge.contains(face.face.point1(), _epsilon) )
            edge01searchIndex = face.edgeIndices[i];
        if (_data.edges[face.edgeIndices[i]].edge.contains(face.face.point1(), _epsilon) &&
            _data.edges[face.edgeIndices[i]].edge.contains(face.face.point2(), _epsilon) )
            edge12searchIndex = face.edgeIndices[i];
        if (_data.edges[face.edgeIndices[i]].edge.contains(face.face.point2(), _epsilon) &&
            _data.edges[face.edgeIndices[i]].edge.contains(face.face.point0(), _epsilon) )
            edge20searchIndex = face.edgeIndices[i];
    }
    if (edge01searchIndex == -1 || edge12searchIndex == -1 || edge20searchIndex == -1)
    {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Error in plMeshIntersectorConnectivityData::_splitFaceOnVect(): Could not find one of the edges in the face. Aborting operation." << std::endl;
        std::cout << _data.faces[faceIndex] << std::endl;
        return false;
    }
    PLuint edge01index((PLuint)edge01searchIndex);
    PLuint edge12index((PLuint)edge12searchIndex);
    PLuint edge20index((PLuint)edge20searchIndex);
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge01 detected is " << _data.edges[edge01index].edge.pt1 << " - " << _data.edges[edge01index].edge.pt2 << std::endl;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge12 detected is " << _data.edges[edge12index].edge.pt1 << " - " << _data.edges[edge12index].edge.pt2 << std::endl;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge20 detected is " << _data.edges[edge20index].edge.pt1 << " - " << _data.edges[edge20index].edge.pt2 << std::endl;
    }
    plMeshConnectivityDataEdge& edge01 = _data.edges[edge01index];
    plMeshConnectivityDataEdge& edge12 = _data.edges[edge12index];
    plMeshConnectivityDataEdge& edge20 = _data.edges[edge20index];

    // create the new cells, storing their eventual indices
    plMeshConnectivityDataVert vertNsearch; vertNsearch.vert = vertex;
    PLint  vertNsearchIndex;
    vertNsearchIndex = _data.verts.findIndex(vertNsearch);
    PLuint vertNindex; // we need to determine this
    if (vertNsearchIndex == -1)
    {
        vertNindex = _data.verts.size();
        _data.verts.add(vertNsearch);
    }
    else
        vertNindex = (PLuint)vertNsearchIndex;
    plMeshConnectivityDataVert& vertN = _data.verts[vertNindex];

    plMeshConnectivityDataEdge edgeN0;
    plMeshConnectivityDataEdge edgeN1;
    plMeshConnectivityDataEdge edgeN2;
    PLuint edgeN0index(_data.edges.size()  );
    PLuint edgeN1index(_data.edges.size()+1);
    PLuint edgeN2index(_data.edges.size()+2);

    plMeshConnectivityDataFace face01N;
    plMeshConnectivityDataFace face12N;
    plMeshConnectivityDataFace face20N;
    PLuint face01Nindex(faceIndex);
    PLuint face12Nindex(_data.faces.size());
    PLuint face20Nindex(_data.faces.size()+1);

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
    _data.faces[faceIndex] = face01N;
    _data.faces.add(face12N);
    _data.faces.add(face20N);

    _data.edges.add(edgeN0);
    _data.edges.add(edgeN1);
    _data.edges.add(edgeN2);

    // update anything else that has been affected by what we just did
    _data.verts[vert0index].faceIndices.add(face20Nindex); // keeping in mind faceIndex is already there, and is now face01Nindex
    _data.verts[vert1index].faceIndices.add(face12Nindex); // see above comment
    _data.verts[vert2index].faceIndices.remove(_data.verts[vert2index].faceIndices.findIndex(faceIndex)); // face01N is no longer connected, so remove it.
    _data.verts[vert2index].faceIndices.add(face20Nindex);
    _data.verts[vert2index].faceIndices.add(face12Nindex);

    _data.verts[vert0index].edgeIndices.add(edgeN0index);
    _data.verts[vert1index].edgeIndices.add(edgeN1index);
    _data.verts[vert2index].edgeIndices.add(edgeN2index);

    _data.edges[edge12index].faceIndices.remove(_data.edges[edge12index].faceIndices.findIndex(faceIndex));
    _data.edges[edge12index].faceIndices.add(face12Nindex);
    _data.edges[edge20index].faceIndices.remove(_data.edges[edge20index].faceIndices.findIndex(faceIndex));
    _data.edges[edge20index].faceIndices.add(face20Nindex); // edge01 doesn't need this because faceIndex == face01Nindex

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

PLbool plMeshAlgorithm::_checkArraySizes( PLuint verbose, PLuint depth )
{
    bool good(true);
    for (PLuint i = 0; i < _data.edges.size(); i++)
    {
        if (_data.edges[i].faceIndices.size() % 2 != 0 || _data.edges[i].faceIndices.size() == 0)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): faceIndices for edge " << i << " is of non positive even size " <<_data.edges[i].faceIndices.size()  << ". Should be even." << std::endl;
            std::cout << _data.edges[i] << std::endl;
            good = false;
        }
        if (_data.edges[i].vertIndices.size() != 2)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for edge " << i << " is of size " << _data.edges[i].vertIndices.size() << ". Should be 2." << std::endl;
            good = false;
            std::cout << _data.edges[i] << std::endl;
        }
    }
    for (PLuint i = 0; i < _data.faces.size(); i++)
    {
        if (_data.faces[i].vertIndices.size() != 3)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for face " << i << " is of size " << _data.faces[i].vertIndices.size() << ". Should be 3." << std::endl;
            good = false;
            std::cout << _data.faces[i] << std::endl;
        }
        if (_data.faces[i].edgeIndices.size() != 3)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): edgeIndices for face " << i << " is of size " << _data.faces[i].edgeIndices.size() << ". Should be 3." << std::endl;
            good = false;
            std::cout << _data.faces[i] << std::endl;
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkNoDuplicates( PLuint verbose, PLuint depth )
{
    bool good(true);
    plSeq<PLuint> indices;
    for (PLuint i=0; i < _data.verts.size(); i++)
    {
        // check edges
        indices.clear();
        for (PLuint j = 0; j < _data.verts[i].edgeIndices.size(); j++)
        {
            if (indices.findIndex(_data.verts[i].edgeIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << i << " contains a duplicate edge: " << _data.verts[i].edgeIndices[j] << "." << std::endl;
                good = false;
                std::cout << _data.verts[i] << std::endl;
            }
            else
            {
                indices.add(_data.verts[i].edgeIndices[j]);
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < _data.verts[i].faceIndices.size(); j++)
        {
            if (indices.findIndex(_data.verts[i].faceIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << i << " contains a duplicate face: " << _data.verts[i].faceIndices[j] << "." << std::endl;
                good = false;
                std::cout << _data.verts[i] << std::endl;
            }
            else
            {
                indices.add(_data.verts[i].faceIndices[j]);
            }
        }
    }
    for (PLuint i=0; i < _data.edges.size(); i++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < _data.edges[i].vertIndices.size(); j++)
        {
            if (indices.findIndex(_data.edges[i].vertIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << i << " contains a duplicate vertex: " << _data.edges[i].vertIndices[j] << "." << std::endl;
                good = false;
                std::cout << _data.edges[i] << std::endl;
            }
            else
            {
                indices.add(_data.edges[i].vertIndices[j]);
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < _data.edges[i].faceIndices.size(); j++)
        {
            if (indices.findIndex(_data.edges[i].faceIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << i << " contains a duplicate face: " << _data.edges[i].faceIndices[j] << "." << std::endl;
                good = false;
                std::cout << _data.edges[i] << std::endl;
            }
            else
            {
                indices.add(_data.edges[i].faceIndices[j]);
            }
        }
    }
    for (PLuint i=0; i < _data.faces.size(); i++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < _data.faces[i].vertIndices.size(); j++)
        {
            if (indices.findIndex(_data.faces[i].vertIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << i << " contains a duplicate vertex: " << _data.faces[i].vertIndices[j] << "." << std::endl;
                good = false;
                std::cout << _data.faces[i] << std::endl;
            }
            else
            {
                indices.add(_data.faces[i].vertIndices[j]);
            }
        }
        // check edges
        indices.clear();
        for (PLuint j = 0; j < _data.faces[i].edgeIndices.size(); j++)
        {
            if (indices.findIndex(_data.faces[i].edgeIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << i << " contains a duplicate edge: " << _data.faces[i].edgeIndices[j] << "." << std::endl;
                good = false;
                std::cout << _data.faces[i] << std::endl;
            }
            else
            {
                indices.add(_data.faces[i].edgeIndices[j]);
            }
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkBidirectional( PLuint verbose, PLuint depth )
{
    bool good(true);
    plSeq<PLuint> indices;
    for (PLuint i=0; i < _data.verts.size(); i++)
    {
        // check edges
        indices.clear();
        for (PLuint j = 0; j < _data.verts[i].edgeIndices.size(); j++)
        {
            if (_data.edges[_data.verts[i].edgeIndices[j]].vertIndices.findIndex(i) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << i << " references edge " << _data.verts[i].edgeIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << _data.verts[i] << std::endl;
                std::cout << _data.edges[_data.verts[i].edgeIndices[j]] << std::endl;
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < _data.verts[i].faceIndices.size(); j++)
        {
            if (_data.faces[_data.verts[i].faceIndices[j]].vertIndices.findIndex(i) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << i << " references face " << _data.verts[i].faceIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << _data.verts[i] << std::endl;
                std::cout << _data.faces[_data.verts[i].faceIndices[j]] << std::endl;
            }
        }
    }
    for (PLuint i=0; i < _data.edges.size(); i++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < _data.edges[i].vertIndices.size(); j++)
        {
            if (_data.verts[_data.edges[i].vertIndices[j]].edgeIndices.findIndex(i) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << i << " references vert " << _data.edges[i].vertIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << _data.edges[i] << std::endl;
                std::cout << _data.verts[_data.edges[i].vertIndices[j]] << std::endl;
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < _data.edges[i].faceIndices.size(); j++)
        {
            if (_data.faces[_data.edges[i].faceIndices[j]].edgeIndices.findIndex(i) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << i << " references face " << _data.edges[i].faceIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << _data.edges[i] << std::endl;
                std::cout << _data.faces[_data.edges[i].faceIndices[j]] << std::endl;
            }
        }
    }
    for (PLuint i=0; i < _data.faces.size(); i++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < _data.faces[i].vertIndices.size(); j++)
        {
            if (_data.verts[_data.faces[i].vertIndices[j]].faceIndices.findIndex(i) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << i << " references vert " << _data.faces[i].vertIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << _data.faces[i] << std::endl;
                std::cout << _data.verts[_data.faces[i].vertIndices[j]] << std::endl;
            }
        }
        // check edges
        indices.clear();
        for (PLuint j = 0; j < _data.faces[i].edgeIndices.size(); j++)
        {
            if (_data.edges[_data.faces[i].edgeIndices[j]].faceIndices.findIndex(i) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << i << " references edge " << _data.faces[i].edgeIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << _data.faces[i] << std::endl;
                std::cout << _data.edges[_data.faces[i].edgeIndices[j]] << std::endl;
            }
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkNoSliverTriangles( PLuint verbose, PLuint depth )
{
    PLbool good(true);
    for (PLuint i = 0; i < _data.faces.size(); i++)
    {
        if (( ((_data.faces[i].face.point1())-(_data.faces[i].face.point0())).normalize() ^
              ((_data.faces[i].face.point2())-(_data.faces[i].face.point0())).normalize()).length() == 0.f )
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoSliverTriangles(): face " << i << " appears to be ultra thin. This is bad." << std::endl;
            good = false;
            std::cout << _data.faces[i] << std::endl;
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkForAllErrors( PLuint verbose, PLuint depth )
{
    return _checkNoDuplicates      (verbose,depth+1) &&
           _checkArraySizes        (verbose,depth+1) &&
           _checkBidirectional     (verbose,depth+1) &&
           _checkNoSliverTriangles (verbose,depth+1) ;
}

void plMeshAlgorithm::_reportSizes( PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_reportSizes()" << std::endl;
    }
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "Size of verts: " << _data.verts.size() << "\n";
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "Size of edges: " << _data.edges.size() << "\n";
    for (PLuint i=0;i<depth;i++)
        std::cout << "\t";
    std::cout << "Size of faces: " << _data.faces.size() << "\n";
}

PLbool plMeshAlgorithm::_importTriSeq(const plSeq<plTriangle> &tris, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_importTriSeq()" << std::endl;
    }

    _data.verts.clear();
    _data.edges.clear();
    _data.faces.clear();
    for (PLuint i = 0; i < tris.size(); i++)
    {
        // add cells as necessary
        plTriangle& currentTriangle = tris[i];

        PLint vert0index,vert1index,vert2index;
        plMeshConnectivityDataVert vert0;
        vert0.vert = currentTriangle[0];
        if (!_findVert(vert0.vert,vert0index,verbose,depth+1)) return false;
        PLbool vert0existed(true);
        if (vert0index == -1)
        {
            vert0index = _data.verts.size();
            _data.verts.add(vert0);
            vert0existed = false;
        }
        plMeshConnectivityDataVert vert1;
        vert1.vert = currentTriangle[1];
        if (!_findVert(vert1.vert,vert1index,verbose,depth+1)) return false;
        PLbool vert1existed(true);
        if (vert1index == -1)
        {
            vert1index = _data.verts.size();
            _data.verts.add(vert1);
            vert1existed = false;
        }
        plMeshConnectivityDataVert vert2;
        vert2.vert = currentTriangle[2];
        if (!_findVert(vert2.vert,vert2index,verbose,depth+1)) return false;
        PLbool vert2existed(true);
        if (vert2index == -1)
        {
            vert2index = _data.verts.size();
            _data.verts.add(vert2);
            vert2existed = false;
        }

        PLint edge01index, edge12index, edge20index;
        plMeshConnectivityDataEdge edge01;
        edge01.edge = plEdge(_data.verts[vert0index].vert,_data.verts[vert1index].vert);
        edge01index = _data.edges.findIndex(edge01);
        if (edge01index == -1)
        {
            edge01index = _data.edges.size();
            _data.edges.add(edge01);
            _data.edges[edge01index].vertIndices.add((PLuint)vert0index);
            _data.edges[edge01index].vertIndices.add((PLuint)vert1index);
        }
        plMeshConnectivityDataEdge edge12;
        edge12.edge = plEdge(_data.verts[vert1index].vert,_data.verts[vert2index].vert);
        edge12index = _data.edges.findIndex(edge12);
        if (edge12index == -1)
        {
            edge12index = _data.edges.size();
            _data.edges.add(edge12);
            _data.edges[edge12index].vertIndices.add((PLuint)vert1index);
            _data.edges[edge12index].vertIndices.add((PLuint)vert2index);
        }
        plMeshConnectivityDataEdge edge20;
        edge20.edge = plEdge(_data.verts[vert2index].vert,_data.verts[vert0index].vert);
        edge20index = _data.edges.findIndex(edge20);
        if (edge20index == -1)
        {
            edge20index = _data.edges.size();
            _data.edges.add(edge20);
            _data.edges[edge20index].vertIndices.add((PLuint)vert2index);
            _data.edges[edge20index].vertIndices.add((PLuint)vert0index);
        }

        plMeshConnectivityDataFace face012;
        face012.face = plTriangle(_data.verts[vert0index].vert,_data.verts[vert1index].vert,_data.verts[vert2index].vert);
        PLint face012index = _data.faces.size();
        _data.faces.add(face012);

        // this should only be done if the vertex did not exist. Didn't do earlier since we didn't know the edge indices yet
        if (_data.verts[vert0index].edgeIndices.findIndex(edge01index) == -1)  _data.verts[vert0index].edgeIndices.add((PLuint)edge01index);
        if (_data.verts[vert0index].edgeIndices.findIndex(edge20index) == -1)  _data.verts[vert0index].edgeIndices.add((PLuint)edge20index);
        if (_data.verts[vert1index].edgeIndices.findIndex(edge01index) == -1)  _data.verts[vert1index].edgeIndices.add((PLuint)edge01index);
        if (_data.verts[vert1index].edgeIndices.findIndex(edge12index) == -1)  _data.verts[vert1index].edgeIndices.add((PLuint)edge12index);
        if (_data.verts[vert2index].edgeIndices.findIndex(edge12index) == -1)  _data.verts[vert2index].edgeIndices.add((PLuint)edge12index);
        if (_data.verts[vert2index].edgeIndices.findIndex(edge20index) == -1)  _data.verts[vert2index].edgeIndices.add((PLuint)edge20index);

        // now update connectivity information
        _data.verts[vert0index].faceIndices.add((PLuint)face012index);
        _data.verts[vert1index].faceIndices.add((PLuint)face012index);
        _data.verts[vert2index].faceIndices.add((PLuint)face012index);
        _data.edges[edge01index].faceIndices.add((PLuint)face012index);
        _data.edges[edge12index].faceIndices.add((PLuint)face012index);
        _data.edges[edge20index].faceIndices.add((PLuint)face012index);
        _data.faces[face012index].vertIndices.add((PLuint)vert0index);
        _data.faces[face012index].vertIndices.add((PLuint)vert1index);
        _data.faces[face012index].vertIndices.add((PLuint)vert2index);
        _data.faces[face012index].edgeIndices.add((PLuint)edge01index);
        _data.faces[face012index].edgeIndices.add((PLuint)edge12index);
        _data.faces[face012index].edgeIndices.add((PLuint)edge20index);
    }
    if (!_checkForAllErrors( verbose,depth+1 )) return false;
    return true;
}

PLbool plMeshAlgorithm::_exportTriSeq(plSeq<plTriangle> &tris, PLuint verbose, PLuint depth )
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
    for (PLuint i = 0; i < _data.faces.size(); i++)
    {
        tris.add(_data.faces[i].face);
    }
    return true;
}
