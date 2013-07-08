#include "plGraft.h"

plGraft::plGraft()
    : plGraftBase()
{

}

/*
void plGraft::readFromCSV( const plSeq<plString> &row )
{
    // Fill in the field            
    plString subfield = row[2];
    
    if (plStringCompareCaseInsensitive(subfield, "harvest model") )
        _harvestModelID = atof( row[3].c_str() );
                       
    else if (plStringCompareCaseInsensitive(subfield, "recipient model") )
        _recipientModelID = atof( row[3].c_str() );

    else if (plStringCompareCaseInsensitive(subfield, "height offset") )
        _heightOffset = atof( row[3].c_str() );
                       
    else if (plStringCompareCaseInsensitive(subfield, "radius") )
        _radius = atof( row[3].c_str() );
        
    else if (plStringCompareCaseInsensitive(subfield, "length") )
        _length = atof( row[3].c_str() );
       
    else if (plStringCompareCaseInsensitive(subfield, "mark direction") )
        _markDirection = plVector3( row[3] ).normalize();
        
    else if (plStringCompareCaseInsensitive(subfield, "recipient origin") )
        recipientTransform.origin( row[3] );
    
    else if (plStringCompareCaseInsensitive(subfield, "recipient x") )            
        recipientTransform.x( row[3] );

    else if (plStringCompareCaseInsensitive(subfield, "recipient y") )
        recipientTransform.y( row[3] );
         
    else if (plStringCompareCaseInsensitive(subfield, "harvest origin") )
        harvestTransform.origin( row[3] );
    
    else if (plStringCompareCaseInsensitive(subfield, "harvest x") )
        harvestTransform.x( row[3] );

    else if (plStringCompareCaseInsensitive(subfield, "harvest y") )
        harvestTransform.y( row[3] );
    else
        std::cerr << "Error in plan, 'graft': Unrecognized word '" << subfield << "' in third column." << std::endl;
}
*/


void plGraft::_setBoneColour() const
{
    if (_isSelected)
    {
        // selected
        if (_plPickingState->index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_BONE_COLOUR_DULL );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_BONE_COLOUR_DULL );
        }
    }
    else
    {
        // not selected
        if (_plPickingState->index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_BONE_COLOUR );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_BONE_COLOUR );
        } 
    }
}

void plGraft::_setCartilageColour() const
{
    if (_isSelected)
    {
        // selected
        if (_plPickingState->index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL );
        }  
    }
    else
    {
        // not selected
        if (_plPickingState->index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
        }
    }      
}

void plGraft::draw() const
{
    if (!isVisible)
        return;

    // Draw at harvest location
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        harvestTransform.apply();
        _plPickingState->index = PL_PICKING_INDEX_GRAFT_DONOR;      
        _drawGraft();
    }
    glPopMatrix();

    // Draw at recipient location
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        recipientTransform.apply();
        glTranslatef( 0, _heightOffset, 0 );
        _plPickingState->index = PL_PICKING_INDEX_GRAFT_DEFECT;
        _drawGraft();
    }
    glPopMatrix();
}


void plGraft::_drawGraft() const
{
    _plPickingState->type = PL_PICKING_TYPE_GRAFT; 
    _plPickingShader->setPickingUniforms(_plPickingState);

    // draw cartilage cap
    if (_cartilageCap.polys.size() > 0)  // may not always have the cartilage top
    {
        _setCartilageColour();
        _cartilageMesh.draw();
    }
    
    // draw bone cap
    _setBoneColour();
    _boneMesh.draw();
    
    // draw marker   
    glColor3f( PL_GRAFT_MARKER_COLOUR );
    plDrawSphere( _markPosition, 0.5 );

}


void plGraft::setCaps()
{
    // generate cap polygons
    plGraftBase::setCaps();
    
    // generate meshes   
    _updateCartilageMesh();   
    _updateBoneMesh();  
 
}



