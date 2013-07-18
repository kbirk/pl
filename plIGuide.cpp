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


void plIGuide::createTemplateBaseShape(const plSeq<plTriangle> &cartilageTris, const plGraft &graft)
{
    templateBase.clear();

    // process
/*    plFindInteriorMesh( cartilageTris , boundary , templateBase ); // find surface

    // now that we have the template base surface, we need to iterate and perform several tasks:
    // 1. we need to determine which of the edges are on the outside of this surface
    plSeq<plEdge> edges;
    for (PLuint triangleIndex = 0; triangleIndex < templateBase.size(); triangleIndex++)
    {
        for (PLuint vertexIndex = 0; vertexIndex < 3; vertexIndex++)
        {
            plEdge edge0 = plEdge(templateBase[triangleIndex][vertexIndex],
                                  templateBase[triangleIndex][(vertexIndex+1)%3]);
            PLint existingIndex ( edges.findIndex(edge0) );
            if (existingIndex == -1)
                edges.add(edge0);
            else
                edges.remove(existingIndex);
        } // end for
    } // end for

    // 2. we need to extrude a bunch of these triangles in the direction of the average normal
    PLuint offsetToUpperTriangles = templateBase.size();
    PLfloat offsetMagnitude(5.f);
    plVector3 offset ( offsetMagnitude * boundary.getAverageNormal() );
    for (PLuint i = 0; i < offsetToUpperTriangles; i++)
    {
        plVector3 point0( templateBase[i].point0() + offset );
        plVector3 point1( templateBase[i].point1() + offset );
        plVector3 point2( templateBase[i].point2() + offset );
        plVector3 normal( templateBase[i].normal() );
        templateBase.add( plTriangle(normal,point0,point1,point2) );
    } // end for

    // 3. we need to reverse the normals of the triangles that lie on the surface of the knee
    for (PLuint i = 0; i < offsetToUpperTriangles; i++)
    {
        templateBase[i].flipTriangle();
    } // end for*/
}


void plIGuide::draw() 
{
    plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY; 
    boundary.draw();
}
