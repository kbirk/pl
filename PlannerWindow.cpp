#include "PlannerWindow.h"

//int Light::nextLight = 0;

PlannerWindow::PlannerWindow( int x, int y, int width, int height, std::string title, int argc, char **argv )
    : ArcballWindow( x, y, width, height, title )
{  
    
    plInit();
    plCameraImportView(".view0");	
    plModelAdd( argv[1], argv[2] );

    // check if plan file provided
    if (argc > 3)
    {
        // load plan CSV file
        plPlanImport( argv[3] );
    }
    else
    {    
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
        case 'b':   plModelBoneToggleVisibility(0);         break;            
        case 'c':   plModelCartilageToggleVisibility(0);    break;      
        case 'p':   plPlanToggleVisibility();               break;    
        case 'z':   plCameraResetToModel(0);                break;          
        case 't':   plGraftSetTranslateMode();              break; 
        case 'r':   plGraftSetRotateMode();                 break;     
        case 'l':   plGraftSetLengthMode();                 break; 
        case 'g':   plGraftToggleVisibilityAll();           break;
        case 's':   plDefectSplineToggleVisibilityAll();    break; 
        case 'd':   plDonorRegionToggleVisibilityAll();     break;         
        case 'q':   plDefectSplineCornersToggleVisibilityAll(); break;
        case 'w':   plDefectSplineBoundaryToggleVisibilityAll(); break;
        
//       
//       /*
//        case '+':
//        case '=':
//            
//            if (m_state.graftEditMode == SPIN_MARK)
//            {
//                m_plan.grafts[m_state.selectedGraft].spinMark( +3 );
//            }
//            break;
//        
//        case '-':
//        case '_':
//        
//            if (m_state.graftEditMode == SPIN_MARK)
//            {
//                m_plan.grafts[m_state.selectedGraft].spinMark( -3 );
//            }
//            break;
//        */
        case 'O':			// output plan 0
        
            plPlanExport("exportPlan");            
            break;
        
//        case 'O':			// output plan 0
//        
//            m_plan.outputForManuela();
//            std::cout << "Saved plan in Manuela's format." << std::endl;
//            break;
//       
//       /* 
//        case 'n':			// new plug

//            {
//                vector sum(0,0,0);
//                for (int i=0; i<m_cartilageModel.nTriangles; i++)
//                {
//                    sum = sum + m_cartilageModel.triangles[i].centroid;
//                }
//                sum = (1.0/(float)m_cartilageModel.nTriangles) * sum;
//        
//                Graft g;
//        
//                g.height = 2;
//                g.radius = 4;
//                g.boneHeight = 18;
//        
//                g.recipientTransform.translation = sum;
//                g.recipientTransform.direction = vector(0,1,0);
//                g.recipientTransform.upDirection = vector(1,0,0);
//        
//                g.recipientTransform.translation = sum + vector(0,10,0);
//                g.recipientTransform.direction = vector(0,1,0);
//                g.recipientTransform.upDirection = vector(1,0,0);
//        
//                m_plan.grafts.add( g );
//            }
//            break;
//       */
//        case 'N':
//        
//            m_plan = Plan();
//            break;
//           
//        case 'H':   
//            
//            m_plan.donorRegions.push_back( Boundary() );
//            m_state.options.addDonorOption();
//            
//            break;
//          
//        case 'D':
//            
//            m_plan.defectSplines.push_back( Spline() );
//            m_state.options.addDefectOption();

//            break;
//          /*  
//        case 'G':

//            //m_plan.addIGuide();

//            break;
//       
        case 127:	 // delete 
           
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


bool PlannerWindow::userMouseMotion( int x, int y )
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



