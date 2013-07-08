#include "plGraftBase.h"

plGraftBase::plGraftBase()
{

}


void plGraftBase::init( const plSeq<plBoneAndCartilage> &models )
{
    // Compute transforms                     
    harvestTransform.compute();      
    recipientTransform.compute();     
    // set model handles
    _recipientModel = &models[_recipientModelID];
    _harvestModel   = &models[_harvestModelID];        
    // Compute cartilage and bone caps
    setCaps();
}


void plGraftBase::readFromCSV( const plSeq<plString> &row )
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


void plGraftBase::setCaps()
{
    // generate cap polygons
    _cartilageCap = _findCap( _harvestModel->cartilage.triangles() );
    _boneCap      = _findCap( _harvestModel->bone.triangles()      );
   
    // generate meshes   
    _updateMarkPosition();  
}


bool _comparePointAndAngle( const plPointAndAngle &a, const plPointAndAngle &b )
{
    return (a.angle < b.angle);
}


plCap plGraftBase::_findCap( const plSeq<plTriangle> &triangles)
{   
    plCap cap;

    // Find polygons on top of graft
    for (PLuint i=0; i<triangles.size(); i++) 
    {
        plPoly p;
        if (triangles[i].normal() * harvestTransform.y() > 0 && _triangleIntersection( triangles[i], p ))
        {
            cap.polys.add( p );
        }
    } 

    // Find vertices of polygons along perimeter of graft
    std::vector<plPointAndAngle> angles;

    for (PLuint i=0; i<cap.polys.size(); i++) 
    {
        for (PLuint j=0; j<cap.polys[i].vertices.size(); j++) 
        {
            plVector3 &v = cap.polys[i].vertices[j];
            
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


bool plGraftBase::_triangleIntersection( const plTriangle &triangle, plPoly &p ) const
{
    static float min = FLT_MAX;

    float radiusSquared = _radius * _radius;

    plVector3 point1 = harvestTransform.applyInverse( triangle.point0() );
    plVector3 point2 = harvestTransform.applyInverse( triangle.point1() );
    plVector3 point3 = harvestTransform.applyInverse( triangle.point2() );

    // Compute distance to graft axis
    float dist1 = harvestTransform.squaredDistToAxis( point1 );
    float dist2 = harvestTransform.squaredDistToAxis( point2 );
    float dist3 = harvestTransform.squaredDistToAxis( point3 );

    // If too far from graft axis, reject.  Note that this will miss
    // some slightly-overlapping triangles!

    float minDist = PL_MIN_OF_3( dist1, dist2, dist3 );

    if (minDist > radiusSquared)
        return false;

    // If too far away from graft origin, reject.
    
    float proj1 = harvestTransform.projectedDistOnAxis( point1 );
    float proj2 = harvestTransform.projectedDistOnAxis( point2 );
    float proj3 = harvestTransform.projectedDistOnAxis( point3 );

    float maxProj = PL_MAX_OF_3( proj1, proj2, proj3 );

    if (maxProj < -_length)
        return false;

    // At least some of the triangle is inside

    p.normal = harvestTransform.applyNormalInverse(triangle.normal());

    // If entirely within the graft, accept the whole triangle (this is cheaper).

    float maxDist = PL_MAX_OF_3( dist1, dist2, dist3 );

    if (maxDist <= radiusSquared) 
    {
        p.vertices.add( point1 );
        p.vertices.add( point2 );
        p.vertices.add( point3 );
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
            p.vertices.add( vs[j] );
        } 
        else if (prevInside && !nextInside) 
        {
            // Find point on edge of graft
            p.vertices.add( _pointOnCircumference(vs[i], vs[j]) );
        } 
        else if (!prevInside && nextInside) 
        {
            // Find entering point and angle 
            p.vertices.add( _pointOnCircumference(vs[i], vs[j]) );    
            // Add inside triangle point    
            p.vertices.add( vs[j] );
        }

        prevInside = nextInside;
    }

    return true;
}


plVector3 plGraftBase::_pointOnCircumference( const plVector3 &u, const plVector3 &v ) const
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


float plGraftBase::_angleOfPoint( const plVector3 &v ) const
{
    return atan2( v*harvestTransform.x(), v*harvestTransform.z() );
}


plVector3 plGraftBase::_pointAtAngle( PLfloat theta ) const
{
    return cos(theta) * harvestTransform.z() + sin(theta) * harvestTransform.x();
}

void plGraftBase::spinMark( PLfloat degrees )
{
    plVector3 axis(0,1,0);
    plMatrix44 rot; rot.setRotationD( degrees, axis );     
    _markDirection = (rot * _markDirection).normalize();    
    _updateMarkPosition();
}

void plGraftBase::_updateMarkPosition()
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


void plGraftBase::translateHarvest( const plVector3 &translation )
{     
    _translate( harvestTransform, *_harvestModel, translation );

    // harvest, re-compute cap  
    setCaps();
}


void plGraftBase::translateRecipient( const plVector3 &translation )
{     
    _translate( recipientTransform, *_recipientModel, translation );
}


void plGraftBase::_translate( plTransform &transform, const plBoneAndCartilage &model, const plVector3 &translation )
{     
    plIntersection intersection = model.bone.rayIntersect( transform.origin() + translation, -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normal_radius = 6.0f;
        plVector3 normal = model.bone.getAverageNormal( normal_radius, transform.origin(), transform.y() );  

        // translate       
        plVector3 y      = normal.normalize();
        plVector3 x      = (y ^ transform.z()).normalize();                       
        plVector3 origin = intersection.point;   
        transform.set( x, y, origin); 
    }
}

