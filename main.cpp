// PLANNER
#include "PlannerWindow.h"
#include "plMeshIntersector.h"

int main( int argc, char **argv )
{   
    plSeq<plTriangle> inputMesh1, inputMesh2;
    plSTL::importFile(inputMesh1,"testMeshes/hiressphere.stl");
    plSTL::importFile(inputMesh2,"testMeshes/sphere1.stl");
    //plSTL::importFile(inputMesh1,"output/badInput3/badInput1.stl");
    //plSTL::importFile(inputMesh2,"output/badInput3/badInput2.stl");

    plSeq<plTriangle> output;

    plMeshIntersector intersector;
    intersector.importTriSeq(inputMesh1,0,PL_LOGGER_LEVEL_WARNING,0);
    intersector.importTriSeq(inputMesh2,1,PL_LOGGER_LEVEL_WARNING,0);
    intersector.intersect(output,PL_LOGGER_LEVEL_DEBUG,0);
    plSTL::exportFileBinary(output,"output/output.stl");
    /*
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
    glutMainLoop();*/
    
}
