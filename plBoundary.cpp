#include "plBoundary.h"

plBoundary::plBoundary()
{
}

plVector3 plBoundary::getAvgNormal() const
{
    plVector3 n(0,0,0);
    for (PLint i=0; i < normals.size(); i++)
    {
        n = n + normals[i];        
    }
    return n.normalize();
}

PLuint plBoundary::size() const
{
    return points.size();
}

plVector3 _plClosestPointOnLineSegment(const plVector3 &point, const plVector3 &a, const plVector3 &b)
{
    plVector3 ab = b - a;
    // Project c onto ab, computing parameterized position d(t) = a + t*(b – a)
    PLfloat t = ((point - a) * ab) / (ab * ab);
    // If outside segment, clamp t (and therefore d) to the closest endpoint
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    // Compute projected position from the clamped t
    return a + t * ab;
}

PLuint plBoundary::addPointAndNormal(const plVector3 &point, const plVector3 &normal)
{
    if (points.size() < 3) 
    {
        // 0 or 1 points, doesnt matter, just add
        points.add( point );
        normals.add( normal );
        return points.size()-1;
    }
    else if (points.size() == 3) 
    {
        // 2 points, ensure third is counter clockwise
        if (((points[1] - points[0]) ^ (point - points[0])) * normal > 0)
        {
            // already counter clockwise
            points.add( point );
            normals.add( normal );
            return 2;
        }
        else
        {
            // clock-wise, add new point between existing two
            points.shift(1);
            normals.shift(1);
            points[1] = point;
            normals[1] = normal;
            return 1;
        }
    } 
    else 
    {    
        // Find the closest edge to the intPoint        
        PLfloat minDist = FLT_MAX;
        PLint shift_i = 0;
        
        for (PLint i = 0; i < points.size(); i++)
        {
            PLint j = (i+1) % points.size();

            plVector3 closest = plClosestPointOnLineSegment(point, points[i], points[j]);

            PLfloat distSquared = (closest - point).squaredLength();

            if (distSquared < minDist)
            {
                minDist = distSquared;
                shift_i = j;
            }
        }

        if (normals.size() == points.size())
        {
            normals.shift(shift_i);
            normals[shift_i] = normal;
        }

        points.shift(shift_i);
        points[shift_i] = point;  
        return shift_i;  
    }

}


