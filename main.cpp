// PLANNER
#include "pl.h"
#include "PlannerWindow.h"

int main( int argc, char **argv )
{   
    // check command line argument count
    if (argc < 2)
    {
        std::cerr << "Required file inputs not provided, aborting" << std::endl;
        abort();
    }

    

    // create window
    PlannerWindow window( 0, 0, 1400, 1050, "Planner", argc, argv); 

    // Run OpenGL
    glutMainLoop();
    
}
