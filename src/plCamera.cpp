#include "plCamera.h"

#include "plWindow.h"

void calcP(int32_t x, int32_t y, float32_t p[4]);
plVector4 calcQuat(int32_t x0, int32_t y0, int32_t x1, int32_t y1);

plCamera::plCamera()
{
    _defaultInit();
}

plCamera::plCamera(plString filename)
{
    // read view parameters from file
    importViewParams(filename);
}

void plCamera::_defaultInit()
{
    // set default position
    position = plVector3(0,0,50);
    lookat = plVector3(0, 0, 0);
    up = plVector3(0,1,0);
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
    plMatrix44 trans(-position.x, -position.y, -position.z);

    return rot * trans;
}

void plCamera::reset(const plVector3 &point)
{
    plVector3 focus_centre = point;
    plVector3 separation = position - focus_centre;
    plVector3 camera_direction = (lookat-position).normalize();
    float32_t projection = separation * camera_direction;

    position = focus_centre + (projection * camera_direction);
    lookat = focus_centre;
}


void plCamera::exportViewParams(const std::string &filename)
{
    std::ofstream out(filename.c_str());

    if (!out)
    {
        LOG_WARN("Failed to open the output file");
    }
    else
    {
        out << position << std::endl;
        out << lookat << std::endl;
        out << up << std::endl;
    }
}


void plCamera::importViewParams(const std::string &filename)
{
    std::ifstream in(filename.c_str());

    if (!in)
    {
        LOG_WARN("Failed to open the input file, loading default camera parameters");
        _defaultInit();
    }
    else
    {
        in >> position;
        in >> lookat;
        in >> up;
    }
}


void plCamera::zoom(float32_t z)
{
    const float32_t ZOOM_SENSITIVITY = 0.005f;

    position = (position + (z*ZOOM_SENSITIVITY)*(lookat - position));

}

void plCamera::translate(int32_t x, int32_t y)
{
    const float32_t TRANSLATION_SENSITIVITY = 0.1f;

    plVector3 ydir = up.normalize();
    plVector3 zdir = (position - lookat).normalize();
    plVector3 xdir = (ydir ^ zdir).normalize();

    lookat = lookat + TRANSLATION_SENSITIVITY * (x * xdir + y * ydir);
    position = position + TRANSLATION_SENSITIVITY * (x * xdir + y * ydir);

}

void plCamera::rotate(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    // create quaternion
    plVector4 quat = calcQuat(x0, y0, x1, y1);
    // get the rotation matrix from quaternion
    plMatrix44 qm;
    qm.setRotation(quat);

    // get current modelview matrix (rotation component only)
    plMatrix44 m = getMatrix();
    m(0,3) = 0.0f;
    m(1,3) = 0.0f;
    m(2,3) = 0.0f;
    m(3,3) = 1.0f;

    // make transform t = m^-1 qm^-1
    plMatrix44 t = m.inverse() * qm.inverse();

    // rotate the view and up direction
    plVector4 prev(0, 0,(position - lookat).length(), 1);

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

void calcP(int32_t x, int32_t y, float32_t p[4])
{
    int32_t width = plWindow::viewportWidth() + plWindow::viewportX() * 2;
    int32_t height = plWindow::viewportHeight() + plWindow::viewportY() * 2;

    p[0] = 2.0f * (x - 0.5f*width) / width;
    p[1] = -2.0f * (-(y-height) - 0.5f*height) / height;
    float32_t r = p[0]*p[0] + p[1]*p[1];
    if (r > 1.0f)
    {
        float32_t s = 1.0f/sqrt(r);
        p[0] *= s;
        p[1] *= s;
        p[2] = 0.0f;
    }
    else
    {
        p[2] = sqrt(1.0f - r);
    }
}


plVector4 calcQuat(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    float32_t p0[4], p1[4];

    calcP(x0, y0, p0);
    calcP(x1, y1, p1);

    plVector4 q;
    q.x = p0[1]*p1[2] - p1[1]*p0[2];
    q.y = p1[0]*p0[2] - p0[0]*p1[2];
    q.z = p0[0]*p1[1] - p1[0]*p0[1];
    q.w = (p0[0]*p1[0] + p0[1]*p1[1] + p0[2]*p1[2]);

    if (q.w < -1.0f)
    {
        q.w = -1.0f;
    }
    return q;
}
