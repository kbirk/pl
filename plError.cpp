#include "plError.h"

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