void _plSetBoundaryColour()
{
    if (PL_BOUNDARY_NONE_SELECTED || PL_BOUNDARY_CURRENT_IS_SELECTED)
    {
        switch (_plPickingState->type)
        {
            case PL_PICKING_TYPE_DEFECT_CORNERS:
                // defect corners 
                glColor3f( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 
                break;

            case PL_PICKING_TYPE_DEFECT_BOUNDARY:
                // defect boundary
                glColor3f( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR ); 
                break;
 
            case PL_PICKING_TYPE_DONOR_BOUNDARY:
                // donor boundary
                glColor3f( PL_BOUNDARY_DONOR_COLOUR );
                break;

            case PL_PICKING_TYPE_IGUIDE_BOUNDARY:     
                // iguide boundary
                glColor3f( PL_BOUNDARY_IGUIDE_COLOUR );   
                break;
        }
    }
    else
    {
        switch (_plPickingState->type)
        {
            case PL_PICKING_TYPE_DEFECT_CORNERS:
                // defect corners 
                glColor3f( PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL ); 
                break;

            case PL_PICKING_TYPE_DEFECT_BOUNDARY:
                // defect boundary
                glColor3f( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR_DULL ); 
                break;
 
            case PL_PICKING_TYPE_DONOR_BOUNDARY:
                // donor boundary
                glColor3f( PL_BOUNDARY_DONOR_COLOUR_DULL );
                break;

            case PL_PICKING_TYPE_IGUIDE_BOUNDARY:     
                // iguide boundary
                glColor3f( PL_BOUNDARY_IGUIDE_COLOUR_DULL );   
                break;
        }
    }
}

void plBoundary::updateMesh()
{
    plVector3 n = getAvgNormal();
    
    //plSeq<plTriangle> triangles( points.size()*6 );

    plSeq<plVector3>    interleaved_vertices( points.size() * 4 * 3 );
    plSeq<unsigned int> indices             ( points.size() * 6 * 3 );

    for (PLint i = 0; i < points.size(); i++)
    {        
        int j = (i+1) % points.size();  // next index
        int k = (i+2) % points.size();  // next next index
        int l = (i == 0) ? points.size()-1 : i-1; // previous index

        // tangent vectors of previous, current, and next segments
        plVector3 t1 = ((points[j] - points[i]) ^ n).normalize();
        plVector3 t0 = ((points[i] - points[l]) ^ n).normalize();        
        plVector3 t2 = ((points[k] - points[j]) ^ n).normalize();           

        // average tangent vectors of current segment end points
        plVector3 x0 = (t0 + t1).normalize();
        plVector3 x1 = (t1 + t2).normalize();

        plVector3 a = points[i] + PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 b = points[j] + PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 c = b + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 d = a + PL_BOUNDARY_MESH_HEIGHT * n;

        plVector3 e = points[i] - PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 f = points[j] - PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 g = f + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 h = e + PL_BOUNDARY_MESH_HEIGHT * n;

        PLuint base = interleaved_vertices.size()/2;

        interleaved_vertices.add(a);    // position
        interleaved_vertices.add(t1);   // normal
        
        interleaved_vertices.add(b);    // position
        interleaved_vertices.add(t1);   // normal
        
        interleaved_vertices.add(c);    // position
        interleaved_vertices.add(t1);   // normal
        
        interleaved_vertices.add(d);    // position
        interleaved_vertices.add(t1);   // normal
        //
        interleaved_vertices.add(d);    // position
        interleaved_vertices.add(n);    // normal
        
        interleaved_vertices.add(c);    // position
        interleaved_vertices.add(n);    // normal
        
        interleaved_vertices.add(g);    // position
        interleaved_vertices.add(n);    // normal
        
        interleaved_vertices.add(h);    // position
        interleaved_vertices.add(n);    // normal        
        //
        interleaved_vertices.add(e);    // position
        interleaved_vertices.add(-t1);  // normal
        
        interleaved_vertices.add(f);    // position
        interleaved_vertices.add(-t1);  // normal
        
        interleaved_vertices.add(g);    // position
        interleaved_vertices.add(-t1);  // normal
        
        interleaved_vertices.add(h);    // position
        interleaved_vertices.add(-t1);  // normal

        indices.add(base+0);    indices.add(base+1);    indices.add(base+2);
        indices.add(base+0);    indices.add(base+2);    indices.add(base+3);
        
        indices.add(base+4);    indices.add(base+5);    indices.add(base+6);
        indices.add(base+4);    indices.add(base+6);    indices.add(base+7);
        
        indices.add(base+9);    indices.add(base+8);    indices.add(base+11);
        indices.add(base+9);    indices.add(base+11);   indices.add(base+10);        
    }

    _mesh.destroy();
    //_mesh = plMesh(triangles);
    _mesh = plMesh(interleaved_vertices, indices);
}


void plBoundary::draw() const
{        
    if (!_isVisible)
        return;

    if (points.size() > 0) 
    {
        _plPickingState->index = -1;  // draw walls with index of -1
        _plPickingShader->setPickingUniforms(_plPickingState);
              
        _plSetBoundaryColour();
        _mesh.draw();

        for (PLint i=0; i<points.size(); i++) 
        {
            _plPickingState->index = i; 
            _plPickingShader->setPickingUniforms(_plPickingState);
            
            if (PL_BOUNDARY_POINT_CURRENT_IS_SELECTED)
            {
                plDrawSphere( points[i], 1.5) ;
            }
            else
            {
                plDrawSphere( points[i], 1 );
            }
        }
    }
}

//////////////////////////////////////

PLint plBoundaryGetSelectedType()
{
    return _plState->boundarySelectedType;
}


