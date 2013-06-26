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

        plVector3 getPosition()         const  { return _position;}      
        plVector3 getViewingDirection() const  { return (_lookat - _position).normalize(); }
        plMatrix44 getMatrix() const;

        void setPosition( const plVector3 position) { _position = position; }
        void setFocus   ( const plVector3 lookat)   { _lookat = lookat;     }
        void setUp      ( const plVector3 up)       { _up = up;             }

        void reset    ( const plVector3 &min, const plVector3 &max );
              
        void exportViewParams( std::string filename );
        void importViewParams( std::string filename );

		void translate( PLint x, PLint y);
        void rotate   ( PLint x0, PLint y0, PLint x1, PLint y1 );      
        void zoom     ( PLfloat z);
        
    private:

        plVector3 _position; 
        plVector3 _lookat;
        plVector3 _up;	      
        
        void defaultInit();
               
};

// Michiel's arcball code
void      calc_p   ( PLint x, PLint y, PLfloat p[]);
plVector4 calc_quat( PLint x0, PLint y0, PLint x1, PLint y1);

#endif 
