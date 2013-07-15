#ifndef __PL_BONE_AND_CARTILAGE_MODEL_H__
#define __PL_BONE_AND_CARTILAGE_MODEL_H__

#include "plCommon.h"
#include "plModel.h"

class plBoneAndCartilage
{
    public:
       
        plModel bone;
		plModel	cartilage;

        plBoneAndCartilage();
        plBoneAndCartilage( plString boneFile, plString cartilageFile );

        void importCSV( const plSeq<plString> &row );
        void exportCSV( PLuint index );

        void draw() const;

        void       getMinMax  (plVector3 &min, plVector3 &max)   const;        
        plVector3  getCentroid() const;     
};


#endif
