#include "plModelSpecific.h" 

plModelSpecific::plModelSpecific() 
    : _modelID(-1), _model(NULL)
{
}


plModelSpecific::plModelSpecific( PLuint modelID, const plBoneAndCartilage& model )
    : _modelID(modelID), _model(&model)
{
}
