#ifndef PLANNER_WINDOW_H
#define PLANNER_WINDOW_H

// PLANNER
#include "Window.h"

#define GLUT_NO_BUTTON             -1

#define CAMERA_ROTATION_MODE 		1
#define CAMERA_TRANSLATION_MODE 	2

class PlannerWindow : public Window 
{
    public:

        PlannerWindow( int x, int y, int width, int height, std::string title, int argc, char **argv );
      
        void display           ();        
        void mouseAction       ( int button, int state, int mx, int my );
        void passiveMouseMotion( int mx, int my);
        void activeMouseMotion ( int mx, int my );       
        void keyAction         ( unsigned char key, int mx, int my );      
        void setCursor         ( int mx, int my );
        void idle              ();
                
    protected:

        // planner
        plPlan           _plan;

        // editing
        plGraftEditor    _graftEditor; 
        plBoundaryEditor _boundaryEditor;
        plModelEditor    _modelEditor;
        
        // camera  
		PLuint	         _cameraMode;     // rotation or translation mode
		plCamera         _camera;		
		
		// glut
        plVector3        _previousMouse;	// last mouse position
        PLint            _button;		    // which button is currently down, must be stored for glutMouseMotion 

};

#endif

