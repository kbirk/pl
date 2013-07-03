#ifndef CAMERA_H
#define CAMERA_H

#include "pl.h"
#include "plModel.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plMatrix44.h"

class plCamera
{
    
    public:
    
        plCamera();
        plCamera( plString filename );

        const plVector3 &position()  const  { return _position;}      
        plVector3        direction() const  { return (_lookat - _position).normalize(); }
        plMatrix44       matrix()    const;

        void position( const plVector3 &position ) { _position = position; }
        void lookat  ( const plVector3 &lookat   ) { _lookat = lookat;     }
        void up      ( const plVector3 &up       ) { _up = up;             }
        
        void importViewParams( std::string filename );             
        void exportViewParams( std::string filename );
        
		void translate( PLint x, PLint y );
        void rotate   ( PLint x0, PLint y0, PLint x1, PLint y1 );      
        void zoom     ( PLfloat z );
        void reset    ( const plVector3 &min, const plVector3 &max );
        
    private:

        plVector3 _position; 
        plVector3 _lookat;
        plVector3 _up;	      
        
        void _defaultInit();
               
};

// Michiel's arcball code
void      calc_p   ( PLint x, PLint y, PLfloat p[]);
plVector4 calc_quat( PLint x0, PLint y0, PLint x1, PLint y1);

#endif 
