#ifndef CAMERA_H
#define CAMERA_H

#include "plCommon.h"
#include "plSeq.h"
#include "plVector3.h"
#include "plVector4.h"
#include "plMatrix44.h"

class plCamera
{
    
    public:
    
        plVector3 position; 
        plVector3 lookat;
        plVector3 up;	
    
        plCamera();
        plCamera( plString filename );
  
        plVector3  getDirection() const  { return (lookat - position).normalize(); }
        plMatrix44 getMatrix   () const;
        
        void importViewParams( const std::string &filename );             
        void exportViewParams( const std::string &filename );
        
		void translate( PLint x, PLint y );
        void rotate   ( PLint x0, PLint y0, PLint x1, PLint y1 );      
        void zoom     ( PLfloat z );       
        void reset    ( const plVector3 &point );
        
    private:

        void _defaultInit();
               
};

// Michiel's arcball code
void      calc_p   ( PLint x, PLint y, PLfloat p[]);
plVector4 calc_quat( PLint x0, PLint y0, PLint x1, PLint y1);

#endif 
