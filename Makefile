LIBS = -Llib -lGLEW -lglut -lGLU -lGL -lm 
#LIBS = -framework OpenGL -framework GLUT -framework Cocoa

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES = ArcballWindow.cpp main.cpp pl.cpp PlannerWindow.cpp plBoundary.cpp \
	plBoundaryIntersection.cpp plCamera.cpp plColourMap.cpp plColourMesh.cpp plCSV.cpp \
	plDonorSite.cpp plDraw.cpp plError.cpp plGraft.cpp plIGuide.cpp plMath.cpp plMatrix44.cpp \
	plMesh.cpp plModel.cpp plPickingTexture.cpp plPlan.cpp plProjection.cpp plRenderable.cpp \
	plSpline.cpp plState.cpp plString.cpp plTransform.cpp plTriangle.cpp plVector3.cpp \
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

objects/ArcballWindow.o: Window.h pl.h plCommon.h plVector3.h plMatrix44.h
objects/ArcballWindow.o: plVector4.h plTriangle.h plString.h plSeq.h
objects/ArcballWindow.o: plCamera.h plModel.h plError.h plState.h
objects/ArcballWindow.o: plTransform.h plRenderable.h plPickingShader.h
objects/ArcballWindow.o: plShader.h plPickingTexture.h plMesh.h
objects/PlannerWindow.o: ArcballWindow.h Window.h pl.h plCommon.h plVector3.h
objects/PlannerWindow.o: plMatrix44.h plVector4.h plTriangle.h plString.h
objects/PlannerWindow.o: plSeq.h plCamera.h plModel.h plError.h plState.h
objects/PlannerWindow.o: plTransform.h plRenderable.h plPickingShader.h
objects/PlannerWindow.o: plShader.h plPickingTexture.h plMesh.h
objects/Window.o: pl.h plCommon.h
objects/pl.o: plCommon.h
objects/plBoundary.o: pl.h plCommon.h plSeq.h plVector3.h plRenderable.h
objects/plBoundary.o: plPickingShader.h plShader.h plMatrix44.h plVector4.h
objects/plBoundary.o: plTriangle.h plString.h plPickingTexture.h plState.h
objects/plBoundary.o: plTransform.h plMesh.h
objects/plBoundaryIntersection.o: plCommon.h plVector3.h pl.h plSeq.h
objects/plCSV.o: pl.h plCommon.h plSeq.h plString.h
objects/plCamera.o: pl.h plCommon.h plModel.h plError.h plState.h plVector3.h
objects/plCamera.o: plTransform.h plRenderable.h plTriangle.h plString.h
objects/plCamera.o: plSeq.h plPickingShader.h plShader.h plMatrix44.h
objects/plCamera.o: plVector4.h plPickingTexture.h plMesh.h
objects/plColourMap.o: pl.h plCommon.h plVector3.h
objects/plColourMesh.o: pl.h plCommon.h plVector3.h plMesh.h plTriangle.h
objects/plColourMesh.o: plString.h plSeq.h plShader.h
objects/plComputeShader.o: pl.h plCommon.h plShader.h
objects/plDonorSite.o: pl.h plCommon.h plVector3.h plVector4.h plBoundary.h
objects/plDonorSite.o: plSeq.h plRenderable.h plPickingShader.h plShader.h
objects/plDonorSite.o: plMatrix44.h plTriangle.h plString.h
objects/plDonorSite.o: plPickingTexture.h plState.h plTransform.h plMesh.h
objects/plDraw.o: pl.h plCommon.h plProjection.h plMatrix44.h plVector3.h
objects/plDraw.o: plVector4.h plTriangle.h plString.h plSeq.h
objects/plDraw.o: plPickingTexture.h plState.h plTransform.h plCamera.h
objects/plDraw.o: plModel.h plError.h plRenderable.h plPickingShader.h
objects/plDraw.o: plShader.h plMesh.h plPlan.h plCSV.h plGraft.h plBoundary.h
objects/plDraw.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/plDraw.o: plIGuide.h plMinimalShader.h
objects/plError.o: pl.h plCommon.h plState.h plVector3.h plTransform.h
objects/plGraft.o: pl.h plCommon.h plRenderable.h plVector3.h plTriangle.h
objects/plGraft.o: plString.h plSeq.h plModel.h plError.h plState.h
objects/plGraft.o: plTransform.h plPickingShader.h plShader.h plMatrix44.h
objects/plGraft.o: plVector4.h plPickingTexture.h plMesh.h
objects/plIGuide.o: pl.h plCommon.h plSeq.h plVector3.h plVector4.h
objects/plIGuide.o: plBoundary.h plRenderable.h plPickingShader.h plShader.h
objects/plIGuide.o: plMatrix44.h plTriangle.h plString.h plPickingTexture.h
objects/plIGuide.o: plState.h plTransform.h plMesh.h
objects/plMath.o: pl.h plCommon.h plVector3.h
objects/plMatrix44.o: pl.h plCommon.h plVector3.h plVector4.h plTriangle.h
objects/plMatrix44.o: plString.h plSeq.h
objects/plMatrixStack.o: pl.h plCommon.h plMatrix44.h plVector3.h plVector4.h
objects/plMatrixStack.o: plTriangle.h plString.h plSeq.h
objects/plMesh.o: pl.h plCommon.h plVector3.h plTriangle.h plString.h plSeq.h
objects/plMesh.o: plShader.h
objects/plMinimalShader.o: pl.h plCommon.h plShader.h
objects/plModel.o: pl.h plCommon.h plError.h plState.h plVector3.h
objects/plModel.o: plTransform.h plRenderable.h plTriangle.h plString.h
objects/plModel.o: plSeq.h plPickingShader.h plShader.h plMatrix44.h
objects/plModel.o: plVector4.h plPickingTexture.h plMesh.h
objects/plPickingShader.o: pl.h plCommon.h plShader.h plMatrix44.h
objects/plPickingShader.o: plVector3.h plVector4.h plTriangle.h plString.h
objects/plPickingShader.o: plSeq.h plPickingTexture.h plState.h plTransform.h
objects/plPickingTexture.o: pl.h plCommon.h plState.h plVector3.h
objects/plPickingTexture.o: plTransform.h
objects/plPlan.o: pl.h plCommon.h plError.h plState.h plVector3.h
objects/plPlan.o: plTransform.h plSeq.h plCSV.h plString.h plGraft.h
objects/plPlan.o: plRenderable.h plTriangle.h plModel.h plPickingShader.h
objects/plPlan.o: plShader.h plMatrix44.h plVector4.h plPickingTexture.h
objects/plPlan.o: plMesh.h plBoundary.h plSpline.h plColourMesh.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plProjection.o: pl.h plCommon.h plMatrix44.h plVector3.h plVector4.h
objects/plProjection.o: plTriangle.h plString.h plSeq.h plPickingTexture.h
objects/plProjection.o: plState.h plTransform.h
objects/plRenderable.o: pl.h plCommon.h plVector3.h
objects/plSeq.o: pl.h plCommon.h
objects/plShader.o: pl.h plCommon.h
objects/plSpline.o: pl.h plCommon.h plVector3.h plVector4.h plBoundary.h
objects/plSpline.o: plSeq.h plRenderable.h plPickingShader.h plShader.h
objects/plSpline.o: plMatrix44.h plTriangle.h plString.h plPickingTexture.h
objects/plSpline.o: plState.h plTransform.h plMesh.h plColourMesh.h
objects/plSpline.o: plColourMap.h
objects/plState.o: pl.h plCommon.h plVector3.h plTransform.h
objects/plString.o: pl.h plCommon.h
objects/plTransform.o: pl.h plCommon.h plVector3.h
objects/plTriangle.o: pl.h plCommon.h plString.h plVector3.h plSeq.h
objects/plVector3.o: pl.h plCommon.h
objects/plVector4.o: pl.h plCommon.h plVector3.h
objects/ArcballWindow.o: ArcballWindow.h Window.h pl.h plCommon.h plVector3.h
objects/ArcballWindow.o: plMatrix44.h plVector4.h plTriangle.h plString.h
objects/ArcballWindow.o: plSeq.h plCamera.h plModel.h plError.h plState.h
objects/ArcballWindow.o: plTransform.h plRenderable.h plPickingShader.h
objects/ArcballWindow.o: plShader.h plPickingTexture.h plMesh.h
objects/PlannerWindow.o: PlannerWindow.h ArcballWindow.h Window.h pl.h
objects/PlannerWindow.o: plCommon.h plVector3.h plMatrix44.h plVector4.h
objects/PlannerWindow.o: plTriangle.h plString.h plSeq.h plCamera.h plModel.h
objects/PlannerWindow.o: plError.h plState.h plTransform.h plRenderable.h
objects/PlannerWindow.o: plPickingShader.h plShader.h plPickingTexture.h
objects/PlannerWindow.o: plMesh.h
objects/Window.o: Window.h pl.h plCommon.h
objects/main.o: pl.h plCommon.h PlannerWindow.h ArcballWindow.h Window.h
objects/main.o: plVector3.h plMatrix44.h plVector4.h plTriangle.h plString.h
objects/main.o: plSeq.h plCamera.h plModel.h plError.h plState.h
objects/main.o: plTransform.h plRenderable.h plPickingShader.h plShader.h
objects/main.o: plPickingTexture.h plMesh.h
objects/pl.o: pl.h plCommon.h plCamera.h plModel.h plError.h plState.h
objects/pl.o: plVector3.h plTransform.h plRenderable.h plTriangle.h
objects/pl.o: plString.h plSeq.h plPickingShader.h plShader.h plMatrix44.h
objects/pl.o: plVector4.h plPickingTexture.h plMesh.h plProjection.h
objects/pl.o: plMatrixStack.h plPlan.h plCSV.h plGraft.h plBoundary.h
objects/pl.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/pl.o: plIGuide.h plDraw.h plMinimalShader.h plComputeShader.h
objects/plBoundary.o: plBoundary.h pl.h plCommon.h plSeq.h plVector3.h
objects/plBoundary.o: plRenderable.h plPickingShader.h plShader.h
objects/plBoundary.o: plMatrix44.h plVector4.h plTriangle.h plString.h
objects/plBoundary.o: plPickingTexture.h plState.h plTransform.h plMesh.h
objects/plBoundaryIntersection.o: plBoundaryIntersection.h plCommon.h
objects/plBoundaryIntersection.o: plVector3.h pl.h plSeq.h
objects/plCSV.o: plCSV.h pl.h plCommon.h plSeq.h plString.h
objects/plCamera.o: plCamera.h pl.h plCommon.h plModel.h plError.h plState.h
objects/plCamera.o: plVector3.h plTransform.h plRenderable.h plTriangle.h
objects/plCamera.o: plString.h plSeq.h plPickingShader.h plShader.h
objects/plCamera.o: plMatrix44.h plVector4.h plPickingTexture.h plMesh.h
objects/plColourMap.o: plColourMap.h pl.h plCommon.h plVector3.h
objects/plColourMesh.o: plColourMesh.h pl.h plCommon.h plVector3.h plMesh.h
objects/plColourMesh.o: plTriangle.h plString.h plSeq.h plShader.h
objects/plDonorSite.o: plDonorSite.h pl.h plCommon.h plVector3.h plVector4.h
objects/plDonorSite.o: plBoundary.h plSeq.h plRenderable.h plPickingShader.h
objects/plDonorSite.o: plShader.h plMatrix44.h plTriangle.h plString.h
objects/plDonorSite.o: plPickingTexture.h plState.h plTransform.h plMesh.h
objects/plDraw.o: plDraw.h pl.h plCommon.h plProjection.h plMatrix44.h
objects/plDraw.o: plVector3.h plVector4.h plTriangle.h plString.h plSeq.h
objects/plDraw.o: plPickingTexture.h plState.h plTransform.h plCamera.h
objects/plDraw.o: plModel.h plError.h plRenderable.h plPickingShader.h
objects/plDraw.o: plShader.h plMesh.h plPlan.h plCSV.h plGraft.h plBoundary.h
objects/plDraw.o: plSpline.h plColourMesh.h plColourMap.h plDonorSite.h
objects/plDraw.o: plIGuide.h plMinimalShader.h
objects/plError.o: plError.h pl.h plCommon.h plState.h plVector3.h
objects/plError.o: plTransform.h
objects/plGraft.o: plGraft.h pl.h plCommon.h plRenderable.h plVector3.h
objects/plGraft.o: plTriangle.h plString.h plSeq.h plModel.h plError.h
objects/plGraft.o: plState.h plTransform.h plPickingShader.h plShader.h
objects/plGraft.o: plMatrix44.h plVector4.h plPickingTexture.h plMesh.h
objects/plIGuide.o: plIGuide.h pl.h plCommon.h plSeq.h plVector3.h
objects/plIGuide.o: plVector4.h plBoundary.h plRenderable.h plPickingShader.h
objects/plIGuide.o: plShader.h plMatrix44.h plTriangle.h plString.h
objects/plIGuide.o: plPickingTexture.h plState.h plTransform.h plMesh.h
objects/plMath.o: plMath.h pl.h plCommon.h plVector3.h
objects/plMatrix44.o: plMatrix44.h pl.h plCommon.h plVector3.h plVector4.h
objects/plMatrix44.o: plTriangle.h plString.h plSeq.h
objects/plMesh.o: plMesh.h pl.h plCommon.h plVector3.h plTriangle.h
objects/plMesh.o: plString.h plSeq.h plShader.h
objects/plModel.o: plModel.h pl.h plCommon.h plError.h plState.h plVector3.h
objects/plModel.o: plTransform.h plRenderable.h plTriangle.h plString.h
objects/plModel.o: plSeq.h plPickingShader.h plShader.h plMatrix44.h
objects/plModel.o: plVector4.h plPickingTexture.h plMesh.h
objects/plPickingTexture.o: plPickingTexture.h pl.h plCommon.h plState.h
objects/plPickingTexture.o: plVector3.h plTransform.h
objects/plPlan.o: plPlan.h pl.h plCommon.h plError.h plState.h plVector3.h
objects/plPlan.o: plTransform.h plSeq.h plCSV.h plString.h plGraft.h
objects/plPlan.o: plRenderable.h plTriangle.h plModel.h plPickingShader.h
objects/plPlan.o: plShader.h plMatrix44.h plVector4.h plPickingTexture.h
objects/plPlan.o: plMesh.h plBoundary.h plSpline.h plColourMesh.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuide.h
objects/plProjection.o: plProjection.h pl.h plCommon.h plMatrix44.h
objects/plProjection.o: plVector3.h plVector4.h plTriangle.h plString.h
objects/plProjection.o: plSeq.h plPickingTexture.h plState.h plTransform.h
objects/plRenderable.o: plRenderable.h pl.h plCommon.h plVector3.h
objects/plSpline.o: plSpline.h pl.h plCommon.h plVector3.h plVector4.h
objects/plSpline.o: plBoundary.h plSeq.h plRenderable.h plPickingShader.h
objects/plSpline.o: plShader.h plMatrix44.h plTriangle.h plString.h
objects/plSpline.o: plPickingTexture.h plState.h plTransform.h plMesh.h
objects/plSpline.o: plColourMesh.h plColourMap.h
objects/plState.o: plState.h pl.h plCommon.h plVector3.h plTransform.h
objects/plString.o: plString.h pl.h plCommon.h
objects/plTransform.o: plTransform.h pl.h plCommon.h plVector3.h
objects/plTriangle.o: plTriangle.h pl.h plCommon.h plString.h plVector3.h
objects/plTriangle.o: plSeq.h
objects/plVector3.o: plVector3.h pl.h plCommon.h
objects/plVector4.o: plVector4.h pl.h plCommon.h plVector3.h
