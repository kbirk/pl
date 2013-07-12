#include "plGraft.h"

plGraft::plGraft()
{

}


void plGraft::readFromCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage> &models )
{
    // Fill in the field            
    plString subfield = row[2];
    
    if (subfield.compareCaseInsensitive( "harvest model") )
    {
        harvest.modelID = atof( row[3].c_str() );
        
        if (models.size() < (harvest.modelID +1) )
        {
            std::cerr << "plGraft readFromCSV() error: harvest model ID read before model data";
            exit(1);
        }    
        harvest.model = &models[harvest.modelID];
    }                  
    else if (subfield.compareCaseInsensitive( "recipient model") )
    {
        recipient.modelID = atof( row[3].c_str() );
        
        if (models.size() < (recipient.modelID +1) )
        {
            std::cerr << "plGraft readFromCSV() error: recipient model ID  read before model data";
            exit(1);
        }    
        recipient.model = &models[recipient.modelID];

    }
    else if (subfield.compareCaseInsensitive( "height offset") )
        _heightOffset = atof( row[3].c_str() );
                       
    else if (subfield.compareCaseInsensitive( "radius") )
        _radius = atof( row[3].c_str() );
        
    else if (subfield.compareCaseInsensitive( "length") )
        _length = atof( row[3].c_str() );
       
    else if (subfield.compareCaseInsensitive( "mark direction") )
        _markDirection = plVector3( row[3] ).normalize();

    else if (subfield.compareCaseInsensitive( "recipient transform") )
        recipient.transform.readFromCSV( row );
    
    
    else if (subfield.compareCaseInsensitive( "harvest transform") )
    {
        harvest.transform.readFromCSV( row );
        if (harvest.model == NULL)
        {
            std::cerr << "plGraft readFromCSV() error: harvest transform read before model ID";
            exit(1);
        }        
        _setCaps();
    }
    else
        std::cerr << "Error in plan, 'graft': Unrecognized word '" << subfield << "' in third column." << std::endl;
}


void plGraft::_setBoneColour() const
{
    if (_isSelected)
    {
        // selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_BONE_COLOUR_DULL );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_BONE_COLOUR_DULL );
        }
    }
    else
    {
        // not selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_BONE_COLOUR );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_BONE_COLOUR );
        } 
    }
}

