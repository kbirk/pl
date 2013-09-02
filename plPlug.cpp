#include "plPlug.h"

plPlug::plPlug()
{
}

plPlug::plPlug( PLuint modelID, const plBoneAndCartilage &model, const plTransform &t  )
    : transform(t), plModelSpecific( modelID, model )
{
}

void plPlug::importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    // Fill in the field            
    plString subfield = row[3];
    
    if (subfield.compareCaseInsensitive("model") )
    {
        _modelID = plString::fromString<PLint>( row[4] ); 
        if (models.size() <= _modelID )
        {
            std::cerr << "plPlug importCSV() error: model ID read before model data";
            exit(1);
        }    
        _model = models[_modelID];
    }
    else if (subfield.compareCaseInsensitive("transform") )
    {
        transform.importCSV( row );
    }
}

void plPlug::translate( const plVector3 &translation )
{        
    plIntersection intersection = _model->bone.rayIntersect( transform.origin() + translation, -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normalRadius = 6.0f;
        plVector3 normal = _model->bone.getAverageNormal( normalRadius, transform.origin(), transform.y() );  

        // translate       
        plVector3 y      = normal;
        plVector3 x      = y ^ transform.z();                       
        plVector3 origin = intersection.point;   
        transform.set( x, y, origin); 
    }
}


void plPlug::translateX( PLfloat distance, const plVector3 &planeNormal )
{                 
    plIntersection intersection = _model->bone.rayIntersect( transform.origin() + distance*transform.x(), -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normalRadius = 6.0f;
        plVector3 normal = _model->bone.getAverageNormal( normalRadius, transform.origin(), transform.y() );  
             
        // translating along x                    
        plVector3 x = (normal ^ planeNormal);                       
        plVector3 y = normal;                        
        plVector3 z = (x ^ y);
        plVector3 origin = intersection.point;
        transform.set( x, y, z, origin); 
    }
}


void plPlug::translateZ( PLfloat distance, const plVector3 &planeNormal )
{                 
    plIntersection intersection = _model->bone.rayIntersect( transform.origin() + distance*transform.z(), -transform.y() );  

    if (intersection.exists)
    {   
        PLfloat normalRadius = 6.0f;
        plVector3 normal = _model->bone.getAverageNormal( normalRadius, transform.origin(), transform.y() );  
        
        // translate       
        plVector3 z = (normal ^ planeNormal);                       
        plVector3 y = normal;                         
        plVector3 x = (y ^ z);                         
        plVector3 origin = intersection.point;   
        transform.set( x, y, z, origin); 
    }
}


void plPlug::rotate( const plVector3 &axis, PLfloat angleDegrees )
{     
    plMatrix44 rot; rot.setRotationD(angleDegrees,  axis);

    transform.set( rot * transform.x(), rot * transform.y() );   
}
