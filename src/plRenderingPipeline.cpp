#include "plRenderingPipeline.h"

namespace plColourStack
{
    std::stack<plVector4> _stack;

    void push(float32_t r, float32_t g, float32_t b, float32_t a) {  _stack.push(plVector4(r,g,b,a));  }
    void push(const plVector4 &colour) { _stack.push(colour); }
    void pop () { if (!_stack.empty()) _stack.pop(); }

    const plVector4& top()    { return _stack.top(); }

    void load(float32_t r, float32_t g, float32_t b, float32_t a)
    {
        if (_stack.size() > 0)
        {
            _stack.top() = plVector4(r,g,b,a);
        }
        else
        {
            _stack.push(plVector4(r,g,b,a));
        }
    }

    void load(const plVector4 &colour)
    {
        load(colour.x, colour.y, colour.z, colour.w);
    }

}


namespace plPickingStack
{
    std::stack<uint32_t> _redStack;
    std::stack<uint32_t> _greenStack;
    std::stack<uint32_t> _blueStack;

    void pushRed   (int32_t r) { _redStack.push(r);   }
    void pushGreen (int32_t g) { _greenStack.push(g); }
    void pushBlue  (int32_t b) { _blueStack.push(b);  }

    void loadRed   (int32_t r)
    {
        if (_redStack.size() > 0)
        {
            _redStack.top() = r;
        }
        else
        {
            _redStack.push(r);
        }
    }


    void loadGreen (int32_t g)
    {
        if (_greenStack.size() > 0)
        {
            _greenStack.top() = g;
        }
        else
        {
            _greenStack.push(g);
        }
    }


    void loadBlue  (int32_t b)
    {
        if (_blueStack.size() > 0)
        {
            _blueStack.top() = b;
        }
        else
        {
            _blueStack.push(b);
        }
    }


    void popRed()   { if (!_redStack.empty()) _redStack.pop(); }
    void popGreen() { if (!_greenStack.empty()) _greenStack.pop(); }
    void popBlue()  { if (!_blueStack.empty()) _blueStack.pop(); }

    int32_t topRed()   { return _redStack.top(); }
    int32_t topGreen() { return _greenStack.top(); }
    int32_t topBlue()  { return _blueStack.top(); }

    plPickingInfo top()
    {
        uint32_t red  = (_redStack.empty()) ? -1 : _redStack.top();
        uint32_t green = (_greenStack.empty()) ? -1 : _greenStack.top();
        uint32_t blue = (_blueStack.empty()) ? -1 : _blueStack.top();

        return plPickingInfo(red, green, blue);
    }
}



namespace plModelStack
{
    plMatrixStack _stack;

    void push()                    { _stack.push(_stack.top()); }
    void push(const plMatrix44 &m) { _stack.push(m); }
    void load(const plMatrix44 &m) { _stack.load(m); }
    void mult(const plMatrix44 &m) { _stack.load(_stack.top() * m); }
    void pop()                     { if (!_stack.empty()) _stack.pop(); }

    void scale(GLfloat s)                                       { _stack.scale(s,s,s); }
    void scale(GLfloat x, GLfloat y, GLfloat z)                 { _stack.scale(x,y,z); }
    void translate(GLfloat x, GLfloat y, GLfloat z)             { _stack.translate(x,y,z); }
    void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z) { _stack.rotate(angle,x,y,z); }

    void scale(const plVector3 &v)                   { _stack.scale(v); }
    void translate(const plVector3 &v)               { _stack.translate(v); }
    void rotate(float32_t angle, const plVector3 &v) { _stack.rotate(angle, v); }

    const plMatrix44& top() { return _stack.top(); }
}


namespace plProjectionStack
{
    plMatrixStack _stack;

    void push()                      { _stack.push(); }
    void push(const plMatrix44 &m)   { _stack.push(m); }
    void push(const plProjection &p) { _stack.push(p.matrix()); }
    void load(const plMatrix44 &m)   { _stack.load(m); }
    void load(const plProjection &p) { _stack.load(p.matrix()); }
    void pop()                       { if (!_stack.empty()) _stack.pop(); }

    const plMatrix44& top() { return _stack.top(); }
}


namespace plCameraStack
{
    plMatrixStack _stack;

    void push()                    { _stack.push(); }
    void push(const plMatrix44 &m) { _stack.push(m); }
    void push(const plCamera &c)   { _stack.push(c.getMatrix()); }
    void load(const plMatrix44 &m) { _stack.load(m); }
    void load(const plCamera &c)   { _stack.load(c.getMatrix()); }
    void pop()                     { if (!_stack.empty()) _stack.pop(); }

    const plMatrix44& top() { return _stack.top(); }

    plVector3 direction()
    {
        return plVector3(-top()[2], -top()[6], -top()[10]);
    }


    plVector3 position()
    {
        // get rotation matrix
        plMatrix44 rot = _stack.top();
        rot.setColumn(3, 0, 0, 0, 1);  // remove translation component
        plVector3 d(-top()[12], -top()[13], -top()[14]);
        return d * rot;
    }


    plVector3 up()
    {
        return plVector3(top()[1], top()[5], top()[9]);
    }

}
