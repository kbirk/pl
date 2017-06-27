#pragma once

#include "plCommon.h"
#include "plMatrix44.h"
#include "plOpenGLCommon.h"

class plMatrixStack {
public:
    plMatrixStack();

    bool empty() { return _stack.size() == 0; }

    void load(const plMatrix44& m);
    void push();
    void push(const plMatrix44& m);
    void mult(const plMatrix44& m);
    void pop();

    void scale(GLfloat x, GLfloat y, GLfloat z);
    void translate(GLfloat x, GLfloat y, GLfloat z);
    void rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

    void scale(const plVector3& v);
    void translate(const plVector3& v);
    void rotate(float32_t angle, const plVector3& v);

    const plMatrix44& top(void);

private:
    std::stack<plMatrix44> _stack;
};