void plGraft::_updateCartilageMesh()
{
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    plSeq<plVector3> interleaved_vertices;
    plSeq<PLuint>    indices;

    // cartilage top
    for (PLuint i = 0; i < _cartilageCap.polys.size(); i++)
    {
        PLint base = interleaved_vertices.size()/2;
        plPoly &p = _cartilageCap.polys[i];

        for (PLuint j = 0; j < p.vertices.size(); j++)
        {
            plVector3 &v = p.vertices[j];
            interleaved_vertices.add( plVector3( v.x, v.y+0.01f, v.z) );    // position
            interleaved_vertices.add( p.normal );                           // normal
        }
        
        for (PLuint j = 1; j <= p.vertices.size()-2; j++) 
        {
            indices.add(base+0);
            indices.add(base+j); 
            indices.add(base+j+1);   
        }
    }

    // cartilage walls
    if (_boneCap.perimeter.size() > 0 && _cartilageCap.perimeter.size() > 0) 
    {
        int c = 0;
        int b = 0;

        float cOffset = 0;
        float bOffset = 0;

        int stepsLeft = _cartilageCap.perimeter.size() + _boneCap.perimeter.size();

        while (stepsLeft > 0) 
        {

            float cAngle = _cartilageCap.perimeter[c].angle + cOffset;
            float bAngle = _boneCap.perimeter[b].angle + bOffset;
        
            plVector3 n = (_cartilageCap.perimeter[c].point).normalize();
    
            indices.add(interleaved_vertices.size()/2);
            interleaved_vertices.add( _cartilageCap.perimeter[c].point );   // position
            interleaved_vertices.add( n );                                  // normal
            
            indices.add(interleaved_vertices.size()/2);
            interleaved_vertices.add( _boneCap.perimeter[b].point );        // position
            interleaved_vertices.add( n );                                  // normal
        
            if (cAngle < bAngle) 
            {	
                // advance cartilage
                c++;
                if (c == _cartilageCap.perimeter.size()) 
                {
                    c = 0;
                    cOffset = 2 * PL_PI;
                }
        
                indices.add(interleaved_vertices.size()/2);
                interleaved_vertices.add( _cartilageCap.perimeter[c].point );    // position
                interleaved_vertices.add( n );                                  // normal
            }  
            else 
            {			
                // advance bone
                b++;
                if (b == _boneCap.perimeter.size()) 
                {
                    b = 0;
                    bOffset = 2 * PL_PI;
                }
        
                indices.add(interleaved_vertices.size()/2);
                interleaved_vertices.add( _boneCap.perimeter[b].point );         // position
                interleaved_vertices.add( n );                                  // normal
            }
            stepsLeft--;
        }
    }
       
    if (indices.size() > 0)
    {   
        _cartilageMesh.setBuffers(interleaved_vertices, indices);  
    }
}


void plGraft::_updateBoneMesh()
{        
    const plVector3 y(0,1,0);		        // y is cylinder axis (pointing upward)

    plSeq<plVector3> interleaved_vertices;
    plSeq<PLuint> indices; 
       
    // bone top (only if no cartilage top)
    if (_cartilageCap.polys.size() == 0)
    {
        for (PLuint i = 0; i < _boneCap.polys.size(); i++)
        {
            PLint base = interleaved_vertices.size()/2;
            plPoly &p = _boneCap.polys[i];
            for (PLuint j = 0; j < p.vertices.size(); j++)
            {
                plVector3 &v = p.vertices[j];
                interleaved_vertices.add( plVector3( v.x, v.y+0.01f, v.z) );        // position
                interleaved_vertices.add( p.normal );                               // normal
            }
            
            for (PLuint j = 1; j <= p.vertices.size()-2; j++) 
            {
                indices.add(base+0);
                indices.add(base+j); 
                indices.add(base+j+1);   
            }
        }
    }

    // bone walls
    plVector3 centreBottom = -(_length) * y;
    plVector3 z(0,0,1);
    plVector3 x(1,0,0);

    if (_boneCap.perimeter.size() > 0) 
    {
        PLint base = interleaved_vertices.size()/2;

        float theta = _boneCap.perimeter[0].angle;
        plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();

        plVector3 prevTop = _boneCap.perimeter[0].point;
        plVector3 prevBot = centreBottom + _radius * cos(theta) * z + _radius * sin(theta) * x;

        // top side
        interleaved_vertices.add( prevTop ); // position
        interleaved_vertices.add( n);        // normal
        // bottom side
        interleaved_vertices.add( prevBot ); // position
        interleaved_vertices.add( n);        // normal
        // bottom bottom
        interleaved_vertices.add( prevBot ); // position 
        interleaved_vertices.add( -y);       // normal
        
        for (PLuint i=0; i<_boneCap.perimeter.size(); i++) 
        {
            float theta = _boneCap.perimeter[i].angle;
            plVector3 top = _boneCap.perimeter[i].point;
            plVector3 bot = centreBottom + _radius * cos(theta) * z + _radius * sin(theta) * x;

            plVector3 n = (cos(theta) * z + sin(theta) * x).normalize();
            // top side
            interleaved_vertices.add( top ); // position
            interleaved_vertices.add( n);    // normal
            // bottom side    
            interleaved_vertices.add( bot ); // position
            interleaved_vertices.add( n);    // normal
            // bottom bottom
            interleaved_vertices.add( bot ); // position 
            interleaved_vertices.add( -y);   // normal
        }
        // top side
        interleaved_vertices.add( prevTop ); // position
        interleaved_vertices.add( n);        // normal
        // bottom side
        interleaved_vertices.add( prevBot ); // position
        interleaved_vertices.add( n);        // normal
        // bottom bottom
        interleaved_vertices.add( prevBot ); // position 
        interleaved_vertices.add( -y);       // normal
        // bottom centre point
        interleaved_vertices.add( centreBottom );   // position
        interleaved_vertices.add( -y );             // normal  
        
        for (PLuint j = 0; j <= _boneCap.perimeter.size()*3; j+=3) 
        {
            // side t1
            indices.add(base+j);
            indices.add(base+j+1);
            indices.add(base+j+3);
            // side t2
            indices.add(base+j+1);
            indices.add(base+j+4);
            indices.add(base+j+3);
            // bottom t3
            indices.add(base+j+2);
            indices.add(interleaved_vertices.size()/2 - 1);
            indices.add(base+j+5);
        }

    }
    _boneMesh.setBuffers(interleaved_vertices, indices);
}
