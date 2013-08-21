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


PLbool plIGuide::generateIGuideMeshes()
{

    // TODO: error checking as necessary
    if (site().boundary.size() <= 2)
    {
        std::cerr << "Error: Boundary does not have enough points. Aborting." << std::endl;
        return false;
    }


    // anatomy TODO: change to bone AND cartilage model
    plString anatomyFilename (prepareFilenameWithVariables(false,'M',0,"bone"));
    iGuideMeshes.add(plIGuideMesh(anatomyFilename,plSeq<plTriangle>(site().model()->bone.triangles()))  );


    // template base TODO: create the template base shape if it needs updating
    plString templateBaseFilename (prepareFilenameWithVariables(true ,'M',0,"templateBase"));
    iGuideMeshes.add(plIGuideMesh(templateBaseFilename,site().templateBase));


    // plug pieces
    plSeq<plTriangle> roundCylinder;
    plSTL::importFile(roundCylinder, "./templateElements/Generator_Cylinder_Round.stl");

    plSeq<plTriangle> sharpCylinder;
    plSTL::importFile(sharpCylinder, "./templateElements/Generator_Cylinder_Sharp.stl");

    plSeq<plTriangle> keyCube;
    plSTL::importFile(keyCube, "./templateElements/Generator_Key_Cube.stl");

    PLfloat   zero        ( 0.f );
    plVector3 zeroVector  ( 0.f, 0.f, 0.f );

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

        plSeq<plTriangle> holeTriangles     ( createTemplatePieceTransformed(sharpCylinder, plugTransform,   zero,                           holeScale,     zero,                    zero) );
        plSeq<plTriangle> sleeveTriangles   ( createTemplatePieceTransformed(roundCylinder, plugTransform,   zero,                           sleeveScale,   zero,                    zero) );
        plSeq<plTriangle> baseTriangles     ( createTemplatePieceTransformed(sharpCylinder, plugTransform,   zero,                           baseScale,     zero,                    zero) );
        plSeq<plTriangle> holderTriangles   ( createTemplatePieceTransformed(roundCylinder, plugTransform,   zero,                           holderScale,   zero,                    zero) );
        plSeq<plTriangle> keyTriangles      ( createTemplatePieceTransformed(keyCube,       plugTransform,   zero,                           keyScale,      keyHarvestTranslation,   zero) );

        plString holeFilename      ( prepareFilenameWithVariables(false,'H',harvestIndices[i],"hole"  ) );
        plString sleeveFilename    ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"sleeve") );
        plString baseFilename      ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"base"  ) );
        plString holderFilename    ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"holder") );
        plString keyFilename       ( prepareFilenameWithVariables(true ,'H',harvestIndices[i],"key"   ) );

        iGuideMeshes.add(plIGuideMesh(holeFilename,holeTriangles));
    }

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

    }

    return true;
}


PLbool plIGuide::exportIGuideMeshes(const plString &directory) {

    if (iGuideMeshes.size() == 0)
    {
        std::cerr << "Error: No IGuide pieces to export. Did you forget to generate them? Aborting." << std::endl;
        return false;
    }
    // TODO: Compare the number of meshes to what we expect based on the sizes of the various arrays, output a WARNING if it isn't what we expect.

    for (PLuint i = 0; i < iGuideMeshes.size(); i++)
    {
        plSTL::exportFileBinary(iGuideMeshes[i].triangles, (directory + iGuideMeshes[i].filename));
    }
    return true;
}


plString plIGuide::prepareFilenameWithVariables( PLbool add, PLchar type, PLint graftIndex, const plString &pieceName ) {
    std::stringstream strstream;
    strstream << (add?"Add":"Subtract") << type << graftIndex << pieceName;
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


void plIGuide::draw() 
{
    //plPicking::value.type = PL_PICKING_TYPE_IGUIDE_BOUNDARY;
    //boundary.draw();
}
