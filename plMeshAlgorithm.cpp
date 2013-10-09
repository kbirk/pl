#include "plMeshAlgorithm.h"

plMeshAlgorithm::plMeshAlgorithm()
{
    _epsilon = PL_EPSILON;
}

PLbool plMeshAlgorithm::_findVert( const plVector3& vertex, const plMeshConnectivityDataVert*& vertPointer, PLuint verbose, PLuint depth )
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
        for (PLuint j=0;j<_data.edges[edgeABindex].faces.size();j++)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Debug: Attached face: " << _data.faces[_data.edges[edgeABindex].faces[j]].face.point0() << " | " << _data.faces[_data.edges[edgeABindex].faces[j]].face.point1() << " | " << _data.faces[_data.edges[edgeABindex].faces[j]].face.point2() << std::endl;
        }
    }*/

    const plMeshConnectivityDataVert* vertA = edgeAB->verts[0];
    const plMeshConnectivityDataVert* vertB = edgeAB->verts[1];

    // create the new cells, storing their eventual indices

    plMeshConnectivityDataEdge edgeANInsert;
    edgeANInsert.verts.add(vertA);
    edgeANInsert.verts.add(vertN);
    edgeANInsert.edge = plEdge(vertA->vert, vertN->vert);
    _data.edges.insert(edgeANInsert);

    plMeshConnectivityDataEdge edgeNBInsert;
    edgeNBInsert.verts.add(vertN);
    edgeNBInsert.verts.add(vertB);
    edgeNBInsert.edge = plEdge(vertN->vert, vertB->vert);
    _data.edges.insert(edgeNBInsert);

    const plMeshConnectivityDataEdge* edgeAN = &(*(_data.edges.find(edgeANInsert)));
    const plMeshConnectivityDataEdge* edgeNB = &(*(_data.edges.find(edgeNBInsert)));

    // fill the cells with data, but faces will be treated separately.
    vertN->edges.add(edgeAN);
    vertN->edges.add(edgeNB);

    // split any attached faces each into two pieces...
    plSeq<const plMeshConnectivityDataFace*> facesToSplit (edgeAB->faces);
    for (PLuint i = 0; i < facesToSplit.size(); i++)
    {
        // find all existing cells, have them available in case they're needed later
        const plMeshConnectivityDataFace* faceABC = facesToSplit[i];

        // TODO: Consider adding a check to make sure that the face doesn't contain one of the edges we're adding...
        const plMeshConnectivityDataVert* vertC(NULL);
        for (PLuint j = 0; j < 3; j++)
        {
            if (faceABC->verts[j] != vertA && faceABC->verts[j] != vertB)
            {
                vertC = faceABC->verts[j];
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
            const plMeshConnectivityDataEdge* currentEdge = faceABC->edges[j];
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
        edgeNCInsert.verts.add(vertN);
        edgeNCInsert.verts.add(vertC);
        edgeNCInsert.edge = plEdge(vertN->vert,vertC->vert);
        _data.edges.insert(edgeNCInsert);
        const plMeshConnectivityDataEdge* edgeNC  = &(*(_data.edges.find(edgeNCInsert )));

        plMeshConnectivityDataFace faceANCInsert;
        plMeshConnectivityDataFace faceBNCInsert;
        if (faceOrientationABC)
        {
            faceANCInsert.face = plTriangle(vertA->vert,vertN->vert,vertC->vert);
            faceBNCInsert.face = plTriangle(vertB->vert,vertC->vert,vertN->vert);
            faceANCInsert.verts.add(vertA);
            faceANCInsert.verts.add(vertN);
            faceANCInsert.verts.add(vertC);
            faceBNCInsert.verts.add(vertB);
            faceBNCInsert.verts.add(vertC);
            faceBNCInsert.verts.add(vertN);
        }
        else
        {
            faceANCInsert.face = plTriangle(vertC->vert,vertN->vert,vertA->vert);
            faceBNCInsert.face = plTriangle(vertN->vert,vertC->vert,vertB->vert);
            faceANCInsert.verts.add(vertC);
            faceANCInsert.verts.add(vertN);
            faceANCInsert.verts.add(vertA);
            faceBNCInsert.verts.add(vertN);
            faceBNCInsert.verts.add(vertC);
            faceBNCInsert.verts.add(vertB);
        }
        faceANCInsert.edges.add(edgeAN);
        faceANCInsert.edges.add(edgeNC);
        faceANCInsert.edges.add(edgeAC);
        faceBNCInsert.edges.add(edgeNB);
        faceBNCInsert.edges.add(edgeNC);
        faceBNCInsert.edges.add(edgeBC);
        _data.faces.insert(faceANCInsert);
        _data.faces.insert(faceBNCInsert);
        const plMeshConnectivityDataFace* faceANC(NULL);
        faceANC = &(*(_data.faces.find(faceANCInsert)));
        const plMeshConnectivityDataFace* faceBNC(NULL);
        faceBNC = &(*(_data.faces.find(faceBNCInsert)));

        // now fill the cells with stuff!
        edgeNC->faces.add(faceANC);
        edgeNC->faces.add(faceBNC);

        // update anything else that has been affected by what we just did
        edgeAN->faces.add(faceANC);
        edgeNB->faces.add(faceBNC);

        edgeAC->faces.remove(edgeAC->faces.findIndex(faceABC));
        edgeAC->faces.add(faceANC);
        edgeBC->faces.remove(edgeBC->faces.findIndex(faceABC));
        edgeBC->faces.add(faceBNC);
        vertA->faces.remove(vertA->faces.findIndex(faceABC));
        vertA->faces.add(faceANC);
        vertB->faces.remove(vertB->faces.findIndex(faceABC));
        vertB->faces.add(faceBNC);

        //if (vertNsearchIndex == -1) // only if the vertex is new should this next thing be added
        vertN->faces.add(faceANC);
        vertN->faces.add(faceBNC);
        vertC->faces.add(faceANC);
        vertC->faces.add(faceBNC);

        vertN->edges.add(edgeNC);
        vertC->edges.add(edgeNC);

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
    vertA->edges.remove(vertA->edges.findIndex(edgeAB));
    vertB->edges.remove(vertB->edges.findIndex(edgeAB));
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

    const plMeshConnectivityDataVert* vert0 = face012->verts[0];
    const plMeshConnectivityDataVert* vert1 = face012->verts[1];
    const plMeshConnectivityDataVert* vert2 = face012->verts[2];

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

    for (PLuint i = 0; i < face012->edges.size(); i++)
    {
        if (face012->edges[i]->edge.contains(vert0->vert, _epsilon) &&
            face012->edges[i]->edge.contains(vert1->vert, _epsilon) )
            edge01 = face012->edges[i];
        if (face012->edges[i]->edge.contains(vert1->vert, _epsilon) &&
            face012->edges[i]->edge.contains(vert2->vert, _epsilon) )
            edge12 = face012->edges[i];
        if (face012->edges[i]->edge.contains(vert2->vert, _epsilon) &&
            face012->edges[i]->edge.contains(vert0->vert, _epsilon) )
            edge20 = face012->edges[i];
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
    edgeN0Insert.verts.add(vertN);
    edgeN0Insert.verts.add(vert0);
    _data.edges.insert(edgeN0Insert);
    const plMeshConnectivityDataEdge* edgeN0(NULL);
    edgeN0 = &(*(_data.edges.find(edgeN0Insert )));

    plMeshConnectivityDataEdge edgeN1Insert;
    edgeN1Insert.edge = plEdge(vertN->vert,vert1->vert);
    edgeN1Insert.verts.add(vertN);
    edgeN1Insert.verts.add(vert1);
    _data.edges.insert(edgeN1Insert);
    const plMeshConnectivityDataEdge* edgeN1(NULL);
    edgeN1 = &(*(_data.edges.find(edgeN1Insert )));

    plMeshConnectivityDataEdge edgeN2Insert;
    edgeN2Insert.edge = plEdge(vertN->vert,vert2->vert);
    edgeN2Insert.verts.add(vertN);
    edgeN2Insert.verts.add(vert2);
    _data.edges.insert(edgeN2Insert);
    const plMeshConnectivityDataEdge* edgeN2(NULL);
    edgeN2 = &(*(_data.edges.find(edgeN2Insert )));

    plMeshConnectivityDataFace face01NInsert;
    face01NInsert.face = plTriangle(vert0->vert,vert1->vert,vertN->vert);
    face01NInsert.edges.add(edge01);
    face01NInsert.edges.add(edgeN1);
    face01NInsert.edges.add(edgeN0);
    face01NInsert.verts.add(vert0);
    face01NInsert.verts.add(vert1);
    face01NInsert.verts.add(vertN);
    _data.faces.insert(face01NInsert);
    const plMeshConnectivityDataFace* face01N(NULL);
    face01N = &(*(_data.faces.find(face01NInsert )));

    plMeshConnectivityDataFace face12NInsert;
    face12NInsert.face = plTriangle(vert1->vert,vert2->vert,vertN->vert);
    face12NInsert.edges.add(edge12);
    face12NInsert.edges.add(edgeN2);
    face12NInsert.edges.add(edgeN1);
    face12NInsert.verts.add(vert1);
    face12NInsert.verts.add(vert2);
    face12NInsert.verts.add(vertN);
    _data.faces.insert(face12NInsert);
    const plMeshConnectivityDataFace* face12N(NULL);
    face12N = &(*(_data.faces.find(face12NInsert )));

    plMeshConnectivityDataFace face20NInsert;
    face20NInsert.face = plTriangle(vert2->vert,vert0->vert,vertN->vert);
    face20NInsert.edges.add(edge20);
    face20NInsert.edges.add(edgeN0);
    face20NInsert.edges.add(edgeN2);
    face20NInsert.verts.add(vert2);
    face20NInsert.verts.add(vert0);
    face20NInsert.verts.add(vertN);
    _data.faces.insert(face20NInsert);
    const plMeshConnectivityDataFace* face20N(NULL);
    face20N = &(*(_data.faces.find(face20NInsert )));

    // fill the cells with data
    vertN->faces.add(face01N);
    vertN->faces.add(face12N);
    vertN->faces.add(face20N);
    vertN->edges.add(edgeN0);
    vertN->edges.add(edgeN1);
    vertN->edges.add(edgeN2);

    edgeN0->faces.add(face20N);
    edgeN0->faces.add(face01N);
    edgeN1->faces.add(face01N);
    edgeN1->faces.add(face12N);
    edgeN2->faces.add(face12N);
    edgeN2->faces.add(face20N);

    // update anything else that has been affected by what we just did
    vert0->faces.remove(vert0->faces.findIndex(face012));
    vert0->faces.add(face20N);
    vert0->faces.add(face01N);
    vert1->faces.remove(vert1->faces.findIndex(face012));
    vert1->faces.add(face01N);
    vert0->faces.add(face12N);
    vert2->faces.remove(vert2->faces.findIndex(face012));
    vert2->faces.add(face12N);
    vert0->faces.add(face20N);

    vert0->edges.add(edgeN0);
    vert1->edges.add(edgeN1);
    vert2->edges.add(edgeN2);

    edge01->faces.remove(edge01->faces.findIndex(face012));
    edge01->faces.add(face01N);
    edge12->faces.remove(edge12->faces.findIndex(face012));
    edge12->faces.add(face12N);
    edge20->faces.remove(edge20->faces.findIndex(face012));
    edge20->faces.add(face20N);

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
        if ((*eit).faces.size() % 2 != 0 || (*eit).faces.size() == 0)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): faces for edge " << &(*eit) << " is of non positive even size " << (*eit).faces.size()  << ". Should be even." << std::endl;
            std::cout << (*eit) << std::endl;
            good = false;
        }
        if ((*eit).verts.size() != 2)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): verts for edge " << &(*eit) << " is of size " << (*eit).verts.size() << ". Should be 2." << std::endl;
            good = false;
            std::cout << (*eit) << std::endl;
        }
    }
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        if ((*fit).verts.size() != 3)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): verts for face " << &(*fit) << " is of size " << (*fit).verts.size() << ". Should be 3." << std::endl;
            good = false;
            std::cout << (*fit) << std::endl;
        }
        if ((*fit).edges.size() != 3)
        {
            for (PLuint i=0;i<depth;i++)
                std::cout << "\t";
            std::cout << "Error in plMeshIntersectorConnectivityData::_checkArraySizes(): edges for face " << &(*fit) << " is of size " << (*fit).edges.size() << ". Should be 3." << std::endl;
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
        for (PLuint j = 0; j < (*vit).edges.size(); j++)
        {
            if (indices.findIndex((*vit).edges[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << &(*vit) << " contains a duplicate edge: " << (*vit).edges[j] << "." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
            }
            else
            {
                indices.add((*vit).edges[j]);
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*vit).faces.size(); j++)
        {
            if (indices.findIndex((*vit).faces[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): vertex " << &(*vit) << " contains a duplicate face: " << (*vit).faces[j] << "." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
            }
            else
            {
                indices.add((*vit).faces[j]);
            }
        }
    }
    for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*eit).verts.size(); j++)
        {
            if (indices.findIndex((*eit).verts[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << &(*eit) << " contains a duplicate vertex: " << (*eit).verts[j] << "." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
            }
            else
            {
                indices.add((*eit).verts[j]);
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*eit).faces.size(); j++)
        {
            if (indices.findIndex((*eit).faces[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): edge " << &(*eit) << " contains a duplicate face: " << (*eit).faces[j] << "." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
            }
            else
            {
                indices.add((*eit).faces[j]);
            }
        }
    }
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*fit).verts.size(); j++)
        {
            if (indices.findIndex((*fit).verts[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << &(*fit) << " contains a duplicate vertex: " << (*fit).verts[j] << "." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
            }
            else
            {
                indices.add((*fit).verts[j]);
            }
        }
        // check edges
        indices.clear();
        for (PLuint j = 0; j < (*fit).edges.size(); j++)
        {
            if (indices.findIndex((*fit).edges[j]) > -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkNoDuplicates(): face " << &(*fit) << " contains a duplicate edge: " << (*fit).edges[j] << "." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
            }
            else
            {
                indices.add((*fit).edges[j]);
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
        for (PLuint j = 0; j < (*vit).edges.size(); j++)
        {
            if ((*vit).edges[j]->verts.findIndex(&(*vit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << &(*vit) << " references edge " << (*vit).edges[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
                std::cout << (*((*vit).edges[j])) << std::endl;
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*vit).faces.size(); j++)
        {
            if ((*vit).faces[j]->verts.findIndex(&(*vit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): vert " << &(*vit) << " references face " << (*vit).faces[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*vit) << std::endl;
                std::cout << (*((*vit).faces[j])) << std::endl;
            }
        }
    }
    for (plMeshConnectivityDataEdgeIterator eit = _data.edges.begin(); eit != _data.edges.end(); eit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*eit).verts.size(); j++)
        {
            if ((*eit).verts[j]->edges.findIndex(&(*eit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << &(*eit) << " references vert " << (*eit).verts[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
                std::cout << (*((*eit).verts[j])) << std::endl;
            }
        }
        // check faces
        indices.clear();
        for (PLuint j = 0; j < (*eit).faces.size(); j++)
        {
            if ((*eit).faces[j]->edges.findIndex(&(*eit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): edge " << &(*eit) << " references face " << (*eit).faces[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*eit) << std::endl;
                std::cout << (*((*eit).faces[j])) << std::endl;
            }
        }
    }
    for (plMeshConnectivityDataFaceIterator fit = _data.faces.begin(); fit != _data.faces.end(); fit++)
    {
        // check vertices
        indices.clear();
        for (PLuint j = 0; j < (*fit).verts.size(); j++)
        {
            if ((*fit).verts[j]->faces.findIndex(&(*fit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << &(*fit) << " references vert " << (*fit).verts[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
                std::cout << (*((*fit).verts[j])) << std::endl;
            }
        }
        // check edges
        indices.clear();
        for (PLuint j = 0; j < (*fit).edges.size(); j++)
        {
            if ((*fit).edges[j]->faces.findIndex(&(*fit)) == -1)
            {
                for (PLuint i=0;i<depth;i++)
                    std::cout << "\t";
                std::cout << "Error in plMeshIntersectorConnectivityData::_checkBidirectionalConnections(): face " << &(*fit) << " references edge " << (*fit).edges[j] << ", but not vice versa." << std::endl;
                good = false;
                std::cout << (*fit) << std::endl;
                std::cout << (*((*fit).edges[j])) << std::endl;
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
        const plMeshConnectivityDataVert* vert0 (NULL);
        if (!_findVert(currentTriangle[0],vert0,verbose,depth+1)) return false;
        if (vert0 == NULL)
        {
            _data.verts.insert(vert0Insert);
            vert0 = &(*(_data.verts.find(vert0Insert)));
        }

        plMeshConnectivityDataVert vert1Insert;
        vert1Insert.vert = currentTriangle[1];
        const plMeshConnectivityDataVert* vert1 (NULL);
        if (!_findVert(currentTriangle[1],vert1,verbose,depth+1)) return false;
        if (vert1 == NULL)
        {
            _data.verts.insert(vert1Insert);
            vert1 = &(*(_data.verts.find(vert1Insert)));
        }

        plMeshConnectivityDataVert vert2Insert;
        vert2Insert.vert = currentTriangle[2];
        const plMeshConnectivityDataVert* vert2 (NULL);
        if (!_findVert(currentTriangle[2],vert2,verbose,depth+1)) return false;
        if (vert2 == NULL)
        {
            _data.verts.insert(vert2Insert);
            vert2 = &(*(_data.verts.find(vert2Insert)));
        }

        plMeshConnectivityDataEdge edge01Insert;
        edge01Insert.edge = plEdge(vert0->vert,vert1->vert);
        edge01Insert.verts.add(vert0);
        edge01Insert.verts.add(vert1);
        _data.edges.insert(edge01Insert);
        const plMeshConnectivityDataEdge* edge01 (NULL);
        edge01 = &(*(_data.edges.find(edge01Insert)));

        plMeshConnectivityDataEdge edge12Insert;
        edge12Insert.edge = plEdge(vert1->vert,vert2->vert);
        edge12Insert.verts.add(vert1);
        edge12Insert.verts.add(vert2);
        _data.edges.insert(edge12Insert);
        const plMeshConnectivityDataEdge* edge12 (NULL);
        edge12 = &(*(_data.edges.find(edge12Insert)));

        plMeshConnectivityDataEdge edge20Insert;
        edge20Insert.edge = plEdge(vert2->vert,vert0->vert);
        edge20Insert.verts.add(vert2);
        edge20Insert.verts.add(vert0);
        _data.edges.insert(edge20Insert);
        const plMeshConnectivityDataEdge* edge20 (NULL);
        edge20 = &(*(_data.edges.find(edge20Insert)));

        // this should only be done if the vertex did not exist. Didn't do earlier since we didn't know the edge indices yet
        if (vert0->edges.findIndex(edge01) == -1)  vert0->edges.add(edge01);
        if (vert0->edges.findIndex(edge20) == -1)  vert0->edges.add(edge20);
        if (vert1->edges.findIndex(edge01) == -1)  vert1->edges.add(edge01);
        if (vert1->edges.findIndex(edge12) == -1)  vert1->edges.add(edge12);
        if (vert2->edges.findIndex(edge12) == -1)  vert2->edges.add(edge12);
        if (vert2->edges.findIndex(edge20) == -1)  vert2->edges.add(edge20);

        // now add the face
        plMeshConnectivityDataFace face012Insert;
        face012Insert.face = plTriangle(vert0->vert,vert1->vert,vert2->vert);
        face012Insert.verts.add(vert0);
        face012Insert.verts.add(vert1);
        face012Insert.verts.add(vert2);
        face012Insert.edges.add(edge01);
        face012Insert.edges.add(edge12);
        face012Insert.edges.add(edge20);
        _data.faces.insert(face012Insert);
        const plMeshConnectivityDataFace* face012 (NULL);
        face012 = &(*(_data.faces.find(face012Insert)));

        // now update connectivity information for faces
        vert0->faces.add(face012);
        vert1->faces.add(face012);
        vert2->faces.add(face012);
        edge01->faces.add(face012);
        edge12->faces.add(face012);
        edge20->faces.add(face012);
    }
    if (!_checkForAllErrors( verbose,depth+1 )) { std::cout << "ABORTING" << std::endl; return false; }
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
