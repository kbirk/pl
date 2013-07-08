#ifndef PLANNER_WINDOW_H
#define PLANNER_WINDOW_H

// PLANNER
#include "Window.h"
#include "plCamera.h"
#include "plPlan.h"
#include "plGraftEditor.h"
#include "plBoundaryEditor.h"
#include "plProjection.h"

#define GLUT_NO_BUTTON             -1

#define CAMERA_ROTATION_MODE 		1
#define CAMERA_TRANSLATION_MODE 	2

class PlannerWindow : public Window 
{
    public:

        PlannerWindow( int x, int y, int width, int height, std::string title, int argc, char **argv );
      
        void display();        
        void mouseAction( int button, int state, int x, int y );
        void passiveMouseMotion( int x, int y);
        void activeMouseMotion( int x, int y );       
        void keyAction( unsigned char key, int x, int y );      
        void setCursor(int x, int y);
                
    protected:
        
        // planner
        plPlan           _plan;

        // editing
        plGraftEditor    _graftEditor; 
        plBoundaryEditor _boundaryEditor;

        // camera  
		PLuint	   _cameraMode;     // rotation or translation mode
		plCamera   _camera;
		
		// projection
		
		plProjection _projection;
		
		// glut
        plVector3  _previousMouse;	// last mouse position
        PLint      _button;		    // which button is currently down, must be stored for glutMouseMotion 

};

#endif

