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
    plInit();

    _graftEditor.attach( _plan );
    _boundaryEditor.attach( _plan );
    _modelEditor.attach( _plan );    
}


void PlannerWindow::idle()
{
    //glutPostRedisplay();
}


void PlannerWindow::display()
{
    // set camera
    plCameraStack::load( _camera );  
    // set perspective projection
    plProjectionStack::load( plProjection( PL_FIELD_OF_VIEW , PL_ASPECT_RATIO, PL_NEAR_PLANE, PL_FAR_PLANE ) );
    // queue plan for drawing
    plRenderer::queue( _plan );
    // queue global coordinate axis
    plRenderer::queueAxis( PL_PLAN_TECHNIQUE, plVector3( 0, 0, 0 ), plVector3( 1, 0, 0 ), plVector3( 0, 1, 0 ) );
    // queue editors
    plRenderer::queue( _graftEditor );
    plRenderer::queue( _boundaryEditor );
    plRenderer::queue( _modelEditor );  
    // dispatch draw call
    plRenderer::draw();
    // swap buffers
    glutSwapBuffers();
}


void PlannerWindow::setCursor( int mx, int my )
{
    switch ( _button )
    {   
        case GLUT_MIDDLE_BUTTON:        glutSetCursor( GLUT_CURSOR_CROSSHAIR );     break;      // middle button:   zoom
        case GLUT_LEFT_BUTTON:          glutSetCursor( GLUT_CURSOR_INFO );          break;      // left button:     click / drag 
        case GLUT_RIGHT_BUTTON:  
           
            // right button:    translate / rotate 
            switch( _cameraMode )
            { 
                case CAMERA_TRANSLATION_MODE:   glutSetCursor( GLUT_CURSOR_CROSSHAIR );     break;    // translation cursor  
                case CAMERA_ROTATION_MODE:      glutSetCursor( GLUT_CURSOR_CYCLE );         break;    // rotation cursor
            }
            break;
            
        case GLUT_NO_BUTTON:            glutSetCursor( GLUT_CURSOR_RIGHT_ARROW );   break;      // no button:     idle cursor   
    }
}


