#ifndef PL_DRAW_H
#define PL_DRAW_H

#include "plCommon.h"
#include "plVAO.h"

namespace plDraw
{

    void arrow    ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void arrow    ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void knob     ( const plVector3 &origin, const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void knob     ( const plVector3 &direction, PLfloat length = PL_HANDLE_LENGTH, PLfloat scale = 1.0f);
    void sphere   ( float radius);
    void sphere   ( const plVector3 &origin, float radius);
    void cylinder ( float radius, float height); 
    void cylinder ( const plVector3 &origin, const plVector3 &direction, float radius, float height);
    void disk     ( const plVector3 &origin, float scale, PLbool flip = false  );
    void disk     ( const plVector3 &origin, const plVector3 &direction, float scale, PLbool flip = false  );
    void disk     ( float scale, PLbool flip = false );
    void cube     ( float halfwidth );
    void cube     ( const plVector3 &origin, float halfwidth );
    void laserLine     ( const plVector3 &origin, const plVector3 &direction, float length);

    // shape generation functions
    plVAO generateCubeVAO     ( float halfWidth );
    plVAO generateCubeLineVAO ( float halfWidth );
    plVAO generateSphereVAO   ( float radius, int slices, int stacks );
    plVAO generateCylinderVAO ( float baseRadius, float topRadius, float height, int slices, int stacks );
    plVAO generateDiskVAO     ( float innerRadius, float outerRadius, int slices, int loops, bool up = true );

}


#endif
