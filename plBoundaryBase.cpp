#include "plBoundaryBase.h"

plBoundaryBase::plBoundaryBase()
{
}


plVector3 plBoundaryBase::getAverageNormal() const
{
    plVector3 n(0,0,0);
    for (PLuint i=0; i < _normals.size(); i++)
    {
        n = n + _normals[i];        
    }
    return n.normalize();
}


PLuint plBoundaryBase::size() const
{
    return _points.size();
}


void plBoundaryBase::readFromCSV( const plSeq<plString> &row )
{
    // assumes points are counter-clockwise
    for ( PLuint j = 3; j < row.size(); j+=2)
    {               
        _points.add(  plVector3( row[j] ) );                  
        _normals.add( plVector3( row[j+1] ) );
    } 
}



PLuint plBoundaryBase::addPointAndNormal(const plVector3 &point, const plVector3 &normal)
{
    if (_points.size() < 3) 
    {
        // 0 or 1 _points, doesnt matter, just add
        _points.add( point );
        _normals.add( normal );
        return _points.size()-1;
    }
    else if (_points.size() == 3) 
    {
        // 2 _points, ensure third is counter clockwise
        plVector3 n = 0.5f * (_normals[1] + _normals[0]);
        
        if ( ((_points[1] - _points[0]) ^ (point - _points[0])) * n > 0)
        {
            // already counter clockwise
            _points.add( point );
            _normals.add( normal );
            return 2;
        }
        else
        {
            // clock-wise, add new point between existing two
            _points.shift(1);
            _normals.shift(1);
            _points[1] = point;
            _normals[1] = normal;
            return 1;
        }
    } 
    else 
    {    
        // Find the closest edge to the new point        
        PLfloat minDist = FLT_MAX;
        PLint shift_i = 0;

        const PLfloat EPSILON = 0.000001f;
               
        for (PLuint i = 0; i < _points.size(); i++)
        {
            PLint j = (i+1) % _points.size();   // next point
            // get closest point on current edge
            plVector3 closest = plClosestPointOnLineSegment(point, _points[i], _points[j]);
            // compare distance to current minimum distance
            PLfloat distSquared = (closest - point).squaredLength();
            
            // check if same point as previous edge, occurs when edges are at acute angles, making a pointy "v"
            if ( fabs(distSquared - minDist) < EPSILON )
            {
                // same point as previous edge,                
                int h = (i == 0) ? _points.size()-1 : i-1; // previous index                      
                // get current and previous wall segments        
                plVector3 previous_segment = (_points[i] - _points[h]); 
                plVector3 current_segment  = (_points[j] - _points[i]);     
                // get current wall tangent                        
                plVector3 current_tangent  = current_segment ^ (0.5f * (_normals[j] + _normals[i]));
                 
                // check if current "v" shape is pointing inwards or outwards 
                if ( (current_segment ^ previous_segment) * _normals[i] < 0)
                {
                    // outward pointing "v"
                    if ((point - closest) * current_tangent  > 0 )
                    {
                        // not behind previous edge
                        minDist = distSquared;
                        shift_i = j;
                    } 
                }
                else
                {
                    // inward pointing "v"
                    if ((point - closest) * current_tangent  < 0 )
                    {
                        // in front of current edge
                        minDist = distSquared;
                        shift_i = j;
                    }
                }
            }
            else if (distSquared < minDist)
            {
                // new minimum, store index
                minDist = distSquared;
                shift_i = j;
            }
        }
        _normals.shift(shift_i);
        _normals[shift_i] = normal;

        _points.shift(shift_i);
        _points[shift_i] = point; 
        return shift_i;  
    }
}


void plBoundaryBase::movePointAndNormal( PLuint index, const plVector3 &point, const plVector3 &normal)
{
    _points[index] = point;    
    _normals[index] = normal;
}


void plBoundaryBase::removePointAndNormal( PLuint index )
{
    _points.remove(index);
    _normals.remove(index);
}


std::ostream& operator << ( std::ostream& out, const plBoundaryBase &b )
{
    for (PLuint j=0; j<b._points.size(); j++)
    {
        out << "," << b._points[j];
        out << "," << b._normals[j];
    }
    return out;
}


