#ifndef __PL_ERROR_H__
#define __PL_ERROR_H__

#include "pl.h"
#include "plPlan.h"
#include "plSeq.h"
#include "plModel.h"
#include "plDraw.h"
#include "plState.h"

PLbool plErrorCheckGraftBounds     ( PLuint index, plString function_name );     
PLbool plErrorCheckModelBounds     ( PLuint index, plString function_name ); 
PLbool plErrorCheckDonorRegionBounds ( PLuint index, plString function_name );
PLbool plErrorCheckDefectSplineBounds( PLuint index, plString function_name );

PLbool plErrorIsGraftSelected        ( plString function_name );
PLbool plErrorIsBoundarySelected     ( plString function_name );
PLbool plErrorIsBoundaryPointSelected( PLuint index, plString function_name );
PLbool plErrorIsDefectSplineSelected ( PLuint index, plString function_name );

#endif
