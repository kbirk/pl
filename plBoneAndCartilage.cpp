#include "plBoneAndCartilage.h"


plBoneAndCartilage::plBoneAndCartilage( const plString &boneFile, const plString &cartilageFile, const plString &combined)
    :   bone(boneFile, 5), cartilage(cartilageFile, 7), combined(combined, 5)
{      
}


void plBoneAndCartilage::draw() const
{
    // DRAW BONE
    plPicking::value.type = PL_PICKING_TYPE_BONE;   
    bone.draw( plVector3(PL_MODEL_BONE_COLOUR) );

    // DRAW CARTILAGE
    plPicking::value.type = PL_PICKING_TYPE_CARTILAGE; 
    cartilage.draw( plVector3(PL_MODEL_CARTILAGE_COLOUR) );
    
}


void plBoneAndCartilage::getMinMax(plVector3 &min, plVector3 &max) const
{
    plVector3 bmin, bmax, cmin, cmax;
    bone.getMinMax(bmin, bmax);
    cartilage.getMinMax(cmin, cmax);
    
    min.x = (bmin.x < cmin.x) ? bmin.x : cmin.x;
    min.y = (bmin.y < cmin.y) ? bmin.y : cmin.y;
    min.z = (bmin.z < cmin.z) ? bmin.z : cmin.z;
    
    max.x = (bmax.x > cmax.x) ? bmax.x : cmax.x;
    max.y = (bmax.y > cmax.y) ? bmax.y : cmax.y;
    max.z = (bmax.z > cmax.z) ? bmax.z : cmax.z;
}     

plVector3 plBoneAndCartilage::getCentroid() const
{
    plVector3 min, max;
    getMinMax(min, max);
    return 0.5f * (max + min);    
}   



