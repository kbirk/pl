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
        _modelID = atof( row[3].c_str() );
        if (models.size() < (_modelID +1) )
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

void plIGuide::createTemplateBaseShape(const plSeq<plTriangle> &cartilageTris, const plGraft &graft, const plBoundary &boundary)
{

    templateBase.clear();

    // process

    std::cout << "A"<< std::endl;
    plFindInteriorMesh( cartilageTris , boundary , templateBase ); // find surface
    std::cout << "B"<< std::endl;
    plSeq<edgeIndices> outsideEdges (collectOutsideEdges());

    for (PLuint i = 0; i < outsideEdges.size(); i++)
    {
        std::cout << outsideEdges[i].triangleIndex << std::endl;
    } // end for;


    // 3. we need to extrude a bunch of these triangles in the direction of the average normal
    /*PLuint indexOfUpperTriangles = templateBase.size();
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

    // 4. we need to reverse the normals of the triangles that lie on the surface of the knee
    for (PLuint i = 0; i < indexOfUpperTriangles; i++)
    {
        templateBase[i].flipTriangle();
    } // end for*/
}


// ---------------------------------- COLLECT OUTSIDE EDGES ---------------------------------

plSeq<plIGuide::edgeIndices> plIGuide::collectOutsideEdges()
{
    plSeq<plIGuide::edgeIndices> edges;
    std::cout << "1"<< std::endl;
    edges = collectOutsideEdgesUnsorted();
    std::cout << "2"<< std::endl;
    edges = collectOutsideEdgesSort(edges);
    std::cout << "3"<< std::endl;
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
                PLuint searchVertexAIndex  (unsortedEdges[unsortedEdgeIndex].triangleIndex);
                PLuint searchVertexBIndex  (unsortedEdges[unsortedEdgeIndex].triangleIndex);

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

// ============================ END THOMAS' SECTION ================================

void plIGuide::draw() 
{
    plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}
