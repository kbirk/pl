#ifndef __PL_BONE_AND_CARTILAGE_MODEL_H__
#define __PL_BONE_AND_CARTILAGE_MODEL_H__

#include "pl.h"
//#include "plRenderer.h"
#include "plModel.h"

class plBoneAndCartilage
{
    public:
       
        plModel bone;
		plModel	cartilage;

        plBoneAndCartilage();
        plBoneAndCartilage( plString bone_file, plString cartilage_file );

        void readFromCSV( const plSeq<plString> &row , const plString &directory );

        void draw() const;

        void       getMinMax  (plVector3 &min, plVector3 &max)   const;        
        plVector3  getCentroid() const;  

        friend std::ostream& operator << ( std::ostream& out, const plPlan &p );
        
};


#endif
