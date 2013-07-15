#include "PlannerWindow.h"

PlannerWindow::PlannerWindow( int x, int y, int width, int height, std::string title, int argc, char **argv )
    : _button( GLUT_NO_BUTTON ),
      _camera( ".view0" ), 
      _cameraMode( CAMERA_ROTATION_MODE ),
      _graftEditor(),
      _boundaryEditor(),
      _plan( argc, argv ), 
      Window( x, y, width, height, title )
{  
    plRenderer::init(); 
    
    plVector3 min, max;
    _plan._models[0].getMinMax(min,max);    
    octree = new plOctree(min,max,5);
    octree->fill( _plan._models[0].bone.triangles(), 5 );
}


void PlannerWindow::display()
{
    plCameraStack::load( _camera );

    plRenderer::queue( _plan );
    plRenderer::queue( _graftEditor );
    plRenderer::queue( _boundaryEditor );
    plRenderer::queue( *octree );
   
    plRenderer::draw();

    glutSwapBuffers();
}


void PlannerWindow::setCursor(int x, int y)
{
    switch (_button)
    {   
        case GLUT_MIDDLE_BUTTON:        glutSetCursor( GLUT_CURSOR_CROSSHAIR );     break;      // middle button:   zoom
        case GLUT_LEFT_BUTTON:          glutSetCursor(GLUT_CURSOR_INFO);            break;      // left button:     click / drag 
        case GLUT_RIGHT_BUTTON: 
           
            // right button:    translate / rotate 
            if (_cameraMode == CAMERA_TRANSLATION_MODE) 
            
                glutSetCursor( GLUT_CURSOR_CROSSHAIR );     // translation cursor

            else
                glutSetCursor( GLUT_CURSOR_CYCLE );         // rotation cursor

     
        case GLUT_NO_BUTTON:            glutSetCursor( GLUT_CURSOR_RIGHT_ARROW );   break;      // no button:     idle cursor   
    }
}


void PlannerWindow::keyAction( unsigned char key, int x, int y )
{
    static int currentView = 0;

    switch (key) 
    {
        case 27:    // esc  
             exit(0);   
                                        
        case ' ':  // spacebar 
            _cameraMode =  (_cameraMode == CAMERA_ROTATION_MODE) ? CAMERA_TRANSLATION_MODE : CAMERA_ROTATION_MODE;  
            break; 
            
        case 'W':   
            _camera.exportViewParams( ".view" + plString::toString(currentView ) );   
            break;
            
        case 'R':   
            _camera.importViewParams( ".view" + plString::toString( currentView ) );   
            break;  
       
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':   // 0-9            
            currentView = (PLint)(key - '0');
            break;                

        case 'b':   _plan._models[0].bone.toggleVisibility();                   break;            
        case 'c':   _plan._models[0].cartilage.toggleVisibility();              break;      
        case 'p':   _plan.toggleVisibility();                                   break;    
        case 'z':   _camera.reset( _plan._models[0].getCentroid() );            break;          
        case 't':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_TRANSLATE );   break; 
        case 'r':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_ROTATE );      break;     
        case 'l':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_LENGTH );      break; 
        case 'm':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_MARKER );      break; 
        //case 'g':   plGraftToggleVisibilityAll();                break;
        //case 's':   plDefectSplineToggleVisibilityAll();         break; 
        //case 'd':   plDonorRegionToggleVisibilityAll();          break;         
        //case 'q':   plDefectSplineCornersToggleVisibilityAll();  break;
        //case 'w':   plDefectSplineBoundaryToggleVisibilityAll(); break;        
        case 'O':   _plan.exportFile("plan");                    break;

        case 'D':   _plan.addDonorSite();                        break;
        case 'S':   _plan.addDefectSite();                       break;
        case 'G':   _plan.addIGuide();                           break;

        case 127:	 // delete 
        {   
            if (glutGetModifiers() == GLUT_ACTIVE_CTRL) 
	        {
                // delete boundary 
	            
            }
            else
            {
                // delete point
                _boundaryEditor.removeSelectedPoint();         	            
	        }
            break;
        } 
    }
    
    glutPostRedisplay();
}


void PlannerWindow::passiveMouseMotion( int x, int y )
{   
    // do nothing   
}

void PlannerWindow::activeMouseMotion( int x, int y )
{    
    switch (_button)
    {
        case GLUT_LEFT_BUTTON: 

            // process drag movements 
            _graftEditor.processMouseDrag   ( _plan, x, glutGet(GLUT_WINDOW_HEIGHT)-y );   
            _boundaryEditor.processMouseDrag( _plan, x, glutGet(GLUT_WINDOW_HEIGHT)-y );  
            break;       

        case GLUT_MIDDLE_BUTTON:    
            
            // zoom camera
            _camera.zoom(_previousMouse.y - y);                             
            break;

        case GLUT_RIGHT_BUTTON:
        
            // previous and current mouse coords will always be very small, as 
            // _previousMouse has already been updated in mouseAction() by this point
            if (_cameraMode == CAMERA_ROTATION_MODE)
            {
                _camera.rotate(_previousMouse.x, _previousMouse.y, x, y);
            }
            else
		    {
			    _camera.translate(_previousMouse.x - x,
			                      (glutGet(GLUT_WINDOW_HEIGHT) - _previousMouse.y) - (glutGet(GLUT_WINDOW_HEIGHT) - y) );
            }
            break;
    }

    // update mouse position on drag    
    _previousMouse.x = x;
    _previousMouse.y = y;

    glutPostRedisplay();
}



// Record button state when mouse button is pressed or released
void PlannerWindow::mouseAction( int button, int state, int x, int y )
{
    // NOTE: this function ALWAYS executes BEFORE activeMouseMotion()
    
    switch (state)
    {
        case GLUT_UP:           _button = GLUT_NO_BUTTON;   return;  // button release, do not process any further (don't want to select anything upon a release)
        case GLUT_DOWN:         _button = button;           break;   // button press
    }
    
    switch (button)
    {   
        case GLUT_LEFT_BUTTON:  
            
            if (glutGetModifiers() == GLUT_ACTIVE_CTRL) 
	        {
                // add new point
                _boundaryEditor.addPoint( _plan, x, glutGet(GLUT_WINDOW_HEIGHT)-y); 
            }
            else
            {
                // process mouse clicks 
                _graftEditor.processMouseClick   ( _plan, x, glutGet(GLUT_WINDOW_HEIGHT)-y);    
                _boundaryEditor.processMouseClick( _plan, x, glutGet(GLUT_WINDOW_HEIGHT)-y); 
            }
            break;
    }    
     
    // update mouse position on key press
    _previousMouse.x = x;
    _previousMouse.y = y;   
    
    glutPostRedisplay();
}
