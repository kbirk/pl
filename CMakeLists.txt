cmake_minimum_required(VERSION 2.8)
project(pl)

message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_CXX_COMPILER_ID = ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")

# CMAKE extensions
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${pl_SOURCE_DIR}/cmake")

# Required packages
find_package(Epoxy REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Warning pedantic flags for all
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -Wfatal-errors -O2 ${CMAKE_CXX_FLAGS}")

# Dependencies and include locations
include_directories("include")
include_directories(${EPOXY_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLUT_INCLUDE_DIR})

## Build Executable

# Add source files
set(sources
	"src/PlannerWindow"
	"src/plArthroCamTechnique"
	"src/plArthroViewable"
	"src/plAutomaticPlanner"
	"src/plBoneAndCartilage"
	"src/plBoundary"
	"src/plBoundaryEditor"
	"src/plBufferObject"
	"src/plCamera"
	"src/plChessBoard"
	"src/plColourMap"
	"src/pl"
	"src/plCSV"
	"src/plDefectSite"
	"src/plDonorSite"
	"src/plDRBTransform"
	"src/plEABO"
	"src/plEditable"
	"src/plEditor"
	"src/plFBO"
	"src/plGraftCap"
	"src/plGraft"
	"src/plGraftEditor"
	"src/plIGuide"
	"src/plIGuideSite"
	"src/plLaserLine"
	"src/plMath"
	"src/plMatrix44"
	"src/plMatrixStack"
	"src/plMeshAlgorithm"
	"src/plMeshConnectivityData"
	"src/plMesh"
	"src/plMeshCutter"
	"src/plMeshExtruder"
	"src/plMeshIntersector"
	"src/plMeshSpecific"
	"src/plMinimalTechnique"
	"src/plModel"
	"src/plModelEditor"
	"src/plOctree"
	"src/plOctreeMesh"
	"src/plOpenGLInfo"
	"src/plOutlineTechnique"
	"src/plPicking"
	"src/plPlan"
	"src/plPlannerShader"
	"src/plPlannerStage0"
	"src/plPlannerStage1"
	"src/plPlannerStage2"
	"src/plPlannerStage3"
	"src/plPlannerStructs"
	"src/plPlanningBufferData"
	"src/plPlanningSite"
	"src/plPlanTechnique"
	"src/plPlug"
	"src/plPolygon"
	"src/plProjection"
	"src/plRenderable"
	"src/plRenderComponent"
	"src/plRenderer"
	"src/plRenderingPipeline"
	"src/plRenderResources"
	"src/plRenderShapes"
	"src/plRenderTechnique"
	"src/plScan"
	"src/plScanPoint"
	"src/plScanVolume"
	"src/plScreenQuadTechnique"
	"src/plShader"
	"src/plSpline"
	"src/plSSBO"
	"src/plString"
	"src/plTexture2D"
	"src/plTexture3D"
	"src/plTimer"
	"src/plTrackedObject"
	"src/plTransform"
	"src/plTransparencyTechnique"
	"src/plTransparentRenderable"
	"src/plTriangle"
	"src/plUniform"
	"src/plUtility"
	"src/plVAO"
	"src/plVBO"
	"src/plVector3"
	"src/plVector4"
	"src/plVertexFragmentShader"
	"src/plVertexSpecBuffer"
	"src/plWindow"
	"src/Window"
	"src/main")

# Construct the executable
add_executable(planner ${sources})

# Link the executable to  libraries
target_link_libraries(planner
	${EPOXY_LIBRARIES}
	${GLUT_LIBRARIES}
	${OPENGL_LIBRARIES})

# Copy resource dir to build
file(COPY
	${CMAKE_CURRENT_SOURCE_DIR}/resources/
	DESTINATION
	${CMAKE_CURRENT_BINARY_DIR}/resources/)
