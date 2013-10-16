#ifndef PL_MODEL_SPECIFIC_H
#define PL_MODEL_SPECIFIC_H

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

    protected:
    
        PLint                     _modelID;
        const plBoneAndCartilage *_model;  
};


#endif

