#ifndef __PL_DRAW_H__
#define __PL_DRAW_H__

#include "pl.h"
#include "plProjection.h"
#include "plModel.h"
#include "plPlan.h"
#include "plSeq.h"
#include "plMinimalShader.h"
#include "plPickingShader.h"
#include "plMesh.h"

void plDraw();
void plDrawSetViewingMatrix();
void plDrawSetProjectionMatrix();

void _plDrawPicking();
void _plDrawArrowCone(float scale);
void _plDrawArrowDisk(float scale);

#endif
