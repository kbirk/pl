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
#include "plState.h"
#include "plMesh.h"

// all this call be encapsulated in a drawing class/section //
plMesh                        *_plSphere;
plMesh                        *_plCylinder;
plMesh                        *_plDisk;

plState                       *_plState;

plComputeShader               *_plTestComputeShader;
plMinimalShader               *_plMinimalShader;
plPickingShader               *_plPickingShader;
plPickingInfo                 *_plPickingState;
plPickingTexture              *_plPickingTexture;

plCamera					  *_plCamera;
plProjection			      *_plProjection;
plMatrixStack			      *_plModelMatrixStack;
/////////////////////////////////////////////////////////////

plPlan                        *_plPlan;
plSeq<plBoneAndCartilage*> 	  _plBoneAndCartilageModels;

void plInit()
{
    _plState            = new plState();

    _plTestComputeShader = new plComputeShader("./shaders/test.comp");
    _plMinimalShader    = new plMinimalShader("./shaders/minimal.vert", "./shaders/minimal.frag");
    _plPickingShader    = new plPickingShader("./shaders/picking.vert", "./shaders/picking.frag");
    
    _plPickingState     = new plPickingInfo();
    _plPickingTexture   = new plPickingTexture(1,1);
    
    _plCamera           = new plCamera();
    _plProjection       = new plProjection(1.6f);
    _plModelMatrixStack = new plMatrixStack();
    _plPlan             = new plPlan();

    _plSphere           = new plMesh(1, 20, 20);
    _plCylinder         = new plMesh(1.0f, 1.0f, 1.0f, 30, 1);    
    _plDisk             = new plMesh(0.0f, 1.0f, 20, 20);   
}
