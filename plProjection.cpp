#include "plProjection.h"

plProjection::plProjection(PLfloat aspect_ratio) 
    :   _fov(7), _aspect(aspect_ratio), _near(100), _far(15000)
{
}

plProjection::plProjection(PLfloat fov, PLfloat aspect_ratio, PLfloat near_plane, PLfloat far_plane)
    :   _fov(fov), _aspect(aspect_ratio), _near(near_plane), _far(far_plane)
{
}

plMatrix44 plProjection::getMatrix()
{
	return plMatrix44( _fov, _aspect, _near, _far );
}		

void plProjection::setFOV(PLfloat fov)
{
    _fov = fov;
}

void plProjection::setNear(PLfloat near_plane)
{
    _near = near_plane;
}

void plProjection::setFar(PLfloat far_plane)
{
    _far = far_plane;
}

void plProjection::setAspectRatio(PLfloat aspect_ratio)
{
    _aspect = aspect_ratio;
}
				
/////////////////////////////////////////////////////////


void plProjectionSetAspect(PLfloat aspect_ratio)
{
    _plProjection->setAspectRatio(aspect_ratio);
}

void plProjectionSetFOV(PLfloat fov)
{
    _plProjection->setFOV(fov);
}

void plProjectionSetNear(PLfloat near_plane)
{
    _plProjection->setNear(near_plane);
}

void plProjectionSetFar(PLfloat far_plane)
{
    _plProjection->setFar(far_plane);
}

void plProjectionSetPerspective(PLfloat fov, PLfloat aspect, PLfloat near_plane, PLfloat far_plane)
{
    delete _plProjection;
    _plProjection = new plProjection(fov, aspect, near_plane, far_plane);
}


void plWindowReshape(PLuint width, PLuint height)
{    
    _plProjection->setAspectRatio( (PLfloat)(width) / (PLfloat)(height) );
    delete _plPickingTexture;
    _plPickingTexture = new plPickingTexture(width, height);
}

/////////////////////////////////////////////////////////

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
