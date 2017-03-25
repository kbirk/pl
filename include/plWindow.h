#pragma once

#include "plCommon.h"
#include "plOpenGLCommon.h"
#include "plMatrix44.h"
#include "plRenderingPipeline.h"

#include <SDL2/SDL.h>

enum class WindowEventType
{
    KEY_PRESS,
    KEY_RELEASE,
    MOUSE_PRESS,
    MOUSE_RELEASE,
    MOUSE_MOVE,
    MOUSE_WHEEL,
    RESIZE,
    CLOSE
};

struct WindowEvent
{
    WindowEventType type;
    SDL_Event* originalEvent;
};

typedef std::function<void(const WindowEvent&)> WindowEventFunc;

namespace plWindow
{
    uint32_t width();
    uint32_t height();
    uint32_t viewportWidth();
    uint32_t viewportHeight();
    uint32_t viewportX();
    uint32_t viewportY();
    int32_t windowToViewportX(int32_t x);
    int32_t windowToViewportY(int32_t y);
    int32_t pixelRatio();

    void setup();
    void teardown();
    plVector3 mouseToWorld(int32_t x, int32_t y, int32_t z);
    plVector3 worldToScreen (float32_t x, float32_t y, float32_t z);
    plVector3 mouseToWorld(const plVector3& position);
    plVector3 worldToScreen(const plVector3& position);
    void cameraToMouseRay(plVector3& rayOrigin, plVector3& rayDirection, int32_t x, int32_t y);

    void swapBuffers();
    void handleEvents();
    const uint8_t* pollKeyboard();
    void on(WindowEventType, WindowEventFunc);

}
