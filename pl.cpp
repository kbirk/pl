#include "pl.h"
#include "plCamera.h"
#include "plProjection.h"
#include "plMatrixStack.h"
#include "plPlan.h"
#include "plSeq.h"
#include "plModel.h"
#include "plDraw.h"
#include "plComputeShader.h"
#include "plMinimalShader.h"
#include "plPickingShader.h"
#include "plPickingTexture.h"
#include "plMesh.h"
#include "plGraftEditor.h"
#include "plBoundaryEditor.h"

plCamera					  *_plCamera;
plProjection			      *_plProjection;
plPlan                        *_plPlan;
plGraftEditor                 *_plGraftEditor;
plBoundaryEditor              *_plBoundaryEditor;

////////// encapsulate these in renderer class //////////////
plComputeShader               *_plComputeShader;
plMinimalShader               *_plMinimalShader;
plPickingShader               *_plPickingShader;

plPickingInfo                 *_plPickingState;
plPickingTexture              *_plPickingTexture;

plMatrixStack			      *_plModelMatrixStack;
/////////////////////////////////////////////////////////////

void plInit()
{
    _plComputeShader    = new plComputeShader("./shaders/test.comp");
    _plMinimalShader    = new plMinimalShader("./shaders/minimal.vert", "./shaders/minimal.frag");
    _plPickingShader    = new plPickingShader("./shaders/picking.vert", "./shaders/picking.frag");   
    _plPickingState     = new plPickingInfo();
    _plPickingTexture   = new plPickingTexture(1,1);
    _plModelMatrixStack = new plMatrixStack();
}
