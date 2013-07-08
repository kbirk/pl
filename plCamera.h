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
    
        plVector3 position; 
        plVector3 lookat;
        plVector3 up;	
    
        plCamera();
        plCamera( plString filename );
  
        plVector3  direction() const  { return (lookat - position).normalize(); }
        plMatrix44 matrix()    const;
        
        void importViewParams( std::string filename );             
        void exportViewParams( std::string filename );
        
		void translate( PLint x, PLint y );
        void rotate   ( PLint x0, PLint y0, PLint x1, PLint y1 );      
        void zoom     ( PLfloat z );
        
        void reset    ( const plVector3 &point );
        void reset    ( const plBoneAndCartilage &model );
        
    private:

        void _defaultInit();
               
};

// Michiel's arcball code
void      calc_p   ( PLint x, PLint y, PLfloat p[]);
plVector4 calc_quat( PLint x0, PLint y0, PLint x1, PLint y1);

#endif 
