#include "plProjection.h"

plProjection::plProjection(PLfloat aspectRatio) 
    :   _fov(7.0f), _aspect(aspectRatio), _nearPlane(100.0f), _farPlane(15000.0f)
{
}

plProjection::plProjection(PLfloat fov, PLfloat aspectRatio, PLfloat nearPlane, PLfloat farPlane)
    :   _fov(fov), _aspect(aspectRatio), _nearPlane(nearPlane), _farPlane(farPlane)
{
}
				
/////////////////////////////////////////////////////////

void plSet( plProjection &projection )
{
    _plProjection = &projection;
}


/////////////////////////////////////////////////////////

void plWindowReshape(PLuint width, PLuint height)
{    
    _plProjection->aspectRatio( (PLfloat)(width) / (PLfloat)(height) );
    delete _plPickingTexture;
    _plPickingTexture = new plPickingTexture(width, height);
}

plVector3 plWindowGetMouseToWorldPos(PLint x, PLint y, PLint z)
{
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint    viewport[4];
    GLdouble objX, objY, objZ;    

    // set projection
    plDrawSetProjectionMatrix();       
    // set viewing matrix
    plDrawSetViewingMatrix();
    
    glGetDoublev( GL_MODELVIEW_MATRIX, modelMatrix );
    glGetDoublev( GL_PROJECTION_MATRIX, projMatrix );
    glGetIntegerv( GL_VIEWPORT, viewport );

    gluUnProject( (GLdouble) x, (GLdouble) y, (GLdouble) z,
                   modelMatrix, projMatrix, viewport,
                   &objX, &objY, &objZ );

    return plVector3( objX, objY, objZ );
}


plVector3 plWindowGetWorldToScreenPos(PLfloat x, PLfloat y, PLfloat z) 
{
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    GLint    viewport[4];
    GLdouble winX, winY, winZ;

    // set projection
    plDrawSetProjectionMatrix();       
    // set viewing matrix
    plDrawSetViewingMatrix();

    glGetDoublev( GL_MODELVIEW_MATRIX, modelMatrix );
    glGetDoublev( GL_PROJECTION_MATRIX, projMatrix );
    glGetIntegerv( GL_VIEWPORT, viewport );   

    gluProject( (GLdouble) x, (GLdouble) y, (GLdouble) z,
                 modelMatrix, projMatrix, viewport,
                 &winX, &winY, &winZ );

    return plVector3( winX, winY, winZ );
}
