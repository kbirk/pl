#include "plCommon.h"
#include "plBoundaryEditor.h"
#include "plCommon.h"
#include "plCamera.h"
#include "plGraftEditor.h"
#include "plModelEditor.h"
#include "plPlan.h"
#include "plAutomaticPlanner.h"
#include "plVector3.h"
#include "plWindow.h"

#define CAMERA_ROTATION_MODE    1
#define CAMERA_TRANSLATION_MODE 2

// planner
std::shared_ptr<plPlan> plan;

// editing
plGraftEditor graftEditor;
plBoundaryEditor boundaryEditor;
plModelEditor modelEditor;

// camera
uint32_t cameraMode = CAMERA_ROTATION_MODE; // rotation or translation mode
plCamera camera;

// interaction
plVector3 previousMouse; // last mouse position
int32_t button = -1; // which button is currently down
bool ctrl = false; // whether ctrl is currently down

void init(int32_t argc, char** argv)
{
    plan = std::make_shared<plPlan>(argc, argv);

    camera = plCamera("./resources/view0");
    cameraMode = CAMERA_ROTATION_MODE;

    // initialize openGL specific objects
    plOpenGLInfo::init();
    plRenderer::init();
    plRenderResources::init();

    graftEditor.attach(plan);
    boundaryEditor.attach(plan);
    modelEditor.attach(plan);
}

void render()
{
    // set camera
    plCameraStack::load(camera);
    // set perspective projection
    plProjectionStack::load(plProjection(PL_FIELD_OF_VIEW , PL_ASPECT_RATIO, PL_NEAR_PLANE, PL_FAR_PLANE));
    // queue plan for drawing
    plRenderer::queue(*plan);
    // queue global coordinate axis
    plRenderer::queueAxis(
        PL_PLAN_TECHNIQUE,
        plVector3(0, 0, 0),
        plVector3(1, 0, 0),
        plVector3(0, 1, 0));
    // queue editors
    plRenderer::queue(graftEditor);
    plRenderer::queue(boundaryEditor);
    plRenderer::queue(modelEditor);
    // dispatch draw call
    plRenderer::draw();
}

// void setCursor(int32_t mx, int32_t my)
// {
//     switch (button)
//     {
//         case GLUT_MIDDLE_BUTTON:
//             // middle button:   zoom
//             glutSetCursor(GLUT_CURSOR_CROSSHAIR);
//             break;
//
//         case GLUT_LEFT_BUTTON:
//             // left button:     click / drag
//             glutSetCursor(GLUT_CURSOR_INFO);
//             break;
//
//         case GLUT_RIGHT_BUTTON:
//
//             // right button:    translate / rotate
//             switch(_cameraMode)
//             {
//                 case CAMERA_TRANSLATION_MODE:
//                     // translation cursor
//                     glutSetCursor(GLUT_CURSOR_CROSSHAIR);
//                     break;
//                 case CAMERA_ROTATION_MODE:
//                     // rotation cursor
//                     glutSetCursor(GLUT_CURSOR_CYCLE);
//                     break;
//             }
//             break;
//
//         default:
//             // no button:     idle cursor
//             glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
//             break;
//     }
// }

void handleKeyRelease(const WindowEvent& event)
{
    auto key = event.originalEvent->key.keysym.sym;

    std::cout << "key release " << uint32_t(key) << std::endl;

    switch (key)
    {
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            ctrl = false;
            break;
    }
}

