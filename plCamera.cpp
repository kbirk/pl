#include "plCamera.h"

plCamera::plCamera()
{
	_defaultInit();
}

plCamera::plCamera( plString filename)
{   
	// read view parameters from file
	importViewParams( filename );
}
  
void plCamera::_defaultInit()
{
	// set default position
	position = plVector3(0,0,50);    
	lookat   = plVector3(0,0,0);
	up       = plVector3(0,1,0);
}  
    
plMatrix44 plCamera::getMatrix() const 
{			 
    plVector3 x, y, z;

    // Make rotation plMatrix44 
    z = (position - lookat).normalize();
    y = up.normalize();

    // X plVector3 = Y cross Z 
    x = (y ^ z).normalize();

    // Recompute Y = Z cross X 
    y = (z ^ x).normalize();

    plMatrix44 rot;
    rot(0,0) = x.x;   rot(0,1) = x.y;   rot(0,2) = x.z;   rot(0,3) = 0.0;
    rot(1,0) = y.x;   rot(1,1) = y.y;   rot(1,2) = y.z;   rot(1,3) = 0.0;
    rot(2,0) = z.x;   rot(2,1) = z.y;   rot(2,2) = z.z;   rot(2,3) = 0.0;
    rot(3,0) = 0.0;   rot(3,1) = 0.0;   rot(3,2) = 0.0;   rot(3,3) = 1.0;

    // Translate Eye to Origin 
    plMatrix44 trans( -position.x, -position.y, -position.z );
   
    return rot * trans;
}

void plCamera::reset(const plVector3 &point)
{
    plVector3 focus_centre = point;     
    plVector3 separation = position - focus_centre;    
    plVector3 camera_direction = (lookat-position).normalize();     
    PLfloat projection = separation * camera_direction;
     
    position = focus_centre + (projection * camera_direction);
    lookat = focus_centre;
}


void plCamera::exportViewParams( std::string filename )
{
    std::ofstream out( filename.c_str() );

    if (!out) 
    {
        std::cerr << "plCamera outputViewParams error: Failed to open the output file\n";
    }
    else 
    {
        out << position << std::endl;
        out << lookat << std::endl;
        out << up << std::endl;
    }
}


void plCamera::importViewParams( std::string filename )
{
    std::ifstream in( filename.c_str() );

    if (!in) 
    {
        std::cerr << "plCamera readViewParams error: Failed to open the input file\n";
        _defaultInit();
    }
    else 
    {
        in >> position;
        in >> lookat;
        in >> up;
    }
}


void plCamera::zoom(PLfloat z)
{     
    const PLfloat ZOOM_SENSITIVITY = 0.005f;
 
    position = (position + (z*ZOOM_SENSITIVITY)*(lookat - position));

}

void plCamera::translate(PLint x, PLint y)
{
    const PLfloat TRANSLATION_SENSITIVITY = 0.1f;
    
    PLfloat distance = (lookat -position).length();
    
    plVector3 ydir = up.normalize();
    plVector3 zdir = (position - lookat).normalize();
    plVector3 xdir = (ydir ^ zdir).normalize();
    
    lookat   = lookat   + TRANSLATION_SENSITIVITY * (x * xdir + y * ydir);
    position = position + TRANSLATION_SENSITIVITY * (x * xdir + y * ydir);  
   
}

void plCamera::rotate( PLint x0, PLint y0, PLint x1, PLint y1 )
{
    // create quaternion  
    plVector4 quat = calc_quat( x0, y0, x1, y1 );
    // get the rotation matrix from quaternion
    plMatrix44 qm;  qm.setRotation( quat );

    // get current modelview matrix (rotation component only)
    plMatrix44 m = getMatrix();
    m(0,3) = 0.0f;
    m(1,3) = 0.0f;
    m(2,3) = 0.0f;
    m(3,3) = 1.0f;
    
    // make transform t = m^-1 qm^-1
    plMatrix44 t = m.inverse() * qm.inverse();

    // rotate the view and up direction
    plVector4 prev( 0, 0,(position - lookat).length(), 1);

    plVector4 next = t * prev;

    position.x = next.x/next.w + lookat.x;
    position.y = next.y/next.w + lookat.y;
    position.z = next.z/next.w + lookat.z;

    prev = plVector4(0,1,0,1);

    next = t * prev;

    up.x = next.x/next.w;
    up.y = next.y/next.w;
    up.z = next.z/next.w;
}

// ================================================================
//                 From Michiel's arcball code
// ================================================================

void calc_p(PLint x, PLint y, PLfloat p[])
{
    PLfloat r,s;
    
    GLint viewport[4];
    glGetIntegerv( GL_VIEWPORT, viewport );

    PLint width  = viewport[2];
    PLint height = viewport[3];
    
    p[0] =  2.0 * ( x - 0.5*width ) / width;
    p[1] = -2.0 * ( -(y-height) - 0.5*height ) / height;
    r = p[0]*p[0] + p[1]*p[1];
    if (r > 1.0) 
    {
        s = 1.0/sqrt(r);
        p[0] *= s;
        p[1] *= s;
        p[2] = 0.0;
    } 
    else 
    {
        p[2] = sqrt(1.0 - r);
    }
}


plVector4 calc_quat(PLint x0, PLint y0, PLint x1, PLint y1)
{
    PLfloat p0[4], p1[4];

    calc_p(x0,y0,p0);
    calc_p(x1,y1,p1);
    
    plVector4 q;
    q.x = p0[1]*p1[2] - p1[1]*p0[2];
    q.y = p1[0]*p0[2] - p0[0]*p1[2];
    q.z = p0[0]*p1[1] - p1[0]*p0[1];
    q.w = (p0[0]*p1[0] + p0[1]*p1[1] + p0[2]*p1[2]);
    
    if (q.w < -1.0) 
        q.w = -1.0;
        
    return q;
}






