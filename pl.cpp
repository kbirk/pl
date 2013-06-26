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

///////////////////////////////////////////////////////

PLbool plErrorCheckGraftBounds(PLuint index, plString function_name)
{
    if (index < 0 && index > _plPlan->_grafts.size())
    {
		std::cerr << function_name << " error: invalid graft id" << std::endl;
		return true;  
	}
	return false;
} 

   
PLbool plErrorCheckModelBounds(PLuint index, plString function_name)
{
    if (index < 0 && index > _plBoneAndCartilageModels.size())
    {
		std::cerr << function_name << " error: invalid model id" << std::endl;
		return true;  
	}
	return false;
}

PLbool plErrorIsGraftSelected( plString function_name )
{
    if (_plState->graftSelectedID < 0)
    {
        std::cerr << function_name << " error: no graft is selected" << std::endl;
		return true;  
    }
    return false;
}

PLbool plErrorIsBoundarySelected( plString function_name )
{
    if (_plState->boundarySelectedID < 0)
    {
        std::cerr << function_name << " error: no boundary is selected" << std::endl;
		return true;  
    }
    return false;
}

PLbool plErrorIsBoundaryPointSelected( plString function_name )
{
    if (plErrorIsBoundarySelected(function_name))
        return true;

    if (_plState->boundarySelectedPointID < 0)
    {
        std::cerr << function_name << " error: no boundary point is selected" << std::endl;
		return true;  
    }
    return false;
}

PLbool plErrorIsDefectSplineSelected( plString function_name )
{
    if (_plState->boundarySelectedType == 0)
    {
        std::cerr << function_name << " error: no defect spline is selected" << std::endl;
		return true;  
    }
    return false;

}

PLbool plErrorCheckDonorRegionBounds(PLuint index, plString function_name)
{
    if (index < 0 && index > _plPlan->_donorRegions.size())
    {
		std::cerr << function_name << " error: invalid donor region id" << std::endl;
		return true;  
	}
	return false;
} 

PLbool plErrorCheckDefectSplineBounds(PLuint index, plString function_name)
{
    if (index < 0 && index > _plPlan->_defectSplines.size())
    {
		std::cerr << function_name << " error: invalid defect spline id" << std::endl;
		return true;  
	}
	return false;
} 