void handleKeyPress(const WindowEvent& event)
{
    static int32_t currentView = 0;

    auto key = event.originalEvent->key.keysym.sym;

    std::cout << "key press " << uint32_t(key) << std::endl;

    switch (key)
    {
        case SDLK_ESCAPE:
            // esc
            exit(0);

        case SDLK_SPACE:
            // spacebar
            cameraMode = (cameraMode == CAMERA_ROTATION_MODE) ? CAMERA_TRANSLATION_MODE : CAMERA_ROTATION_MODE;
            break;

        case SDLK_LCTRL:
        case SDLK_RCTRL:
            // ctrl
            ctrl = true;

        case SDLK_DELETE:
            // delete
            if (ctrl)
            {
                // delete boundary
                boundaryEditor.removeSelectedSite();
            }
            else
            {
                // delete point
                boundaryEditor.removeSelectedPoint();
            }
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

            currentView = (int32_t)(key - '0');
            break;

        case 'a':   /* UN-USED */ break;
        case 'b':   /* UN-USED */ break;
        case 'c':

            camera.up = plVector3(0, 1, 0);
            camera.lookat = camera.position + plVector3(0, 0, 1);
            break;

        case 'd':   /* UN-USED */ break;
        case 'e':   /* UN-USED */ break;
        case 'f':   /* UN-USED */ break;
        case 'g':   /* UN-USED */ break;
        case 'h':   /* UN-USED */ break;
        case 'i':   /* UN-USED */ break;
        case 'j':   /* UN-USED */ break;
        case 'k':   /* UN-USED */ break;
        case 'l':

            graftEditor.setEditMode(PL_GRAFT_EDIT_MODE_LENGTH);
            break;

        case 'm':

            plan->updateGraftMarkerPositions();
            break;

        case 'n':   /* UN-USED */ break;
        case 'o':

            if (modelEditor.isModelSelected())
            {
                plan->models(modelEditor.selectedModelID()).toggleOctreeVisibility();
            }
            break;

        case 'p':
            plan->toggleVisibility();
            break;

        case 'q':   /* UN-USED */ break;
        case 'r':
            graftEditor.setEditMode(PL_GRAFT_EDIT_MODE_ROTATE);
            break;

        case 's':   /* UN-USED */ break;
        case 't':
            graftEditor.setEditMode(PL_GRAFT_EDIT_MODE_TRANSLATE);
            break;

        case 'u':   /* UN-USED */ break;
        case 'v':

            graftEditor.toggleSelectedVisibility();
            boundaryEditor.toggleSelectedVisibility();
            modelEditor.toggleSelectedVisibility();
            break;

        case 'w':   /* UN-USED */ break;
        case 'x':   /* UN-USED */ break;
        case 'y':   /* UN-USED */ break;
        case 'z':

            if (modelEditor.isModelSelected())
            {
                camera.reset(plan->models(modelEditor.selectedModelID()).getCentroid());
            }
            break;

        case 'A':   /* UN-USED */ break;
        case 'B':   /* UN-USED */ break;
        case 'C':   /* UN-USED */ break;
        case 'D':

            plan->addDonorSite(modelEditor.selectedModelID());
            break;

        case 'E':   /* UN-USED */ break;
        case 'F':   /* UN-USED */ break;
        case 'G':   /* UN-USED */ break;
        case 'I':   /* UN-USED */ break;
        case 'J':   /* UN-USED */ break;
        case 'K':   /* UN-USED */ break;
        case 'L':   /* UN-USED */ break;
        case 'M':   /* UN-USED */ break;
        case 'N':

            plan->clear();
            graftEditor.clearSelection();
            boundaryEditor.clearSelection();
            modelEditor.clearSelection();
            break;

        case 'O':

            plan->exportFile("plan");
            break;

        case 'P':

            plAutomaticPlanner::calculate(plan, 0);
            break;

        case 'Q':   /* UN-USED */ break;
        case 'R':

            camera.importViewParams("./resources/view" + std::to_string(currentView));
            break;

        case 'S':

            plan->addDefectSite(modelEditor.selectedModelID());
            break;

        case 'T':   /* UN-USED */ break;
        case 'U':   /* UN-USED */ break;
        case 'V':   /* UN-USED */ break;
        case 'W':

            camera.exportViewParams("./resources/view" + std::to_string(currentView));
            break;

        case 'X':   /* UN-USED */ break;
        case 'Y':   /* UN-USED */ break;
        case 'Z':   /* UN-USED */ break;

    }
}

