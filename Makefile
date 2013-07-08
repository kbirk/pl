LIBS = -Llib -lGLEW -lglut -lGLU -lGL -lm 
#LIBS = -framework OpenGL -framework GLUT -framework Cocoa

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES = main.cpp pl.cpp PlannerWindow.cpp plBoundary.cpp plBoundaryBase.cpp plBoundaryEditor.cpp \
	plBoundaryIntersection.cpp plCamera.cpp plColourMap.cpp plColourMesh.cpp plCSV.cpp plDefectSite.cpp \
	plDonorSite.cpp plDraw.cpp plEditable.cpp plGraft.cpp plGraftBase.cpp plGraftEditor.cpp plIGuide.cpp plMath.cpp plMatrix44.cpp \
	plMesh.cpp plModel.cpp plModelBase.cpp plPickingTexture.cpp plPlan.cpp plProjection.cpp plRenderable.cpp \
	plSpline.cpp plString.cpp plTransform.cpp plTriangle.cpp plVector3.cpp \
	plVector4.cpp Window.cpp
       
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
objects/PlannerWindow.o: plRenderable.h plVector3.h plPickingShader.h
objects/PlannerWindow.o: plShader.h plMatrix44.h plVector4.h plTriangle.h
objects/PlannerWindow.o: plString.h plSeq.h plPickingTexture.h plModelBase.h
objects/PlannerWindow.o: plMesh.h plPlan.h plCSV.h plGraft.h plEditable.h
objects/PlannerWindow.o: plGraftBase.h plTransform.h plBoundary.h
objects/PlannerWindow.o: plBoundaryBase.h plDefectSite.h plSpline.h
objects/PlannerWindow.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/PlannerWindow.o: plIGuide.h plGraftEditor.h plBoundaryEditor.h
objects/PlannerWindow.o: plProjection.h
objects/Window.o: pl.h plCommon.h
objects/pl.o: plCommon.h
objects/plBoundary.o: pl.h plCommon.h plBoundaryBase.h plSeq.h plVector3.h
objects/plBoundary.o: plRenderable.h plEditable.h plPickingShader.h
objects/plBoundary.o: plShader.h plMatrix44.h plVector4.h plTriangle.h
objects/plBoundary.o: plString.h plPickingTexture.h plMesh.h
objects/plBoundaryBase.o: pl.h plCommon.h plSeq.h plVector3.h
objects/plBoundaryEditor.o: pl.h plCommon.h plSeq.h plVector3.h plPlan.h
objects/plBoundaryEditor.o: plString.h plCSV.h plRenderable.h plGraft.h
objects/plBoundaryEditor.o: plEditable.h plGraftBase.h plTriangle.h plModel.h
objects/plBoundaryEditor.o: plPickingShader.h plShader.h plMatrix44.h
objects/plBoundaryEditor.o: plVector4.h plPickingTexture.h plCamera.h
objects/plBoundaryEditor.o: plModelBase.h plMesh.h plTransform.h plBoundary.h
objects/plBoundaryEditor.o: plBoundaryBase.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuide.h
objects/plBoundaryIntersection.o: plCommon.h plVector3.h pl.h plSeq.h
objects/plCSV.o: pl.h plCommon.h plSeq.h plString.h
objects/plCamera.o: pl.h plCommon.h plModel.h plRenderable.h plVector3.h
objects/plCamera.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plCamera.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/plCamera.o: plCamera.h plModelBase.h plMesh.h
objects/plColourMap.o: pl.h plCommon.h plVector3.h
objects/plColourMesh.o: pl.h plCommon.h plVector3.h plMesh.h plTriangle.h
objects/plColourMesh.o: plString.h plSeq.h plShader.h
objects/plComputeShader.o: pl.h plCommon.h plShader.h
objects/plDefectSite.o: pl.h plCommon.h plVector3.h plSpline.h plVector4.h
objects/plDefectSite.o: plBoundary.h plBoundaryBase.h plSeq.h plRenderable.h
objects/plDefectSite.o: plEditable.h plPickingShader.h plShader.h
objects/plDefectSite.o: plMatrix44.h plTriangle.h plString.h
objects/plDefectSite.o: plPickingTexture.h plMesh.h plColourMesh.h
objects/plDefectSite.o: plColourMap.h plModel.h plCamera.h plModelBase.h
objects/plDonorSite.o: pl.h plCommon.h plVector3.h plBoundary.h
objects/plDonorSite.o: plBoundaryBase.h plSeq.h plRenderable.h plEditable.h
objects/plDonorSite.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plDonorSite.o: plTriangle.h plString.h plPickingTexture.h plMesh.h
objects/plDraw.o: pl.h plCommon.h plProjection.h plMatrix44.h plVector3.h
objects/plDraw.o: plVector4.h plTriangle.h plString.h plSeq.h
objects/plDraw.o: plPickingTexture.h plCamera.h plModel.h plRenderable.h
objects/plDraw.o: plPickingShader.h plShader.h plModelBase.h plMesh.h
objects/plDraw.o: plPlan.h plCSV.h plGraft.h plEditable.h plGraftBase.h
objects/plDraw.o: plTransform.h plBoundary.h plBoundaryBase.h plDefectSite.h
objects/plDraw.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/plDraw.o: plIGuide.h plMinimalShader.h plGraftEditor.h
objects/plDraw.o: plBoundaryEditor.h
objects/plEditable.o: pl.h plCommon.h plVector3.h
objects/plGraft.o: pl.h plCommon.h plRenderable.h plVector3.h plEditable.h
objects/plGraft.o: plGraftBase.h plTriangle.h plString.h plSeq.h plModel.h
objects/plGraft.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plGraft.o: plPickingTexture.h plCamera.h plModelBase.h plMesh.h
objects/plGraft.o: plTransform.h
objects/plGraftBase.o: pl.h plCommon.h plVector3.h plTriangle.h plString.h
objects/plGraftBase.o: plSeq.h plModel.h plRenderable.h plPickingShader.h
objects/plGraftBase.o: plShader.h plMatrix44.h plVector4.h plPickingTexture.h
objects/plGraftBase.o: plCamera.h plModelBase.h plMesh.h plTransform.h
objects/plGraftEditor.o: pl.h plCommon.h plSeq.h plVector3.h plTransform.h
objects/plGraftEditor.o: plPlan.h plString.h plCSV.h plRenderable.h plGraft.h
objects/plGraftEditor.o: plEditable.h plGraftBase.h plTriangle.h plModel.h
objects/plGraftEditor.o: plPickingShader.h plShader.h plMatrix44.h
objects/plGraftEditor.o: plVector4.h plPickingTexture.h plCamera.h
objects/plGraftEditor.o: plModelBase.h plMesh.h plBoundary.h plBoundaryBase.h
objects/plGraftEditor.o: plDefectSite.h plSpline.h plColourMesh.h
objects/plGraftEditor.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plIGuide.o: pl.h plCommon.h plSeq.h plVector3.h plVector4.h
objects/plIGuide.o: plBoundary.h plBoundaryBase.h plRenderable.h plEditable.h
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
objects/plModel.o: pl.h plCommon.h plRenderable.h plVector3.h
objects/plModel.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plModel.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/plModel.o: plCamera.h plModel.h plModelBase.h plMesh.h
objects/plModelBase.o: pl.h plCommon.h plSeq.h plTriangle.h plString.h
objects/plModelBase.o: plVector3.h plMesh.h plShader.h
objects/plPickingShader.o: pl.h plCommon.h plShader.h plMatrix44.h
objects/plPickingShader.o: plVector3.h plVector4.h plTriangle.h plString.h
objects/plPickingShader.o: plSeq.h plPickingTexture.h
objects/plPickingTexture.o: pl.h plCommon.h
objects/plPlan.o: pl.h plCommon.h plString.h plSeq.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plGraft.h plEditable.h plGraftBase.h
objects/plPlan.o: plTriangle.h plModel.h plPickingShader.h plShader.h
objects/plPlan.o: plMatrix44.h plVector4.h plPickingTexture.h plCamera.h
objects/plPlan.o: plModelBase.h plMesh.h plTransform.h plBoundary.h
objects/plPlan.o: plBoundaryBase.h plDefectSite.h plSpline.h plColourMesh.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plProjection.o: pl.h plCommon.h plMatrix44.h plVector3.h plVector4.h
objects/plProjection.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/plRenderable.o: pl.h plCommon.h plVector3.h
objects/plSeq.o: pl.h plCommon.h
objects/plShader.o: pl.h plCommon.h
objects/plSpline.o: pl.h plCommon.h plVector3.h plVector4.h plBoundary.h
objects/plSpline.o: plBoundaryBase.h plSeq.h plRenderable.h plEditable.h
objects/plSpline.o: plPickingShader.h plShader.h plMatrix44.h plTriangle.h
objects/plSpline.o: plString.h plPickingTexture.h plMesh.h plColourMesh.h
objects/plSpline.o: plColourMap.h plModel.h plCamera.h plModelBase.h
objects/plString.o: pl.h plCommon.h
objects/plTransform.o: pl.h plCommon.h plVector3.h
objects/plTriangle.o: pl.h plCommon.h plString.h plVector3.h plSeq.h
objects/plVector3.o: pl.h plCommon.h
objects/plVector4.o: pl.h plCommon.h plVector3.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h plCamera.h
objects/PlannerWindow.o: plModel.h plRenderable.h plVector3.h
objects/PlannerWindow.o: plPickingShader.h plShader.h plMatrix44.h
objects/PlannerWindow.o: plVector4.h plTriangle.h plString.h plSeq.h
objects/PlannerWindow.o: plPickingTexture.h plModelBase.h plMesh.h plPlan.h
objects/PlannerWindow.o: plCSV.h plGraft.h plEditable.h plGraftBase.h
objects/PlannerWindow.o: plTransform.h plBoundary.h plBoundaryBase.h
objects/PlannerWindow.o: plDefectSite.h plSpline.h plColourMesh.h
objects/PlannerWindow.o: plColourMap.h plDonorSite.h plIGuide.h
objects/PlannerWindow.o: plGraftEditor.h plBoundaryEditor.h plProjection.h
objects/Window.o: Window.h pl.h plCommon.h
objects/main.o: pl.h plCommon.h PlannerWindow.h Window.h plCamera.h plModel.h
objects/main.o: plRenderable.h plVector3.h plPickingShader.h plShader.h
objects/main.o: plMatrix44.h plVector4.h plTriangle.h plString.h plSeq.h
objects/main.o: plPickingTexture.h plModelBase.h plMesh.h plPlan.h plCSV.h
objects/main.o: plGraft.h plEditable.h plGraftBase.h plTransform.h
objects/main.o: plBoundary.h plBoundaryBase.h plDefectSite.h plSpline.h
objects/main.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuide.h
objects/main.o: plGraftEditor.h plBoundaryEditor.h plProjection.h
objects/pl.o: pl.h plCommon.h plCamera.h plModel.h plRenderable.h plVector3.h
objects/pl.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/pl.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/pl.o: plModelBase.h plMesh.h plProjection.h plMatrixStack.h plPlan.h
objects/pl.o: plCSV.h plGraft.h plEditable.h plGraftBase.h plTransform.h
objects/pl.o: plBoundary.h plBoundaryBase.h plDefectSite.h plSpline.h
objects/pl.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuide.h plDraw.h
objects/pl.o: plMinimalShader.h plGraftEditor.h plBoundaryEditor.h
objects/pl.o: plComputeShader.h
objects/plBoundary.o: plBoundary.h pl.h plCommon.h plBoundaryBase.h plSeq.h
objects/plBoundary.o: plVector3.h plRenderable.h plEditable.h
objects/plBoundary.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plBoundary.o: plTriangle.h plString.h plPickingTexture.h plMesh.h
objects/plBoundaryBase.o: plBoundaryBase.h pl.h plCommon.h plSeq.h
objects/plBoundaryBase.o: plVector3.h
objects/plBoundaryEditor.o: plBoundaryEditor.h pl.h plCommon.h plSeq.h
objects/plBoundaryEditor.o: plVector3.h plPlan.h plString.h plCSV.h
objects/plBoundaryEditor.o: plRenderable.h plGraft.h plEditable.h
objects/plBoundaryEditor.o: plGraftBase.h plTriangle.h plModel.h
objects/plBoundaryEditor.o: plPickingShader.h plShader.h plMatrix44.h
objects/plBoundaryEditor.o: plVector4.h plPickingTexture.h plCamera.h
objects/plBoundaryEditor.o: plModelBase.h plMesh.h plTransform.h plBoundary.h
objects/plBoundaryEditor.o: plBoundaryBase.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuide.h
objects/plBoundaryIntersection.o: plBoundaryIntersection.h plCommon.h
objects/plBoundaryIntersection.o: plVector3.h pl.h plSeq.h
objects/plCSV.o: plCSV.h pl.h plCommon.h plSeq.h plString.h
objects/plCamera.o: plCamera.h pl.h plCommon.h plModel.h plRenderable.h
objects/plCamera.o: plVector3.h plPickingShader.h plShader.h plMatrix44.h
objects/plCamera.o: plVector4.h plTriangle.h plString.h plSeq.h
objects/plCamera.o: plPickingTexture.h plModelBase.h plMesh.h
objects/plColourMap.o: plColourMap.h pl.h plCommon.h plVector3.h
objects/plColourMesh.o: plColourMesh.h pl.h plCommon.h plVector3.h plMesh.h
objects/plColourMesh.o: plTriangle.h plString.h plSeq.h plShader.h
objects/plDefectSite.o: plDefectSite.h pl.h plCommon.h plVector3.h plSpline.h
objects/plDefectSite.o: plVector4.h plBoundary.h plBoundaryBase.h plSeq.h
objects/plDefectSite.o: plRenderable.h plEditable.h plPickingShader.h
objects/plDefectSite.o: plShader.h plMatrix44.h plTriangle.h plString.h
objects/plDefectSite.o: plPickingTexture.h plMesh.h plColourMesh.h
objects/plDefectSite.o: plColourMap.h plModel.h plCamera.h plModelBase.h
objects/plDonorSite.o: plDonorSite.h pl.h plCommon.h plVector3.h plBoundary.h
objects/plDonorSite.o: plBoundaryBase.h plSeq.h plRenderable.h plEditable.h
objects/plDonorSite.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plDonorSite.o: plTriangle.h plString.h plPickingTexture.h plMesh.h
objects/plDraw.o: plDraw.h pl.h plCommon.h plProjection.h plMatrix44.h
objects/plDraw.o: plVector3.h plVector4.h plTriangle.h plString.h plSeq.h
objects/plDraw.o: plPickingTexture.h plCamera.h plModel.h plRenderable.h
objects/plDraw.o: plPickingShader.h plShader.h plModelBase.h plMesh.h
objects/plDraw.o: plPlan.h plCSV.h plGraft.h plEditable.h plGraftBase.h
objects/plDraw.o: plTransform.h plBoundary.h plBoundaryBase.h plDefectSite.h
objects/plDraw.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/plDraw.o: plIGuide.h plMinimalShader.h plGraftEditor.h
objects/plDraw.o: plBoundaryEditor.h
objects/plEditable.o: plEditable.h pl.h plCommon.h plVector3.h
objects/plGraft.o: plGraft.h pl.h plCommon.h plRenderable.h plVector3.h
objects/plGraft.o: plEditable.h plGraftBase.h plTriangle.h plString.h plSeq.h
objects/plGraft.o: plModel.h plPickingShader.h plShader.h plMatrix44.h
objects/plGraft.o: plVector4.h plPickingTexture.h plCamera.h plModelBase.h
objects/plGraft.o: plMesh.h plTransform.h
objects/plGraftBase.o: plGraftBase.h pl.h plCommon.h plVector3.h plTriangle.h
objects/plGraftBase.o: plString.h plSeq.h plModel.h plRenderable.h
objects/plGraftBase.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plGraftBase.o: plPickingTexture.h plCamera.h plModelBase.h plMesh.h
objects/plGraftBase.o: plTransform.h
objects/plGraftEditor.o: plGraftEditor.h pl.h plCommon.h plSeq.h plVector3.h
objects/plGraftEditor.o: plTransform.h plPlan.h plString.h plCSV.h
objects/plGraftEditor.o: plRenderable.h plGraft.h plEditable.h plGraftBase.h
objects/plGraftEditor.o: plTriangle.h plModel.h plPickingShader.h plShader.h
objects/plGraftEditor.o: plMatrix44.h plVector4.h plPickingTexture.h
objects/plGraftEditor.o: plCamera.h plModelBase.h plMesh.h plBoundary.h
objects/plGraftEditor.o: plBoundaryBase.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuide.h
objects/plIGuide.o: plIGuide.h pl.h plCommon.h plSeq.h plVector3.h
objects/plIGuide.o: plVector4.h plBoundary.h plBoundaryBase.h plRenderable.h
objects/plIGuide.o: plEditable.h plPickingShader.h plShader.h plMatrix44.h
objects/plIGuide.o: plTriangle.h plString.h plPickingTexture.h plMesh.h
objects/plMath.o: plMath.h pl.h plCommon.h plVector3.h
objects/plMatrix44.o: plMatrix44.h pl.h plCommon.h plVector3.h plVector4.h
objects/plMatrix44.o: plTriangle.h plString.h plSeq.h
objects/plMesh.o: plMesh.h pl.h plCommon.h plVector3.h plTriangle.h
objects/plMesh.o: plString.h plSeq.h plShader.h
objects/plModel.o: plModel.h pl.h plCommon.h plRenderable.h plVector3.h
objects/plModel.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plModel.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/plModel.o: plCamera.h plModelBase.h plMesh.h
objects/plModelBase.o: plModelBase.h pl.h plCommon.h plSeq.h plTriangle.h
objects/plModelBase.o: plString.h plVector3.h plMesh.h plShader.h
objects/plPickingTexture.o: plPickingTexture.h pl.h plCommon.h
objects/plPlan.o: plPlan.h pl.h plCommon.h plString.h plSeq.h plVector3.h
objects/plPlan.o: plCSV.h plRenderable.h plGraft.h plEditable.h plGraftBase.h
objects/plPlan.o: plTriangle.h plModel.h plPickingShader.h plShader.h
objects/plPlan.o: plMatrix44.h plVector4.h plPickingTexture.h plCamera.h
objects/plPlan.o: plModelBase.h plMesh.h plTransform.h plBoundary.h
objects/plPlan.o: plBoundaryBase.h plDefectSite.h plSpline.h plColourMesh.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plProjection.o: plProjection.h pl.h plCommon.h plMatrix44.h
objects/plProjection.o: plVector3.h plVector4.h plTriangle.h plString.h
objects/plProjection.o: plSeq.h plPickingTexture.h
objects/plRenderable.o: plRenderable.h pl.h plCommon.h plVector3.h
objects/plSpline.o: plSpline.h pl.h plCommon.h plVector3.h plVector4.h
objects/plSpline.o: plBoundary.h plBoundaryBase.h plSeq.h plRenderable.h
objects/plSpline.o: plEditable.h plPickingShader.h plShader.h plMatrix44.h
objects/plSpline.o: plTriangle.h plString.h plPickingTexture.h plMesh.h
objects/plSpline.o: plColourMesh.h plColourMap.h plModel.h plCamera.h
objects/plSpline.o: plModelBase.h
objects/plString.o: plString.h pl.h plCommon.h
objects/plTransform.o: plTransform.h pl.h plCommon.h plVector3.h
objects/plTriangle.o: plTriangle.h pl.h plCommon.h plString.h plVector3.h
objects/plTriangle.o: plSeq.h
objects/plVector3.o: plVector3.h pl.h plCommon.h
objects/plVector4.o: plVector4.h pl.h plCommon.h plVector3.h
