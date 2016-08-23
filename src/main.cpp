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
#include "plOpenGLInfo.h"

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
std::shared_ptr<plCamera> camera;

// interaction
plVector3 previousMouse; // last mouse position
int32_t button = -1; // which button is currently down
bool ctrl = false; // whether ctrl is currently down
bool shift = false; // whether shift is currently down

void init(int32_t argc, char** argv)
{
    // initialize the plan
    plan = std::make_shared<plPlan>(argc, argv);

    // intialize the camera
    camera = std::make_shared<plCamera>("./resources/view0");

    // initialize openGL specific objects
    plOpenGLInfo::init();
    plRenderer::init();
    plRenderResources::init();

    // set initial size for buffers
    plRenderResources::reshape(
        plWindow::viewportWidth(),
        plWindow::viewportHeight());

    // attach plan to editors
    graftEditor.attach(plan);
    boundaryEditor.attach(plan);
    modelEditor.attach(plan);
}

void render()
{
    // set camera
    plCameraStack::load(*camera);
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
    // DEBUG
    plOpenGLInfo::reportError();
}

void handleKeyRelease(const WindowEvent& event)
{
    auto key = event.originalEvent->key.keysym.sym;

    // unmodifiable keys
    switch (key)
    {
        case SDLK_LCTRL:
        case SDLK_RCTRL:
            // ctrl
            ctrl = false;
            break;

        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            // shift
            shift = false;
            break;
    }
}

void handleKeyPress(const WindowEvent& event)
{
    static int32_t currentView = 0;

    auto key = event.originalEvent->key.keysym.sym;

    // unmodifiable keys
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
            break;

        case SDLK_LSHIFT:
        case SDLK_RSHIFT:
            // shift
            shift = true;
            break;

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

        case SDLK_0:
        case SDLK_1:
        case SDLK_2:
        case SDLK_3:
        case SDLK_4:
        case SDLK_5:
        case SDLK_6:
        case SDLK_7:
        case SDLK_8:
        case SDLK_9:
            // 0-9
            currentView = (int32_t)(key - '0');
            break;
    }

    if (shift)
    {
        // uppercase keys
        switch (key)
        {
            case SDLK_a: /* UN-USED */ break;
            case SDLK_b: /* UN-USED */ break;
            case SDLK_c: /* UN-USED */ break;
            case SDLK_d:

                // add a donor site
                plan->addDonorSite(modelEditor.selectedModelID());
                break;

            case SDLK_e: /* UN-USED */ break;
            case SDLK_f: /* UN-USED */ break;
            case SDLK_g: /* UN-USED */ break;
            case SDLK_h: /* UN-USED */ break;
            case SDLK_i: /* UN-USED */ break;
            case SDLK_j: /* UN-USED */ break;
            case SDLK_k: /* UN-USED */ break;
            case SDLK_l: /* UN-USED */ break;
            case SDLK_m: /* UN-USED */ break;
            case SDLK_n:

                // clear plan, start new
                plan->clear();
                graftEditor.clearSelection();
                boundaryEditor.clearSelection();
                modelEditor.clearSelection();
                break;

            case SDLK_o:

                // export plan file
                plan->exportFile("plan");
                break;

            case SDLK_p:

                // calculate plan
                plAutomaticPlanner::calculate(plan, 0);
                break;

            case SDLK_q: /* UN-USED */ break;
            case SDLK_r:

                // import view
                camera->importViewParams("./resources/view" + std::to_string(currentView));
                break;

            case SDLK_s:

                // add defect site
                plan->addDefectSite(modelEditor.selectedModelID());
                break;

            case SDLK_t: /* UN-USED */ break;
            case SDLK_u: /* UN-USED */ break;
            case SDLK_v: /* UN-USED */ break;
            case SDLK_w:

                // export view
                camera->exportViewParams("./resources/view" + std::to_string(currentView));
                break;

            case SDLK_x: /* UN-USED */ break;
            case SDLK_y: /* UN-USED */ break;
            case SDLK_z: /* UN-USED */ break;
        }

    }
    else
    {
        // lowercase keys
        switch (key)
        {
            case SDLK_a: /* UN-USED */ break;
            case SDLK_b: /* UN-USED */ break;
            case SDLK_c: /* UN-USED */ break;
            case SDLK_d: /* UN-USED */ break;
            case SDLK_e: /* UN-USED */ break;
            case SDLK_f: /* UN-USED */ break;
            case SDLK_g: /* UN-USED */ break;
            case SDLK_h: /* UN-USED */ break;
            case SDLK_i: /* UN-USED */ break;
            case SDLK_j: /* UN-USED */ break;
            case SDLK_k: /* UN-USED */ break;
            case SDLK_l:

                // set graft edit mode to length
                graftEditor.setEditMode(PL_GRAFT_EDIT_MODE_LENGTH);
                break;

            case SDLK_m:

                // re-align graft markers based on camera position
                plan->updateGraftMarkerPositions();
                break;

            case SDLK_n: /* UN-USED */ break;
            case SDLK_o:

                // toggle octree view for selected model
                if (modelEditor.isModelSelected())
                {
                    plan->models(modelEditor.selectedModelID())->toggleOctreeVisibility();
                }
                break;

            case SDLK_p:

                // toggle plan visibility
                plan->toggleVisibility();
                break;

            case SDLK_q: /* UN-USED */ break;
            case SDLK_r:

                // set graft edit mode to rotation
                graftEditor.setEditMode(PL_GRAFT_EDIT_MODE_ROTATE);
                break;

            case SDLK_s: /* UN-USED */ break;
            case SDLK_t:

                // set graft edit mode to translation
                graftEditor.setEditMode(PL_GRAFT_EDIT_MODE_TRANSLATE);
                break;

            case SDLK_u: /* UN-USED */ break;
            case SDLK_v:

                // toggle visibility of selected component (solid, transparent, invisible)
                graftEditor.toggleSelectedVisibility();
                boundaryEditor.toggleSelectedVisibility();
                modelEditor.toggleSelectedVisibility();
                break;

            case SDLK_w: /* UN-USED */ break;
            case SDLK_x: /* UN-USED */ break;
            case SDLK_y: /* UN-USED */ break;
            case SDLK_z:

                // reset camera onto model
                if (modelEditor.isModelSelected())
                {
                    camera->reset(plan->models(modelEditor.selectedModelID())->getCentroid());
                }
                break;
        }
    }
}