void plGraft::_setCartilageColour() const
{
    if (_isSelected)
    {
        // selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL );
        }  
    }
    else
    {
        // not selected
        if (plPicking::value.index == PL_PICKING_INDEX_GRAFT_DEFECT)
        {
            plColourStack::load( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
        }
        else
        {
            plColourStack::load( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
        }
    }      
}

void plGraft::draw() const
{
    if (!isVisible)
        return;

    // Draw at harvest location
    plModelStack::push( harvest.transform.matrix() );
    {
        plPicking::value.index = PL_PICKING_INDEX_GRAFT_DONOR;              
        _drawGraft();
    }
    plModelStack::pop();


    // Draw at recipient location
    plModelStack::push( recipient.transform.matrix() );
    {
        plModelStack::translate( 0, _heightOffset, 0 );
        plPicking::value.index = PL_PICKING_INDEX_GRAFT_DEFECT;
        _drawGraft();
    }
    plModelStack::pop();

}


void plGraft::_drawGraft() const
{
    plPicking::value.type = PL_PICKING_TYPE_GRAFT; 

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
    plColourStack::load( PL_GRAFT_MARKER_COLOUR );
    plDraw::sphere( _markPosition, 0.5 );

}


void plGraft::_setCaps()
{
    // generate cap polygons
    _cartilageCap = _findCap( harvest.model->cartilage.triangles() );
    _boneCap      = _findCap( harvest.model->bone.triangles()      );
   
    // generate meshes   
    _updateCartilageMesh();   
    _updateBoneMesh();  
    _updateMarkPosition();  
}


bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b )
{
    return (a.angle < b.angle);
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
        plPolygon &p = _cartilageCap.polys[i];

        for (PLuint j = 0; j < p.points.size(); j++)
        {
            const plVector3 &v = p.points[j];
            interleaved_vertices.add( plVector3( v.x, v.y+0.01f, v.z) );    // position
            interleaved_vertices.add( p.normal );                           // normal
        }
        
        for (PLuint j = 1; j <= p.points.size()-2; j++)
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
            plPolygon &p = _boneCap.polys[i];
            for (PLuint j = 0; j < p.points.size(); j++)
            {
                const plVector3 &v = p.points[j];
                interleaved_vertices.add( plVector3( v.x, v.y+0.01f, v.z) );        // position
                interleaved_vertices.add( p.normal );                               // normal
            }
            
            for (PLuint j = 1; j <= p.points.size()-2; j++)
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


plCap plGraft::_findCap( const plSeq<plTriangle> &triangles)
{   
    plCap cap;

    // Find polygons on top of graft
    for (PLuint i=0; i<triangles.size(); i++) 
    {
        plPolygon p;
        if (triangles[i].normal() * harvest.transform.y() > 0 && _triangleIntersection( triangles[i], p ))
        {
            cap.polys.add( p );
        }
    } 

    // Find vertices of polygons along perimeter of graft
    std::vector<plPointAndAngle> angles;

    for (PLuint i=0; i<cap.polys.size(); i++) 
    {
        for (PLuint j=0; j<cap.polys[i].points.size(); j++)
        {
            const plVector3 &v = cap.polys[i].points[j];
            
            if ((v.x*v.x + v.z*v.z) > 0.97f * _radius* _radius)
            {
                angles.push_back( plPointAndAngle( atan2( v.x, v.z ), v ));
            }
        }
    }

    if (angles.size() > 0) 
    {
        std::sort(angles.begin(), angles.end(), _comparePointAndAngle);
     
        cap.perimeter.add( plPointAndAngle( angles[0].angle, angles[0].point ) );
          
        for (PLuint i=1; i<angles.size(); i++) 
        {
            if ( (angles[i].point - angles[i-1].point).squaredLength() > 0.01f)
            {
                cap.perimeter.add( angles[i]) ; 
            }
        }
	}

	return cap;
}


// Return the Poly that is the intersection of a triangle with the
// interior of the cylinder.


bool plGraft::_triangleIntersection( const plTriangle &triangle, plPolygon &p ) const
{
    static float min = FLT_MAX;

    float radiusSquared = _radius * _radius;

    plVector3 point1 = harvest.transform.applyInverse( triangle.point0() );
    plVector3 point2 = harvest.transform.applyInverse( triangle.point1() );
    plVector3 point3 = harvest.transform.applyInverse( triangle.point2() );

    // Compute distance to graft axis
    float dist1 = harvest.transform.squaredDistToAxis( point1 );
    float dist2 = harvest.transform.squaredDistToAxis( point2 );
    float dist3 = harvest.transform.squaredDistToAxis( point3 );

    // If too far from graft axis, reject.  Note that this will miss
    // some slightly-overlapping triangles!

    float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

    if (minDist > radiusSquared)
        return false;

    // If too far away from graft origin, reject.
    
    float proj1 = harvest.transform.projectedDistOnAxis( point1 );
    float proj2 = harvest.transform.projectedDistOnAxis( point2 );
    float proj3 = harvest.transform.projectedDistOnAxis( point3 );

    float maxProj = PL_MAX_OF_3( proj1, proj2, proj3 );

    if (maxProj < -_length)
        return false;

    // At least some of the triangle is inside

    p.normal = harvest.transform.applyNormalInverse(triangle.normal());

    // If entirely within the graft, accept the whole triangle (this is cheaper).

    float maxDist = PL_MAX_OF_3( dist1, dist2, dist3 );

    if (maxDist <= radiusSquared) 
    {
        p.points.add( point1 );
        p.points.add( point2 );
        p.points.add( point3 );
        return true;
    }

    // Not entirely within the graft, so find the intersection of the
    // triangle with the cylinder wall

    // Find a first vertex that is inside

    plVector3 vs[3];
    float ds[3];
  
    if (dist1 <= radiusSquared) 
    {
        vs[0] = point1; ds[0] = dist1;
        vs[1] = point2; ds[1] = dist2;
        vs[2] = point3; ds[2] = dist3;
    } 
    else if (dist2 <= radiusSquared)
    {
        vs[0] = point2; ds[0] = dist2;
        vs[1] = point3; ds[1] = dist3;
        vs[2] = point1; ds[2] = dist1;
    } 
    else 
    {    
        vs[0] = point3; ds[0] = dist3;
        vs[1] = point1; ds[1] = dist1;
        vs[2] = point2; ds[2] = dist2;
    }

    bool prevInside = true; // always starts as true (ds[0] <= radiusSquared)

    for (int i=0; i<3; i++) 
    {
        int j = (i+1) % 3;		// vertex at next end of edge

        bool nextInside = (ds[j] <= radiusSquared);

        if (prevInside && nextInside) 
        {
            // Add inside triangle point
            p.points.add( vs[j] );
        } 
        else if (prevInside && !nextInside) 
        {
            // Find point on edge of graft
            p.points.add( _pointOnCircumference(vs[i], vs[j]) );
        } 
        else if (!prevInside && nextInside) 
        {
            // Find entering point and angle 
            p.points.add( _pointOnCircumference(vs[i], vs[j]) );
            // Add inside triangle point    
            p.points.add( vs[j] );
        }

        prevInside = nextInside;
    }

    return true;
}


plVector3 plGraft::_pointOnCircumference( const plVector3 &u, const plVector3 &v ) const
{
    plVector3 uProj = u;		// ignore component along axis
    plVector3 vProj = v;

    uProj.y = 0;
    vProj.y = 0;

    float a = (vProj-uProj)*(vProj-uProj);
    float b = 2*(uProj*(vProj-uProj));
    float c = uProj*uProj-_radius*_radius;

    float radical = b*b - 4*a*c;
    
    if (radical < 0) 
    {
        std::cout << "radical = " << radical << std::endl;
        return plVector3(0,0,0);	// error
    }

    float root = sqrt(radical);

    float t1 = (-b + root)/(2*a);
    float t2 = (-b - root)/(2*a);

    float t;
    if (t1 >= 0 && t1 <= 1 && (t1 <= t2 || t2 <= 0))
    {
        t = t1;
    }
    else if (t2 >= 0 && t2 <= 1 && (t2 <= t1 || t1 >= 1))
    {
        t = t2;
    }
    else 
    {
        std::cout << "t1 = " << t1 << ", t2 = " << t2 << std::endl;
        return plVector3(0,0,0);	// error
    }

    return u + t*(v-u);
    
}


float plGraft::_angleOfPoint( const plVector3 &v ) const
{
    return atan2( v*harvest.transform.x(), v*harvest.transform.z() );
}


plVector3 plGraft::_pointAtAngle( PLfloat theta ) const
{
    return cos(theta) * harvest.transform.z() + sin(theta) * harvest.transform.x();
}

void plGraft::spinMark( PLfloat degrees )
{
    plVector3 axis(0,1,0);
    plMatrix44 rot; rot.setRotationD( degrees, axis );     
    _markDirection = (rot * _markDirection).normalize();    
    _updateMarkPosition();
}

void plGraft::_updateMarkPosition()
{
    // Mark at tool alignment direction on cartilage
    _markPosition = _radius * _markDirection;

    // First, find the closest top perimeter point in the mark direction.
    float minDist = FLT_MAX;
    float minY;

    const plCap &cap = (_cartilageCap.polys.size() == 0) ? _boneCap : _cartilageCap;

    for (PLuint i=0; i<cap.perimeter.size(); i++) 
    {
        const plVector3 &v = cap.perimeter[i].point;
        float dist = (v.x-_markPosition.x)*(v.x-_markPosition.x) + (v.z-_markPosition.z)*(v.z-_markPosition.z);
        if (dist < minDist) 
        {
            minDist = dist;
            minY = v.y;
        }
    }

    // Draw marker  
    _markPosition.y = minY;
}

const plTransform &plGraft::transform( PLuint type ) const
{
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            return harvest.transform;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            return recipient.transform;
            
        default:
        
            std::cerr << "plGraft transform() error: invalid type enumeration provided, defaulting to recipient \n";
            return recipient.transform;   
    } 
}

