#include "plWindow.h"

namespace plWindow
{

    uint32_t _width = 0;
    uint32_t _height =0;
    uint32_t _viewportWidth = 0;
    uint32_t _viewportHeight = 0;
    uint32_t _viewportX = 0;
    uint32_t _viewportY = 0;


    void reshape(uint32_t width, uint32_t height)
    {
        _width = width;
        _height = height;

        _viewportHeight = _width / PL_ASPECT_RATIO ;

        if (_viewportHeight <= _height)
        {
            _viewportY = (_height - _viewportHeight)*0.5f;
            _viewportX = 0;
            _viewportWidth = _width;
        }
        else
        {
            _viewportWidth = _height * PL_ASPECT_RATIO;
            _viewportHeight = _height;
            _viewportX = (_width - _viewportWidth)*0.5f;
            _viewportY = 0;
        }
    }


    uint32_t width()
    {
        return _width;
    }


    uint32_t height()
    {
        return _height;
    }


    uint32_t viewportWidth()
    {
        return _viewportWidth;
    }


    uint32_t viewportHeight()
    {
        return _viewportHeight;
    }


    uint32_t viewportX()
    {
        return _viewportX;
    }


    uint32_t viewportY()
    {
        return _viewportY;
    }


    int32_t windowToViewportX(int32_t x)
    {
        return x - _viewportX;
    }


    int32_t windowToViewportY(int32_t y)
    {
        return y - _viewportY;
    }


    plVector3 mouseToWorld(int32_t x, int32_t y, int32_t z)
    {
        plMatrix44 mvp = (plProjectionStack::top() * plCameraStack::top() * plModelStack::top());
        plMatrix44 mvpInverse = mvp.inverse();

        // map window coords to range [0 .. 1]
        float32_t nx = ((float32_t)(x)-(float32_t)(_viewportX)) / (float32_t)(_viewportWidth);
        float32_t ny = ((float32_t)(y)-(float32_t)(_viewportY)) / (float32_t)(_viewportHeight);
        float32_t nz = (float32_t)(z);

        // map to range of [-1 .. 1]
        plVector4 input((nx * 2.0f) - 1.0f,
                         (ny * 2.0f) - 1.0f,
                         (nz * 2.0f) - 1.0f,
                         1.0f);

        plVector4 output = mvpInverse * input;

        if (output.w == 0.0f)
        {
             std::cerr << "plWindow::mouseToWorld() error, w == 0" << std::endl;
             return plVector3();
        }

        return plVector3 (output.x / output.w,
                          output.y / output.w,
                          output.z / output.w);
    }


    plVector3 worldToScreen(float32_t x, float32_t y, float32_t z)
    {
        plVector4 projected = (plProjectionStack::top() * plCameraStack::top() * plModelStack::top()) * plVector4(x,y,z,1);

        if (projected.w == 0.0f)
        {
            std::cerr << "plWindow::mouseToWorld() error, w == 0" << std::endl;
            return plVector3();
        }

        // perspective division
        projected.x /= projected.w;
        projected.y /= projected.w;
        projected.z /= projected.w;

        return plVector3((projected.x*0.5f + 0.5f)* _viewportWidth  + _viewportX,
                          (projected.y*0.5f + 0.5f)* _viewportHeight + _viewportY,
                          (1.0f+projected.z)*0.5f);

    }


    plVector3 mouseToWorld(const plVector3& position)
    {
        return mouseToWorld(position.x, position.y, position.z);
    }


    plVector3 worldToScreen(const plVector3& position)
    {
        return worldToScreen(position.x, position.y, position.z);
    }


    void cameraToMouseRay(plVector3 &rayOrigin, plVector3 &rayDirection, int32_t x, int32_t y)
    {
        plVector3 mouseInWorld = mouseToWorld(x, y, 0);

        rayOrigin = plCameraStack::position();
        rayDirection = (mouseInWorld - rayOrigin).normalize();
    }

}
