#ifndef PLANNER_WINDOW_H
#define PLANNER_WINDOW_H

// PLANNER
#include "ArcballWindow.h"
#include "pl.h"

#define DRAGEE_NONE 		       -1
#define DRAGEE_GRAFT_HANDLE         1
#define DRAGEE_BOUNDARY_POINT 	    2
#define DRAGEE_SPLINE_HANDLE        3

class PlannerWindow : public ArcballWindow 
{
    
    public:

        PlannerWindow( int x, int y, int width, int height, std::string title, int argc, char **argv );

        void draw();
                
    private:
        
        PLint _currentDragee;
        
        void userKeyAction( PLbyte key, int x, int y );
        bool userMouseAction( int button, int state, int x, int y );
        bool userActiveMouseMotion( int x, int y );
        void userPassiveMouseMotion( int x, int y);
        void userSetCursor( int x, int y);
 
};


#endif
