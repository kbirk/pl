#include "ArcballWindow.h"

ArcballWindow::ArcballWindow( int x, int y, int width, int height, std::string title )
    : Window( x, y, width, height, title )
{
    _button = GLUT_NO_BUTTON;
	_cameraMode = CAMERA_ROTATION_MODE;
}


void ArcballWindow::display()
{
    draw();
    glutSwapBuffers();
}


void ArcballWindow::setCursor(int x, int y)
{
    #ifndef sgi   
    if (_button == GLUT_MIDDLE_BUTTON) 
    {
        // zoom cursor
        glutSetCursor( GLUT_CURSOR_CROSSHAIR );
    }
    else if (_button == GLUT_RIGHT_BUTTON) 
    {        
        if (_cameraMode == CAMERA_TRANSLATION_MODE)
        {
            // translation cursor
            glutSetCursor( GLUT_CURSOR_CROSSHAIR );
        }
        else
        {
            // rotation cursor
            glutSetCursor( GLUT_CURSOR_CYCLE );
        } 
    } 
    else
    {
        // idle or select cursor
        userSetCursor(x,y);
    }          
    #endif
}


void ArcballWindow::keyAction( unsigned char key, int x, int y )
{
    static int currentView = 0;
    char name[8];

    switch (key) 
    {
        case 27:    // esc
            exit(0);
            
        case ' ':   // spacebar
            _cameraMode =  (_cameraMode == CAMERA_ROTATION_MODE) ? CAMERA_TRANSLATION_MODE : CAMERA_ROTATION_MODE;
            break;
            
        case 'W':   // ctrl+w      
            sprintf( name, ".view%d", currentView );
			plCameraExportView( name );

            break;
            
        case 'R':   // ctrl+r 
            sprintf( name, ".view%d", currentView );
			plCameraImportView( name );

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
            currentView = (int) (key - '0');
            break;                
        /*
        case '.':
            if (m_projection == PERSPECTIVE) 
            {
                m_projection = ORTHOGRAPHIC;
                if (m_mode == ROTATE) 
                {
                    m_mode = TRANSLATE;
                    m_camera.setCursor();
                }
            }
            else
            {
                m_projection = PERSPECTIVE;
            }
            glutPostRedisplay();
            break;
        */
        default:
            userKeyAction(key,x,y);
    }
    
    glutPostRedisplay();
}


void ArcballWindow::passiveMouseMotion( int x, int y )
{   
    userPassiveMouseMotion(x,y);      
}


void ArcballWindow::mouseMotion( int x, int y )
{    
    if (userMouseMotion(x,y))
        return;

    // left mouse button
    if (_button == GLUT_LEFT_BUTTON)
    {
        
    }   
    // middle mouse button  
    else if (_button == GLUT_MIDDLE_BUTTON)
    {
        plCameraZoom(_previousMouse.y - y);  // zoom camera
    }
    // right mouse button
    else if (_button == GLUT_RIGHT_BUTTON)
    {
        if (_cameraMode == CAMERA_ROTATION_MODE)
        {
			plCameraArcballRotate(_previousMouse.x, _previousMouse.y, x, y);
		}	
		else
		{
			plCameraTranslate(_previousMouse.x - x,
			                  (glutGet(GLUT_WINDOW_HEIGHT) - _previousMouse.y) - (glutGet(GLUT_WINDOW_HEIGHT) - y) );
        }
    }

    // update mouse position on drag    
    _previousMouse.x = x;
    _previousMouse.y = y;

    glutPostRedisplay();
}



// Record button state when mouse button is pressed or released
void ArcballWindow::mouseAction( int button, int state, int x, int y )
{
    if (userMouseAction( button, state, x, y))
        return;
  
    // update mouse position on key press
    _previousMouse.x = x;
    _previousMouse.y = y;
  
    // button release
    if (state == GLUT_UP)   
    {       
        _button = GLUT_NO_BUTTON;
    } 
    // button press
    else if (state == GLUT_DOWN)    
    {
        //userMouseClick( button, x, y );       
        _button = button;
    }
    
}
