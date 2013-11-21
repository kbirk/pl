#include "plWindow.h"

namespace plWindow
{

    PLuint _width;
    PLuint _height;
    PLuint _viewportWidth;
    PLuint _viewportHeight;
    PLuint _viewportX;
    PLuint _viewportY;


    void reshape( PLuint width, PLuint height )
    {    
        _width = width;
        _height = height;
    
        _viewportHeight = _width / PL_ASPECT_RATIO ;

        if ( _viewportHeight <= _height )
        {
            _viewportY = ( _height - _viewportHeight )*0.5f;
            _viewportX = 0;
            _viewportWidth = _width;
        }
        else
        {
            _viewportWidth = _height * PL_ASPECT_RATIO;
            _viewportHeight = _height;
            _viewportX = ( _width - _viewportWidth )*0.5f;
            _viewportY = 0; 
        } 
    }
    

    PLuint width()
    {
        return _width;
    }


    PLuint height()
    {
        return _height;
    }


    PLuint viewportWidth()
    {
        return _viewportWidth;
    }


    PLuint viewportHeight()
    {
        return _viewportHeight;
    }
    
    
    PLuint viewportX()
    {
        return _viewportX;
    }


    PLuint viewportY()
    {
        return _viewportY;
    }
    
    
    PLint windowToViewportX( PLint x )
    {
        return x - _viewportX;
    }
    
    
    PLint windowToViewportY( PLint y )
    {
        return y - _viewportY;
    }


    plVector3 mouseToWorld( PLint x, PLint y, PLint z )
    {
        plMatrix44 mvp = ( plProjectionStack::top() * plCameraStack::top() * plModelStack::top() );   
        plMatrix44 mvpInverse = mvp.inverse();
        
        // map window coords to range [0 .. 1]       
        PLfloat nx = ( (PLfloat)(x)-(PLfloat)( _viewportX ) ) / (PLfloat)( _viewportWidth );
        PLfloat ny = ( (PLfloat)(y)-(PLfloat)( _viewportY   ) ) / (PLfloat)( _viewportHeight );
        PLfloat nz = (PLfloat)(z);
        
        // map to range of [-1 .. 1] 
        plVector4 input( (nx * 2.0f) - 1.0f,
                         (ny * 2.0f) - 1.0f,
                         (nz * 2.0f) - 1.0f,
                         1.0f );
         
        plVector4 output = mvpInverse * input;

        if( output.w == 0.0f)
        {
             std::cerr << "plWindow::mouseToWorld() error, w == 0" << std::endl;
             return plVector3();
        }

        return plVector3 (output.x / output.w, 
                          output.y / output.w, 
                          output.z / output.w); 
    }


    plVector3 worldToScreen( PLfloat x, PLfloat y, PLfloat z )
    {
        plVector4 temp1 = ( plCameraStack::top() * plModelStack::top() ) * plVector4(x,y,z,1);
        plVector4 temp2 = plProjectionStack::top() * temp1;

        if (temp2.w == 0.0f)
        {
            std::cerr << "plWindow::mouseToWorld() error, w == 0" << std::endl;
            return plVector3();
        }
        
        // perspective division
        temp2.x /= temp2.w;
        temp2.y /= temp2.w;
        temp2.z /= temp2.w;

        return plVector3( ( temp2.x*0.5f + 0.5f)* _viewportWidth  + _viewportX,
                          ( temp2.y*0.5f + 0.5f)* _viewportHeight + _viewportY,
                          ( 1.0f+temp2.z)*0.5f ); 
                          
    }


    void cameraToMouseRay( plVector3 &rayOrigin, plVector3 &rayDirection, PLint x, PLint y ) 
    {
        plVector3 mouseInWorld = mouseToWorld( x, y, 0 );  

        rayOrigin = plCameraStack::position(); 
        rayDirection = ( mouseInWorld - rayOrigin ).normalize();    
    }

}

