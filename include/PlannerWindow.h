#pragma once

#include "plBoundaryEditor.h"
#include "plCommon.h"
#include "plCamera.h"
#include "plGraftEditor.h"
#include "plModelEditor.h"
#include "plPlan.h"
#include "plVector3.h"
#include "Window.h"

#define GLUT_NO_BUTTON             -1

#define CAMERA_ROTATION_MODE        1
#define CAMERA_TRANSLATION_MODE     2

class PlannerWindow : public Window
{
    public:

        PlannerWindow(int32_t x, int32_t y, int32_t width, int32_t height, std::string title, int32_t argc, char **argv);

        void display();
        void mouseAction(int32_t button, int32_t state, int32_t mx, int32_t my);
        void passiveMouseMotion(int32_t mx, int32_t my);
        void activeMouseMotion (int32_t mx, int32_t my);
        void keyAction(unsigned char key, int32_t mx, int32_t my);
        void setCursor(int32_t mx, int32_t my);
        void idle();

    protected:

        // planner
        plPlan           _plan;

        // editing
        plGraftEditor    _graftEditor;
        plBoundaryEditor _boundaryEditor;
        plModelEditor    _modelEditor;

        // camera
        uint32_t           _cameraMode; // rotation or translation mode
        plCamera         _camera;

        // glut
        plVector3        _previousMouse; // last mouse position
        int32_t            _button; // which button is currently down, must be stored for glutMouseMotion

};
