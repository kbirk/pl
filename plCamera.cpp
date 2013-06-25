#include "plCamera.h"

plCamera::plCamera()
{
	defaultInit();
}

plCamera::plCamera( std::string filename)
{   
	// read view parameters from file
	importViewParams( filename );
}
  
void plCamera::defaultInit()
{
	// set default position
	_position = plVector3(0,0,50);    
	_lookat   = plVector3(0,0,0);
	_up       = plVector3(0,1,0);

}  
    
plMatrix44 plCamera::getMatrix() const 
{			 
    plVector3 x, y, z;

    // Make rotation plMatrix44 
    z = (_position - _lookat).normalize();
    y = _up;

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
    plMatrix44 trans( -_position.x, -_position.y, -_position.z );
   
    return rot * trans;
}

void plCamera::reset(const plVector3 &min, const plVector3 &max)
{
     plVector3 focus_centre = 0.5f * (max + min);     
     plVector3 separation = _position - focus_centre;    
     plVector3 camera_direction = (_lookat-_position).normalize();     
     PLfloat projection = separation * camera_direction;
     
     _position = focus_centre + (projection * camera_direction);
     _lookat = focus_centre;
}

void plCamera::exportViewParams( std::string filename )
{
    std::ofstream out( filename );

    if (!out) 
    {
        std::cerr << "plCamera outputViewParams error: Failed to open the output file\n";
    }
    else 
    {
        out << _position << std::endl;
        out << _lookat << std::endl;
        out << _up << std::endl;
    }
}


void plCamera::importViewParams( std::string filename )
{
    std::ifstream in( filename );

    if (!in) 
    {
        std::cerr << "plCamera readViewParams error: Failed to open the input file\n";
        defaultInit();
    }
    else 
    {
        in >> _position;
        in >> _lookat;
        in >> _up;
    }
}


void plCamera::zoom(PLfloat z)
{     
    const PLfloat ZOOM_SENSITIVITY = 0.005f;
 
    _position = (_position + (z*ZOOM_SENSITIVITY)*(_lookat - _position));

}

void plCamera::translate(PLint x, PLint y)
{
    const PLfloat TRANSLATION_SENSITIVITY = 0.1f;
    
    PLfloat distance = (_lookat -_position).length();
    
    plVector3 ydir = _up.normalize();
    plVector3 zdir = (_position - _lookat).normalize();
    plVector3 xdir = (ydir ^ zdir).normalize();
    
    _lookat = _lookat + TRANSLATION_SENSITIVITY * (x * xdir + y * ydir);
    _position = _position + TRANSLATION_SENSITIVITY * (x * xdir + y * ydir);  
   
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
    plVector4 prev( 0, 0,(_position - _lookat).length(), 1);

    plVector4 next = t * prev;

    _position.x = next.x/next.w + _lookat.x;
    _position.y = next.y/next.w + _lookat.y;
    _position.z = next.z/next.w + _lookat.z;

    prev = plVector4(0,1,0,1);

    next = t * prev;

    _up.x = next.x/next.w;
    _up.y = next.y/next.w;
    _up.z = next.z/next.w;
}

// ================================================================
//                 From Michiel's arcball code
// ================================================================

void calc_p(PLint x, PLint y, PLfloat p[])
{
    PLfloat r,s;
    PLint width = glutGet(GLUT_WINDOW_WIDTH);
    PLint height = glutGet(GLUT_WINDOW_HEIGHT);
    
    p[0] = 2.0*(x - 0.5*width)/width;
    p[1] = -2.0*(y - 0.5*height)/height;
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

/////////////////////////////////////////

plVector3 plCameraGetDirection()
{
	return _plCamera->getViewingDirection();
}

plVector3 plCameraGetPosition()
{
    return _plCamera->getPosition();
}

void plCameraTranslate(PLint x, PLint y)
{
	_plCamera->translate(x, y);
}

void plCameraZoom(PLfloat z)
{
	_plCamera->zoom(z);
}

void plCameraArcballRotate(PLint x0, PLint x1, PLint y0, PLint y1)
{
	_plCamera->rotate(x0, x1, y0, y1);
}

void plCameraImportView(plString filename)
{
	_plCamera->importViewParams(filename);
}

void plCameraExportView(plString filename)
{
	_plCamera->exportViewParams(filename);
}

void plCameraResetToModel(PLuint model_id)
{
    if ( plErrorCheckModelBounds(model_id, "plCameraResetToModel") )
        return;

	plVector3 min, max;
	_plBoneAndCartilageModels[model_id]->getMinMax(min, max);
	_plCamera->reset(min, max);
}







