#include "plIGuide.h"

plIGuide::plIGuide() 
{
    siteIndex = -1;
}


plIGuide::plIGuide( PLuint modelID, const plBoneAndCartilage &model )
    : plModelSpecific( modelID, model )
{
    siteIndex = -1;
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


PLbool plIGuide::generateIGuideModels()
{
    const PLuint PL_OCTREE_DEPTH_IGUIDE_MODELS(1);

    iGuideModelsToAdd.     clear();
    iGuideModelsToSubtract.clear();

    std::cout << "DEBUG: 10" << std::endl;
    // TODO: error checking as necessary
    if (siteIndex == -1)
    {
        std::cerr << "Error: No site chosen for iGuide. Aborting." << std::endl;
        return false;
    }
    std::cout << "DEBUG: 15" << std::endl;
    if (site().boundary.size() <= 2)
    {
        std::cerr << "Error: Boundary does not have enough points. Aborting." << std::endl;
        return false;
    }


    std::cout << "DEBUG: 20" << std::endl;
    plString anatomyFilename (prepareFilenameWithVariables(false,'M',0,"bone"));
    iGuideModelsToSubtract.add( new plModel( plSeq<plTriangle>(site().model().combined.triangles()),anatomyFilename,PL_OCTREE_DEPTH_IGUIDE_MODELS ) );


    std::cout << "DEBUG: 30" << std::endl;
    // template base TODO: recreate the template base shape ONLY if it needs updating
    plString templateBaseFilename (prepareFilenameWithVariables(true ,'M',0,"templateBase"));
    std::cout << "DEBUG: 32" << std::endl;
    plSeq<plTriangle> templateBaseTris ( site().templateBase( site().model().cartilage.triangles() ));
    std::cout << "DEBUG: 35" << std::endl;
    if (templateBaseTris.size() > 0)
    {
        std::cout << "DEBUG: 37" << std::endl;
        iGuideModelsToAdd.add( new plModel(templateBaseTris,templateBaseFilename,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
    }


    std::cout << "DEBUG: 40" << std::endl;
    // plug pieces
    plSeq<plTriangle> roundCylinder;
    plSTL::importFile(roundCylinder, "./iGuideElements/Generator_Cylinder_Round.stl");

    plSeq<plTriangle> sharpCylinder;
    plSTL::importFile(sharpCylinder, "./iGuideElements/Generator_Cylinder_Sharp.stl");

    plSeq<plTriangle> keyCube;
    plSTL::importFile(keyCube, "./iGuideElements/Generator_Key_Cube.stl");

    PLfloat   zero        ( 0.f );
    plVector3 zeroVector  ( 0.f, 0.f, 0.f );

    std::cout << "DEBUG: 50" << std::endl;
    for (PLuint i = 0; i < harvestIndices.size(); i++)
    {
        plMatrix44 plugTransform   ( harvestGraft(i).harvest.transform.matrix() );

        PLfloat   keyHarvestTranslation        (  6.f  );
        PLfloat   keyRotation ( 0.f );

        // TODO: take into account extra diameter of the harvest tool
        PLfloat   holeDiameter                 ( harvestGraft(i).radius() * 2.f - printerOffset );
        PLfloat   sleeveDiameter               ( holeDiameter + 4.f   );
        PLfloat   baseDiameter                 ( sleeveDiameter + 2.f );
        PLfloat   holderDiameter               ( baseDiameter + 8.f   );

        plVector3 holeScale     ( holeDiameter,   36.f, holeDiameter   );
        plVector3 sleeveScale   ( sleeveDiameter, 30.f, sleeveDiameter );
        plVector3 baseScale     ( baseDiameter,   20.f, baseDiameter   );
        plVector3 holderScale   ( holderDiameter, 20.f, holderDiameter );
        plVector3 keyScale      ( 3.f, 29.9f, 4.f );

        plSeq<plTriangle> holeTriangles     ( createTemplatePieceTransformed(sharpCylinder, plugTransform,   zero, holeScale,     zero,                    zero) );
        plSeq<plTriangle> sleeveTriangles   ( createTemplatePieceTransformed(roundCylinder, plugTransform,   zero, sleeveScale,   zero,                    zero) );
        plSeq<plTriangle> baseTriangles     ( createTemplatePieceTransformed(sharpCylinder, plugTransform,   zero, baseScale,     zero,                    zero) );
        plSeq<plTriangle> holderTriangles   ( createTemplatePieceTransformed(roundCylinder, plugTransform,   zero, holderScale,   zero,                    zero) );
        plSeq<plTriangle> keyTriangles      ( createTemplatePieceTransformed(keyCube,       plugTransform,   zero, keyScale,      keyHarvestTranslation,   zero) );

        plString holeFilename      ( prepareFilenameWithVariables(false,'H',harvestIndices[i],"hole"  ) );
        plString sleeveFilename    ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"sleeve") );
        plString baseFilename      ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"base"  ) );
        plString holderFilename    ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"holder") );
        plString keyFilename       ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"key"   ) );

        std::cout << "DEBUG: 60" << std::endl;
        iGuideModelsToSubtract.add( new plModel(holeTriangles  ,holeFilename  ,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(sleeveTriangles,sleeveFilename,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(baseTriangles  ,baseFilename  ,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(holderTriangles,holderFilename,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(keyTriangles   ,keyFilename   ,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
    }

    std::cout << "DEBUG: 70" << std::endl;
    for (PLuint i = 0; i < recipientIndices.size(); i++)
    {
        plMatrix44 recipientTransform ( recipientGraft(i).recipient.transform.matrix() );

        PLfloat   cylinderRecipientSupportOffset  ( 12.f );

        PLfloat   keyRecipientTranslation      (  6.f  );
        PLfloat   keyRotation ( 0.f );

        PLfloat   holeDiameter                 ( harvestGraft(i).radius() * 2.f - printerOffset );
        PLfloat   sleeveDiameter               ( holeDiameter + 4.f   );
        PLfloat   baseDiameter                 ( sleeveDiameter + 2.f );
        PLfloat   holderDiameter               ( baseDiameter + 8.f   );

        plVector3 holeScale   ( holeDiameter  , 36.f, holeDiameter   );
        plVector3 sleeveScale ( sleeveDiameter, 30.f, sleeveDiameter );
        plVector3 baseScale   ( baseDiameter  , 20.f, baseDiameter   );
        plVector3 holderScale ( holderDiameter, 20.f, holderDiameter );
        //plVector3 supportScale( 18.f,  4.f, 18.f  );
        plVector3 keyScale    (  3.f, 29.9f, 4.f );

        plSeq<plTriangle> holeTriangles   ( createTemplatePieceTransformed(sharpCylinder, recipientTransform, zero,                           holeScale,   zero,                    zero) );
        plSeq<plTriangle> sleeveTriangles ( createTemplatePieceTransformed(roundCylinder, recipientTransform, zero,                           sleeveScale, zero,                    zero) );
        plSeq<plTriangle> baseTriangles   ( createTemplatePieceTransformed(sharpCylinder, recipientTransform, zero,                           baseScale,   zero,                    zero) );
        plSeq<plTriangle> holderTriangles ( createTemplatePieceTransformed(roundCylinder, recipientTransform, zero,                           holderScale, zero,                    zero) );
        //plSeq<plTriangle> supportTriangles( createTemplatePieceTransformed(sharpCylinder, recipientTransform, cylinderRecipientSupportOffset, supportScale,zero,                    zero) );
        plSeq<plTriangle> keyTriangles    ( createTemplatePieceTransformed(keyCube,       recipientTransform, zero,                           keyScale,                     keyRecipientTranslation, zero) );

        plString holeFilename      ( prepareFilenameWithVariables(false,'R',harvestIndices[i],"hole"  ) );
        plString sleeveFilename    ( prepareFilenameWithVariables(true ,'R',harvestIndices[i],"sleeve") );
        plString baseFilename      ( prepareFilenameWithVariables(true ,'R',harvestIndices[i],"base"  ) );
        plString holderFilename    ( prepareFilenameWithVariables(true ,'R',harvestIndices[i],"holder") );
        plString keyFilename       ( prepareFilenameWithVariables(true ,'R',harvestIndices[i],"key"   ) );

        std::cout << "DEBUG: 80" << std::endl;
        iGuideModelsToSubtract.add( new plModel(holeTriangles  ,holeFilename  ,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(sleeveTriangles,sleeveFilename,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(baseTriangles  ,baseFilename  ,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(holderTriangles,holderFilename,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
        iGuideModelsToAdd     .add( new plModel(keyTriangles   ,keyFilename   ,PL_OCTREE_DEPTH_IGUIDE_MODELS) );
    }

    std::cout << "DEBUG: 90" << std::endl;
    return true;
}


PLbool plIGuide::exportIGuideModels(const plString &directory) {

    if (iGuideModelsToAdd.size() == 0 && iGuideModelsToSubtract.size() == 0)
    {
        std::cerr << "Error: No IGuide pieces to export. Did you forget to generate them? Aborting." << std::endl;
        return false;
    }
    // TODO: Compare the number of meshes to what we expect based on the sizes of the various arrays, output a WARNING if it isn't what we expect.

    for (PLuint i = 0; i < iGuideModelsToAdd.size(); i++)
    {
        plSTL::exportFileBinary(iGuideModelsToAdd[i]->triangles(), (directory + iGuideModelsToAdd[i]->filename()));
    }
    for (PLuint i = 0; i < iGuideModelsToSubtract.size(); i++)
    {
        plSTL::exportFileBinary(iGuideModelsToSubtract[i]->triangles(), (directory + iGuideModelsToSubtract[i]->filename()));
    }
    return true;
}


plString plIGuide::prepareFilenameWithVariables( PLbool add, PLchar type, PLint graftIndex, const plString &pieceName ) {
    std::stringstream strstream;
    strstream << (add?"Add":"Subtract") << type << graftIndex << pieceName << ".stl";
    return strstream.str();
}


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


void plIGuide::clearIGuideModels ()
{
    for (PLuint i = 0; i < iGuideModelsToAdd.size(); i++)
    {
        delete iGuideModelsToAdd[i];
    }
    iGuideModelsToAdd.clear();
    for (PLuint i = 0; i < iGuideModelsToSubtract.size(); i++)
    {
        delete iGuideModelsToSubtract[i];
    }
    iGuideModelsToSubtract.clear();
}


void plIGuide::draw() 
{
    for (PLuint i = 0; i < iGuideModelsToAdd.size(); i++)
    {
        iGuideModelsToAdd[i]->draw( plVector3( 0.75f,0.75f,1.0f ) );
    }
    for (PLuint i = 0; i < iGuideModelsToSubtract.size(); i++)
    {
        iGuideModelsToSubtract[i]->draw( plVector3( 1.0f,0.75f,0.75f ) );
    }
    //plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY;
    //boundary.draw();
}