void handleMouseMove(const WindowEvent& event)
{
    auto mouseEvent = event.originalEvent->motion;

    // convert from TOP-LEFT origin to BOTTOM-LEFT origin
    int32_t x = mouseEvent.x;
    int32_t y = plWindow::height() - mouseEvent.y;

    std::cout << "mouse move " << x << ", " << y << std::endl;

    switch (button)
    {
        case SDL_BUTTON_LEFT:

            // process drag movements
            graftEditor.processMouseDrag(x, y);
            boundaryEditor.processMouseDrag(x, y);
            modelEditor.processMouseDrag(x, y);
            break;

        case SDL_BUTTON_MIDDLE:

            // zoom camera
            camera.zoom(y - previousMouse.y);
            break;

        case SDL_BUTTON_RIGHT:

            // previous and current mouse coords should be very small
            if (cameraMode == CAMERA_ROTATION_MODE)
            {
                camera.rotate(previousMouse.x, previousMouse.y, x, y);
            }
            else
            {
                camera.translate(previousMouse.x - x, previousMouse.y - y);
            }
            break;
    }

    // update mouse position on drag
    previousMouse.x = x;
    previousMouse.y = y;
}


void handleMousePress(const WindowEvent& event)
{
    auto mouseEvent = event.originalEvent->button;

    // convert from TOP-LEFT origin to BOTTOM-LEFT origin
    int32_t x = mouseEvent.x;
    int32_t y = plWindow::height() - mouseEvent.y;

    std::cout << "mouse press " << uint32_t(mouseEvent.button) << std::endl;

    if (mouseEvent.button == SDL_BUTTON_LEFT) {
        if (ctrl)
        {
            // add new point
            boundaryEditor.addPoint(x, y);
        }
        else
        {
            // process mouse clicks
            graftEditor.processMousePress(x, y);
            boundaryEditor.processMousePress(x, y);
            modelEditor.processMousePress(x, y);
        }
    }

    // store which button is down
    button = mouseEvent.button;
}


void handleMouseRelease(const WindowEvent& event)
{
    auto mouseEvent = event.originalEvent->button;

    // convert from TOP-LEFT origin to BOTTOM-LEFT origin
    int32_t x = mouseEvent.x;
    int32_t y = plWindow::height() - mouseEvent.y;

    std::cout << "mouse release " << uint32_t(mouseEvent.button) << std::endl;

    // process mouse release
    graftEditor.processMouseRelease(x, y);
    boundaryEditor.processMouseRelease(x, y);
    modelEditor.processMouseRelease(x, y);

    // clear button state
    button = -1;
}

void handleClose(const WindowEvent& event)
{
    exit(0);
}

void handleResize(const WindowEvent& event)
{
    uint32_t width = event.originalEvent->window.data1;
    uint32_t height = event.originalEvent->window.data2;
    std::cout << width << ", " << height << std::endl;
    plWindow::reshape(width, height);
    plRenderResources::reshape(plWindow::viewportWidth(), plWindow::viewportHeight());
}

void handleSignal(int32_t signal)
{
    std::cout << "Caught signal: " << signal << ", shutting down..." << std::endl;
    exit(0);
}

int32_t main(int32_t argc, char **argv)
{
    // check command line argument count
    if (argc < 2)
    {
        std::cerr << "Required file inputs not provided, aborting" << std::endl;
        exit(1);
    }

	std::signal(SIGINT, handleSignal);
	std::signal(SIGQUIT, handleSignal);
	std::signal(SIGTERM, handleSignal);

    plWindow::setup();

    init(argc, argv);

    plWindow::on(WindowEventType::MOUSE_LEFT_PRESS, handleMousePress);
	plWindow::on(WindowEventType::MOUSE_LEFT_RELEASE, handleMouseRelease);
	plWindow::on(WindowEventType::MOUSE_MOVE, handleMouseMove);
	//plWindow::on(WindowEventType::MOUSE_WHEEL, handle_mouse_wheel);
	plWindow::on(WindowEventType::KEY_PRESS, handleKeyPress);
	plWindow::on(WindowEventType::KEY_RELEASE, handleKeyRelease);
	plWindow::on(WindowEventType::RESIZE, handleResize);
	plWindow::on(WindowEventType::CLOSE, handleClose);

    while (true) {
        // handle events
        plWindow::handleEvents();
        // render
        render();
        // set cursor
        // setCursor();
    	// swap back buffer
    	plWindow::swapBuffers();
    }

	plWindow::teardown();
}
