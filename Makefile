LIBS = -Llib -lGLEW -lglut -lGLU -lGL -lm 
#LIBS = -framework OpenGL -framework GLUT -framework Cocoa

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES = main.cpp PlannerWindow.cpp plBoneAndCartilage.cpp plBoundary.cpp plBoundaryEditor.cpp \
	plBoundaryIntersection.cpp plCamera.cpp plColourMap.cpp plColourMesh.cpp plCSV.cpp plDefectSite.cpp \
	plDonorSite.cpp plDraw.cpp plDRBTransform.cpp plEditable.cpp plGraft.cpp plGraftEditor.cpp plIGuide.cpp plMath.cpp plMatrix44.cpp \
	plMatrixStack.cpp plMesh.cpp plModel.cpp plModelSpecific.cpp plPickingTexture.cpp plPlan.cpp plPlug.cpp plPolygon.cpp plProjection.cpp plRenderable.cpp \
	plRenderer.cpp plRenderingPipeline.cpp plSpline.cpp plString.cpp plTrackedObject.cpp plTransform.cpp plTriangle.cpp plVector3.cpp \
	plVector4.cpp plWindow.cpp Window.cpp
       
EXE  = planner

Window:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJECTS) $(LIBS)

$(OBJECTS): | objects

objects:
	@mkdir -p $@