void handleMouseMove(const WindowEvent& event)
{
    auto mouseEvent = event.originalEvent->motion;

    // convert from TOP-LEFT origin to BOTTOM-LEFT origin
    int32_t x = mouseEvent.x * plWindow::pixelRatio();
    int32_t y = plWindow::height() - (mouseEvent.y * plWindow::pixelRatio());

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
            camera->zoom(y - previousMouse.y);
            break;

        case SDL_BUTTON_RIGHT:

            // previous and current mouse coords should be very small
            if (cameraMode == CAMERA_ROTATION_MODE)
            {
                camera->rotate(previousMouse.x, previousMouse.y, x, y);
            }
            else
            {
                camera->translate(previousMouse.x - x, previousMouse.y - y);
            }
            break;
    }

    // update mouse position on drag
    previousMouse.x = x;
    previousMouse.y = y;
}

void handleMouseWheel(const WindowEvent& event)
{
    const float32_t SCROLL_FACTOR = 5.0f;
    float32_t y = event.originalEvent->wheel.y;
    camera->zoom(y * SCROLL_FACTOR);
}

void handleMousePress(const WindowEvent& event)
{
    auto mouseEvent = event.originalEvent->button;

    // convert from TOP-LEFT origin to BOTTOM-LEFT origin
    int32_t x = mouseEvent.x * plWindow::pixelRatio();
    int32_t y = plWindow::height() - (mouseEvent.y * plWindow::pixelRatio());

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
    int32_t x = mouseEvent.x * plWindow::pixelRatio();
    int32_t y = plWindow::height() - (mouseEvent.y * plWindow::pixelRatio());

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
    plRenderResources::reshape(
        plWindow::viewportWidth(),
        plWindow::viewportHeight());
}

void handleSignal(int32_t signal)
{
    exit(0);
}

int32_t main(int32_t argc, char **argv)
{
    // check command line argument count
    if (argc < 2)
    {
        LOG_WARN("Required file inputs not provided, aborting");
        exit(1);
    }

	std::signal(SIGINT, handleSignal);
	std::signal(SIGQUIT, handleSignal);
	std::signal(SIGTERM, handleSignal);

    plWindow::setup();

    init(argc, argv);

    plWindow::on(WindowEventType::MOUSE_PRESS, handleMousePress);
	plWindow::on(WindowEventType::MOUSE_RELEASE, handleMouseRelease);
	plWindow::on(WindowEventType::MOUSE_MOVE, handleMouseMove);
	plWindow::on(WindowEventType::MOUSE_WHEEL, handleMouseWheel);
	plWindow::on(WindowEventType::KEY_PRESS, handleKeyPress);
	plWindow::on(WindowEventType::KEY_RELEASE, handleKeyRelease);
	plWindow::on(WindowEventType::RESIZE, handleResize);
	plWindow::on(WindowEventType::CLOSE, handleClose);

    while (true) {
        // handle events
        plWindow::handleEvents();
        // render
        render();
    	// swap back buffer
    	plWindow::swapBuffers();
    }

	plWindow::teardown();
}
