#include "plBoneAndCartilage.h"


plBoneAndCartilage::plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combinedFile)
    :   bone( boneFile ), cartilage( cartilageFile ), combined( combinedFile )
{
}

/*
void plBoneAndCartilage::draw() const
{
    // DRAW BONE
    plPickingStack::loadRed( PL_PICKING_TYPE_BONE );
    bone.draw( plVector3( PL_MODEL_BONE_COLOUR ) );

    // DRAW CARTILAGE
    plPickingStack::loadRed( PL_PICKING_TYPE_CARTILAGE );
    cartilage.draw( plVector3( PL_MODEL_CARTILAGE_COLOUR ) );  
}
*/

plVector3 plBoneAndCartilage::getCentroid() const
{
    plVector3 min, max;
    
    combined.mesh().getMinMax( min, max );

    return 0.5f * (max + min);    
}   



