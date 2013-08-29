#include "plAutomaticPlanner.h"

namespace plAutomaticPlanner
{
                          
    PLbool _generateSiteGrids( plPlan &plan );                

    void   _dispatch();      
    void   _dispatchStage0();    // find defect sites
    //void   _dispatchStage1();    // calc rms of each possibility for donor sites
    //void   _dispatchStage2();    // find donor sites

    plSeq<plSiteGrid>  _donorSiteGrids;
    plSeq<plSiteGrid>  _defectSiteGrids; 
    plSeq<plVector3>   DEBUG_GRAFT_LOCATIONS; 

    void calculate( plPlan &plan )
    {
        std::cout << "Calculating Plan ... \n";
        if ( plan.defectSites().size() == 0 )          { std::cerr << "plAutomaticPlanner::calculate() error: No defect sites specified\n";   return; }
        if ( plan.donorSites().size() == 0 )           { std::cerr << "plAutomaticPlanner::calculate() error: No donor sites specified\n";    return; }
        if ( plan.defectSites(0).spline.size() < 4 )   { std::cerr << "plAutomaticPlanner::calculate() error: No defect spline specified\n";  return; } 
        if ( plan.defectSites(0).boundary.size() < 3 ) { std::cerr << "plAutomaticPlanner::calculate() error: No recipient area specified\n"; return; }

        // clear sites
        _donorSiteGrids.clear();
        _defectSiteGrids.clear();

        // generate site grids
        if ( _generateSiteGrids( plan ) )
        {
            _dispatch();
        }
    } 

    PLbool _generateSite( plSeq<plSiteGrid> &grids, const plSeq<plTriangle> &triangles, const plBoundary &boundary )
    {       
        plSiteGrid grid( triangles, boundary );                
                   
        if ( grid.size() == 0 )
        {
            _donorSiteGrids.clear();
            _defectSiteGrids.clear();
            std::cerr << "_generateSiteGrids() error: could not produce site mesh\n";
            return false;
        }
        
        std::cout << "\t\t" <<  grid.meshSize() << " triangles calculated \n";
        std::cout << "\t\t" <<  grid.size()     << " grid points calculated \n";
        grids.add( grid );
        return true; 

    }    
    
    PLbool _generateSiteGrids( plPlan &plan )
    {
        
        for ( PLuint i = 0; i < plan.defectSites().size(); i++)
        {   
            std::cout << "\tCalculating defect site grid " << i << " \n";
            if ( !_generateSite( _defectSiteGrids, 
                                 plan.defectSites(i).spline.triangles(), 
                                 plan.defectSites(i).boundary) )
            {
                return false;
            }
        }
        
        for ( PLuint i = 0; i < plan.donorSites().size(); i++)
        {    
            std::cout << "\tCalculating donor site grid " << i << " \n";
            if ( !_generateSite( _donorSiteGrids, 
                                 plan.donorSites(i).model().cartilage.triangles(), 
                                 plan.donorSites(i).boundary) )
            {
                return false;
            }
        }  

        return true;
    }


    void _dispatch()
    {    
        PLtime t0 = plTimer::now();

        _dispatchStage0();
        
        PLtime t1 = plTimer::now();
        std::cout << "\nAutomatic planner stage 0 complete:\n";     
        std::cout << "\tCompute shader execution time: " << t1 - t0 << " milliseconds \n";
    }


    void _dispatchStage0()
    {
        plAnnealingState state = plPlannerStage0::run( _defectSiteGrids[0] );
    
        DEBUG_GRAFT_LOCATIONS.clear();
    
        for (PLuint i=0; i < state.graftCount; i++)
        {
            plVector4 &p = state.graftPositions[i];
            plVector4 &n = state.graftNormals  [i];
            PLfloat   &r = state.graftRadii    [i];
            
            DEBUG_GRAFT_LOCATIONS.add( plVector3( p.x, p.y, p.z ) );                                              
            DEBUG_GRAFT_LOCATIONS.add( plVector3( n.x, n.y, n.z ) );
            DEBUG_GRAFT_LOCATIONS.add( plVector3( r, 0.0f, 0.0f ) ); 
        }
    }
 
 
}

