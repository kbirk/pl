// PLANNER
#include "PlannerWindow.h"

int main( int argc, char **argv )
{   
    // check command line argument count
    if (argc < 2)
    {
        std::cerr << "Required file inputs not provided, aborting" << std::endl;
        abort();
    }

    srand ( time(NULL) );

    // Set up windows
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

    // create window
    PlannerWindow window( 0, 0, 1400, 1050, "Planner", argc, argv); 

    // Run OpenGL
    glutMainLoop();
}
