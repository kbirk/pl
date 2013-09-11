#ifndef __PL_MODEL_SPECIFIC_H__
#define __PL_MODEL_SPECIFIC_H__

#include "plCommon.h"
#include "plVector3.h"
#include "plBoneAndCartilage.h"

class plModelSpecific
{
    public:
               
        plModelSpecific();
        plModelSpecific( PLuint _modelID, const plBoneAndCartilage &_model );

        PLint                     modelID() const { return _modelID; }
        const plBoneAndCartilage& model()   const { return *_model;  }

        //virtual void importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models ) = 0;

    protected:
    
        PLint                     _modelID;
        const plBoneAndCartilage *_model;  
};


#endif

