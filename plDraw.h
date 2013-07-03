#ifndef __PL_DRAW_H__
#define __PL_DRAW_H__

#include "pl.h"
#include "plProjection.h"
#include "plCamera.h"
#include "plModel.h"
#include "plPlan.h"
#include "plSeq.h"
#include "plMinimalShader.h"
#include "plPickingShader.h"
#include "plMesh.h"

/*
plMesh             *_plSphere;
plMesh             *_plCylinder;
plMesh             *_plDisk;

plState            *_plState;

plComputeShader    *_plTestComputeShader;
plMinimalShader    *_plMinimalShader;
plPickingShader    *_plPickingShader;
plPickingInfo      *_plPickingState;
plPickingTexture   *_plPickingTexture;

plCamera		   *_plCamera;
plProjection	   *_plProjection;
plMatrixStack	   *_plModelMatrixStack;

void plDrawInit()
{
    _plState            = new plState();

    _plComputeShader    = new plComputeShader("./shaders/test.comp");
    _plMinimalShader    = new plMinimalShader("./shaders/minimal.vert", "./shaders/minimal.frag");
    _plPickingShader    = new plPickingShader("./shaders/picking.vert", "./shaders/picking.frag");
    
    _plPickingState     = new plPickingInfo();
    _plPickingTexture   = new plPickingTexture(1,1);
    
    _plCamera           = new plCamera();
    _plProjection       = new plProjection(1.6f);
    _plModelMatrixStack = new plMatrixStack();

    _plSphere           = new plMesh(1, 20, 20);
    _plCylinder         = new plMesh(1.0f, 1.0f, 1.0f, 30, 1);    
    _plDisk             = new plMesh(0.0f, 1.0f, 20, 20); 
}


void plDrawBoneAndCartilage( const plBoneAndCartilage &model )
{
    // DRAW BONE
    if (model.isBoneTransparent())
    {
        glEnable( GL_STENCIL_TEST );                // if transparent, prevent overwriting plan pixels during picking
        glColor4f(PL_MODEL_BONE_COLOUR, 0.2);
    }
    else
    {
        glDisable( GL_STENCIL_TEST );
        glColor4f(PL_MODEL_BONE_COLOUR, 1.0);    
    }    
    _plPickingState->type = PL_PICKING_TYPE_BONE;
    _plPickingShader->setPickingUniforms(_plPickingState->type,_plPickingState->id, 0);    
    model.drawBone();
    
    // DRAW CARTILAGE
    if (model.isTransparent())
    {
        glEnable( GL_STENCIL_TEST );                // if transparent, prevent overwriting plan pixels during picking
        glColor4f(PL_MODEL_CARTILAGE_COLOUR, 0.2);
    }    
    else
    {
        glDisable( GL_STENCIL_TEST );
        glColor4f(PL_MODEL_CARTILAGE_COLOUR, 1.0);  
    }    
    _plPickingState->type = PL_PICKING_TYPE_CARTILAGE;
    _plPickingShader->setPickingUniforms(_plPickingState->type,_plPickingState->id, 0);    
    model.drawCartilage();
}


void _plSetBoundaryColour()
{
    if (PL_BOUNDARY_NONE_SELECTED || PL_BOUNDARY_CURRENT_IS_SELECTED)
    {
        switch (_plPickingState->type)
        {
            case PL_PICKING_TYPE_DEFECT_CORNERS:
                // defect corners 
                glColor3f( PL_BOUNDARY_DEFECT_CORNER_COLOUR ); 
                break;

            case PL_PICKING_TYPE_DEFECT_BOUNDARY:
                // defect boundary
                glColor3f( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR ); 
                break;
 
            case PL_PICKING_TYPE_DONOR_BOUNDARY:
                // donor boundary
                glColor3f( PL_BOUNDARY_DONOR_COLOUR );
                break;

            case PL_PICKING_TYPE_IGUIDE_BOUNDARY:     
                // iguide boundary
                glColor3f( PL_BOUNDARY_IGUIDE_COLOUR );   
                break;
        }
    }
    else
    {
        switch (_plPickingState->type)
        {
            case PL_PICKING_TYPE_DEFECT_CORNERS:
                // defect corners 
                glColor3f( PL_BOUNDARY_DEFECT_CORNER_COLOUR_DULL ); 
                break;

            case PL_PICKING_TYPE_DEFECT_BOUNDARY:
                // defect boundary
                glColor3f( PL_BOUNDARY_DEFECT_BOUNDARY_COLOUR_DULL ); 
                break;
 
            case PL_PICKING_TYPE_DONOR_BOUNDARY:
                // donor boundary
                glColor3f( PL_BOUNDARY_DONOR_COLOUR_DULL );
                break;

            case PL_PICKING_TYPE_IGUIDE_BOUNDARY:     
                // iguide boundary
                glColor3f( PL_BOUNDARY_IGUIDE_COLOUR_DULL );   
                break;
        }
    }
}


void plDrawBoundary( const plBoundary &boundary) 
{   
    if (!boundary.isVisible())
        return;
    
    // DRAW WALLS    
    _plSetBoundaryColour();         // set colour
    // set picking values
    _plPickingState->index = -1;  // draw walls with index of -1
    _plPickingShader->setPickingUniforms(_plPickingState);   
    boundary.drawWalls();       // draw wall
    
    // DRAW POINTS    
    for (PLuint i=0; i<boundary.points.size(); i++) 
    {
        _plPickingState->index = i; 
        _plPickingShader->setPickingUniforms(_plPickingState);
        
        if (PL_BOUNDARY_POINT_CURRENT_IS_SELECTED)
        {
            plDrawSphere( points[i], 1.5) ;
        }
        else
        {
            plDrawSphere( points[i], 1 );
        }
    } 
}


void _plSetGraftBoneColour()
{
    if (PL_GRAFT_NONE_SELECTED || PL_GRAFT_CURRENT_IS_SELECTED)
    {
        // no grafts are selected
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_BONE_COLOUR );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_BONE_COLOUR );
        }
    }
    else
    {
        // a graft is selected and it is not this one, dull colour
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_BONE_COLOUR_DULL );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_BONE_COLOUR_DULL );
        }       
    }
       
}

void _plSetGraftCartilageColour()
{
    if (PL_GRAFT_NONE_SELECTED || PL_GRAFT_CURRENT_IS_SELECTED)
    {
        // no grafts are selected
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_CARTILAGE_COLOUR );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_CARTILAGE_COLOUR );
        }
    }
    else
    {
        // a graft is selected and it is not this one, dull colour
        if (PL_GRAFT_IS_DEFECT)
        {
            glColor3f( PL_GRAFT_DEFECT_CARTILAGE_COLOUR_DULL );
        }
        else
        {
            glColor3f( PL_GRAFT_DONOR_CARTILAGE_COLOUR_DULL );
        }       
    } 
}

void plDrawGraft( const plGraft &graft ) const
{
    if (!graft.isVisible())
        return;

    // Draw at harvest location
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        graft.harvestTransform.apply();
        _plPickingState->index = PL_PICKING_INDEX_GRAFT_DONOR;
        _plPickingShader->setPickingUniforms(_plPickingState);
        plDrawIndividualGraft();

        if (PL_GRAFT_CURRENT_IS_SELECTED && PL_GRAFT_SELECTED_IS_DONOR) 
        {
            plDrawGraftInterface();
            _plPickingState->type = PL_PICKING_TYPE_GRAFT;   // reset after handle is drawn
        }
    }
    glPopMatrix();

    // Draw at recipient location
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    {
        recipientTransform.apply();
        glTranslatef( 0, heightOffset, 0 );
        _plPickingState->index = PL_PICKING_INDEX_GRAFT_DEFECT;
        _plPickingShader->setPickingUniforms(_plPickingState);
        drawGraft();

        if (PL_GRAFT_CURRENT_IS_SELECTED && PL_GRAFT_SELECTED_IS_DEFECT) 
        {
            plDrawGraftInterface();
        }
    }
    glPopMatrix();
}


void _plDrawIndividualGraft( const plGraft &graft ) const
{
    // draw cartilage cap
    _plSetGraftCartilageColour();   
    graft.drawCartilageCap();

    // draw bone cap
    _plSetGraftBoneColour();
    graft.drawBoneCap();

    // draw marker
    glColor3f( PL_GRAFT_MARKER_COLOUR );
    plDrawSphere( graft.markPosition, 0.5 );

}


       
void plDrawGraftInterface()
{
    if (!PL_GRAFT_HANDLES_ENABLED)
        return;

    // draw axes and handles
    glMatrixMode( GL_MODELVIEW );
    
    glPushMatrix();
    glTranslatef( 0, PL_GRAFT_HANDLE_OFFSET, 0 );

    switch(_plState->graftEditMode)
    {
        case PL_GRAFT_EDIT_MODE_TRANSLATE:   
        case PL_GRAFT_EDIT_MODE_ROTATE:
        {
            glColor3f( 0.2, 0.2, 0.2 ); 
            plDrawSphere( 1.5 );

            // x axis
            glColor3f( 1.0, 0.2, 0.2 ); 
            
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_X;
            _plPickingShader->setPickingUniforms(_plPickingState);
        
            (_plState->graftEditMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(1,0,0) ) : plDrawCircleArrow( plVector3(1,0,0) );

            if (PL_GRAFT_EDIT_SHOW_Y_HANDLE)    
            {
                // y axis
                glColor3f( 0.2, 1.0, 0.2 );  

                _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
                _plPickingShader->setPickingUniforms(_plPickingState);

                (_plState->graftEditMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(0,1,0) ) : plDrawCircleArrow( plVector3(0,1,0) );
            }

            // z axis
            glColor3f( 0.2, 0.2, 1.0 ); 
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Z;
            _plPickingShader->setPickingUniforms(_plPickingState);
        
            (_plState->graftEditMode == PL_GRAFT_EDIT_MODE_TRANSLATE) ? plDrawArrow( plVector3(0,0,1) ) : plDrawCircleArrow( plVector3(0,0,1) );
            
            break;
        }
        case PL_GRAFT_EDIT_MODE_LENGTH:
        {     
            glColor3f( 0.2, 0.2, 0.2 ); 
            glPushMatrix();
                glTranslatef( 0, PL_HANDLE_LENGTH/2.0, 0 );
                plDrawSphere( 1.5 );
            glPopMatrix();
                    
            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
            _plPickingShader->setPickingUniforms(_plPickingState);
            glColor3f( 0.2, 1.0, 0.2 ); 
            plDrawArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);    
            glColor3f( 1.0, 0.2, 0.2 );         
            plDrawArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);

            break;     
        }
        
        case PL_GRAFT_EDIT_MODE_MARKER:
        {
            glColor3f( 0.2, 0.2, 0.2 ); 
            glPushMatrix();
                glTranslatef( 0, PL_HANDLE_LENGTH/2.0, 0 );
                plDrawSphere( 1.5 );
            glPopMatrix();

            _plPickingState->type = PL_PICKING_TYPE_GRAFT_HANDLE_Y;
            _plPickingShader->setPickingUniforms(_plPickingState);
            glColor3f( 0.2, 1.0, 0.2 ); 
            plDrawCircleArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH-1, 0), plVector3(0,1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);            
            plDrawCircleArrow( plVector3(0, PL_HANDLE_LENGTH-PL_ARROW_LENGTH, 0), plVector3(0,-1,0), PL_HANDLE_LENGTH-PL_ARROW_LENGTH);
           
            break;
        }
    }
    glPopMatrix();
}


void plDrawPlan()
{
    if (!_isVisible)
        return;
        
    // Draw defect boundary 
    for ( PLuint i = 0; i < _defectSplines.size(); i++)
    {
        _plPickingState->id = i; 
        _defectSplines[i].draw();
    }
   
    // Draw harvest boundaries   
    for ( PLuint i = 0; i < _donorRegions.size(); i++)
    {
        _plPickingState->type = PL_PICKING_TYPE_DONOR_BOUNDARY; 
        _plPickingState->id = i;        
        _donorRegions[i].draw();            
    }    

    // Draw grafts
    for ( PLuint i = 0; i < _grafts.size(); i++)
    {       
        _plPickingState->type = PL_PICKING_TYPE_GRAFT; 
        _plPickingState->id = i; 
        _grafts[i].draw();
    }
    
    
    // Draw iGuides
    if (selectedIGuide != -1)
    {
        iGuides[selectedIGuide].draw(plVector3(IGUIDE_BOUNDARY_COLOUR), ctrlPointColourPickingValue, state.selectedCtrlPoint );
        ctrlPointColourPickingValue+=MAX_CTRL_POINTS;
    }
    
}
*/


void plDraw(); 
void plDrawSetViewingMatrix();
void plDrawSetProjectionMatrix();

void _plDrawPicking();
void _plDrawArrowCone(float scale);

#endif
