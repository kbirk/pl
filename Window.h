#ifndef WINDOW_H
#define WINDOW_H

#include "pl.h"

#define MAX_NUM_WINDOWS 20

void GLUTdisplay();
void GLUTreshape( int x, int y );
void GLUTmouseAction( int button, int state, int x, int y );
void GLUTactiveMouseMotion( int x, int y );
void GLUTpassiveMouseMotion( int x, int y );
void GLUTkeyAction( unsigned char c, int x, int y );

class Window 
{
    public:

        Window( int x, int y, int width, int height, std::string title );

        virtual void display() = 0;
        virtual void mouseAction( int button, int state, int x, int y ) {};
        virtual void activeMouseMotion( int x, int y ) {};
        virtual void passiveMouseMotion( int x, int y ) {};
        virtual void keyAction( unsigned char key, int x, int y ) {};        
        virtual void reshape( int width, int height );
        virtual void setCursor( int x, int y) {};
        
    private:
        
        int m_windowId;
        
        void registerWindow( int windowId );
};

#endif