void plGraft::translate( PLuint type, const plVector3 &translation )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.translate( translation );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.translate( translation );
            break;
            
        default:
        
            std::cerr << "plGraft translate() error: invalid type enumeration provided \n";
            break;    
    } 
}

void plGraft::translateX( PLuint type, PLfloat distance, const plVector3 &planeNormal  )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.translateX( distance, planeNormal );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.translateX( distance, planeNormal );
            break;
            
        default:
        
            std::cerr << "plGraft translateX() error: invalid type enumeration provided \n";
            break;    
    } 
}

void plGraft::translateZ( PLuint type, PLfloat distance, const plVector3 &planeNormal  )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.translateZ( distance, planeNormal );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.translateZ( distance, planeNormal );
            break;
            
        default:
        
            std::cerr << "plGraft translateZ() error: invalid type enumeration provided \n";
            break;    
    } 
}

void plGraft::rotate( PLuint type, const plVector3 &axis, PLfloat angleDegrees )
{    
    switch (type)
    {
        case PL_PICKING_INDEX_GRAFT_DONOR:
        
            harvest.rotate( axis, angleDegrees );
            _setCaps();
            break;
        
        case PL_PICKING_INDEX_GRAFT_DEFECT:
        
            recipient.rotate( axis, angleDegrees );
            break;
            
        default:
        
            std::cerr << "plGraft rotate() error: invalid type enumeration provided \n";
            break;    
    } 
}


