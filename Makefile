
OPENGL_DRIVER_DIR = /usr/lib/nvidia-331

# LINUX
LIBS = -L$(OPENGL_DRIVER_DIR) -lglut -lGL

# OSX
#LIBS = -framework GLUT -framework OpenGL -framework Cocoa

INCS =

CXXFLAGS = -Wfatal-errors -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst src/%.cpp,objects/%.o,$(SOURCES))

SOURCES =   src/main.cpp \
            src/PlannerWindow.cpp \
            src/pl.cpp \
            src/plArthroCamTechnique.cpp \
            src/plArthroViewable.cpp \
            src/plAutomaticPlanner.cpp \
            src/plBoneAndCartilage.cpp \
            src/plBoundary.cpp \
            src/plBoundaryEditor.cpp \
            src/plBufferObject.cpp \
            src/plCamera.cpp \
            src/plChessBoard.cpp \
            src/plColourMap.cpp \
            src/plCSV.cpp \
            src/plDefectSite.cpp \
            src/plDonorSite.cpp \
            src/plDRBTransform.cpp \
            src/plEABO.cpp \
            src/plEditable.cpp \
            src/plEditor.cpp \
            src/plFBO.cpp \
            src/plGraft.cpp \
            src/plGraftCap.cpp \
            src/plGraftEditor.cpp \
            src/plIGuide.cpp \
            src/plIGuideSite.cpp \
            src/plMath.cpp \
            src/plMatrix44.cpp \
            src/plMatrixStack.cpp \
            src/plMesh.cpp \
            src/plMeshAlgorithm.cpp \
            src/plMeshConnectivityData.cpp \
            src/plMeshCutter.cpp \
            src/plMeshExtruder.cpp \
            src/plMeshIntersector.cpp \
            src/plMeshSpecific.cpp \
            src/plMinimalTechnique.cpp \
            src/plModel.cpp \
            src/plModelEditor.cpp \
            src/plOctree.cpp \
            src/plOctreeMesh.cpp \
            src/plOpenGLInfo.cpp \
            src/plOutlineTechnique.cpp \
            src/plPicking.cpp \
            src/plPlan.cpp \
            src/plPlannerShader.cpp \
            src/plPlannerStage0.cpp \
            src/plPlannerStage1.cpp \
            src/plPlannerStage2.cpp \
            src/plPlannerStage3.cpp \
            src/plPlannerStructs.cpp \
            src/plPlanningBufferData.cpp \
            src/plPlanningSite.cpp \
            src/plPlanTechnique.cpp \
            src/plPlug.cpp \
            src/plPolygon.cpp \
            src/plProjection.cpp \
            src/plRenderComponent.cpp \
            src/plRenderResources.cpp \
            src/plRenderShapes.cpp \
            src/plRenderTechnique.cpp \
            src/plRenderable.cpp \
            src/plRenderer.cpp \
            src/plRenderingPipeline.cpp \
            src/plScan.cpp \
            src/plScanPoint.cpp \
            src/plScanVolume.cpp \
            src/plScreenQuadTechnique.cpp \
            src/plShader.cpp \
            src/plSpline.cpp \
            src/plSSBO.cpp \
            src/plString.cpp \
            src/plTexture2D.cpp \
            src/plTexture3D.cpp \
            src/plTimer.cpp \
            src/plTrackedObject.cpp \
            src/plTransform.cpp \
            src/plTransparencyTechnique.cpp \
            src/plTransparentRenderable.cpp \
            src/plTriangle.cpp \
            src/plUtility.cpp \
            src/plUniform.cpp \
            src/plVAO.cpp \
            src/plVBO.cpp \
            src/plVector3.cpp \
            src/plVector4.cpp \
            src/plVertexFragmentShader.cpp \
            src/plVertexSpecBuffer.cpp \
            src/plWindow.cpp \
            src/Window.cpp

EXE = planner

Window:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJECTS) $(LIBS)

objects:
	@mkdir -p $@

objects/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f objects/*.o *~ core $(EXE) Makefile.bak

depend:
	makedepend -Y -pobjects/ *.h *.cpp

# DO NOT DELETE
