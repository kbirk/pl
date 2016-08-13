#include "plBoneAndCartilage.h"


plBoneAndCartilage::plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combinedFile)
    :   bone( boneFile ), cartilage( cartilageFile ), combined( combinedFile )
{
}


void plBoneAndCartilage::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{

    // draw combined
    plPickingStack::loadRed( PL_PICKING_TYPE_CARTILAGE );
    plColourStack::load( PL_MODEL_BONE_COLOUR );
    combined.extractRenderComponents( renderMap, technique );

    /*
    // draw cartilage
    plPickingStack::loadRed( PL_PICKING_TYPE_CARTILAGE );
    plColourStack::load( PL_MODEL_CARTILAGE_COLOUR );
    cartilage.extractRenderComponents( renderMap, technique );

    // draw bone
    plPickingStack::loadRed( PL_PICKING_TYPE_BONE );
    plColourStack::load( PL_MODEL_BONE_COLOUR );
    bone.extractRenderComponents( renderMap, technique );
    */
}


void plBoneAndCartilage::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


void plBoneAndCartilage::toggleArthroView()
{
    plArthroViewable::toggleArthroView();
    bone.toggleArthroView();
    cartilage.toggleArthroView();
    combined.toggleArthroView();
}


plVector3 plBoneAndCartilage::getCentroid() const
{
    plVector3 min, max;

    combined.mesh().getMinMax( min, max );

    return 0.5f * (max + min);
}
