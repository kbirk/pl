#include "PlannerWindow.h"

//int Light::nextLight = 0;

PlannerWindow::PlannerWindow( int x, int y, int width, int height, std::string title, int argc, char **argv )
    : ArcballWindow( x, y, width, height, title )
{  
    
    plInit();
    plCameraImportView(".view0");	

    // check if plan file provided
    if (argc == 2)
    {
        // load plan CSV file
        plPlanImport( argv[1] );
        
    }
    else
    {
        // check model count
        if ( (argc-1) % 2 != 0)  
        {
            std::cerr << "Model files must be submitted in twos (bone and cartilage)\n";
        }
        
        // load models
        for (PLint i = 1; i < argc; i+=2)
        {
            // model formats submitted bone, cartilage, bone, cartilage, etc...
            plModelAdd( argv[i], argv[i+1] ); 
        }
        
        // create empty plan
        plPlanCreateNew();
    }
    
}

void PlannerWindow::draw()
{
    plDraw();
}


void PlannerWindow::userSetCursor(int x, int y)
{
    if (_currentDragee != DRAGEE_NONE) 
    {
        glutSetCursor(GLUT_CURSOR_INFO);  
    }
    //else if (m_state.options.isMouseOver(x,y))
    //{
    //    glutSetCursor(GLUT_CURSOR_INFO);  
    //}
    else
    {
        glutSetCursor( GLUT_CURSOR_RIGHT_ARROW );
    }
}

void PlannerWindow::userKeyAction( unsigned char key, int x, int y )
{

    static int planNum = 1;
//    static int currentView = 0;

    switch (key) 
    {   
        case 'b':   plModelBoneToggleVisibility(0);              break;            
        case 'c':   plModelCartilageToggleVisibility(0);         break;      
        case 'p':   plPlanToggleVisibility();                    break;    
        case 'z':   plCameraResetToModel(0);                     break;          
        case 't':   plGraftSetTranslateMode();                   break; 
        case 'r':   plGraftSetRotateMode();                      break;     
        case 'l':   plGraftSetLengthMode();                      break; 
        case 'm':   plGraftSetMarkerMode();                      break; 
        case 'g':   plGraftToggleVisibilityAll();                break;
        case 's':   plDefectSplineToggleVisibilityAll();         break; 
        case 'd':   plDonorRegionToggleVisibilityAll();          break;         
        case 'q':   plDefectSplineCornersToggleVisibilityAll();  break;
        case 'w':   plDefectSplineBoundaryToggleVisibilityAll(); break;        
        case 'O':   plPlanExport("exportPlan");                  break;

        case 127:	 // delete 
        {   
            if (glutGetModifiers() == GLUT_ACTIVE_CTRL) 
	        {
                // delete group 
	            
            }
            else
            {
                // delete point
                plBoundaryPointRemove();         	            
	        }
            break;
        }  
    }   
      
    glutPostRedisplay();
}


bool PlannerWindow::userMouseAction( int button, int state, int x, int y )
{
    if (state == GLUT_UP) 
    {
        // mouse button is released
        _currentDragee = DRAGEE_NONE;
        return false; 
    }
    
    if (button == GLUT_LEFT_BUTTON) 
    {
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
        {
            // add boundary point
            PLint new_index = plBoundaryPointAdd(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
            if (new_index > 0)  // may not always add a new point (ex. selecting over bone)
            {
                plBoundaryPointSelect(new_index);
            } 
            glutPostRedisplay();   
            return true;                               
        }
        
        PLint type = plPickingSelect(x, glutGet(GLUT_WINDOW_HEIGHT)-y);

        switch (type)
        {
            case PL_PICKING_TYPE_NONE:
            {
                glutPostRedisplay();
                return false; 
            }
            
            case PL_PICKING_TYPE_GRAFT_HANDLE_X:
            case PL_PICKING_TYPE_GRAFT_HANDLE_Y:
            case PL_PICKING_TYPE_GRAFT_HANDLE_Z:
            {
                plGraftSetDragOrigin(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
                _currentDragee = DRAGEE_GRAFT_HANDLE;
                glutPostRedisplay();
                return true;                                              
            }
            
            case PL_PICKING_TYPE_DEFECT_CORNERS:
            case PL_PICKING_TYPE_DEFECT_BOUNDARY:
            case PL_PICKING_TYPE_DONOR_BOUNDARY:
            case PL_PICKING_TYPE_IGUIDE_BOUNDARY:
            {
                _currentDragee = DRAGEE_BOUNDARY_POINT;
                glutPostRedisplay();
                return true; 
            } 
            
            case PL_PICKING_TYPE_DEFECT_HANDLE_0:
            case PL_PICKING_TYPE_DEFECT_HANDLE_1:
            case PL_PICKING_TYPE_DEFECT_HANDLE_2:
            case PL_PICKING_TYPE_DEFECT_HANDLE_3:
            case PL_PICKING_TYPE_DEFECT_HANDLE_C:
            {
                   
            }
            
            default:
            {
                glutPostRedisplay();
                return true; 
            }            
        }
    }

    return false; 
}

void PlannerWindow::userPassiveMouseMotion(int x, int y)
{ 
    userSetCursor(x,y);
}


bool PlannerWindow::userActiveMouseMotion( int x, int y )
{   

    switch (_currentDragee)
    {
        case DRAGEE_GRAFT_HANDLE:
        {        
            plGraftDragEdit(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
            glutPostRedisplay();    
            return true;
        }
        
        case DRAGEE_BOUNDARY_POINT:
        {
            plBoundaryPointMove(x, glutGet(GLUT_WINDOW_HEIGHT) - y);
            glutPostRedisplay();
            return true;
        }
    }

    return false;
}



