#include "plWindow.h"

namespace plWindow {

    uint32_t _width = 0;
    uint32_t _height =0;
    uint32_t _viewportWidth = 0;
    uint32_t _viewportHeight = 0;
    uint32_t _viewportX = 0;
    uint32_t _viewportY = 0;

    const std::string WINDOW_TITLE = "Planner";
    const uint32_t DEFAULT_WINDOW_WIDTH = 1400;
    const uint32_t DEFAULT_WINDOW_HEIGHT = 1050;

    SDL_Window* window;
    SDL_GLContext context;
    std::map<WindowEventType, std::vector<WindowEventFunc>> callbacks;

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

        return plVector3(
            output.x / output.w,
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

        return plVector3(
            (projected.x*0.5f + 0.5f) * _viewportWidth  + _viewportX,
            (projected.y*0.5f + 0.5f) * _viewportHeight + _viewportY,
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

    void on(WindowEventType id, WindowEventFunc func)
    {
        auto iter = callbacks.find(id);
        if (iter == callbacks.end())
        {
            callbacks[id] = std::vector<WindowEventFunc>();
        }
        callbacks[id].push_back(func);
    }

    void executeCallbacks(WindowEventType type, SDL_Event* event)
    {
        auto iter = callbacks.find(type);
        if (iter != callbacks.end())
        {
            WindowEvent msg{
                type,
                event
            };
            for (auto func : iter->second)
            {
                func(msg);
            }
        }
    }

    void setup()
    {
        // initialize SDL2
        SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        // set the opengl context version
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        // set byte depths
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        // muli-sampling
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
        // create an application window with the following settings:
        window = SDL_CreateWindow(
            WINDOW_TITLE.c_str(), // window title
            SDL_WINDOWPOS_UNDEFINED, // initial x position
            SDL_WINDOWPOS_UNDEFINED, // initial y position
            DEFAULT_WINDOW_WIDTH, // width, in pixels
            DEFAULT_WINDOW_HEIGHT, // height, in pixels
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI); // flags
        // check that the window was successfully made
        if (window == nullptr)
        {
            // In the event that the window could not be made...
            std::cout << "Could not create window: " << SDL_GetError() << std::endl;
            return;
        }
        // create the OpenGL context
        context = SDL_GL_CreateContext(window);
        // disable relative mouse mode
        SDL_SetRelativeMouseMode(SDL_FALSE);
        // sync buffer swap with monitor's vertical refresh rate
        SDL_GL_SetSwapInterval(1);
    }

    void teardown()
    {
        SDL_GL_DeleteContext(context);
        // close and destroy the window
        SDL_DestroyWindow(window);
        // clean up
        SDL_Quit();
    }

    void swapBuffers()
    {
        SDL_GL_SwapWindow(window);
    }

    // glm::ivec2 size()
    // {
    //     int32_t w = 0;
    //     int32_t h = 0;
    //     SDL_GL_GetDrawableSize(window, &w, &h);
    //     return glm::ivec2(w, h);
    // }

    void handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {

                case SDL_KEYDOWN:
                    executeCallbacks(WindowEventType::KEY_PRESS, &event);
                    break;

                case SDL_KEYUP:
                    executeCallbacks(WindowEventType::KEY_RELEASE, &event);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    switch (event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            executeCallbacks(WindowEventType::MOUSE_LEFT_PRESS, &event);
                            break;

                        case SDL_BUTTON_RIGHT:
                            executeCallbacks(WindowEventType::MOUSE_RIGHT_PRESS, &event);
                            break;

                        case SDL_BUTTON_MIDDLE:
                            executeCallbacks(WindowEventType::MOUSE_MIDDLE_PRESS, &event);
                            break;
                    }
                    break;

                case SDL_MOUSEBUTTONUP:
                    switch (event.button.button)
                    {
                        case SDL_BUTTON_LEFT:
                            executeCallbacks(WindowEventType::MOUSE_LEFT_RELEASE, &event);
                            break;

                        case SDL_BUTTON_RIGHT:
                            executeCallbacks(WindowEventType::MOUSE_RIGHT_RELEASE, &event);
                            break;

                        case SDL_BUTTON_MIDDLE:
                            executeCallbacks(WindowEventType::MOUSE_MIDDLE_RELEASE, &event);
                            break;
                    }
                    break;

                case SDL_MOUSEWHEEL:
                    executeCallbacks(WindowEventType::MOUSE_WHEEL, &event);
                    break;

                case SDL_MOUSEMOTION:
                    executeCallbacks(WindowEventType::MOUSE_MOVE, &event);
                    break;

                case SDL_QUIT:
                    executeCallbacks(WindowEventType::CLOSE, &event);
                    break;

                case SDL_WINDOWEVENT:
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            executeCallbacks(WindowEventType::RESIZE, &event);
                            break;
                    }
                    break;
            }
        }
    }

    const uint8_t* pollKeyboard()
    {
        return SDL_GetKeyboardState(nullptr);
    }

}
