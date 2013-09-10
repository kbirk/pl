#include "plAutomaticPlanner.h"

namespace plAutomaticPlanner
{
    plSeq<plSiteGrid>  _donorSiteGrids;
    plSeq<plSiteGrid>  _defectSiteGrids; 
    
    PLbool _generateSite     ( plSeq<plSiteGrid> &grids, const plSeq<plTriangle> &triangles, const plBoundary &boundary, PLbool fineGrain );                    
    PLbool _generateSiteGrids( plPlan &plan );     
               
    void   _dispatch( plPlan &plan );      

    void calculate( plPlan &plan )
    {   
        std::cout << "Calculating Plan ... \n";
        if ( plan.defectSites().size() == 0 )          { std::cerr << "plAutomaticPlanner::calculate() error: No defect sites specified\n";   return; }
        if ( plan.donorSites().size()  == 0 )          { std::cerr << "plAutomaticPlanner::calculate() error: No donor sites specified\n";    return; }
        if ( plan.defectSites(0).spline.size() < 4 )   { std::cerr << "plAutomaticPlanner::calculate() error: No defect spline specified\n";  return; } 
        if ( plan.defectSites(0).boundary.size() < 3 ) { std::cerr << "plAutomaticPlanner::calculate() error: No recipient area specified\n"; return; }

        // clear previous sites
        _donorSiteGrids.clear();
        _defectSiteGrids.clear();

        // clear previous grafts
        PLuint previousCount = plan.grafts().size();
        for ( PLuint i=0; i < previousCount; i++ )
        {
            plan.removeGraft( 0 );
        }

        // generate site grids
        if ( _generateSiteGrids( plan ) )
        {            
            _dispatch( plan );
        }
    } 

    PLbool _generateSite( plSeq<plSiteGrid> &grids, const plSeq<plTriangle> &triangles, const plBoundary &boundary, PLbool fineGrain )
    {       
        plSiteGrid site( triangles, boundary, fineGrain );                
      
        if ( site.gridSize() == 0 )
        {
            _donorSiteGrids.clear();
            _defectSiteGrids.clear();
            std::cerr << "plAutomaticPlanner::_generateSite() error: could not produce site mesh\n";
            return false;
        }
        
        std::cout << "\t\t" <<  site.meshSize() << " triangles calculated \n";
        std::cout << "\t\t" <<  site.gridSize() << " grid points calculated \n";
        grids.add( site );
        return true; 

    }    
    
    PLbool _generateSiteGrids( plPlan &plan )
    {
        for ( PLuint i = 0; i < plan.defectSites().size(); i++)
        {   
            std::cout << "\tCalculating defect site grid " << i << " \n";
            if ( !_generateSite( _defectSiteGrids, 
                                 plan.defectSites(i).spline.triangles(), 
                                 plan.defectSites(i).boundary,
                                 false ) )
            {
                return false;
            }
        }

        for ( PLuint i = 0; i < plan.donorSites().size(); i++)
        {    
            std::cout << "\tCalculating donor site grid " << i << " \n";
            
            if ( !_generateSite( _donorSiteGrids, 
                                 plan.donorSites(i).model().cartilage.triangles(), 
                                 plan.donorSites(i).boundary,
                                 true ) )
            {
                return false;
            }
        }  

        return true;
    }


    void _dispatch( plPlan &plan )
    {    
        PLtime t0, t1;
        
        // stage 0 timing //
        std::cout << "\n--------------------------- Initiating Stage 0 --------------------------- \n" <<
                       "-------------------------------------------------------------------------- \n";
        t0 = plTimer::now();

        plDefectState defectState = plPlannerStage0::run( _defectSiteGrids[0] );    
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 0 Complete --------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ---------------------- \n";
        ////////////////////
        
        // stage 1 timing //
        std::cout << "\n--------------------------- Initiating Stage 1 --------------------------- \n" <<
                       "-------------------------------------------------------------------------- \n";
        t0 = plTimer::now();

        plRmsData rmsData = plPlannerStage1::run( _defectSiteGrids[0], _donorSiteGrids, defectState );
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 1 Complete ---------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ------------------------ \n";
        ////////////////////
           
        // stage 2 timing //
        std::cout << "\n--------------------------- Initiating Stage 2 --------------------------- \n" <<
                       "-------------------------------------------------------------------------- \n";
        t0 = plTimer::now();

        plDonorState donorState = plPlannerStage2::run( _donorSiteGrids, defectState, rmsData );    
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 2 Complete --------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ------------------------ \n";
        ////////////////////
               
        if ( donorState.graftPositions.size() > 0 )
        {
            for ( PLuint i=0; i < defectState.graftCount; i++ )
            {    
                // all graft origins generated from the planner shaders have their origins flush with the cartilage surface, these
                // must be changed to be flush with the bone surface for mouse drag editing to behave nicely
                       
                // ray cast from cartilage positions in negative direction of normal to get harvest bone position
                plVector3 originalHarvestOrigin( donorState.graftPositions[i].x, donorState.graftPositions[i].y, donorState.graftPositions[i].z );
                plVector3 originalHarvestY     ( donorState.graftNormals[i].x,   donorState.graftNormals[i].y,   donorState.graftNormals[i].z );    
                     
                plIntersection intersection = plan.models(0).bone.rayIntersect( originalHarvestOrigin, -originalHarvestY );   
                // set correct harvest origin to bone intersection point             
                plVector3 correctHarvestOrigin = intersection.point;            

                // calculate how thick the cartilage is ( the distance from the cartilage point to bone point )
                PLfloat cartilageThickness = ( correctHarvestOrigin - originalHarvestOrigin ).length();
            
                // ray cast from cartilage positions in negative direction of normal to get recipient bone position
                plVector3 originalRecipientOrigin( defectState.graftPositions[i].x,  defectState.graftPositions[i].y,   defectState.graftPositions[i].z );
                plVector3 originalRecipientY     ( defectState.graftNormals[i].x,    defectState.graftNormals[i].y,     defectState.graftNormals[i].z   );        
                plVector3 originalRecipientZ     ( donorState.graftZDirections[i].x, donorState.graftZDirections[i].y,  donorState.graftZDirections[i].z ); // use from donor state!
                plVector3 originalRecipientX = (originalRecipientY ^ originalRecipientZ).normalize();
                                               
                intersection = plan.models(0).bone.rayIntersect( originalRecipientOrigin, -originalRecipientY );      
                // set correct recipient origin to bone intersection point   
                plVector3 correctRecipientOrigin = intersection.point; 

                // get height offset so that cap is flush with cartilage surface, remember to subtract cartilage thickness 
                PLfloat recipientHeightOffset = ( correctRecipientOrigin - originalRecipientOrigin ).length() - cartilageThickness;
                          
                plPlug recipient( 0, plan.models(0), plTransform( originalRecipientX, originalRecipientY, correctRecipientOrigin ) );
                plPlug harvest  ( 0, plan.models(0), plTransform( originalHarvestY,   correctHarvestOrigin   ) );
                plan.addGraft( harvest, recipient, defectState.graftRadii[i], recipientHeightOffset );

            } 
        }
    }
 

}

