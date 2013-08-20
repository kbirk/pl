#include "plIGuide.h" 

plIGuide::plIGuide() 
{
}

plIGuide::plIGuide( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model )
{
}

void plIGuide::importCSV( const plSeq<plString> &row, const plSeq<plBoneAndCartilage*> &models )
{
    // Fill in the field
    plString subfield = row[2];

    if (subfield.compareCaseInsensitive( "model") )
    {
        _modelID = plString::fromString<PLint>( row[3] );
        if (models.size() <= _modelID )
        {
            std::cerr << "plDefectSite importCSV() error: model ID read before model data";
            exit(1);
        }
        _model = models[_modelID];
    }
    else if (subfield.compareCaseInsensitive( "site") )
    {
        siteIndex = atoi( row[3].c_str() );
    }
    else if (subfield.compareCaseInsensitive( "recipient" ) )
    {
        recipientIndices.add( atoi( row[3].c_str() ) );
    }
    else if (subfield.compareCaseInsensitive( "harvest" ) )
    {
        harvestIndices.add( atoi( row[3].c_str() ) );
    }
    else if (subfield.compareCaseInsensitive( "kwire" ))
    {
        kWireIndices.add( atoi( row[3].c_str() ) );
    }
    else
    {
        std::cerr << "Error importing plan,'iguide': Unrecognized word '" << subfield << "' in third column." << std::endl;
    }
}

// ============================= BEGIN TEMPLATE CREATION =================================

/*PLbool plIGuide::generateMeshes()
{

    // TODO: error checking

    plSeq<plTriangle> roundCylinder;
    plSTL::importFile(roundCylinder, "./templateElements/Generator_Cylinder_Round.stl");

    plSeq<plTriangle> sharpCylinder;
    plSTL::importFile(sharpCylinder, "./templateElements/Generator_Cylinder_Sharp.stl");

    plSeq<plTriangle> keyCube;
    plSTL::importFile(keyCube, "./templateElements/Generator_Key_Cube.stl");



    std::cout << "Initializing transforms" << std::endl;
    plMatrix44 harvestTransform   ( ((*grafts)[0]).harvest.transform.matrix() );
    plMatrix44 recipientTransform ( ((*grafts)[0]).recipient.transform.matrix() );



    std::cout << "Initializing variables" << std::endl;
    PLfloat   cylinderRecipientSupportOffset  ( 12.f );

    plVector3 cylinderRecipientHoleScale   (  8.f, 36.f,  8.f  );
    plVector3 cylinderRecipientSleeveScale ( 12.f, 30.f, 12.f  );
    plVector3 cylinderRecipientBaseScale   ( 14.f, 20.f, 14.f  );
    plVector3 cylinderRecipientHolderScale ( 22.f, 20.f, 22.f  );
    plVector3 cylinderRecipientSupportScale( 18.f,  4.f, 18.f  );

    plVector3 cylinderHarvestHoleScale     (  8.f, 36.f,  8.f  );
    plVector3 cylinderHarvestSleeveScale   ( 12.f, 30.f, 12.f  );
    plVector3 cylinderHarvestBaseScale     ( 14.f, 20.f, 14.f  );
    plVector3 cylinderHarvestHolderScale   ( 22.f, 20.f, 22.f  );

    plVector3 keyScale                     (  3.f, 29.9f, 4.f );
    PLfloat   keyHarvestTranslation        (  6.f  );
    PLfloat   keyRecipientTranslation      (  6.f  );

    PLfloat   keyRotation ( 0.f );

    PLfloat   zero        ( 0.f );
    plVector3 zeroVector  ( 0.f, 0.f, 0.f );



    std::cout << "Transforming meshes" << std::endl;
    plSeq<plTriangle> cylinderRecipientHole   ( createTemplatePieceTransformed(sharpCylinder, recipientTransform, zero,                           cylinderRecipientHoleScale,   zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientSleeve ( createTemplatePieceTransformed(roundCylinder, recipientTransform, zero,                           cylinderRecipientSleeveScale, zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientBase   ( createTemplatePieceTransformed(sharpCylinder, recipientTransform, zero,                           cylinderRecipientBaseScale,   zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientHolder ( createTemplatePieceTransformed(roundCylinder, recipientTransform, zero,                           cylinderRecipientHolderScale, zero,                    zero) );
    plSeq<plTriangle> cylinderRecipientSupport( createTemplatePieceTransformed(sharpCylinder, recipientTransform, cylinderRecipientSupportOffset, cylinderRecipientSupportScale,zero,                    zero) );
    plSeq<plTriangle> keyRecipient            ( createTemplatePieceTransformed(keyCube,       recipientTransform, zero,                           keyScale,                     keyRecipientTranslation, zero) );

    plSeq<plTriangle> cylinderHarvestHole     ( createTemplatePieceTransformed(sharpCylinder, harvestTransform,   zero,                           cylinderHarvestHoleScale,     zero,                    zero) );
    plSeq<plTriangle> cylinderHarvestSleeve   ( createTemplatePieceTransformed(roundCylinder, harvestTransform,   zero,                           cylinderHarvestSleeveScale,   zero,                    zero) );
    plSeq<plTriangle> cylinderHarvestBase     ( createTemplatePieceTransformed(sharpCylinder, harvestTransform,   zero,                           cylinderHarvestBaseScale,     zero,                    zero) );
    plSeq<plTriangle> cylinderHarvestHolder   ( createTemplatePieceTransformed(roundCylinder, harvestTransform,   zero,                           cylinderHarvestHolderScale,   zero,                    zero) );
    plSeq<plTriangle> keyHarvest              ( createTemplatePieceTransformed(keyCube,       harvestTransform,   zero,                           keyScale,                     keyHarvestTranslation,   zero) );



    std::cout << "Exporting Meshes" << std::endl;
    plSTL::exportFileBinary(cylinderRecipientHole,   "./output/ALRecipientHole.stl"   );
    plSTL::exportFileBinary(cylinderRecipientSleeve, "./output/ALRecipientSleeve.stl" );
    plSTL::exportFileBinary(cylinderRecipientBase,   "./output/ALRecipientBase.stl"   );
    plSTL::exportFileBinary(cylinderRecipientHolder, "./output/ALRecipientHolder.stl" );
    plSTL::exportFileBinary(cylinderRecipientSupport,"./output/ALRecipientSupport.stl");
    plSTL::exportFileBinary(keyRecipient,            "./output/ALRecipientKey.stl"    );
    plSTL::exportFileBinary(cylinderHarvestHole,     "./output/ALHarvestHole.stl"     );
    plSTL::exportFileBinary(cylinderHarvestSleeve,   "./output/ALHarvestSleeve.stl"   );
    plSTL::exportFileBinary(cylinderHarvestBase,     "./output/ALHarvestBase.stl"     );
    plSTL::exportFileBinary(cylinderHarvestHolder,   "./output/ALHarvestHolder.stl"   );
    plSTL::exportFileBinary(keyHarvest,              "./output/ALHarvestKey.stl"      );



    return true;
}*/

