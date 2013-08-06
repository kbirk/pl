#include "plIGuide.h" 

plIGuide::plIGuide() 
{
}

plIGuide::plIGuide( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model ), boundary( model )
{
}

void plIGuide::importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    // Fill in the field
    plString subfield = row[2];
    
    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = plString::fromString<PLint>( row[3] ); 
        if (models.size() <= _modelID )
        {
            std::cerr << "plDefectSite importCSV() error: model ID read before model data";
            exit(1);
        }
        _model = models[_modelID];
    }                   
    else if (subfield.compareCaseInsensitive( "boundary") )
    {      
        if (_model == NULL)
        {
            std::cerr << "plDefectSite importCSV() error: spline data read before model ID";
            exit(1);
        }                 
        boundary.importCSV( row, *_model );        
    } 
    else if (subfield.compareCaseInsensitive( "graft indices") ) 
    {
        for (PLuint j=3; j < row.size(); j++)
        {
            graftIndices.add( atoi( row[j].c_str() ) );
        }  
    } 
    else
    {
        std::cerr << "Error importing plan,'iguide': Unrecognized word '" << subfield << "' in third column." << std::endl;
    }
}

// ============================= BEGIN TEMPLATE CREATION =================================

PLbool plIGuide::generateMeshes()
{

    // error checking
    /*if (boundary.size() <= 2)
    {
        std::cerr << "Error: Boundary is of insufficient size. Aborting." << std::endl;
        return false;
    }*/

    plSeq<plTriangle> roundCylinder;
    plSTL::importFile(roundCylinder, "./templateElements/Generator_Cylinder_Round.stl");

    plSeq<plTriangle> sharpCylinder;
    plSTL::importFile(sharpCylinder, "./templateElements/Generator_Cylinder_Sharp.stl");

    plSeq<plTriangle> keyCube;
    plSTL::importFile(keyCube, "./templateElements/Generator_Key_Cube.stl");



    std::cout << "Initializing transforms" << std::endl;
    plMatrix44 harvestTransform   ( (*grafts)[0]->harvest.transform.matrix()   );
    plMatrix44 recipientTransform ( (*grafts)[0]->recipient.transform.matrix() );



    std::cout << "Initializing variables" << std::endl;
    PLfloat   cylinderRecipientSupportOffset  ( 12.f );

    plVector3 cylinderRecipientHoleScale   (  8.f, 36.f,  8.f  );
    plVector3 cylinderRecipientSleeveScale ( 12.f, 30.f, 12.f  );
    plVector3 cylinderRecipientBaseScale   ( 14.f, 20.f, 14.f  );
    plVector3 cylinderRecipientHolderScale ( 22.f, 20.f, 22.f  );
    plVector3 cylinderRecipientSupportScale( 18.f,  4.f, 18.f  );

    plVector3 cylinderHarvestHoleScale     (  8.f, 36.f,  8.f  );
    plVector3 cylinderHarvestSleeveScale   ( 12.f, 30.f, 12.f  );
    plVector3 cylinderHarvestBaseScale     ( 14.f, 20.f, 14.f  );
    plVector3 cylinderHarvestHolderScale   ( 22.f, 20.f, 22.f  );

    plVector3 keyScale                     (  3.f, 29.9f, 4.f );
    PLfloat   keyHarvestTranslation        (  6.f  );
    PLfloat   keyRecipientTranslation      (  6.f  );

    PLfloat   keyRotation ( 0.f );

    PLfloat   zero        ( 0.f );
    plVector3 zeroVector  ( 0.f, 0.f, 0.f );



    std::cout << "Transforming meshes" << std::endl;
    plSeq<plTriangle> cylinderRecipientHole   ( createTemplatePieceTransformed(sharpCylinder, recipientTransform, zero,                           cylinderRecipientHoleScale,   zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientSleeve ( createTemplatePieceTransformed(roundCylinder, recipientTransform, zero,                           cylinderRecipientSleeveScale, zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientBase   ( createTemplatePieceTransformed(sharpCylinder, recipientTransform, zero,                           cylinderRecipientBaseScale,   zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientHolder ( createTemplatePieceTransformed(roundCylinder, recipientTransform, zero,                           cylinderRecipientHolderScale, zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientSupport( createTemplatePieceTransformed(sharpCylinder, recipientTransform, cylinderRecipientSupportOffset, cylinderRecipientSupportScale,zero,                    zero) );
    plSeq<plTriangle> keyRecipient            ( createTemplatePieceTransformed(keyCube,       recipientTransform, zero,                           keyScale,                     keyRecipientTranslation, zero) );

    plSeq<plTriangle> cylinderHarvestHole     ( createTemplatePieceTransformed(sharpCylinder, harvestTransform,   zero,                           cylinderHarvestHoleScale,     zero,                    zero) );
    plSeq<plTriangle> cylinderHarvestSleeve   ( createTemplatePieceTransformed(roundCylinder, harvestTransform,   zero,                           cylinderHarvestSleeveScale,   zero,                    zero) );
    plSeq<plTriangle> cylinderHarvestBase     ( createTemplatePieceTransformed(sharpCylinder, harvestTransform,   zero,                           cylinderHarvestBaseScale,     zero,                    zero) );
    plSeq<plTriangle> cylinderHarvestHolder   ( createTemplatePieceTransformed(roundCylinder, harvestTransform,   zero,                           cylinderHarvestHolderScale,   zero,                    zero) );
    plSeq<plTriangle> keyHarvest              ( createTemplatePieceTransformed(keyCube,       harvestTransform,   zero,                           keyScale,                     keyHarvestTranslation,   zero) );



    std::cout << "Exporting Meshes" << std::endl;
    plSTL::exportFileBinary(cylinderRecipientHole,   "./output/ALRecipientHole.stl"   );
    plSTL::exportFileBinary(cylinderRecipientSleeve, "./output/ALRecipientSleeve.stl" );
    plSTL::exportFileBinary(cylinderRecipientBase,   "./output/ALRecipientBase.stl"   );
    plSTL::exportFileBinary(cylinderRecipientHolder, "./output/ALRecipientHolder.stl" );
    plSTL::exportFileBinary(cylinderRecipientSupport,"./output/ALRecipientSupport.stl");
    plSTL::exportFileBinary(keyRecipient,            "./output/ALRecipientKey.stl"    );
    plSTL::exportFileBinary(cylinderHarvestHole,     "./output/ALHarvestHole.stl"     );
    plSTL::exportFileBinary(cylinderHarvestSleeve,   "./output/ALHarvestSleeve.stl"   );
    plSTL::exportFileBinary(cylinderHarvestBase,     "./output/ALHarvestBase.stl"     );
    plSTL::exportFileBinary(cylinderHarvestHolder,   "./output/ALHarvestHolder.stl"   );
    plSTL::exportFileBinary(keyHarvest,              "./output/ALHarvestKey.stl"      );



    return true;
}

// --------------------------- CREATE TEMPLATE BASE SHAPE ------------------------------

void plIGuide::createTemplateBaseShape(const plSeq<plTriangle> &cartilageTris, const plGraft &graft, const plBoundary &boundary)
{

    templateBase.clear();
    //templateBase = cartilageTris; // should be safe to remove this

    // find the surface for the iGuide
    plMeshCutter::findInteriorMesh( cartilageTris , boundary , templateBase ); // find surface
    plSeq<edgeIndices> outsideEdges (collectOutsideEdges());

    // store these for bookkeeping. This is the index where upper (extruded) edges/triangles will start
    PLuint indexOfUpperEdges (outsideEdges.size());
    PLuint indexOfUpperTriangles (templateBase.size());

    // we need to extrude a bunch of these triangles in the direction of the average normal
    PLfloat offsetMagnitude(5.f);
    plVector3 offset ( offsetMagnitude * boundary.getAverageNormal() );
    for (PLuint i = 0; i < indexOfUpperTriangles; i++)
    {
        plVector3 point0( templateBase[i].point0() + offset );
        plVector3 point1( templateBase[i].point1() + offset );
        plVector3 point2( templateBase[i].point2() + offset );
        plVector3 normal( templateBase[i].normal() );
        templateBase.add( plTriangle(normal,point0,point1,point2) );
    } // end for

    // update edgeIndices
    for (PLuint i = 0; i < indexOfUpperEdges; i++)
    {
        outsideEdges.add(outsideEdges[i]);
        outsideEdges[i+indexOfUpperEdges].triangleIndex = outsideEdges[i+indexOfUpperEdges].triangleIndex + indexOfUpperTriangles;
    }

    // we need to reverse the normals of the triangles that lie on top of the knee surface
    for (PLuint i = 0; i < indexOfUpperTriangles; i++)
    {
        templateBase[i].swapVertices0And1(); // reverses the normal
    } // end for

    // update edgeIndices
    for (PLuint i = 0; i < indexOfUpperEdges; i++)
    {
        outsideEdges[i].vertexIndexA = (outsideEdges[i].vertexIndexA==2?2:!outsideEdges[i].vertexIndexA); // results in mapping: 2 => 2, 1 => 0, 0 => 1. Since vertices 0 and 1 were flipped.
        outsideEdges[i].vertexIndexB = (outsideEdges[i].vertexIndexB==2?2:!outsideEdges[i].vertexIndexB);
    }

    // now fill in the sides of the template with triangles
    for (PLuint i = 0; i < indexOfUpperEdges; i++)
    {
        PLuint lowerTriangleIndex (outsideEdges[i].triangleIndex);
        PLuint lowerVertexIndexA  (outsideEdges[i].vertexIndexA);
        PLuint lowerVertexIndexB  (outsideEdges[i].vertexIndexB);
        PLuint upperTriangleIndex (outsideEdges[i+indexOfUpperEdges].triangleIndex);
        PLuint upperVertexIndexA  (outsideEdges[i+indexOfUpperEdges].vertexIndexA);
        PLuint upperVertexIndexB  (outsideEdges[i+indexOfUpperEdges].vertexIndexB);
        templateBase.add(plTriangle(templateBase[lowerTriangleIndex][lowerVertexIndexA],
                                    templateBase[lowerTriangleIndex][lowerVertexIndexB],
                                    templateBase[upperTriangleIndex][upperVertexIndexA] ) );
        templateBase.add(plTriangle(templateBase[lowerTriangleIndex][lowerVertexIndexB],
                                    templateBase[upperTriangleIndex][upperVertexIndexB],
                                    templateBase[upperTriangleIndex][upperVertexIndexA] ) );
    } // end for

}


// ---------------------------------- COLLECT OUTSIDE EDGES ---------------------------------

plSeq<plIGuide::edgeIndices> plIGuide::collectOutsideEdges()
{
    plSeq<plIGuide::edgeIndices> edges;
    edges = collectOutsideEdgesUnsorted();
    edges = collectOutsideEdgesSort(edges);
    return edges;
}

// helper to collectOutsideEdges
plSeq<plIGuide::edgeIndices> plIGuide::collectOutsideEdgesUnsorted()
{
    // all edges that are on the border will be unique -
    // that is to say, it will occur exactly once within the set of triangles.
    // any edge that occurs twice must be between two faces, and is therefore not on the border

    plSeq<edgeIndices> unsortedEdges;  // this stores the indices for bookkeeping and future manipulation
    for (PLuint triangleIndex = 0; triangleIndex < templateBase.size(); triangleIndex++)
    {
        // examine each edge
        for (PLuint vertexIndex = 0; vertexIndex < 3; vertexIndex++)
        {
            // vertex pair
            PLuint vertexAIndex = vertexIndex;
            PLuint vertexBIndex = (vertexIndex+1) % 3;

            // try to find the edge in question
            PLuint unsortedEdgeIndex;
            for (unsortedEdgeIndex = 0; unsortedEdgeIndex < unsortedEdges.size(); unsortedEdgeIndex++)
            {
                // for readability - these are the indices to get to the vertices that we'll actually be looking at
                PLuint searchTriangleIndex (unsortedEdges[unsortedEdgeIndex].triangleIndex);
                PLuint searchVertexAIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexA);
                PLuint searchVertexBIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexB);

                // any edge that is being examined will be oriented opposite to what was already found
                if (templateBase[searchTriangleIndex][searchVertexAIndex] == templateBase[triangleIndex][vertexBIndex] &&
                    templateBase[searchTriangleIndex][searchVertexBIndex] == templateBase[triangleIndex][vertexAIndex])
                {
                    break;
                }
            } // end for

            if (unsortedEdgeIndex == unsortedEdges.size()) // if the edge does not already exist, add it to the list
            {
                unsortedEdges.add( edgeIndices(triangleIndex, vertexIndex, ((vertexIndex+1)%3) ) );
            }
            else // if the edge DOES exist, it must be between two faces and therefore can't be on the border... so remove it
            {
                unsortedEdges.remove(unsortedEdgeIndex);
            } // end if
        } // end for
    } // end for
    return unsortedEdges;
}

// helper to collectOutsideEdges
plSeq<plIGuide::edgeIndices> plIGuide::collectOutsideEdgesSort(plSeq<plIGuide::edgeIndices> unsortedEdges)
{
    // traverse along the edges and return to the first vertex
    plSeq<edgeIndices> sortedEdges;  // this stores the indices for bookkeeping and future manipulation

    plVector3 finalVertex   = templateBase[unsortedEdges[0].triangleIndex][unsortedEdges[0].vertexIndexA]; // the final vertex should be this, this should be checked
    plVector3 currentVertex = templateBase[unsortedEdges[0].triangleIndex][unsortedEdges[0].vertexIndexB]; // the current vertex, we'll look to 'hop' to another edge with this vertex

    sortedEdges.add(unsortedEdges[0]); // first edge, arbitrarily chosen to be here
    unsortedEdges.remove(0);           // don't need to search here, so remove it from the list of unsorted edges

    while (unsortedEdges.size())
    {
        // search for the current vertex in the list of edges. It will appear as the first vertex in an ordered pair, since all edges are counter-clockwise
        PLuint unsortedEdgeIndex;
        for (unsortedEdgeIndex = 0; unsortedEdgeIndex < unsortedEdges.size(); unsortedEdgeIndex++)
        {
            PLuint triangleIndex (unsortedEdges[unsortedEdgeIndex].triangleIndex);
            PLuint vertexAIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexA);
            if (templateBase[triangleIndex][vertexAIndex] == currentVertex)
            {
                break;
            }
        } // end for

        if (unsortedEdgeIndex == unsortedEdges.size() || (unsortedEdges.size() == 0 && currentVertex != finalVertex)) // should not happen if the edges contain a closed surface
        {
            std::cerr << "Error in plIGuide::collectOutsideEdges: the edges don't contain a closed surface! Aborting." << std::endl;
            exit(1);
        }
        else
        {
            // update the current vertex
            PLuint triangleIndex (unsortedEdges[unsortedEdgeIndex].triangleIndex);
            PLuint vertexBIndex  (unsortedEdges[unsortedEdgeIndex].vertexIndexB);
            currentVertex = (templateBase[triangleIndex][vertexBIndex]);

            // move this entry into the sorted list
            sortedEdges.add(unsortedEdges[unsortedEdgeIndex]);
            unsortedEdges.remove(unsortedEdgeIndex);
        } // end if
    } // end while

    return sortedEdges;
}

// ---------------------- CREATE TRANSFORMED COPY OF PIECE -------------------------

plSeq<plTriangle> plIGuide::createTemplatePieceTransformed ( const plSeq<plTriangle>& baseTriObject ,
                                                             const plMatrix44&  plugTransform,
                                                             const PLdouble&    zOffset,
                                                             const plVector3&   scale ,
                                                             const PLdouble&    keyTranslationXAxis ,
                                                             const PLdouble&    keyRotationZAxis )
{
    plMatrix44 zTranslationMatrix =     plMatrix44();     zTranslationMatrix.setTranslation(0., zOffset, 0.);
    plMatrix44 scaleMatrix =            plMatrix44();     scaleMatrix.setScale(scale);
    plMatrix44 keyTranslationMatrix =   plMatrix44();     keyTranslationMatrix.setTranslation(keyTranslationXAxis, 0., 0.);
    plMatrix44 keyRotationMatrix =      plMatrix44();     keyRotationMatrix.setRotationD(keyRotationZAxis, plVector3(0,1,0));
    plMatrix44 transformationMatrix = plugTransform * zTranslationMatrix * keyRotationMatrix * keyTranslationMatrix * scaleMatrix;

    plSeq<plTriangle> outputTriObject;
    for (int i=0; i<baseTriObject.size(); i++)
    {
        outputTriObject.add( transformationMatrix * baseTriObject[i] );
    }
    return outputTriObject;
}


// ============================ END THOMAS' SECTION ================================

void plIGuide::draw() 
{
    plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}
