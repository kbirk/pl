#ifndef WINDOW_H
#define WINDOW_H

#include"pl.h"

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
        virtual void mouseAction       ( int button, int state, int mx, int my ) = 0;
        virtual void activeMouseMotion ( int mx, int my )                        = 0;
        virtual void passiveMouseMotion( int mx, int my )                        = 0;
        virtual void keyAction( unsigned char key, int mx, int my )              = 0;       
        virtual void setCursor( int mx, int my)                                  = 0;
        virtual void reshape  ( int width, int height );
        
        
    private:
        
        int _windowID;
        
        void registerWindow( int windowId );
};

#endif