// ---------------------- CREATE TRANSFORMED COPY OF PIECE -------------------------

plSeq<plTriangle> plIGuide::createTemplatePieceTransformed ( const plSeq<plTriangle>& baseTriObject ,
                                                             const plMatrix44&  plugTransform,
                                                             const PLdouble&    zOffset,
                                                             const plVector3&   scale ,
                                                             const PLdouble&    keyTranslationXAxis ,
                                                             const PLdouble&    keyRotationZAxis )
{
    plMatrix44 zTranslationMatrix =     plMatrix44();     zTranslationMatrix.setTranslation(0., zOffset, 0.);
    plMatrix44 scaleMatrix =            plMatrix44();     scaleMatrix.setScale(scale);
    plMatrix44 keyTranslationMatrix =   plMatrix44();     keyTranslationMatrix.setTranslation(keyTranslationXAxis, 0., 0.);
    plMatrix44 keyRotationMatrix =      plMatrix44();     keyRotationMatrix.setRotationD(keyRotationZAxis, plVector3(0,1,0));
    plMatrix44 transformationMatrix = plugTransform * zTranslationMatrix * keyRotationMatrix * keyTranslationMatrix * scaleMatrix;

    plSeq<plTriangle> outputTriObject;
    for (int i=0; i<baseTriObject.size(); i++)
    {
        outputTriObject.add( transformationMatrix * baseTriObject[i] );
    }
    return outputTriObject;
}


// ============================ END THOMAS' SECTION ================================

void plIGuide::draw() 
{
    //plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY;
    //boundary.draw();
}
