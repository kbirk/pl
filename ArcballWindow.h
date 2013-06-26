#ifndef ARCBALL_WINDOW_H
#define ARCBALL_WINDOW_H

// PLANNER
#include "Window.h"
#include "plVector3.h"
#include "plMatrix44.h"
#include "plCamera.h"

#define GLUT_NO_BUTTON             -1

#define CAMERA_ROTATION_MODE 		1
#define CAMERA_TRANSLATION_MODE 	2

class ArcballWindow : public Window 
{
    public:

        ArcballWindow( int x, int y, int width, int height, std::string title );

        virtual void draw()                                                 = 0;
        virtual void userKeyAction( unsigned char key, int x, int y )       = 0;
        virtual bool userMouseAction( int button, int state, int x, int y ) = 0;
        virtual bool userMouseMotion( int x, int y )                        = 0;
        virtual void userPassiveMouseMotion( int x, int y )                 = 0;
        virtual void userSetCursor(int x, int y)                            = 0;        
        
        void display();        
        void mouseAction( int button, int state, int x, int y );
        void passiveMouseMotion( int x, int y);
        void mouseMotion( int x, int y );       
        void keyAction( unsigned char key, int x, int y );      
        void setCursor(int x, int y);
                
    protected:
          
		PLuint	   _cameraMode;
        plVector3  _previousMouse;	// last mouse position
        PLint      _button;		    // which button is currently down, must be stored for glutMouseMotion 

};

#endif

