#include "plBoundary.h"

plBoundary::plBoundary()
    : plBoundaryBase()
{
    _showWalls = true;
}

void plBoundary::toggleVisibility()
{
    if (isVisible && _showWalls)
    {
        _showWalls = false;
    } 
    else if (isVisible && !_showWalls)
    {
        isVisible = false;
    }
    else
    {
        isVisible = true;
        _showWalls = true;
    }
}


void plBoundary::readFromCSV( const plSeq<plString> &row )
{
    plBoundaryBase::readFromCSV(row);
    _updateMesh(); 
}


PLuint plBoundary::addPointAndNormal(const plVector3 &point, const plVector3 &normal)
{
    PLuint newIndex = plBoundaryBase::addPointAndNormal(point, normal);
    _updateMesh();
    return newIndex;
}


void plBoundary::movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal)
{
    plBoundaryBase::movePointAndNormal(index, point, normal);
    _updateMesh(); 
}


void plBoundary::removePointAndNormal( PLuint index )
{
    plBoundaryBase::removePointAndNormal(index);
    _updateMesh(); 
}


void plBoundary::_setColour() const
{
    if (_isSelected)
    {
        // selected
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
    else
    {
        // not selected
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
}


void plBoundary::_updateMesh()
{
    plVector3 n = getAverageNormal();

    plSeq<plVector3> interleaved_vertices( _points.size() * 10 );
    plSeq<PLuint>    indices             ( _points.size() * 6 * 4 );

    for (PLuint i = 0; i < _points.size(); i++)
    {        
        int j = (i+1) % _points.size();  // next index
        int k = (i+2) % _points.size();  // next next index
        int l = (i == 0) ? _points.size()-1 : i-1; // previous index

        // tangent vectors of previous, current, and next segments
        plVector3 t1 = ((_points[j] - _points[i]) ^ n).normalize();
        plVector3 t0 = ((_points[i] - _points[l]) ^ n).normalize();        
        plVector3 t2 = ((_points[k] - _points[j]) ^ n).normalize();           

        // average tangent vectors of current segment end _points
        plVector3 x0 = (t0 + t1).normalize();
        plVector3 x1 = (t1 + t2).normalize();

        plVector3 a = _points[i] + PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 b = _points[j] + PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 c = b + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 d = a + PL_BOUNDARY_MESH_HEIGHT * n;

        plVector3 e = _points[i] - PL_BOUNDARY_MESH_WIDTH_HALF * x0;
        plVector3 f = _points[j] - PL_BOUNDARY_MESH_WIDTH_HALF * x1;
        plVector3 g = f + PL_BOUNDARY_MESH_HEIGHT * n;
        plVector3 h = e + PL_BOUNDARY_MESH_HEIGHT * n;

        PLuint base = interleaved_vertices.size()/2;

        // "right" quad
        interleaved_vertices.add(a);    // position
        interleaved_vertices.add(t1);   // normal
        
        interleaved_vertices.add(b);    // position
        interleaved_vertices.add(t1);   // normal
        
        interleaved_vertices.add(c);    // position
        interleaved_vertices.add(t1);   // normal
        
        interleaved_vertices.add(d);    // position
        interleaved_vertices.add(t1);   // normal
        
        // top ridge 
        interleaved_vertices.add(0.5 * (d+h) + PL_BOUNDARY_MESH_CURVE_HEIGHT * n);    // position
        interleaved_vertices.add(n);    // normal
        
        interleaved_vertices.add(0.5 * (c+g) + PL_BOUNDARY_MESH_CURVE_HEIGHT * n);    // position
        interleaved_vertices.add(n);    // normal       
         
        // "left" quad
        interleaved_vertices.add(e);    // position
        interleaved_vertices.add(-t1);  // normal
        
        interleaved_vertices.add(f);    // position
        interleaved_vertices.add(-t1);  // normal
        
        interleaved_vertices.add(g);    // position
        interleaved_vertices.add(-t1);  // normal
        
        interleaved_vertices.add(h);    // position
        interleaved_vertices.add(-t1);  // normal

        // triangle indices
        indices.add(base+0);    indices.add(base+1);    indices.add(base+2);
        indices.add(base+0);    indices.add(base+2);    indices.add(base+3);
        
        indices.add(base+3);    indices.add(base+2);    indices.add(base+5);
        indices.add(base+3);    indices.add(base+5);    indices.add(base+4);
        
        indices.add(base+4);    indices.add(base+5);    indices.add(base+8);
        indices.add(base+4);    indices.add(base+8);    indices.add(base+9);
        
        indices.add(base+7);    indices.add(base+6);    indices.add(base+9);
        indices.add(base+7);    indices.add(base+9);   indices.add(base+8);   
  
    }

    _mesh.setBuffers(interleaved_vertices, indices);
}


void plBoundary::draw() const
{        
    if (!isVisible)
        return;

    if (_points.size() > 0) 
    {
        _setColour();
        
        // draw walls
        if (_showWalls)
        {
            _plPickingState->index = -1;  // draw walls with index of -1
            _plPickingShader->setPickingUniforms(_plPickingState);
            _mesh.draw();
        }
        
        // draw _points
        for (PLuint i=0; i<_points.size(); i++) 
        {
            _plPickingState->index = i; 
            _plPickingShader->setPickingUniforms(_plPickingState);
            
            if (_isSelected && _selectedValue == i)   // is the current point selected?
            {
                plDrawSphere( _points[i], 1.5) ;
            }
            else
            {
                plDrawSphere( _points[i], 1 );
            }
        }
    }
}

