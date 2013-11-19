#include "plWindow.h"

namespace plWindow
{

    PLuint width()
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        return viewport[2] + viewport[0]*2;
    }


    PLuint height()
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        return viewport[3] + viewport[1]*2;
    }


    PLuint viewportWidth()
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        return viewport[2];
    }


    PLuint viewportHeight()
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        return viewport[3];
    }
    

    PLint windowToViewportX( PLint x )
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        return x - viewport[0];
    }
    
    
    PLint windowToViewportY( PLint y )
    {
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        return y - viewport[1];
    }


    void reshape( PLuint width, PLuint height )
    {    
        
        float viewportHeight = width / PL_ASPECT_RATIO ;
        float viewportWidth;
        float vBuffer, hBuffer;
        if (viewportHeight <= height)
        {
            vBuffer = (height-viewportHeight)*0.5f;
            hBuffer = 0;
            viewportWidth = width;

        }
        else
        {
            viewportWidth = height * PL_ASPECT_RATIO;
            viewportHeight = height;
            hBuffer = (width-viewportWidth)*0.5f;
            vBuffer = 0;

        }
        glViewport( hBuffer, vBuffer, viewportWidth, viewportHeight );
        
        plProjectionStack::load( plProjection( 7.0f, PL_ASPECT_RATIO, PL_NEAR_PLANE, PL_FAR_PLANE ) );    
        
        // use window size dimensions for picking texture, while not as memory efficient it allows us to keep the current viewport and use native window coords
        plPicking::resize( width, height );
        
        plRenderer::resize( viewportWidth, viewportHeight );
    }


    plVector3 mouseToWorld( PLint x, PLint y, PLint z )
    {
        plMatrix44 mvp = ( plProjectionStack::top() * plCameraStack::top() * plModelStack::top() );   
        plMatrix44 mvpInverse = mvp.inverse();
        
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        
        // map window coords to range [0 .. 1]       
        PLfloat nx = ( (PLfloat)(x)-(PLfloat)(viewport[0]) ) / (PLfloat)(viewport[2]);
        PLfloat ny = ( (PLfloat)(y)-(PLfloat)(viewport[1]) ) / (PLfloat)(viewport[3]);
        PLfloat nz = (PLfloat)(z);
        
        // map to range of [-1 .. 1] 
        plVector4 input( (nx * 2.0f) - 1.0f,
                         (ny * 2.0f) - 1.0f,
                         (nz * 2.0f) - 1.0f,
                         1.0f );
         
        plVector4 output = mvpInverse * input;

        if( output.w == 0.0f)
        {
             std::cerr << "plWindow mouseToWorld() error \n";
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
            std::cerr << "plWindow worldToScreen() error \n";
            return plVector3();
        }
        // perspective division
        temp2.x /= temp2.w;
        temp2.y /= temp2.w;
        temp2.z /= temp2.w;

        GLint    viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );  

        return plVector3( (temp2.x*0.5f+0.5f)*viewport[2]+viewport[0],
                          (temp2.y*0.5f+0.5f)*viewport[3]+viewport[1],
                          (1.0+temp2.z)*0.5 ); 
                          
    }


    void cameraToMouseRay( plVector3 &rayOrigin, plVector3 &rayDirection, PLint x, PLint y ) 
    {
        plVector3 mouseInWorld = mouseToWorld( x, y, 0 );  

        rayOrigin = plCameraStack::position(); 
        rayDirection = (mouseInWorld - rayOrigin).normalize();    
    }

}