objects/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f objects/*.o *~ core $(EXE) Makefile.bak

depend:	
	makedepend -Y -pobjects/ *.h *.cpp

# DO NOT DELETE

objects/PlannerWindow.o: Window.h pl.h plCommon.h plCamera.h plModel.h
objects/PlannerWindow.o: plSeq.h plRenderable.h plVector3.h plTriangle.h
objects/PlannerWindow.o: plString.h plPickingShader.h plShader.h plMatrix44.h
objects/PlannerWindow.o: plVector4.h plPickingTexture.h plMesh.h plPlan.h
objects/PlannerWindow.o: plCSV.h plGraft.h plEditable.h plTransform.h
objects/PlannerWindow.o: plPolygon.h plBoundary.h plDefectSite.h plSpline.h
objects/PlannerWindow.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/PlannerWindow.o: plIGuide.h plGraftEditor.h plBoundaryEditor.h
objects/PlannerWindow.o: plProjection.h
objects/Window.o: pl.h plCommon.h
objects/pl.o: plCommon.h
objects/plBoundary.o: pl.h plCommon.h plSeq.h plVector3.h plRenderable.h
objects/plBoundary.o: plEditable.h plPickingShader.h plShader.h plMatrix44.h
objects/plBoundary.o: plVector4.h plTriangle.h plString.h plPickingTexture.h
objects/plBoundary.o: plMesh.h
objects/plBoundaryEditor.o: pl.h plCommon.h plSeq.h plVector3.h plPlan.h
objects/plBoundaryEditor.o: plString.h plCSV.h plRenderable.h plGraft.h
objects/plBoundaryEditor.o: plEditable.h plTriangle.h plModel.h
objects/plBoundaryEditor.o: plPickingShader.h plShader.h plMatrix44.h
objects/plBoundaryEditor.o: plVector4.h plPickingTexture.h plMesh.h
objects/plBoundaryEditor.o: plCamera.h plTransform.h plPolygon.h plBoundary.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plColourMesh.h
objects/plBoundaryEditor.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plBoundaryIntersection.o: plCommon.h plVector3.h pl.h plSeq.h
objects/plBoundaryIntersection.o: plTriangle.h plString.h plPolygon.h
objects/plCSV.o: pl.h plCommon.h plSeq.h plString.h
objects/plCamera.o: pl.h plCommon.h plModel.h plSeq.h plRenderable.h
objects/plCamera.o: plVector3.h plTriangle.h plString.h plPickingShader.h
objects/plCamera.o: plShader.h plMatrix44.h plVector4.h plPickingTexture.h
objects/plCamera.o: plMesh.h plCamera.h
objects/plColourMap.o: pl.h plCommon.h plVector3.h
objects/plColourMesh.o: pl.h plCommon.h plVector3.h plMesh.h plTriangle.h
objects/plColourMesh.o: plString.h plSeq.h plShader.h
objects/plComputeShader.o: pl.h plCommon.h plShader.h
objects/plDefectSite.o: pl.h plCommon.h plVector3.h plSpline.h plVector4.h
objects/plDefectSite.o: plBoundary.h plSeq.h plRenderable.h plEditable.h
objects/plDefectSite.o: plPickingShader.h plShader.h plMatrix44.h
objects/plDefectSite.o: plTriangle.h plString.h plPickingTexture.h plMesh.h
objects/plDefectSite.o: plColourMesh.h plColourMap.h plModel.h plCamera.h
objects/plDonorSite.o: pl.h plCommon.h plVector3.h plBoundary.h plSeq.h
objects/plDonorSite.o: plRenderable.h plEditable.h plPickingShader.h
objects/plDonorSite.o: plShader.h plMatrix44.h plVector4.h plTriangle.h
objects/plDonorSite.o: plString.h plPickingTexture.h plMesh.h
objects/plDraw.o: pl.h plCommon.h plProjection.h plMatrix44.h plVector3.h
objects/plDraw.o: plVector4.h plTriangle.h plString.h plSeq.h
objects/plDraw.o: plPickingTexture.h plCamera.h plModel.h plRenderable.h
objects/plDraw.o: plPickingShader.h plShader.h plMesh.h plPlan.h plCSV.h
objects/plDraw.o: plGraft.h plEditable.h plTransform.h plPolygon.h
objects/plDraw.o: plBoundary.h plDefectSite.h plSpline.h plColourMesh.h
objects/plDraw.o: plColourMap.h plDonorSite.h plIGuide.h plMinimalShader.h
objects/plDraw.o: plGraftEditor.h plBoundaryEditor.h
objects/plEditable.o: pl.h plCommon.h plVector3.h
objects/plGraft.o: pl.h plCommon.h plRenderable.h plVector3.h plEditable.h
objects/plGraft.o: plTriangle.h plString.h plSeq.h plModel.h
objects/plGraft.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plGraft.o: plPickingTexture.h plMesh.h plCamera.h plTransform.h
objects/plGraft.o: plPlan.h plCSV.h plGraft.h plPolygon.h plBoundary.h
objects/plGraft.o: plDefectSite.h plSpline.h plColourMesh.h plColourMap.h
objects/plGraft.o: plDonorSite.h plIGuide.h
objects/plGraftEditor.o: pl.h plCommon.h plSeq.h plVector3.h plTransform.h
objects/plGraftEditor.o: plPlan.h plString.h plCSV.h plRenderable.h plGraft.h
objects/plGraftEditor.o: plEditable.h plTriangle.h plModel.h
objects/plGraftEditor.o: plPickingShader.h plShader.h plMatrix44.h
objects/plGraftEditor.o: plVector4.h plPickingTexture.h plMesh.h plCamera.h
objects/plGraftEditor.o: plPolygon.h plBoundary.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuide.h
objects/plIGuide.o: pl.h plCommon.h plSeq.h plVector3.h plVector4.h
objects/plIGuide.o: plBoundary.h plRenderable.h plEditable.h
objects/plIGuide.o: plPickingShader.h plShader.h plMatrix44.h plTriangle.h
objects/plIGuide.o: plString.h plPickingTexture.h plMesh.h
objects/plMath.o: pl.h plCommon.h plVector3.h
objects/plMatrix44.o: pl.h plCommon.h plVector3.h plVector4.h plTriangle.h
objects/plMatrix44.o: plString.h plSeq.h
objects/plMatrixStack.o: pl.h plCommon.h plMatrix44.h plVector3.h plVector4.h
objects/plMatrixStack.o: plTriangle.h plString.h plSeq.h
objects/plMesh.o: pl.h plCommon.h plVector3.h plTriangle.h plString.h plSeq.h
objects/plMesh.o: plShader.h
objects/plMinimalShader.o: pl.h plCommon.h plShader.h
objects/plModel.o: pl.h plCommon.h plSeq.h plRenderable.h plVector3.h
objects/plModel.o: plTriangle.h plString.h plPickingShader.h plShader.h
objects/plModel.o: plMatrix44.h plVector4.h plPickingTexture.h plMesh.h
objects/plModel.o: plCamera.h plModel.h
objects/plPickingShader.o: pl.h plCommon.h plShader.h plMatrix44.h
objects/plPickingShader.o: plVector3.h plVector4.h plTriangle.h plString.h
objects/plPickingShader.o: plSeq.h plPickingTexture.h
objects/plPickingTexture.o: pl.h plCommon.h
objects/plPlan.o: pl.h plCommon.h plString.h plSeq.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plGraft.h plEditable.h plTriangle.h
objects/plPlan.o: plModel.h plPickingShader.h plShader.h plMatrix44.h
objects/plPlan.o: plVector4.h plPickingTexture.h plMesh.h plCamera.h
objects/plPlan.o: plTransform.h plPlan.h plBoundary.h plDefectSite.h
objects/plPlan.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/plPlan.o: plIGuide.h plPolygon.h
objects/plPolygon.o: pl.h plCommon.h plSeq.h plVector3.h
objects/plProjection.o: pl.h plCommon.h plMatrix44.h plVector3.h plVector4.h
objects/plProjection.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/plRenderable.o: pl.h plCommon.h plVector3.h
objects/plSeq.o: pl.h plCommon.h
objects/plShader.o: pl.h plCommon.h
objects/plSpline.o: pl.h plCommon.h plVector3.h plVector4.h plBoundary.h
objects/plSpline.o: plSeq.h plRenderable.h plEditable.h plPickingShader.h
objects/plSpline.o: plShader.h plMatrix44.h plTriangle.h plString.h
objects/plSpline.o: plPickingTexture.h plMesh.h plColourMesh.h plColourMap.h
objects/plSpline.o: plModel.h plCamera.h
objects/plString.o: pl.h plCommon.h
objects/plTransform.o: pl.h plCommon.h plVector3.h
objects/plTriangle.o: pl.h plCommon.h plString.h plVector3.h plSeq.h
objects/plVector3.o: pl.h plCommon.h
objects/plVector4.o: pl.h plCommon.h plVector3.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h plCamera.h
objects/PlannerWindow.o: plModel.h plSeq.h plRenderable.h plVector3.h
objects/PlannerWindow.o: plTriangle.h plString.h plPickingShader.h plShader.h
objects/PlannerWindow.o: plMatrix44.h plVector4.h plPickingTexture.h plMesh.h
objects/PlannerWindow.o: plPlan.h plCSV.h plGraft.h plEditable.h
objects/PlannerWindow.o: plTransform.h plPolygon.h plBoundary.h
objects/PlannerWindow.o: plDefectSite.h plSpline.h plColourMesh.h
objects/PlannerWindow.o: plColourMap.h plDonorSite.h plIGuide.h
objects/PlannerWindow.o: plGraftEditor.h plBoundaryEditor.h plProjection.h
objects/Window.o: Window.h pl.h plCommon.h
objects/main.o: pl.h plCommon.h PlannerWindow.h Window.h plCamera.h plModel.h
objects/main.o: plSeq.h plRenderable.h plVector3.h plTriangle.h plString.h
objects/main.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/main.o: plPickingTexture.h plMesh.h plPlan.h plCSV.h plGraft.h
objects/main.o: plEditable.h plTransform.h plPolygon.h plBoundary.h
objects/main.o: plDefectSite.h plSpline.h plColourMesh.h plColourMap.h
objects/main.o: plDonorSite.h plIGuide.h plGraftEditor.h plBoundaryEditor.h
objects/main.o: plProjection.h
objects/pl.o: pl.h plCommon.h plCamera.h plModel.h plSeq.h plRenderable.h
objects/pl.o: plVector3.h plTriangle.h plString.h plPickingShader.h
objects/pl.o: plShader.h plMatrix44.h plVector4.h plPickingTexture.h plMesh.h
objects/pl.o: plProjection.h plMatrixStack.h plPlan.h plCSV.h plGraft.h
objects/pl.o: plEditable.h plTransform.h plPolygon.h plBoundary.h
objects/pl.o: plDefectSite.h plSpline.h plColourMesh.h plColourMap.h
objects/pl.o: plDonorSite.h plIGuide.h plDraw.h plMinimalShader.h
objects/pl.o: plGraftEditor.h plBoundaryEditor.h plComputeShader.h
objects/plBoundary.o: plBoundary.h pl.h plCommon.h plSeq.h plVector3.h
objects/plBoundary.o: plRenderable.h plEditable.h plPickingShader.h
objects/plBoundary.o: plShader.h plMatrix44.h plVector4.h plTriangle.h
objects/plBoundary.o: plString.h plPickingTexture.h plMesh.h
objects/plBoundaryEditor.o: plBoundaryEditor.h pl.h plCommon.h plSeq.h
objects/plBoundaryEditor.o: plVector3.h plPlan.h plString.h plCSV.h
objects/plBoundaryEditor.o: plRenderable.h plGraft.h plEditable.h
objects/plBoundaryEditor.o: plTriangle.h plModel.h plPickingShader.h
objects/plBoundaryEditor.o: plShader.h plMatrix44.h plVector4.h
objects/plBoundaryEditor.o: plPickingTexture.h plMesh.h plCamera.h
objects/plBoundaryEditor.o: plTransform.h plPolygon.h plBoundary.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plColourMesh.h
objects/plBoundaryEditor.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plBoundaryIntersection.o: plBoundaryIntersection.h plCommon.h
objects/plBoundaryIntersection.o: plVector3.h pl.h plSeq.h plTriangle.h
objects/plBoundaryIntersection.o: plString.h plPolygon.h
objects/plCSV.o: plCSV.h pl.h plCommon.h plSeq.h plString.h
objects/plCamera.o: plCamera.h pl.h plCommon.h plModel.h plSeq.h
objects/plCamera.o: plRenderable.h plVector3.h plTriangle.h plString.h
objects/plCamera.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plCamera.o: plPickingTexture.h plMesh.h
objects/plColourMap.o: plColourMap.h pl.h plCommon.h plVector3.h
objects/plColourMesh.o: plColourMesh.h pl.h plCommon.h plVector3.h plMesh.h
objects/plColourMesh.o: plTriangle.h plString.h plSeq.h plShader.h
objects/plDefectSite.o: plDefectSite.h pl.h plCommon.h plVector3.h plSpline.h
objects/plDefectSite.o: plVector4.h plBoundary.h plSeq.h plRenderable.h
objects/plDefectSite.o: plEditable.h plPickingShader.h plShader.h
objects/plDefectSite.o: plMatrix44.h plTriangle.h plString.h
objects/plDefectSite.o: plPickingTexture.h plMesh.h plColourMesh.h
objects/plDefectSite.o: plColourMap.h plModel.h plCamera.h
objects/plDonorSite.o: plDonorSite.h pl.h plCommon.h plVector3.h plBoundary.h
objects/plDonorSite.o: plSeq.h plRenderable.h plEditable.h plPickingShader.h
objects/plDonorSite.o: plShader.h plMatrix44.h plVector4.h plTriangle.h
objects/plDonorSite.o: plString.h plPickingTexture.h plMesh.h
objects/plDraw.o: plDraw.h pl.h plCommon.h plProjection.h plMatrix44.h
objects/plDraw.o: plVector3.h plVector4.h plTriangle.h plString.h plSeq.h
objects/plDraw.o: plPickingTexture.h plCamera.h plModel.h plRenderable.h
objects/plDraw.o: plPickingShader.h plShader.h plMesh.h plPlan.h plCSV.h
objects/plDraw.o: plGraft.h plEditable.h plTransform.h plPolygon.h
objects/plDraw.o: plBoundary.h plDefectSite.h plSpline.h plColourMesh.h
objects/plDraw.o: plColourMap.h plDonorSite.h plIGuide.h plMinimalShader.h
objects/plDraw.o: plGraftEditor.h plBoundaryEditor.h
objects/plEditable.o: plEditable.h pl.h plCommon.h plVector3.h
objects/plGraft.o: plGraft.h pl.h plCommon.h plRenderable.h plVector3.h
objects/plGraft.o: plEditable.h plTriangle.h plString.h plSeq.h plModel.h
objects/plGraft.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plGraft.o: plPickingTexture.h plMesh.h plCamera.h plTransform.h
objects/plGraft.o: plPlan.h plCSV.h plBoundary.h plDefectSite.h plSpline.h
objects/plGraft.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuide.h
objects/plGraft.o: plPolygon.h
objects/plGraftEditor.o: plGraftEditor.h pl.h plCommon.h plSeq.h plVector3.h
objects/plGraftEditor.o: plTransform.h plPlan.h plString.h plCSV.h
objects/plGraftEditor.o: plRenderable.h plGraft.h plEditable.h plTriangle.h
objects/plGraftEditor.o: plModel.h plPickingShader.h plShader.h plMatrix44.h
objects/plGraftEditor.o: plVector4.h plPickingTexture.h plMesh.h plCamera.h
objects/plGraftEditor.o: plPolygon.h plBoundary.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuide.h
objects/plIGuide.o: plIGuide.h pl.h plCommon.h plSeq.h plVector3.h
objects/plIGuide.o: plVector4.h plBoundary.h plRenderable.h plEditable.h
objects/plIGuide.o: plPickingShader.h plShader.h plMatrix44.h plTriangle.h
objects/plIGuide.o: plString.h plPickingTexture.h plMesh.h
objects/plMath.o: plMath.h pl.h plCommon.h plVector3.h
objects/plMatrix44.o: plMatrix44.h pl.h plCommon.h plVector3.h plVector4.h
objects/plMatrix44.o: plTriangle.h plString.h plSeq.h
objects/plMesh.o: plMesh.h pl.h plCommon.h plVector3.h plTriangle.h
objects/plMesh.o: plString.h plSeq.h plShader.h
objects/plModel.o: plModel.h pl.h plCommon.h plSeq.h plRenderable.h
objects/plModel.o: plVector3.h plTriangle.h plString.h plPickingShader.h
objects/plModel.o: plShader.h plMatrix44.h plVector4.h plPickingTexture.h
objects/plModel.o: plMesh.h plCamera.h
objects/plPickingTexture.o: plPickingTexture.h pl.h plCommon.h
objects/plPlan.o: plPlan.h pl.h plCommon.h plString.h plSeq.h plVector3.h
objects/plPlan.o: plCSV.h plRenderable.h plGraft.h plEditable.h plTriangle.h
objects/plPlan.o: plModel.h plPickingShader.h plShader.h plMatrix44.h
objects/plPlan.o: plVector4.h plPickingTexture.h plMesh.h plCamera.h
objects/plPlan.o: plTransform.h plPolygon.h plBoundary.h plDefectSite.h
objects/plPlan.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/plPlan.o: plIGuide.h
objects/plPolygon.o: plPolygon.h pl.h plCommon.h plSeq.h plVector3.h
objects/plProjection.o: plProjection.h pl.h plCommon.h plMatrix44.h
objects/plProjection.o: plVector3.h plVector4.h plTriangle.h plString.h
objects/plProjection.o: plSeq.h plPickingTexture.h
objects/plRenderable.o: plRenderable.h pl.h plCommon.h plVector3.h
objects/plSpline.o: plSpline.h pl.h plCommon.h plVector3.h plVector4.h
objects/plSpline.o: plBoundary.h plSeq.h plRenderable.h plEditable.h
objects/plSpline.o: plPickingShader.h plShader.h plMatrix44.h plTriangle.h
objects/plSpline.o: plString.h plPickingTexture.h plMesh.h plColourMesh.h
objects/plSpline.o: plColourMap.h plModel.h plCamera.h
objects/plString.o: plString.h pl.h plCommon.h
objects/plTransform.o: plTransform.h pl.h plCommon.h plVector3.h
objects/plTriangle.o: plTriangle.h pl.h plCommon.h plString.h plVector3.h
objects/plTriangle.o: plSeq.h
objects/plVector3.o: plVector3.h pl.h plCommon.h
objects/plVector4.o: plVector4.h pl.h plCommon.h plVector3.h