void PlannerWindow::keyAction( unsigned char key, int mx, int my )
{
    static int currentView = 0;

    switch (key) 
    {
        case 27:    // esc 
         
             exit(0);   
                                        
        case ' ':  // spacebar 
        
            _cameraMode =  (_cameraMode == CAMERA_ROTATION_MODE) ? CAMERA_TRANSLATION_MODE : CAMERA_ROTATION_MODE;  
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
       
        case 'a':   _plan.toggleArthroView();                                   break;                  
        case 'b':   
        
            /*
            if ( _modelEditor.isModelSelected() )
            {
                _plan.models( _modelEditor.selectedModelID() ).toggleVisibility();    
            }                     
            break; 
            */         
        case 'c':  
         
            _camera.up     = plVector3( 0, 1, 0 );  
            _camera.lookat = _camera.position + plVector3( 0, 0, 1 ); 
            break;
            
        case 'd':   /* UN-USED */ break;
        case 'e':   _plan.iGuides(0).exportIGuideModels("output/");             break;
        case 'f':   /* UN-USED */ break;
        case 'g':   /* UN-USED */ break;                 
        case 'h':   /* UN-USED */ break;
        case 'i':   _plan.iGuides(0).toggleVisibility();                        break;
        case 'j':   /* UN-USED */ break;
        case 'k':   /* UN-USED */ break;
        case 'l':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_LENGTH );      break; 
        case 'm':   _plan.updateGraftMarkerPositions();                         break;
        case 'n':   /* UN-USED */ break;
        case 'o': 
          
            if ( _modelEditor.isModelSelected() )
            {
                _plan.models( _modelEditor.selectedModelID() ).toggleOctreeVisibility();  
            }
            break;   
                 
        case 'p':   _plan.toggleVisibility();                                   break;    
        case 'q':   /* UN-USED */ break;
        case 'r':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_ROTATE );      break;     
        case 's':   /* UN-USED */ break;
        case 't':   _graftEditor.setEditMode( PL_GRAFT_EDIT_MODE_TRANSLATE );   break; 
        case 'u':   /* UN-USED */ break;
        case 'v': 
          
            _graftEditor.toggleSelectedVisibility(); 
            _boundaryEditor.toggleSelectedVisibility();                 
            _modelEditor.toggleSelectedVisibility();                
            break;
            
        case 'w':   /* UN-USED */ break;
        case 'x':   /* UN-USED */ break;
        case 'y':   /* UN-USED */ break;
        case 'z':   
        
            if ( _modelEditor.isModelSelected() )
            {
                _camera.reset( _plan.models( _modelEditor.selectedModelID() ).getCentroid() );
            }                         
            break;          
                           
        case 'A':   /* UN-USED */ break;
        case 'B':   /* UN-USED */ break;
        case 'C':   /* UN-USED */ break;    
        case 'D':   _plan.addDonorSite( _modelEditor.selectedModelID() );       break;
        case 'E':   /* UN-USED */ break;
        case 'F':   /* UN-USED */ break;
        case 'G':   _plan.iGuides(0).generateIGuideModels();                    break;
        case 'H':   /* UN-USED */ break;
        case 'I':   _plan.addIGuideSite( _modelEditor.selectedModelID() );      break;
        case 'J':   /* UN-USED */ break;
        case 'K':   /* UN-USED */ break;
        case 'L':   /* UN-USED */ break;
        case 'M':   /* UN-USED */ break;
        case 'N':   
        
            _plan.clear();  
            _graftEditor.clearSelection();   
            _boundaryEditor.clearSelection();  
            _modelEditor.clearSelection();                                        
            break;  
            
        case 'O':   _plan.exportFile("plan");                                   break;
        case 'P':   plAutomaticPlanner::calculate( _plan, 0 );                  break;
        case 'Q':   /* UN-USED */ break;
        case 'R':  
         
            _camera.importViewParams( ".view" + std::to_string( currentView ) );
            break;
            
        case 'S':   _plan.addDefectSite( _modelEditor.selectedModelID() );      break;
        case 'T':   /* UN-USED */ break;
        case 'U':   /* UN-USED */ break;
        case 'V':   /* UN-USED */ break;
        case 'W':  
         
            _camera.exportViewParams( ".view" + std::to_string( currentView ) );   
            break;
            
        case 'X':   /* UN-USED */ break;
        case 'Y':   /* UN-USED */ break;
        case 'Z':   /* UN-USED */ break;

        case 127:	 // delete 
        {   
            if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) 
	        {
                // delete boundary 
	            _boundaryEditor.clearSelectedBoundary();
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


void PlannerWindow::passiveMouseMotion( int mx, int my )
{   
    // do nothing   
}


void PlannerWindow::activeMouseMotion( int mx, int my )
{   
    // convert from GLUT window coords to OpenGL coords
    PLint x = mx;
    PLint y = glutGet( GLUT_WINDOW_HEIGHT ) - my;

    switch ( _button )
    {
        case GLUT_LEFT_BUTTON: 

            // process drag movements 
            _graftEditor.processMouseDrag   ( x, y );   
            _boundaryEditor.processMouseDrag( x, y );
            _modelEditor.processMouseDrag( x, y );         
            break;       

        case GLUT_MIDDLE_BUTTON:    
            
            // zoom camera
            _camera.zoom( y - _previousMouse.y );                            
            break;

        case GLUT_RIGHT_BUTTON:
        
            // previous and current mouse coords will always be very small, as 
            // _previousMouse has already been updated in mouseAction() by this point
            if ( _cameraMode == CAMERA_ROTATION_MODE )
            {
                _camera.rotate( _previousMouse.x, _previousMouse.y, x, y );
            }
            else
		    {
			    _camera.translate( _previousMouse.x - x, _previousMouse.y - y );
            }
            break;  
    }

    // update mouse position on drag
    _previousMouse.x = x;
    _previousMouse.y = y;

    glutPostRedisplay();
}



// Record button state when mouse button is pressed or released
void PlannerWindow::mouseAction( int button, int state, int mx, int my )
{
    // NOTE: this function ALWAYS executes BEFORE activeMouseMotion()
    
    // convert from GLUT window coords to OpenGL coords
    PLint x = mx;
    PLint y = glutGet( GLUT_WINDOW_HEIGHT ) - my;
        
    switch ( state )
    {
        case GLUT_UP:           
        {
            _graftEditor.processMouseRelease   ( x, y ); 
            _boundaryEditor.processMouseRelease( x, y ); 
            _modelEditor.processMouseRelease( x, y );
            _button = GLUT_NO_BUTTON;             
            break; 
        }
        case GLUT_DOWN:  _button = button;  break;   // button press
    }
    
    switch ( _button )
    {   
        case GLUT_LEFT_BUTTON:  

            if ( glutGetModifiers() == GLUT_ACTIVE_CTRL ) 
	        {
                // add new point
                _boundaryEditor.addPoint( x, y ); 
            }
            else
            {
                // process mouse clicks 
                _graftEditor.processMouseClick   ( x, y );    
                _boundaryEditor.processMouseClick( x, y ); 
                _modelEditor.processMouseClick( x, y );                 
            }
            break;
    }    
     
    // update mouse position on key press
    _previousMouse.x = x;
    _previousMouse.y = y;   
    
    glutPostRedisplay();
}


