#pragma once

#include "plCommon.h"
#include "plVector4.h"
#include "plMatrixStack.h"
#include "plProjection.h"
#include "plCamera.h"
#include "plPicking.h"


namespace plColourStack
{
    void push(float32_t r, float32_t g, float32_t b, float32_t a = 1.0f);
    void push(const plVector4 &colour);

    void load(float32_t r, float32_t g, float32_t b, float32_t a = 1.0f);
    void load(const plVector4 &colour);

    void pop ();

    const plVector4& top();
}


namespace plPickingStack
{
    void pushRed   (int32_t r);
    void pushGreen (int32_t g);
    void pushBlue  (int32_t b);

    void loadRed   (int32_t r);
    void loadGreen (int32_t g);
    void loadBlue  (int32_t b);

    void popRed   ();
    void popGreen ();
    void popBlue  ();

    int32_t topRed();
    int32_t topBlue();
    int32_t topGreen();

    plPickingInfo top();
}


namespace plModelStack
{
    void push();
    void push(const plMatrix44 &m);
    void load(const plMatrix44 &m);
    void mult(const plMatrix44 &m);
    void pop();

    void scale    (GLfloat s);
    void scale    (GLfloat x, GLfloat y, GLfloat z);
    void translate(GLfloat x, GLfloat y, GLfloat z);
    void rotate   (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

    void scale    (const plVector3 &v);
    void translate(const plVector3 &v);
    void rotate   (float32_t angle, const plVector3 &v);

    const plMatrix44& top();
}


namespace plCameraStack
{
    void push();
    void push(const plMatrix44 &m);
    void push(const plCamera   &c);
    void load(const plMatrix44 &m);
    void load(const plCamera   &c);
    void pop();

    plVector3 direction();
    plVector3 position();
    plVector3 up();

    const plMatrix44& top();
}


namespace plProjectionStack
{
    void push();
    void push(const plMatrix44   &m);
    void push(const plProjection &p);
    void load(const plMatrix44   &m);
    void load(const plProjection &p);
    void pop();

    const plMatrix44& top();
}
