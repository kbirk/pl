#include "plIGuide.h"

plIGuide::plIGuide() 
    : site( NULL ) 
{
}

plIGuide::plIGuide( plIGuideSite *site, 
                  PLuint siteID, 
                  const std::vector<plPlugInfo>&      plugs, 
                  const std::vector<plKWire*>&        kwires, 
                  const std::vector<PLuint>&          kwireIDs, 
                  const std::vector<const plSpline*>& splines, 
                  std::vector<PLuint>&                defectIDs )
    : site( site ), siteID( siteID ), plugs( plugs ), kWires( kwires ), kWireIDs( kwireIDs ), splines( splines ), defectIDs( defectIDs )
{
}


PLbool plIGuide::generateIGuideModels()
{
    const PLuint PL_OCTREE_DEPTH_IGUIDE_MODELS = 1;

    _modelsToAdd.clear();
    _modelsToSubtract.clear();

    if ( !site )
    {
        std::cerr << "Error: No site chosen for iGuide. Aborting." << std::endl;
        return false;
    }
    
    if ( site->boundary.size() <= 2 )
    {
        std::cerr << "Error: Boundary does not have enough points. Aborting." << std::endl;
        return false;
    }

    _modelsToSubtract.push_back( new plModel( site->boundary.mesh().triangles(), 
                                              _prepareFilenameWithVariables( false, 'M', 0, "bone" ), 
                                              PL_OCTREE_DEPTH_IGUIDE_MODELS ) );

    // generate template base
    if ( site->generateTemplateBase() )
    {
        // if successful
        std::string templateBaseFilename = _prepareFilenameWithVariables( true , 'M', 0, "templateBase" );
        _modelsToAdd.push_back( new plModel( site->templateBase().triangles(), templateBaseFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS) );
    }

    // generate base over the defect site
    for ( PLuint i = 0; i < splines.size(); i++ )
    {
        // create the base shape over the defect site
        plMatrix44 baseTranslation;  
        baseTranslation.setTranslation( PL_BASE_TRANSLATION_DISTANCE * splines[i]->getAverageNormal() );

        plMesh baseOverDefect = plMeshExtruder::extrudeMesh( baseTranslation * splines[i]->surfaceMesh(),
                                                             PL_BASE_THICKNESS, 
                                                             splines[i]->getAverageNormal() );
                                                                  
        _modelsToAdd.push_back( new plModel( baseOverDefect.triangles(), 
                                             _prepareFilenameWithVariables( true, 'S', i, "templateBase" ), 
                                             PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        
        // create the defect site volume
        plMatrix44 interiorTranslation;  
        interiorTranslation.setTranslation( PL_INTERIOR_TRANSLATION_DISTANCE * splines[i]->getAverageNormal() );
       
        plMesh interiorDefect = plMeshExtruder::extrudeMesh( interiorTranslation * splines[i]->surfaceMesh(),
                                                             PL_INTERIOR_THICKNESS,  
                                                             splines[i]->getAverageNormal() );
                                                               
        _modelsToSubtract.push_back( new plModel( interiorDefect.triangles(), 
                                                  _prepareFilenameWithVariables( false, 'M', i, "defectInterior" ), 
                                                  PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
    }

    // plug pieces
    std::vector<plTriangle> roundCylinder;
    plSTL::importFile( roundCylinder, "./iGuideElements/Generator_Cylinder_Round.stl" );

    std::vector<plTriangle> sharpCylinder;
    plSTL::importFile( sharpCylinder, "./iGuideElements/Generator_Cylinder_Sharp.stl" );

    std::vector<plTriangle> correctCylinder; // correction cylinder
    plSTL::importFile( correctCylinder, "./iGuideElements/Generator_Cylinder_Correction.stl" );

    std::vector<plTriangle> keyCube;
    plSTL::importFile( keyCube, "./iGuideElements/Generator_Key_Cube.stl" );

    for ( PLuint i = 0; i < plugs.size(); i++ )
    {   
        plMatrix44 cylinderToPlugTransform = plugs[i].transform().matrix();

        PLfloat keyTranslation = 6.0f;

        PLfloat holeDiameter    = plugs[i].radius();
        PLfloat sleeveDiameter  = holeDiameter   + 2.f ;
        PLfloat baseDiameter    = sleeveDiameter + 1.f ;
        PLfloat holderDiameter  = baseDiameter   + 4.f ;
        PLfloat correctDiameter = holderDiameter + 0.1f;

        PLfloat sleeveHeight = PL_TOOL_DEPTH - plugs[i].length();
        
        plVector3 holeScale   ( holeDiameter,   36.f, holeDiameter   );
        plVector3 sleeveScale ( sleeveDiameter, sleeveHeight, sleeveDiameter );
        plVector3 baseScale   ( baseDiameter,   20.f, baseDiameter   );
        plVector3 holderScale ( holderDiameter, 10.f, holderDiameter );
        plVector3 keyScale    ( 3.f, 29.9f, 4.f );
        plVector3 correctScale( correctDiameter, PL_BASE_THICKNESS - 1.f, correctDiameter   );

        //if ( plugs[i].type == PL_PICKING_INDEX_GRAFT_DONOR )
            //plVector3 supportScale( 18.f,  4.f, 18.f  );

        std::vector<plTriangle> holeTriangles    = _createTemplatePieceTransformed( sharpCylinder,   cylinderToPlugTransform, 0.0f, holeScale,    0.0f,           0.0f );
        std::vector<plTriangle> sleeveTriangles  = _createTemplatePieceTransformed( roundCylinder,   cylinderToPlugTransform, 0.0f, sleeveScale,  0.0f,           0.0f );
        std::vector<plTriangle> baseTriangles    = _createTemplatePieceTransformed( sharpCylinder,   cylinderToPlugTransform, 0.0f, baseScale,    0.0f,           0.0f );
        std::vector<plTriangle> holderTriangles  = _createTemplatePieceTransformed( roundCylinder,   cylinderToPlugTransform, 0.0f, holderScale,  0.0f,           0.0f );
        std::vector<plTriangle> keyTriangles     = _createTemplatePieceTransformed( keyCube,         cylinderToPlugTransform, 0.0f, keyScale,     keyTranslation, 0.0f );
        std::vector<plTriangle> correctTriangles = _createTemplatePieceTransformed( correctCylinder, cylinderToPlugTransform, 0.0f, correctScale, 0.0f,           0.0f );

        //if (plugs[i].type == PL_PICKING_INDEX_GRAFT_DONOR)
            //std::vector<plTriangle> supportTriangles( createTemplatePieceTransformed(sharpCylinder, recipientTransform, cylinderRecipientSupportOffset, supportScale, 0.0f, 0.0f) );

        PLchar typeLetter = ( plugs[i].type() == PL_PICKING_INDEX_GRAFT_DONOR ) ? 'H' : 'R';
            
        plString holeFilename    = _prepareFilenameWithVariables( PL_IGUIDE_BOOLEAN_MESH_DIFFERENCE, typeLetter, plugs[i].graftID(), "hole"          );
        plString sleeveFilename  = _prepareFilenameWithVariables( PL_IGUIDE_BOOLEAN_MESH_UNION     , typeLetter, plugs[i].graftID(), "sleeve"        );
        plString baseFilename    = _prepareFilenameWithVariables( true                             , typeLetter, plugs[i].graftID(), "base"          );
        plString holderFilename  = _prepareFilenameWithVariables( PL_IGUIDE_BOOLEAN_MESH_UNION     , typeLetter, plugs[i].graftID(), "holder"        );
        plString keyFilename     = _prepareFilenameWithVariables( PL_IGUIDE_BOOLEAN_MESH_UNION     , typeLetter, plugs[i].graftID(), "key"           );
        plString correctFilename = _prepareFilenameWithVariables( PL_IGUIDE_BOOLEAN_MESH_INTERSECTION , typeLetter, plugs[i].graftID(), "correction" );

        _modelsToSubtract.push_back( new plModel( holeTriangles  , holeFilename  , PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        _modelsToAdd.push_back     ( new plModel( sleeveTriangles, sleeveFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        _modelsToAdd.push_back     ( new plModel( baseTriangles  , baseFilename  , PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        _modelsToAdd.push_back     ( new plModel( holderTriangles, holderFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        _modelsToAdd.push_back     ( new plModel( keyTriangles   , keyFilename   , PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        //_modelsToSubtract.push_back( new plModel( correctTriangles, correctFilename, PL_OCTREE_DEPTH_IGUIDE_MODELS ) );
        //if (plugs[i].type() == PL_PICKING_INDEX_GRAFT_DONOR)
    }
    
    return true;
}


std::vector<plTriangle> plIGuide::_translateTriangles( const std::vector< plTriangle >& triangles, const plVector3& translation )
{
    std::vector<plTriangle> translatedTriangles;
    
    for ( const plTriangle& triangle : triangles )
    {   
        translatedTriangles.push_back( plTriangle( triangle.normal(), 
                                                   triangle.point0() + translation, 
                                                   triangle.point1() + translation, 
                                                   triangle.point2() + translation ) );
    }
    
    return translatedTriangles;
}


PLbool plIGuide::exportIGuideModels( const std::string &directory ) 
{
    if ( _modelsToAdd.empty() && _modelsToSubtract.empty() )
    {
        std::cerr << "Error: No IGuide pieces to export. Did you forget to generate them? Aborting." << std::endl;
        return false;
    }
    // TODO: Compare the number of meshes to what we expect based on the sizes of the various arrays, output a WARNING if it isn't what we expect.

    for ( PLuint i = 0; i < _modelsToAdd.size(); i++ )
    {
        plSTL::exportFileBinary( _modelsToAdd[i]->mesh().triangles(), ( directory + _modelsToAdd[i]->filename ) );
    }
    for ( PLuint i = 0; i < _modelsToSubtract.size(); i++ )
    {
        plSTL::exportFileBinary( _modelsToSubtract[i]->mesh().triangles(), ( directory + _modelsToSubtract[i]->filename ) );
    }
    return true;
}


std::string plIGuide::_prepareFilenameWithVariables( PLint operation, PLchar type, PLint graftIndex, const std::string &pieceName )
{
    std::stringstream strstream;
    strstream << type << graftIndex;
    switch (operation) // TODO: a DEFINE or enum or something
    {
        case PL_IGUIDE_BOOLEAN_MESH_DIFFERENCE:
            strstream << "Difference";
            break;
        case PL_IGUIDE_BOOLEAN_MESH_UNION:
            strstream << "Union";
            break;
        case PL_IGUIDE_BOOLEAN_MESH_INTERSECTION:
            strstream << "Intersect";
            break;
    }
    strstream << pieceName << ".stl";
    return strstream.str();
}


std::vector<plTriangle> plIGuide::_createTemplatePieceTransformed ( const std::vector<plTriangle>& baseTriObject,
                                                              const plMatrix44& plugTransform,
                                                              const PLfloat&    zOffset,
                                                              const plVector3&  scale,
                                                              const PLfloat&    keyTranslationXAxis,
                                                              const PLfloat&    keyRotationZAxis )
{
    plMatrix44 zTranslationMatrix;    zTranslationMatrix.setTranslation(0.0f, zOffset, 0.0f);
    plMatrix44 scaleMatrix;           scaleMatrix.setScale(scale);
    plMatrix44 keyTranslationMatrix;  keyTranslationMatrix.setTranslation(keyTranslationXAxis, 0.0f, 0.0f);
    plMatrix44 keyRotationMatrix;     keyRotationMatrix.setRotationD(keyRotationZAxis, plVector3(0,1,0));
    plMatrix44 transformationMatrix = plugTransform * zTranslationMatrix * keyRotationMatrix * keyTranslationMatrix * scaleMatrix;

    std::vector<plTriangle> outputTriObject;
    for (PLuint i=0; i<baseTriObject.size(); i++)
    {
        outputTriObject.push_back( transformationMatrix * baseTriObject[i] );
    }
    return outputTriObject;
}


void plIGuide::clearIGuideModels ()
{
    for (PLuint i = 0; i < _modelsToAdd.size(); i++)
    {
        delete _modelsToAdd[i];
    }
    _modelsToAdd.clear();
    for (PLuint i = 0; i < _modelsToSubtract.size(); i++)
    {
        delete _modelsToSubtract[i];
    }
    _modelsToSubtract.clear();
}


void plIGuide::extractRenderComponents( plRenderMap& renderMap, PLuint technique ) const
{
    if ( !_isVisible )
        return;

    plColourStack::load( plVector3( 0.75f,0.75f,1.0f ) );
    for ( PLuint i = 0; i < _modelsToAdd.size(); i++ )
    {
        plRenderer::queue( *_modelsToAdd[i] );
    }
    
    plColourStack::load( plVector3( 1.0f,0.75f,0.75f ) );
    for ( PLuint i = 0; i < _modelsToSubtract.size(); i++ )
    {
        plRenderer::queue( *_modelsToSubtract[i] );
    }
}


void plIGuide::extractRenderComponents( plRenderMap& renderMap ) const
{
    extractRenderComponents( renderMap, PL_PLAN_TECHNIQUE );
}


void plIGuide::toggleVisibility()
{
    for (PLuint i = 0; i < _modelsToAdd.size(); i++)
    {
        _modelsToAdd[i]->toggleVisibility();
    }
    for (PLuint i = 0; i < _modelsToSubtract.size(); i++)
    {
        _modelsToSubtract[i]->toggleVisibility();
    }
}
