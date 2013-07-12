#include "plWindow.h"

void plWindow::reshape( PLuint width, PLuint height )
{    
    plProjectionStack::load( plProjection( 7.0f, (PLfloat)(width) / (PLfloat)(height), 100.0f, 15000.0f) );    
    plPicking::texture->setFBO(width, height); 
}

plVector3 plWindow::mouseToWorld( PLint x, PLint y, PLint z )
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
         std::cerr << "plWindow unproject error \n";
         return plVector3();
    }

    return plVector3 (output.x / output.w, 
                      output.y / output.w, 
                      output.z / output.w); 
}


plVector3 plWindow::worldToScreen( PLfloat x, PLfloat y, PLfloat z ) 
{
    plVector4 temp1 = (plCameraStack::top() * plModelStack::top()) * plVector4(x,y,z,1);
    plVector4 temp2 =  plProjectionStack::top() * temp1;

    if (temp2.w == 0.0f)
    {
        std::cerr << "plWindow project error \n";
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

void plWindow::mouseToRay( plVector3 &rayOrigin, plVector3 &rayDirection, PLint x, PLint y ) 
{
    plVector3 mouseInWorld = plWindow::mouseToWorld(x, y, 0);  

    // NEED TO GET CAMERA POSITION PROPERLY!
    //*intentional bug!
    rayOrigin = plCameraStack::position(); 

    rayDirection = (mouseInWorld - rayOrigin).normalize();    
}

