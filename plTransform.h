#ifndef __PL_TRANSFORM_H__
#define __PL_TRANSFORM_H__

#include "plCommon.h"
#include "plSeq.h"
#include "plString.h"
#include "plVector3.h"
#include "plMatrix44.h"

class plTransform 
{
    public:

        plTransform();
        plTransform( const plVector3 &y, const plVector3 &origin );
        plTransform( const plVector3 &x, const plVector3 &y, const plVector3 &origin );

        const plVector3&          x() const { return _x; }
        const plVector3&          y() const { return _y; }
        const plVector3&          z() const { return _z; }        
        const plVector3&     origin() const { return _origin; }
        const plMatrix44&    matrix() const { return _transform; }

        void x     ( const plVector3 &x )      { _x = x.normalize(); _compute(); }
        void y     ( const plVector3 &y )      { _y = y.normalize(); _compute(); }
        void z     ( const plVector3 &z )      { _z = z.normalize(); _compute(); } 
        void origin( const plVector3 &origin ) { _origin = origin;   _compute(); } 
        
        void set( const plVector3 &x, const plVector3 &y);
        void set( const plVector3 &x, const plVector3 &y, const plVector3 &origin);   
        void set( const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &origin );
       
        void importCSV( const plSeq<plString> &row );

        plVector3 applyInverse       ( const plVector3 &v ) const;
        plVector3 applyNormalInverse ( const plVector3 &v ) const;
        PLfloat   squaredDistToAxis  ( const plVector3 &v ) const;
        PLfloat   projectedDistOnAxis( const plVector3 &v ) const;

        friend std::ostream& operator << ( std::ostream& out, const plTransform &t );
  
    private:
    
        plMatrix44  _transform;
        plVector3   _x,_y,_z;
        plVector3   _origin;      
        
        void _compute(); 
        
        
};

std::ostream& operator << ( std::ostream& out, const plTransform &t );

#endif 
