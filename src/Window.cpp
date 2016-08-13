#include "Window.h"

#include "plWindow.h"
#include "plRenderResources.h"

static Window *windows[MAX_NUM_WINDOWS];

Window::Window(int32_t x, int32_t y, int32_t width, int32_t height, std::string title)
{
    glutInitWindowPosition(x, y);
    glutInitWindowSize    (width, height);
    _windowID = glutCreateWindow(title.c_str());
    registerWindow  (_windowID);
    glutSetWindow   (_windowID);
    glutDisplayFunc (GLUTdisplay);
    glutReshapeFunc (GLUTreshape);
    glutMouseFunc   (GLUTmouseAction);
    glutMotionFunc  (GLUTactiveMouseMotion);
    glutPassiveMotionFunc(GLUTpassiveMouseMotion);
    glutKeyboardFunc(GLUTkeyAction);
    glutIdleFunc    (GLUTidle);

}

void Window::registerWindow(int32_t windowId)
{
    if (_windowID < 0 || _windowID >= MAX_NUM_WINDOWS)
    {
        std::cerr << "New window has windowId " << _windowID
                  << ", which is outside the allowed range 0.." << MAX_NUM_WINDOWS-1
                  << std::endl;
        abort();
    }
    windows[_windowID] = this;
}

void Window::reshape(int32_t width, int32_t height)
{
    plWindow::reshape(width, height);
    plRenderResources::reshape(plWindow::viewportWidth(), plWindow::viewportHeight());

    glutPostRedisplay();
}

void GLUTdisplay()
{
    windows[glutGetWindow()]->display();
}

void GLUTreshape(int32_t x, int32_t y)
{
    windows[glutGetWindow()]->reshape(x, y);
}


void GLUTmouseAction(int32_t button, int32_t state, int32_t x, int32_t y)
{
    windows[glutGetWindow()]->mouseAction(button, state, x, y);
    windows[glutGetWindow()]->setCursor(x, y);
}


void GLUTactiveMouseMotion(int32_t x, int32_t y)
{
    windows[glutGetWindow()]->activeMouseMotion(x, y);
    windows[glutGetWindow()]->setCursor(x, y);
}


void GLUTpassiveMouseMotion(int32_t x, int32_t y)
{
    windows[glutGetWindow()]->passiveMouseMotion(x, y);
    windows[glutGetWindow()]->setCursor(x, y);
}


void GLUTkeyAction(unsigned char c, int32_t x, int32_t y)
{
    windows[glutGetWindow()]->keyAction(c, x, y);
}

void GLUTidle()
{
    windows[glutGetWindow()]->idle();
}
