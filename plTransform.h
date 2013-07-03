#ifndef __PL_TRANSFORM_H__
#define __PL_TRANSFORM_H__

#include "pl.h"
#include "plVector3.h"

class plTransform 
{
    public:

        plTransform();

        const plVector3& x     () const { return _x; }
        const plVector3& y     () const { return _y; }
        const plVector3& z     () const { return _z; }        
        const plVector3& origin() const { return _origin; }
        
        void x     ( const plVector3 &x )      { _x = x.normalize(); compute(); }
        void y     ( const plVector3 &y )      { _y = y.normalize(); compute(); }
        void z     ( const plVector3 &z )      { _z = z.normalize(); compute(); } 
        void origin( const plVector3 &origin ) { _origin = origin;   compute(); } 
        
        void x     ( const plString &x )      { _x = plVector3( x.c_str() ).normalize(); }
        void y     ( const plString &y )      { _y = plVector3( y.c_str() ).normalize(); }
        void origin( const plString &origin ) { _origin = plVector3( origin.c_str() );   } 
        
        void set( const plVector3 &x, const plVector3 &y);
        void set( const plVector3 &x, const plVector3 &y, const plVector3 &z);   
        void set( const plVector3 &x, const plVector3 &y, const plVector3 &z, const plVector3 &origin );

        void compute();
        void apply() const;

        plVector3 applyInverse       ( const plVector3 &v ) const;
        plVector3 applyNormalInverse ( const plVector3 &v ) const;
        PLfloat   squaredDistToAxis  ( const plVector3 &v ) const;
        PLfloat   projectedDistOnAxis( const plVector3 &v ) const;
  
    private:
    
        PLfloat   _transform[16];
        plVector3 _x,_y,_z;
        plVector3 _origin;       
        
        
};

#endif 
