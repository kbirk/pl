#include "plMeshAlgorithm.h"

plMeshAlgorithm::plMeshAlgorithm()
{
    _epsilon = PL_EPSILON;
}

PLbool plMeshAlgorithm::_findVert( const plVector3& vertex, const plMeshConnectivityDataVert* vertPointer, PLuint verbose, PLuint depth )
{
    if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_findVert()" << std::endl;
    }

    vertPointer = (NULL);
    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); vit++)
    {
        if (( (*vit).vert - vertex).length() <= _epsilon)
        {
            if (vertPointer == NULL)
                vertPointer = &(*vit);
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
    _data.edges.erase(*edgeAB);

    return true;
}

// add a vertex somewhere in the middle of the triangle, then divide the triangle into three smaller triangles.
PLbool plMeshAlgorithm::_splitFaceOnVect( const plMeshConnectivityDataFace* face012, const plMeshConnectivityDataVert* vertN, PLuint verbose, PLuint depth )
{
    /*if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Entering plMeshIntersectorConnectivityData::_splitFaceOnVect()" << std::endl;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Splitting on vertex " << vertex << std::endl;
    }*/

    // find all existing cells, have them available in case they're needed later
    /*if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Face being split is " << _data.faces[faceIndex].face.point0() << " | "
                                                   << _data.faces[faceIndex].face.point1() << " | "
                                                   << _data.faces[faceIndex].face.point2() << std::endl;
    }*/

    const plMeshConnectivityDataVert* vert0 = face012->vertIndices[0];
    const plMeshConnectivityDataVert* vert1 = face012->vertIndices[1];
    const plMeshConnectivityDataVert* vert2 = face012->vertIndices[2];

    /*if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Vertices detected are " << _data.verts[vert0index].vert << " | "
                                                     << _data.verts[vert1index].vert << " | "
                                                     << _data.verts[vert2index].vert << std::endl;
    }*/

    const plMeshConnectivityDataEdge* edge01(NULL);
    const plMeshConnectivityDataEdge* edge12(NULL);
    const plMeshConnectivityDataEdge* edge20(NULL);

    for (PLuint i = 0; i < face012->edgeIndices.size(); i++)
    {
        if (face012->edgeIndices[i]->edge.contains(vert0->vert, _epsilon) &&
            face012->edgeIndices[i]->edge.contains(vert1->vert, _epsilon) )
            edge01 = face012->edgeIndices[i];
        if (face012->edgeIndices[i]->edge.contains(vert1->vert, _epsilon) &&
            face012->edgeIndices[i]->edge.contains(vert2->vert, _epsilon) )
            edge12 = face012->edgeIndices[i];
        if (face012->edgeIndices[i]->edge.contains(vert2->vert, _epsilon) &&
            face012->edgeIndices[i]->edge.contains(vert0->vert, _epsilon) )
            edge20 = face012->edgeIndices[i];
    }
    if (edge01 == NULL || edge12 == NULL || edge20 == NULL)
    {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Error in plMeshIntersectorConnectivityData::_splitFaceOnVect(): Could not find one of the edges in the face. Aborting operation." << std::endl;
        //std::cout << _data.faces[faceIndex] << std::endl;
        return false;
    }
    /*if (verbose >= PL_LOGGER_LEVEL_DEBUG) {
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge01 detected is " << _data.edges[edge01index].edge.pt1 << " - " << _data.edges[edge01index].edge.pt2 << std::endl;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge12 detected is " << _data.edges[edge12index].edge.pt1 << " - " << _data.edges[edge12index].edge.pt2 << std::endl;
        for (PLuint i=0;i<depth;i++)
            std::cout << "\t";
        std::cout << "Debug: Edge20 detected is " << _data.edges[edge20index].edge.pt1 << " - " << _data.edges[edge20index].edge.pt2 << std::endl;
    }*/

    // create the new cells, storing their eventual indices
    plMeshConnectivityDataEdge edgeN0Insert;
    edgeN0Insert.edge = plEdge(vertN->vert,vert0->vert);
    _data.edges.insert(edgeN0Insert);

    plMeshConnectivityDataEdge edgeN1Insert;
    edgeN1Insert.edge = plEdge(vertN->vert,vert1->vert);
    _data.edges.insert(edgeN1Insert);

    plMeshConnectivityDataEdge edgeN2Insert;
    edgeN2Insert.edge = plEdge(vertN->vert,vert2->vert);
    _data.edges.insert(edgeN2Insert);

    plMeshConnectivityDataFace face01NInsert;
    face01NInsert.face = plTriangle(vert0->vert,vert1->vert,vertN->vert);
    _data.faces.insert(face01NInsert);

    plMeshConnectivityDataFace face12NInsert;
    face12NInsert.face = plTriangle(vert1->vert,vert2->vert,vertN->vert);
    _data.faces.insert(face12NInsert);

    plMeshConnectivityDataFace face20NInsert;
    face20NInsert.face = plTriangle(vert2->vert,vert0->vert,vertN->vert);
    _data.faces.insert(face20NInsert);

    const plMeshConnectivityDataEdge* edgeN0  = &(*(_data.edges.find(edgeN0Insert )));;
    const plMeshConnectivityDataEdge* edgeN1  = &(*(_data.edges.find(edgeN1Insert )));;
    const plMeshConnectivityDataEdge* edgeN2  = &(*(_data.edges.find(edgeN2Insert )));;
    const plMeshConnectivityDataFace* face01N = &(*(_data.faces.find(face01NInsert )));;
    const plMeshConnectivityDataFace* face12N = &(*(_data.faces.find(face12NInsert )));;
    const plMeshConnectivityDataFace* face20N = &(*(_data.faces.find(face20NInsert )));;

    // fill the cells with data
    vertN->faceIndices.add(face01N);
    vertN->faceIndices.add(face12N);
    vertN->faceIndices.add(face20N);
    vertN->edgeIndices.add(edgeN0);
    vertN->edgeIndices.add(edgeN1);
    vertN->edgeIndices.add(edgeN2);

    face01N->edgeIndices.add(edge01);
    face01N->edgeIndices.add(edgeN1);
    face01N->edgeIndices.add(edgeN0);
    face01N->vertIndices.add(vert0);
    face01N->vertIndices.add(vert1);
    face01N->vertIndices.add(vertN);

    face12N->edgeIndices.add(edge12);
    face12N->edgeIndices.add(edgeN2);
    face12N->edgeIndices.add(edgeN1);
    face12N->vertIndices.add(vert1);
    face12N->vertIndices.add(vert2);
    face12N->vertIndices.add(vertN);

    face20N->edgeIndices.add(edge20);
    face20N->edgeIndices.add(edgeN0);
    face20N->edgeIndices.add(edgeN2);
    face20N->vertIndices.add(vert2);
    face20N->vertIndices.add(vert0);
    face20N->vertIndices.add(vertN);

    edgeN0->vertIndices.add(vertN);
    edgeN0->vertIndices.add(vert0);
    edgeN0->faceIndices.add(face20N);
    edgeN0->faceIndices.add(face01N);

    edgeN1->vertIndices.add(vertN);
    edgeN1->vertIndices.add(vert1);
    edgeN1->faceIndices.add(face01N);
    edgeN1->faceIndices.add(face12N);

    edgeN2->vertIndices.add(vertN);
    edgeN2->vertIndices.add(vert2);
    edgeN2->faceIndices.add(face12N);
    edgeN2->faceIndices.add(face20N);

    // update anything else that has been affected by what we just did
    vert0->faceIndices.remove(vert0->faceIndices.findIndex(face012));
    vert0->faceIndices.add(face20N);
    vert0->faceIndices.add(face01N);
    vert1->faceIndices.remove(vert1->faceIndices.findIndex(face012));
    vert1->faceIndices.add(face01N);
    vert0->faceIndices.add(face12N);
    vert2->faceIndices.remove(vert2->faceIndices.findIndex(face012));
    vert2->faceIndices.add(face12N);
    vert0->faceIndices.add(face20N);

    vert0->edgeIndices.add(edgeN0);
    vert1->edgeIndices.add(edgeN1);
    vert2->edgeIndices.add(edgeN2);

    edge01->faceIndices.remove(edge01->faceIndices.findIndex(face012));
    edge01->faceIndices.add(face01N);
    edge12->faceIndices.remove(edge12->faceIndices.findIndex(face012));
    edge12->faceIndices.add(face12N);
    edge20->faceIndices.remove(edge20->faceIndices.findIndex(face012));
    edge20->faceIndices.add(face20N);

    _data.faces.erase(_data.faces.find(*face012));

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
    for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
    {
        if ((*eit).faceIndices.size() % 2 != 0 || (*eit).faceIndices.size() == 0)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): faceIndices for edge " << &(*eit) << " is of non positive even size " << (*eit).faceIndices.size()  << ". Should be even." << std::endl;
            std::cout << (*eit) << std::endl;
            good = false;
        }
        if ((*eit).vertIndices.size() != 2)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for edge " << &(*eit) << " is of size " << (*eit).vertIndices.size() << ". Should be 2." << std::endl;
            good = false;
            std::cout << (*eit) << std::endl;
        }
    }
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        if ((*fit).vertIndices.size() != 3)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): vertIndices for face " << &(*fit) << " is of size " << (*fit).vertIndices.size() << ". Should be 3." << std::endl;
            good = false;
            std::cout << (*fit) << std::endl;
        }
        if ((*fit).edgeIndices.size() != 3)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): edgeIndices for face " << &(*fit) << " is of size " << (*fit).edgeIndices.size() << ". Should be 3." << std::endl;
            good = false;
            std::cout << (*fit) << std::endl;
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkNoDuplicates( PLuint verbose, PLuint depth )
{
    bool good(true);
    plSeq<const void*> indices; // untyped, since we're only checking for duplicate memory addresses.
    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); vit++)
    {
        // check edges
        indices.clear();
        for (PLuint j = 0; j < (*vit).edgeIndices.size(); j++)
        {
            if (indices.findIndex((*vit).edgeIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << &(*vit) << " contains a duplicate edge: " << (*vit).edgeIndices[j] << "." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
            }
            else
            {
                indices.add((*vit).edgeIndices[j]);
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*vit).faceIndices.size(); j++)
        {
            if (indices.findIndex((*vit).faceIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << &(*vit) << " contains a duplicate face: " << (*vit).faceIndices[j] << "." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
            }
            else
            {
                indices.add((*vit).faceIndices[j]);
            }
        }
    }
    for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*eit).vertIndices.size(); j++)
        {
            if (indices.findIndex((*eit).vertIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << &(*eit) << " contains a duplicate vertex: " << (*eit).vertIndices[j] << "." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
            }
            else
            {
                indices.add((*eit).vertIndices[j]);
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*eit).faceIndices.size(); j++)
        {
            if (indices.findIndex((*eit).faceIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << &(*eit) << " contains a duplicate face: " << (*eit).faceIndices[j] << "." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
            }
            else
            {
                indices.add((*eit).faceIndices[j]);
            }
        }
    }
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*fit).vertIndices.size(); j++)
        {
            if (indices.findIndex((*fit).vertIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << &(*fit) << " contains a duplicate vertex: " << (*fit).vertIndices[j] << "." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
            }
            else
            {
                indices.add((*fit).vertIndices[j]);
            }
        }
        // check edges
        indices.clear();
        for (PLuint j = 0; j < (*fit).edgeIndices.size(); j++)
        {
            if (indices.findIndex((*fit).edgeIndices[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << &(*fit) << " contains a duplicate edge: " << (*fit).edgeIndices[j] << "." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
            }
            else
            {
                indices.add((*fit).edgeIndices[j]);
            }
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkBidirectional( PLuint verbose, PLuint depth )
{
    bool good(true);
    plSeq<PLuint> indices;
    for (plMeshConnectivityDataVertIterator vit = _data.verts.begin(); vit != _data.verts.end(); vit++)
    {
        // check edges
        indices.clear();
        for (PLuint j = 0; j < (*vit).edgeIndices.size(); j++)
        {
            if ((*vit).edgeIndices[j]->vertIndices.findIndex(&(*vit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << &(*vit) << " references edge " << (*vit).edgeIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
                std::cout << (*((*vit).edgeIndices[j])) << std::endl;
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*vit).faceIndices.size(); j++)
        {
            if ((*vit).faceIndices[j]->vertIndices.findIndex(&(*vit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << &(*vit) << " references face " << (*vit).faceIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
                std::cout << (*((*vit).faceIndices[j])) << std::endl;
            }
        }
    }
    for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*eit).vertIndices.size(); j++)
        {
            if ((*eit).vertIndices[j]->edgeIndices.findIndex(&(*eit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << &(*eit) << " references vert " << (*eit).vertIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
                std::cout << (*((*eit).vertIndices[j])) << std::endl;
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*eit).faceIndices.size(); j++)
        {
            if ((*eit).faceIndices[j]->edgeIndices.findIndex(&(*eit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << &(*eit) << " references face " << (*eit).faceIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
                std::cout << (*((*eit).faceIndices[j])) << std::endl;
            }
        }
    }
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*fit).vertIndices.size(); j++)
        {
            if ((*fit).vertIndices[j]->faceIndices.findIndex(&(*fit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << &(*fit) << " references vert " << (*fit).vertIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
                std::cout << (*((*fit).vertIndices[j])) << std::endl;
            }
        }
        // check edges
        indices.clear();
        for (PLuint j = 0; j < (*fit).edgeIndices.size(); j++)
        {
            if ((*fit).edgeIndices[j]->faceIndices.findIndex(&(*fit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << &(*fit) << " references edge " << (*fit).edgeIndices[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
                std::cout << (*((*fit).edgeIndices[j])) << std::endl;
            }
        }
    }
    return good;
}

PLbool plMeshAlgorithm::_checkNoSliverTriangles( PLuint verbose, PLuint depth )
{
    PLbool good(true);
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        if (( (((*fit).face.point1())-((*fit).face.point0())).normalize() ^
              (((*fit).face.point2())-((*fit).face.point0())).normalize()).length() == 0.f )
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoSliverTriangles(): face " << &(*fit) << " appears to be ultra thin. This is bad." << std::endl;
            good = false;
            std::cout << (*fit) << std::endl;
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
        const plTriangle& currentTriangle = tris[i];

        plMeshConnectivityDataVert vert0Insert;
        vert0Insert.vert = currentTriangle[0];
        const plMeshConnectivityDataVert* vert0;
        if (!_findVert(currentTriangle[0],vert0,verbose,depth+1)) return false;
        if (vert0 == NULL)
        {
            _data.verts.insert(vert0Insert);
            vert0 = &(*(_data.verts.find(*vert0)));
        }

        plMeshConnectivityDataVert vert1Insert;
        vert1Insert.vert = currentTriangle[1];
        const plMeshConnectivityDataVert* vert1;
        if (!_findVert(currentTriangle[1],vert1,verbose,depth+1)) return false;
        if (vert1 == NULL)
        {
            _data.verts.insert(vert1Insert);
            vert1 = &(*(_data.verts.find(*vert1)));
        }

        plMeshConnectivityDataVert vert2Insert;
        vert2Insert.vert = currentTriangle[2];
        const plMeshConnectivityDataVert* vert2;
        if (!_findVert(currentTriangle[2],vert2,verbose,depth+1)) return false;
        if (vert2 == NULL)
        {
            _data.verts.insert(vert2Insert);
            vert2 = &(*(_data.verts.find(*vert2)));
        }

        // edges

        plMeshConnectivityDataEdge edge01Insert;
        edge01Insert.edge = plEdge(vert0->vert,vert1->vert);
        edge01Insert.vertIndices.add(vert0);
        edge01Insert.vertIndices.add(vert1);
        _data.edges.insert(edge01Insert);
        const plMeshConnectivityDataEdge* edge01;
        edge01 = &(*(_data.edges.find(edge01Insert)));

        plMeshConnectivityDataEdge edge12Insert;
        edge12Insert.edge = plEdge(vert1->vert,vert2->vert);
        edge12Insert.vertIndices.add(vert1);
        edge12Insert.vertIndices.add(vert2);
        _data.edges.insert(edge12Insert);
        const plMeshConnectivityDataEdge* edge12;
        edge12 = &(*(_data.edges.find(edge12Insert)));

        plMeshConnectivityDataEdge edge20Insert;
        edge20Insert.edge = plEdge(vert2->vert,vert0->vert);
        edge20Insert.vertIndices.add(vert2);
        edge20Insert.vertIndices.add(vert0);
        _data.edges.insert(edge20Insert);
        const plMeshConnectivityDataEdge* edge20;
        edge20 = &(*(_data.edges.find(edge20Insert)));

        // this should only be done if the vertex did not exist. Didn't do earlier since we didn't know the edge indices yet
        if (vert0->edgeIndices.findIndex(edge01) == -1)  vert0->edgeIndices.add(edge01);
        if (vert0->edgeIndices.findIndex(edge20) == -1)  vert0->edgeIndices.add(edge20);
        if (vert1->edgeIndices.findIndex(edge01) == -1)  vert1->edgeIndices.add(edge01);
        if (vert1->edgeIndices.findIndex(edge12) == -1)  vert1->edgeIndices.add(edge12);
        if (vert2->edgeIndices.findIndex(edge12) == -1)  vert2->edgeIndices.add(edge12);
        if (vert2->edgeIndices.findIndex(edge20) == -1)  vert2->edgeIndices.add(edge20);

        // now add the face
        plMeshConnectivityDataFace face012Insert;
        face012Insert.face = plTriangle(vert0->vert,vert1->vert,vert2->vert);
        _data.faces.insert(face012Insert);
        const plMeshConnectivityDataFace* face012;
        face012 = &(*(_data.faces.find(face012Insert)));

        // now update connectivity information for faces
        vert0->faceIndices.add(face012);
        vert1->faceIndices.add(face012);
        vert2->faceIndices.add(face012);
        edge01->faceIndices.add(face012);
        edge12->faceIndices.add(face012);
        edge20->faceIndices.add(face012);
        face012->vertIndices.add(vert0);
        face012->vertIndices.add(vert1);
        face012->vertIndices.add(vert2);
        face012->edgeIndices.add(edge01);
        face012->edgeIndices.add(edge12);
        face012->edgeIndices.add(edge20);
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
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        tris.add((*fit).face);
    }
    return true;
}
