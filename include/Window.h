#pragma once

#include "plCommon.h"

#define MAX_NUM_WINDOWS 20

void GLUTdisplay();
void GLUTreshape(int32_t x, int32_t y);
void GLUTmouseAction(int32_t button, int32_t state, int32_t x, int32_t y);
void GLUTactiveMouseMotion(int32_t x, int32_t y);
void GLUTpassiveMouseMotion(int32_t x, int32_t y);
void GLUTkeyAction(unsigned char c, int32_t x, int32_t y);
void GLUTidle();

class Window
{
    public:

        Window(int32_t x, int32_t y, int32_t width, int32_t height, std::string title);

        virtual void display() = 0;
        virtual void mouseAction(int32_t button, int32_t state, int32_t mx, int32_t my) = 0;
        virtual void activeMouseMotion (int32_t mx, int32_t my) = 0;
        virtual void passiveMouseMotion(int32_t mx, int32_t my) = 0;
        virtual void keyAction(unsigned char key, int32_t mx, int32_t my) = 0;
        virtual void setCursor(int32_t mx, int32_t my) = 0;
        virtual void idle() = 0;
        virtual void reshape(int32_t width, int32_t height);

    private:

        int32_t _windowID;

        void registerWindow(int32_t windowId);
};
