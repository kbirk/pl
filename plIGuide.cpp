#include "plIGuide.h"

plIGuide::plIGuide() 
    : site( NULL ) 
{
}


plIGuide::plIGuide( plIGuideSite *s, PLuint sid, const plSeq<plPlugInfo> &p, const plSeq<plKWire*> &k, const plSeq<PLuint> &kids )
    : site( s ), siteID( sid ), plugs( p ), kWires( k ), kWireIDs( kids )
{
}


PLbool plIGuide::generateIGuideModels()
{
    const PLuint PL_OCTREE_DEPTH_IGUIDE_MODELS = 1;

    iGuideModelsToAdd.clear();
    iGuideModelsToSubtract.clear();

    std::cout << "DEBUG: 10" << std::endl;
    // TODO: error checking as necessary
    if ( site == NULL )
    {
        std::cerr << "Error: No site chosen for iGuide. Aborting." << std::endl;
        return false;
    }
    std::cout << "DEBUG: 15" << std::endl;
    if (site->boundary.size() <= 2)
    {
        std::cerr << "Error: Boundary does not have enough points. Aborting." << std::endl;
        return false;
    }


    std::cout << "DEBUG: 20" << std::endl;
    plString anatomyFilename ( _prepareFilenameWithVariables(false,'M',0,"bone") );
    iGuideModelsToSubtract.add( new plModel( plSeq<plTriangle>( site->model().combined.triangles() ), anatomyFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS ) );

    std::cout << "DEBUG: 30" << std::endl;
    // template base TODO: recreate the template base shape ONLY if it needs updating
    plString templateBaseFilename ( _prepareFilenameWithVariables(true ,'M',0,"templateBase") );
    
    std::cout << "DEBUG: 35" << std::endl;
    
    // generate template base
    if ( site->generateTemplateBase() )
    {
        // if successful
        std::cout << "DEBUG: 37" << std::endl;
        iGuideModelsToAdd.add( new plModel( site->templateBase(), templateBaseFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS) );
    }


    std::cout << "DEBUG: 40" << std::endl;
    // plug pieces
    plSeq<plTriangle> roundCylinder;
    plSTL::importFile( roundCylinder, "./iGuideElements/Generator_Cylinder_Round.stl" );

    plSeq<plTriangle> sharpCylinder;
    plSTL::importFile( sharpCylinder, "./iGuideElements/Generator_Cylinder_Sharp.stl" );

    plSeq<plTriangle> keyCube;
    plSTL::importFile( keyCube, "./iGuideElements/Generator_Key_Cube.stl" );

    PLfloat   zero        ( 0.f );
    plVector3 zeroVector  ( 0.f, 0.f, 0.f );

    std::cout << "DEBUG: 50" << std::endl;
    
    for ( PLuint i = 0; i < plugs.size(); i++ )
    {   
        plMatrix44 plugTransform = plugs[i].transform().matrix();
        
        PLfloat   keyTranslation = 6.f;
        PLfloat   keyRotation    = 0.f;
        
        //if ( plugs[i].type == PL_PICKING_INDEX_GRAFT_DONOR )
            PLfloat   holeDiameter   = plugs[i].radius() * 2.f - printerOffset;
            
        PLfloat   sleeveDiameter = holeDiameter + 4.f;
        PLfloat   baseDiameter   = sleeveDiameter + 2.f;
        PLfloat   holderDiameter = baseDiameter + 8.f;
        
        plVector3 holeScale   ( holeDiameter,   36.f, holeDiameter   );
        plVector3 sleeveScale ( sleeveDiameter, 30.f, sleeveDiameter );
        plVector3 baseScale   ( baseDiameter,   20.f, baseDiameter   );
        plVector3 holderScale ( holderDiameter, 20.f, holderDiameter );
        plVector3 keyScale    ( 3.f, 29.9f, 4.f );
        
        //if ( plugs[i].type == PL_PICKING_INDEX_GRAFT_DONOR )
            //plVector3 supportScale( 18.f,  4.f, 18.f  );
        
        plSeq<plTriangle> holeTriangles   ( _createTemplatePieceTransformed(sharpCylinder, plugTransform, 0.0f, holeScale,   0.0f,           0.0f) );
        plSeq<plTriangle> sleeveTriangles ( _createTemplatePieceTransformed(roundCylinder, plugTransform, 0.0f, sleeveScale, 0.0f,           0.0f) );
        plSeq<plTriangle> baseTriangles   ( _createTemplatePieceTransformed(sharpCylinder, plugTransform, 0.0f, baseScale,   0.0f,           0.0f) );
        plSeq<plTriangle> holderTriangles ( _createTemplatePieceTransformed(roundCylinder, plugTransform, 0.0f, holderScale, 0.0f,           0.0f) );
        plSeq<plTriangle> keyTriangles    ( _createTemplatePieceTransformed(keyCube,       plugTransform, 0.0f, keyScale,    keyTranslation, 0.0f) );

        //if (plugs[i].type == PL_PICKING_INDEX_GRAFT_DONOR)
            //plSeq<plTriangle> supportTriangles( createTemplatePieceTransformed(sharpCylinder, recipientTransform, cylinderRecipientSupportOffset, supportScale, 0.0f, 0.0f) );

        PLchar typeLetter = (plugs[i].type() == PL_PICKING_INDEX_GRAFT_DONOR) ? 'H' : 'R';
            
        plString holeFilename    ( _prepareFilenameWithVariables(false, typeLetter, plugs[i].graftID(), "hole"  ) );
        plString sleeveFilename  ( _prepareFilenameWithVariables(true , typeLetter, plugs[i].graftID(), "sleeve") );
        plString baseFilename    ( _prepareFilenameWithVariables(true , typeLetter, plugs[i].graftID(), "base"  ) );
        plString holderFilename  ( _prepareFilenameWithVariables(true , typeLetter, plugs[i].graftID(), "holder") );
        plString keyFilename     ( _prepareFilenameWithVariables(true , typeLetter, plugs[i].graftID(), "key"   ) );

        std::cout << "DEBUG: 60" << std::endl;
        iGuideModelsToSubtract.add( new plModel( holeTriangles  , holeFilename  , PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        iGuideModelsToAdd.add     ( new plModel( sleeveTriangles, sleeveFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        iGuideModelsToAdd.add     ( new plModel( baseTriangles  , baseFilename  , PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        iGuideModelsToAdd.add     ( new plModel( holderTriangles, holderFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        iGuideModelsToAdd.add     ( new plModel( keyTriangles   , keyFilename   , PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
    }
    
    return true;
}


PLbool plIGuide::exportIGuideModels( const plString &directory ) 
{
    if ( iGuideModelsToAdd.size() == 0 && iGuideModelsToSubtract.size() == 0 )
    {
        std::cerr << "Error: No IGuide pieces to export. Did you forget to generate them? Aborting." << std::endl;
        return false;
    }
    // TODO: Compare the number of meshes to what we expect based on the sizes of the various arrays, output a WARNING if it isn't what we expect.

    for (PLuint i = 0; i < iGuideModelsToAdd.size(); i++)
    {
        plSTL::exportFileBinary( iGuideModelsToAdd[i]->triangles(), ( directory + iGuideModelsToAdd[i]->filename() ) );
    }
    for (PLuint i = 0; i < iGuideModelsToSubtract.size(); i++)
    {
        plSTL::exportFileBinary( iGuideModelsToSubtract[i]->triangles(), ( directory + iGuideModelsToSubtract[i]->filename() ) );
    }
    return true;
}


plString plIGuide::_prepareFilenameWithVariables( PLbool add, PLchar type, PLint graftIndex, const plString &pieceName ) 
{
    std::stringstream strstream;
    strstream << (add?"Add":"Subtract") << type << graftIndex << pieceName << ".stl";
    return strstream.str();
}


plSeq<plTriangle> plIGuide::_createTemplatePieceTransformed ( const plSeq<plTriangle>& baseTriObject,
                                                             const plMatrix44&  plugTransform,
                                                             const PLdouble&    zOffset,
                                                             const plVector3&   scale,
                                                             const PLdouble&    keyTranslationXAxis,
                                                             const PLdouble&    keyRotationZAxis )
{
    plMatrix44 zTranslationMatrix   = plMatrix44();     zTranslationMatrix.setTranslation(0.0f, zOffset, 0.0f);
    plMatrix44 scaleMatrix          = plMatrix44();     scaleMatrix.setScale(scale);
    plMatrix44 keyTranslationMatrix = plMatrix44();     keyTranslationMatrix.setTranslation(keyTranslationXAxis, 0.0f, 0.0f);
    plMatrix44 keyRotationMatrix    = plMatrix44();     keyRotationMatrix.setRotationD(keyRotationZAxis, plVector3(0,1,0));
    plMatrix44 transformationMatrix = plugTransform * zTranslationMatrix * keyRotationMatrix * keyTranslationMatrix * scaleMatrix;

    plSeq<plTriangle> outputTriObject;
    for (PLuint i=0; i<baseTriObject.size(); i++)
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


void plIGuide::toggleVisibility()
{
    for (PLuint i = 0; i < iGuideModelsToAdd.size(); i++)
    {
        iGuideModelsToAdd[i]->toggleVisibility();
    }
    for (PLuint i = 0; i < iGuideModelsToSubtract.size(); i++)
    {
        iGuideModelsToSubtract[i]->toggleVisibility();
    }
}

