#include "plAutomaticPlanner.h"


namespace plAutomaticPlanner
{
    // private function prototypes             
    void _dispatch          (  plPlan &plan, const plPlanningBufferData &planningData );      
    void _clearPreviousPlan ( plPlan &plan );

    // public functions
    void calculate( plPlan &plan )
    {   
        // error checking
        if ( plan.defectSites().size() == 0 )          { std::cerr << "plAutomaticPlanner::calculate() error: No defect sites specified\n";   return; }
        if ( plan.donorSites().size()  == 0 )          { std::cerr << "plAutomaticPlanner::calculate() error: No donor sites specified\n";    return; }
        if ( plan.defectSites(0).spline.size() < 4 )   { std::cerr << "plAutomaticPlanner::calculate() error: No defect spline specified\n";  return; } 
        if ( plan.defectSites(0).boundary.size() < 3 ) { std::cerr << "plAutomaticPlanner::calculate() error: No recipient area specified\n"; return; }

        std::cout << "Assembling planning data structures ... \n";
        plPlanningBufferData planningData( plan.defectSites( 0 ), plan.donorSites() );
        
        // clear previous plan
        _clearPreviousPlan( plan );

        // if data is good, dispatch planner 
        if ( !planningData.good() )
        {    
            std::cerr << "plAutomaticPlanner::calculate() error: could not produce coherent planning data buffers" << std::endl;
        }

        // proceed with plan
        std::cout << "Calculating plan ... \n";
        _dispatch( plan, planningData );
    } 


    void _clearPreviousPlan( plPlan &plan )
    {
        // clear previous grafts
        PLuint previousCount = plan.grafts().size();
        for ( PLuint i=0; i < previousCount; i++ )
        {
            plan.removeGraft( 0 );
        }
    }


    // private functions
    void _dispatch(  plPlan &plan, const plPlanningBufferData &planningData )
    {    
        PLtime t0, t1;
        
        plDefectSolution defectSolution;
        plCapIndices     capIndices;
        plRmsData        rmsData;
        plDonorSolution  donorSolution;

        // stage 0 timing //
        std::cout << "\n--------------------------- Initiating Stage 0 --------------------------- \n" <<
                       "-------------------------------------------------------------------------- \n";
        t0 = plTimer::now();

        plPlannerStage0::run( defectSolution, planningData, plan );    
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 0 Complete --------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ----------------------" << std::endl;
        ////////////////////
        // stage 1 timing //
        std::cout << "\n--------------------------- Initiating Stage 1 --------------------------- \n" <<
                       "--------------------------------------------------------------------------" << std::endl;
        t0 = plTimer::now();

        plPlannerStage1::run( capIndices, planningData, defectSolution );
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 1 Complete ---------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ------------------------ \n";
        //////////////////// 
        // stage 2 timing //
        std::cout << "\n--------------------------- Initiating Stage 2 --------------------------- \n" <<
                       "--------------------------------------------------------------------------" << std::endl;
        t0 = plTimer::now();

        plPlannerStage2::run( rmsData, planningData, defectSolution, capIndices );
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 2 Complete ---------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ------------------------ \n";

        ////////////////////
        // stage 3 timing //
        std::cout << "\n--------------------------- Initiating Stage 3 --------------------------- \n" <<
                       "-------------------------------------------------------------------------- \n";
        t0 = plTimer::now();

        plPlannerStage3::run( donorSolution, planningData, defectSolution, rmsData );    
        
        t1 = plTimer::now();
        std::cout << "\n---------------------------- Stage 3 Complete --------------------------- \n" <<
                       "------------------------- Execution time: " << t1 - t0 << " ms ------------------------ \n";
        ////////////////////
           
        if ( donorSolution.graftPositions.size() > 0 )
        {
            for ( PLuint i=0; i < defectSolution.graftCount; i++ )
            {    
                /*
                // all graft origins generated from the planner shaders have their origins flush with the cartilage surface, these
                // must be changed to be flush with the bone surface for mouse drag editing to behave nicely
                       
                // ray cast from cartilage positions in negative direction of normal to get harvest bone position
                  
                plIntersection intersection = plan.models(0).bone.rayIntersect( originalHarvestOrigin, -originalHarvestY );   
                // set correct harvest origin to bone intersection point             
                plVector3 correctHarvestOrigin = intersection.point;            

                // calculate how thick the cartilage is ( the distance from the cartilage point to bone point )
                PLfloat cartilageThickness = ( correctHarvestOrigin - originalHarvestOrigin ).length();
            
                // ray cast from cartilage positions in negative direction of normal to get recipient bone position
                plVector3 originalRecipientOrigin( defectSolution.graftPositions[i].x,  defectSolution.graftPositions[i].y,   defectSolution.graftPositions[i].z );
                plVector3 originalRecipientY     ( defectSolution.graftNormals[i].x,    defectSolution.graftNormals[i].y,     defectSolution.graftNormals[i].z   );        
                plVector3 originalRecipientZ     ( donorSolution.graftZDirections[i].x, donorSolution.graftZDirections[i].y,  donorSolution.graftZDirections[i].z ); // use from donor state!
                plVector3 originalRecipientX = (originalRecipientY ^ originalRecipientZ).normalize();
                                               
                intersection = plan.models(0).bone.rayIntersect( originalRecipientOrigin, -originalRecipientY );      
                // set correct recipient origin to bone intersection point   
                plVector3 correctRecipientOrigin = intersection.point; 

                // get height offset so that cap is flush with cartilage surface, remember to subtract cartilage thickness 
                PLfloat recipientHeightOffset = ( correctRecipientOrigin - originalRecipientOrigin ).length() - cartilageThickness;
    
                plPlug recipient( 0, plan.models(0), plTransform( originalRecipientX, originalRecipientY, correctRecipientOrigin ) );
                plPlug harvest  ( 0, plan.models(0), plTransform( originalHarvestY,   correctHarvestOrigin   ) );
                */ 

                plVector3 originalHarvestOrigin( donorSolution.graftPositions[i] );
                plVector3 originalHarvestY     ( donorSolution.graftNormals[i]   );    
                plVector3 originalHaverstX     ( donorSolution.graftXAxes[i]     );   
                   
                plVector3 originalRecipientOrigin( defectSolution.graftPositions[i] );
                plVector3 originalRecipientY     ( defectSolution.graftNormals[i]   );        
                plVector3 originalRecipientX     ( (originalRecipientY ^ plVector3( 0, 0, 1 ) ).normalize() ); 
                
                plPlug harvest  ( 0, plan.models(0), plTransform( originalHaverstX ,  originalHarvestY,   originalHarvestOrigin   ) );
                plPlug recipient( 0, plan.models(0), plTransform( originalRecipientX, originalRecipientY, originalRecipientOrigin ) );
                
                
                plan.addGraft( harvest, recipient, defectSolution.graftRadii[i], 0, 0 );

            } 
        }
        
        
        for ( PLuint i = 0; i < plan.defectSites().size(); i++ )
        {
            plan.defectSites(i).spline.setInvisible();
            plan.defectSites(i).boundary.setInvisible();
        }
        
        for ( PLuint i = 0; i < plan.donorSites().size(); i++ )
        {
            plan.donorSites(i).boundary.setInvisible();
        }
        
    }
 
}

