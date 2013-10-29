
OPENGL_DRIVER_DIR = /usr/lib/nvidia-331

LIBS = -L$(OPENGL_DRIVER_DIR) -lglut -lGL 

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES = main.cpp PlannerWindow.cpp pl.cpp plAutomaticPlanner.cpp plBoneAndCartilage.cpp \
          plBoundary.cpp plBoundaryEditor.cpp plCamera.cpp plChessBoard.cpp plColourMap.cpp \
          plColourMesh.cpp plCSV.cpp plDebugSphere.cpp plDefectSite.cpp plDonorSite.cpp \
          plDraw.cpp plDRBTransform.cpp plEditable.cpp plGraft.cpp plGraftEditor.cpp \
          plIGuide.cpp plIGuideSite.cpp plLineMesh.cpp plMath.cpp plMatrix44.cpp \
          plMatrixStack.cpp plMesh.cpp plMeshAlgorithm.cpp plMeshConnectivityData.cpp \
          plMeshCutter.cpp plMeshExtruder.cpp plMeshIntersector.cpp plMinimalShader.cpp \
          plModel.cpp plModelSpecific.cpp plOctree.cpp plPhongShader.cpp plPickingShader.cpp \
          plPickingTexture.cpp plPlan.cpp plPlannerStage0.cpp plPlannerStage1.cpp \
          plPlannerStage2.cpp plPlannerStage3.cpp plPlannerStage0Shader.cpp \
          plPlannerStage1Shader.cpp plPlannerStage2Shader.cpp plPlannerStage3Shader.cpp \
          plPlug.cpp plPolygon.cpp plProjection.cpp plRenderable.cpp plRenderer.cpp \
          plRenderingPipeline.cpp plScan.cpp plScanPoint.cpp plScanVolume.cpp plShader.cpp \
          plShapeMesh.cpp plSiteGrid.cpp plSpline.cpp plString.cpp plTexture2D.cpp \
          plTexture2DMesh.cpp plTexture2DShader.cpp plTexture3D.cpp plTexture3DMesh.cpp \
          plTexture3DShader.cpp plTrackedObject.cpp plTransform.cpp plTransparentRenderable.cpp \
          plTriangle.cpp plUtility.cpp plVector3.cpp plVector4.cpp plWindow.cpp Window.cpp
       
EXE  = planner

Window:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJECTS) $(LIBS) 

objects:
	@mkdir -p $@

objects/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f objects/*.o *~ core $(EXE) Makefile.bak

depend:	
	makedepend -Y -pobjects/ *.h *.cpp

# DO NOT DELETE

objects/PlannerWindow.o: Window.h pl.h plCommon.h plAutomaticPlanner.h
objects/PlannerWindow.o: plSeq.h plPlan.h plString.h plVector3.h plCSV.h
objects/PlannerWindow.o: plRenderable.h plGraft.h plEditable.h plTriangle.h
objects/PlannerWindow.o: plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/PlannerWindow.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/PlannerWindow.o: plMatrix44.h plProjection.h plCamera.h
objects/PlannerWindow.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/PlannerWindow.o: plPickingTexture.h plTrackedObject.h
objects/PlannerWindow.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/PlannerWindow.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/PlannerWindow.o: plPolygon.h plLineMesh.h plTransform.h
objects/PlannerWindow.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/PlannerWindow.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/PlannerWindow.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/PlannerWindow.o: plMeshExtruder.h plIGuide.h plSiteGrid.h
objects/PlannerWindow.o: plPlannerStage0.h plPlannerStage0Shader.h
objects/PlannerWindow.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/PlannerWindow.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/PlannerWindow.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/PlannerWindow.o: plBoundaryEditor.h plWindow.h plChessBoard.h
objects/PlannerWindow.o: plGraftEditor.h plRenderer.h plScan.h plScanPoint.h
objects/PlannerWindow.o: plScanVolume.h plTexture2D.h plTexture2DMesh.h
objects/PlannerWindow.o: plTexture2DShader.h plTexture3D.h plTexture3DMesh.h
objects/PlannerWindow.o: plTexture3DShader.h
objects/Window.o: pl.h plCommon.h plAutomaticPlanner.h plSeq.h plPlan.h
objects/Window.o: plString.h plVector3.h plCSV.h plRenderable.h plGraft.h
objects/Window.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/Window.o: plShader.h plRenderingPipeline.h plVector4.h
objects/Window.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/Window.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/Window.o: plPickingTexture.h plTrackedObject.h plDRBTransform.h
objects/Window.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/Window.o: plOctree.h plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/Window.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/Window.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/Window.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plSiteGrid.h plPlannerStage0.h
objects/Window.o: plPlannerStage0Shader.h plPlannerStage1.h
objects/Window.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/Window.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/Window.o: plPlannerStage3Shader.h plBoundaryEditor.h plWindow.h
objects/Window.o: plChessBoard.h plGraftEditor.h plRenderer.h plScan.h
objects/Window.o: plScanPoint.h plScanVolume.h plTexture2D.h
objects/Window.o: plTexture2DMesh.h plTexture2DShader.h plTexture3D.h
objects/Window.o: plTexture3DMesh.h plTexture3DShader.h
objects/pl.o: plCommon.h plAutomaticPlanner.h plSeq.h plPlan.h plString.h
objects/pl.o: plVector3.h plCSV.h plRenderable.h plGraft.h plEditable.h
objects/pl.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/pl.o: plRenderingPipeline.h plVector4.h plMatrixStack.h plMatrix44.h
objects/pl.o: plProjection.h plCamera.h plPickingShader.h plMinimalShader.h
objects/pl.o: plPhongShader.h plPickingTexture.h plTrackedObject.h
objects/pl.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/pl.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h plPolygon.h
objects/pl.o: plLineMesh.h plTransform.h plTransparentRenderable.h
objects/pl.o: plUtility.h plDefectSite.h plSpline.h plBoundary.h
objects/pl.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuideSite.h
objects/pl.o: plMeshCutter.h plMeshExtruder.h plIGuide.h plSiteGrid.h
objects/pl.o: plPlannerStage0.h plPlannerStage0Shader.h plPlannerStage1.h
objects/pl.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/pl.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/pl.o: plPlannerStage3Shader.h plBoundaryEditor.h plWindow.h
objects/pl.o: plChessBoard.h plGraftEditor.h plRenderer.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plTexture2D.h plTexture2DMesh.h
objects/pl.o: plTexture2DShader.h plTexture3D.h plTexture3DMesh.h
objects/pl.o: plTexture3DShader.h
objects/plArthroscope.o: plCommon.h
objects/plAutomaticPlanner.o: plCommon.h plSeq.h plPlan.h plString.h
objects/plAutomaticPlanner.o: plVector3.h plCSV.h plRenderable.h plGraft.h
objects/plAutomaticPlanner.o: plEditable.h plTriangle.h plDraw.h
objects/plAutomaticPlanner.o: plShapeMesh.h plMesh.h plShader.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plVector4.h
objects/plAutomaticPlanner.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plAutomaticPlanner.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plAutomaticPlanner.o: plPhongShader.h plPickingTexture.h
objects/plAutomaticPlanner.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/plAutomaticPlanner.o: plModelSpecific.h plBoneAndCartilage.h
objects/plAutomaticPlanner.o: plModel.h plOctree.h plMath.h plPolygon.h
objects/plAutomaticPlanner.o: plLineMesh.h plTransform.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plUtility.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plAutomaticPlanner.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plAutomaticPlanner.o: plIGuide.h plSiteGrid.h plPlannerStage0.h
objects/plAutomaticPlanner.o: plPlannerStage0Shader.h plPlannerStage1.h
objects/plAutomaticPlanner.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/plAutomaticPlanner.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plAutomaticPlanner.o: plPlannerStage3Shader.h
objects/plBoneAndCartilage.o: plCommon.h plModel.h plSeq.h plMesh.h
objects/plBoneAndCartilage.o: plVector3.h plString.h plTriangle.h plShader.h
objects/plBoneAndCartilage.o: plRenderingPipeline.h plVector4.h
objects/plBoneAndCartilage.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plBoneAndCartilage.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plBoneAndCartilage.o: plPhongShader.h plPickingTexture.h plOctree.h
objects/plBoneAndCartilage.o: plMath.h plPolygon.h plLineMesh.h
objects/plBoneAndCartilage.o: plRenderable.h plTransform.h
objects/plBoneAndCartilage.o: plTransparentRenderable.h
objects/plBoundary.o: plCommon.h plMath.h plVector3.h plString.h plSeq.h
objects/plBoundary.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plBoundary.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plBoundary.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plBoundary.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plBoundary.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plBoundary.o: plPhongShader.h plOctree.h plLineMesh.h plTransform.h
objects/plBoundary.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plBoundary.o: plTrackedObject.h plDRBTransform.h
objects/plBoundaryEditor.o: plCommon.h plSeq.h plVector3.h plString.h
objects/plBoundaryEditor.o: plPlan.h plCSV.h plRenderable.h plGraft.h
objects/plBoundaryEditor.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h
objects/plBoundaryEditor.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plBoundaryEditor.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plBoundaryEditor.o: plProjection.h plCamera.h plPickingShader.h
objects/plBoundaryEditor.o: plMinimalShader.h plPhongShader.h
objects/plBoundaryEditor.o: plPickingTexture.h plTrackedObject.h
objects/plBoundaryEditor.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plBoundaryEditor.o: plBoneAndCartilage.h plModel.h plOctree.h
objects/plBoundaryEditor.o: plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plBoundaryEditor.o: plTransparentRenderable.h plUtility.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plBoundary.h
objects/plBoundaryEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plWindow.h
objects/plCSV.o: plCommon.h plSeq.h plString.h
objects/plCamera.o: plCommon.h plSeq.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plCommon.h plTransparentRenderable.h plRenderable.h
objects/plChessBoard.o: plVector3.h plString.h plRenderingPipeline.h
objects/plChessBoard.o: plVector4.h plMatrixStack.h plMatrix44.h plTriangle.h
objects/plChessBoard.o: plSeq.h plProjection.h plCamera.h plPickingShader.h
objects/plChessBoard.o: plMinimalShader.h plShader.h plPhongShader.h
objects/plChessBoard.o: plPickingTexture.h plTransform.h plColourMesh.h
objects/plChessBoard.o: plMesh.h plDRBTransform.h
objects/plColourMap.o: plCommon.h plVector3.h plString.h
objects/plColourMesh.o: plCommon.h plVector3.h plString.h plMesh.h
objects/plColourMesh.o: plTriangle.h plSeq.h plShader.h plRenderingPipeline.h
objects/plColourMesh.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plColourMesh.o: plProjection.h plCamera.h plPickingShader.h
objects/plColourMesh.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plDRBTransform.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plDRBTransform.o: plVector4.h plTriangle.h plSeq.h
objects/plDefectSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDefectSite.o: plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plDefectSite.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plDefectSite.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plDefectSite.o: plProjection.h plCamera.h plPickingShader.h
objects/plDefectSite.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plDefectSite.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/plDefectSite.o: plRenderable.h plTransform.h
objects/plDefectSite.o: plTransparentRenderable.h plSpline.h plBoundary.h
objects/plDefectSite.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plDefectSite.o: plDRBTransform.h plColourMesh.h plColourMap.h
objects/plDonorSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDonorSite.o: plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plDonorSite.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plDonorSite.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plDonorSite.o: plProjection.h plCamera.h plPickingShader.h
objects/plDonorSite.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plDonorSite.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/plDonorSite.o: plRenderable.h plTransform.h plTransparentRenderable.h
objects/plDonorSite.o: plBoundary.h plEditable.h plDraw.h plShapeMesh.h
objects/plDonorSite.o: plTrackedObject.h plDRBTransform.h
objects/plDraw.o: plCommon.h plShapeMesh.h plMesh.h plVector3.h plString.h
objects/plDraw.o: plTriangle.h plSeq.h plShader.h plRenderingPipeline.h
objects/plDraw.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plDraw.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plDraw.o: plPhongShader.h plPickingTexture.h plTrackedObject.h
objects/plDraw.o: plRenderable.h plDRBTransform.h
objects/plEditable.o: plCommon.h
objects/plGraft.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plEditable.h plTriangle.h plSeq.h plDraw.h plShapeMesh.h
objects/plGraft.o: plMesh.h plShader.h plRenderingPipeline.h plVector4.h
objects/plGraft.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plGraft.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plGraft.o: plPickingTexture.h plTrackedObject.h plDRBTransform.h
objects/plGraft.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraft.o: plOctree.h plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plGraft.o: plTransparentRenderable.h plUtility.h
objects/plGraftEditor.o: plCommon.h plSeq.h plVector3.h plString.h
objects/plGraftEditor.o: plTransform.h plMatrix44.h plVector4.h plTriangle.h
objects/plGraftEditor.o: plPlan.h plCSV.h plRenderable.h plGraft.h
objects/plGraftEditor.o: plEditable.h plDraw.h plShapeMesh.h plMesh.h
objects/plGraftEditor.o: plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plGraftEditor.o: plProjection.h plCamera.h plPickingShader.h
objects/plGraftEditor.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plGraftEditor.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/plGraftEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraftEditor.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/plGraftEditor.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plGraftEditor.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plGraftEditor.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plGraftEditor.o: plMeshExtruder.h plIGuide.h plWindow.h
objects/plIGuide.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plIGuide.o: plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plIGuide.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plIGuide.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plIGuide.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plIGuide.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plIGuide.o: plLineMesh.h plRenderable.h plTransform.h
objects/plIGuide.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuide.o: plDraw.h plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plIGuide.o: plGraft.h plPlug.h plUtility.h plIGuideSite.h
objects/plIGuide.o: plMeshCutter.h plMeshExtruder.h plSpline.h plColourMesh.h
objects/plIGuide.o: plColourMap.h
objects/plIGuideSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plIGuideSite.o: plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plIGuideSite.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plIGuideSite.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plIGuideSite.o: plProjection.h plCamera.h plPickingShader.h
objects/plIGuideSite.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plIGuideSite.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/plIGuideSite.o: plRenderable.h plTransform.h
objects/plIGuideSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuideSite.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plIGuideSite.o: plDRBTransform.h plMeshCutter.h plMeshExtruder.h
objects/plLineMesh.o: plCommon.h plMesh.h plVector3.h plString.h plTriangle.h
objects/plLineMesh.o: plSeq.h plShader.h plRenderingPipeline.h plVector4.h
objects/plLineMesh.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plLineMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plLineMesh.o: plPickingTexture.h
objects/plMath.o: plCommon.h plVector3.h plString.h plSeq.h plPolygon.h
objects/plMath.o: plTriangle.h plMatrix44.h plVector4.h
objects/plMatrix44.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plMatrix44.o: plTriangle.h plSeq.h
objects/plMatrixStack.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plMatrixStack.o: plVector4.h plTriangle.h plSeq.h
objects/plMesh.o: plCommon.h plVector3.h plString.h plTriangle.h plSeq.h
objects/plMesh.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plMesh.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMesh.o: plPickingTexture.h
objects/plMeshAlgorithm.o: plVector3.h plCommon.h plString.h plPolygon.h
objects/plMeshAlgorithm.o: plSeq.h plTriangle.h plMeshConnectivityData.h
objects/plMeshConnectivityData.o: plSeq.h plCommon.h plTriangle.h plString.h
objects/plMeshConnectivityData.o: plVector3.h plPolygon.h
objects/plMeshCutter.o: plCommon.h plVector3.h plString.h plSeq.h
objects/plMeshCutter.o: plTriangle.h plPolygon.h plBoundary.h plMath.h
objects/plMeshCutter.o: plMatrix44.h plVector4.h plRenderable.h plEditable.h
objects/plMeshCutter.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plMeshCutter.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h
objects/plMeshCutter.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMeshCutter.o: plOctree.h plLineMesh.h plTransform.h
objects/plMeshCutter.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plMeshCutter.o: plTrackedObject.h plDRBTransform.h
objects/plMeshExtruder.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plMeshExtruder.o: plSeq.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshAlgorithm.h plVector3.h plCommon.h
objects/plMeshIntersector.o: plString.h plPolygon.h plSeq.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plMath.h plMatrix44.h
objects/plMeshIntersector.o: plVector4.h
objects/plMinimalShader.o: plCommon.h plShader.h plSeq.h plMatrix44.h
objects/plMinimalShader.o: plVector3.h plString.h plVector4.h plTriangle.h
objects/plModel.o: plCommon.h plSeq.h plMesh.h plVector3.h plString.h
objects/plModel.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plModel.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plModel.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plModel.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plModel.o: plLineMesh.h plRenderable.h plTransform.h
objects/plModel.o: plTransparentRenderable.h
objects/plModelSpecific.o: plCommon.h plVector3.h plString.h
objects/plModelSpecific.o: plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plModelSpecific.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plModelSpecific.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plModelSpecific.o: plProjection.h plCamera.h plPickingShader.h
objects/plModelSpecific.o: plMinimalShader.h plPhongShader.h
objects/plModelSpecific.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plModelSpecific.o: plLineMesh.h plRenderable.h plTransform.h
objects/plModelSpecific.o: plTransparentRenderable.h
objects/plOctree.o: plCommon.h plSeq.h plVector3.h plString.h plMath.h
objects/plOctree.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plOctree.o: plLineMesh.h plMesh.h plShader.h plRenderingPipeline.h
objects/plOctree.o: plMatrixStack.h plProjection.h plCamera.h
objects/plOctree.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plOctree.o: plPickingTexture.h plRenderable.h plTransform.h
objects/plPhongShader.o: plCommon.h plMinimalShader.h plShader.h plSeq.h
objects/plPhongShader.o: plMatrix44.h plVector3.h plString.h plVector4.h
objects/plPhongShader.o: plTriangle.h
objects/plPickingShader.o: plCommon.h plMinimalShader.h plShader.h plSeq.h
objects/plPickingShader.o: plMatrix44.h plVector3.h plString.h plVector4.h
objects/plPickingShader.o: plTriangle.h
objects/plPickingTexture.o: plCommon.h
objects/plPlan.o: plCommon.h plString.h plSeq.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plGraft.h plEditable.h plTriangle.h plDraw.h
objects/plPlan.o: plShapeMesh.h plMesh.h plShader.h plRenderingPipeline.h
objects/plPlan.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plPlan.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plPlan.o: plPhongShader.h plPickingTexture.h plTrackedObject.h
objects/plPlan.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plPlan.o: plPolygon.h plLineMesh.h plTransform.h
objects/plPlan.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plPlan.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plPlan.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage0.o: plCommon.h plUtility.h plSeq.h plVector4.h
objects/plPlannerStage0.o: plString.h plVector3.h plSiteGrid.h plTriangle.h
objects/plPlannerStage0.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage0.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage0.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage0.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage0.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage0.o: plLineMesh.h plTransform.h
objects/plPlannerStage0.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage0.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage0.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage0.o: plColourMap.h
objects/plPlannerStage0Shader.o: plCommon.h plShader.h plSeq.h plSiteGrid.h
objects/plPlannerStage0Shader.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage0Shader.o: plTriangle.h plBoundary.h plMath.h
objects/plPlannerStage0Shader.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage0Shader.o: plEditable.h plPickingTexture.h
objects/plPlannerStage0Shader.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage0Shader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0Shader.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage0Shader.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage0Shader.o: plLineMesh.h plTransform.h
objects/plPlannerStage0Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage0Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage0Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage0Shader.o: plColourMesh.h plColourMap.h
objects/plPlannerStage1.o: plCommon.h plSeq.h plVector4.h plString.h
objects/plPlannerStage1.o: plVector3.h plSiteGrid.h plTriangle.h plBoundary.h
objects/plPlannerStage1.o: plMath.h plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage1.o: plEditable.h plPickingTexture.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage1.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage1.o: plLineMesh.h plTransform.h
objects/plPlannerStage1.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage1.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage1.o: plPlannerStage0.h plUtility.h
objects/plPlannerStage1.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage1.o: plColourMap.h plPlannerStage1Shader.h
objects/plPlannerStage1Shader.o: plCommon.h plShader.h plSeq.h plSiteGrid.h
objects/plPlannerStage1Shader.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage1Shader.o: plTriangle.h plBoundary.h plMath.h
objects/plPlannerStage1Shader.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage1Shader.o: plEditable.h plPickingTexture.h
objects/plPlannerStage1Shader.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage1Shader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1Shader.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage1Shader.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage1Shader.o: plLineMesh.h plTransform.h
objects/plPlannerStage1Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage1Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage1Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage1Shader.o: plColourMesh.h plColourMap.h
objects/plPlannerStage2.o: plCommon.h plSeq.h plVector4.h plString.h
objects/plPlannerStage2.o: plVector3.h plSiteGrid.h plTriangle.h plBoundary.h
objects/plPlannerStage2.o: plMath.h plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage2.o: plEditable.h plPickingTexture.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage2.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage2.o: plLineMesh.h plTransform.h
objects/plPlannerStage2.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage2.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage2.o: plPlannerStage1.h plPlannerStage0.h plUtility.h
objects/plPlannerStage2.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage2.o: plColourMap.h plPlannerStage1Shader.h
objects/plPlannerStage2.o: plPlannerStage2Shader.h
objects/plPlannerStage2Shader.o: plCommon.h plShader.h plSeq.h plSiteGrid.h
objects/plPlannerStage2Shader.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage2Shader.o: plTriangle.h plBoundary.h plMath.h
objects/plPlannerStage2Shader.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage2Shader.o: plEditable.h plPickingTexture.h
objects/plPlannerStage2Shader.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage2Shader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2Shader.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage2Shader.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage2Shader.o: plLineMesh.h plTransform.h
objects/plPlannerStage2Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage2Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage2Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage2Shader.o: plColourMesh.h plColourMap.h
objects/plPlannerStage3.o: plCommon.h plSeq.h plVector4.h plString.h
objects/plPlannerStage3.o: plVector3.h plSiteGrid.h plTriangle.h plBoundary.h
objects/plPlannerStage3.o: plMath.h plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage3.o: plEditable.h plPickingTexture.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage3.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage3.o: plLineMesh.h plTransform.h
objects/plPlannerStage3.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage3.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage3.o: plPlannerStage0.h plUtility.h
objects/plPlannerStage3.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage3.o: plColourMap.h plPlannerStage2.h plPlannerStage1.h
objects/plPlannerStage3.o: plPlannerStage1Shader.h plPlannerStage2Shader.h
objects/plPlannerStage3.o: plPlannerStage3Shader.h
objects/plPlannerStage3Shader.o: plCommon.h plShader.h plSeq.h plSiteGrid.h
objects/plPlannerStage3Shader.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage3Shader.o: plTriangle.h plBoundary.h plMath.h
objects/plPlannerStage3Shader.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerStage3Shader.o: plEditable.h plPickingTexture.h
objects/plPlannerStage3Shader.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage3Shader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3Shader.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage3Shader.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage3Shader.o: plLineMesh.h plTransform.h
objects/plPlannerStage3Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage3Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage3Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage3Shader.o: plColourMesh.h plColourMap.h
objects/plPlug.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plPlug.o: plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plPlug.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plPlug.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plPlug.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plPlug.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plPlug.o: plLineMesh.h plRenderable.h plTransform.h
objects/plPlug.o: plTransparentRenderable.h
objects/plPolygon.o: plCommon.h plSeq.h plVector3.h plString.h
objects/plProjection.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plProjection.o: plVector4.h plTriangle.h plSeq.h
objects/plRenderable.o: plCommon.h plVector3.h plString.h
objects/plRenderer.o: plCommon.h plSeq.h plVector3.h plString.h
objects/plRenderer.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plRenderer.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plRenderer.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plRenderer.o: plPhongShader.h plPickingTexture.h plPlan.h plCSV.h
objects/plRenderer.o: plRenderable.h plGraft.h plEditable.h plDraw.h
objects/plRenderer.o: plShapeMesh.h plMesh.h plTrackedObject.h
objects/plRenderer.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plRenderer.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plRenderer.o: plPolygon.h plLineMesh.h plTransform.h
objects/plRenderer.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plRenderer.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plRenderer.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plRenderer.o: plMeshExtruder.h plIGuide.h plGraftEditor.h plWindow.h
objects/plRenderer.o: plBoundaryEditor.h plAutomaticPlanner.h plSiteGrid.h
objects/plRenderer.o: plPlannerStage0.h plPlannerStage0Shader.h
objects/plRenderer.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plRenderer.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plRenderer.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plRenderer.o: plChessBoard.h plScan.h plScanPoint.h plScanVolume.h
objects/plRenderingPipeline.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plRenderingPipeline.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderingPipeline.o: plSeq.h plProjection.h plCamera.h
objects/plRenderingPipeline.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plRenderingPipeline.o: plPhongShader.h plPickingTexture.h
objects/plScan.o: plScanPoint.h plVector3.h plCommon.h plString.h plSeq.h
objects/plScan.o: plScanVolume.h plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plScan.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plScan.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plScan.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plScan.o: plPickingTexture.h plTrackedObject.h plRenderable.h
objects/plScan.o: plDRBTransform.h plLineMesh.h
objects/plScanPoint.o: plVector3.h plCommon.h plString.h plSeq.h
objects/plScanVolume.o: plVector3.h plCommon.h plString.h plSeq.h plDraw.h
objects/plScanVolume.o: plShapeMesh.h plMesh.h plTriangle.h plShader.h
objects/plScanVolume.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plScanVolume.o: plMatrix44.h plProjection.h plCamera.h
objects/plScanVolume.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plScanVolume.o: plPickingTexture.h plTrackedObject.h plRenderable.h
objects/plScanVolume.o: plDRBTransform.h plLineMesh.h
objects/plSeq.o: plCommon.h
objects/plShader.o: plCommon.h plSeq.h
objects/plShapeMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plShapeMesh.o: plTriangle.h plSeq.h plShader.h plRenderingPipeline.h
objects/plShapeMesh.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plShapeMesh.o: plProjection.h plCamera.h plPickingShader.h
objects/plShapeMesh.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plSiteGrid.o: plCommon.h plSeq.h plVector4.h plString.h plVector3.h
objects/plSiteGrid.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plSiteGrid.o: plMatrix44.h plRenderable.h plEditable.h
objects/plSiteGrid.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plSiteGrid.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plSiteGrid.o: plMatrixStack.h plProjection.h plCamera.h
objects/plSiteGrid.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plSiteGrid.o: plOctree.h plLineMesh.h plTransform.h
objects/plSiteGrid.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plSiteGrid.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plSpline.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plSeq.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plEditable.h
objects/plSpline.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plSpline.o: plMesh.h plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plSpline.o: plProjection.h plCamera.h plPickingShader.h
objects/plSpline.o: plMinimalShader.h plPhongShader.h plOctree.h plLineMesh.h
objects/plSpline.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plSpline.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plSpline.o: plColourMesh.h plColourMap.h
objects/plString.o: plCommon.h
objects/plTexture2D.o: plCommon.h plVector3.h plString.h plSeq.h
objects/plTexture2DMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plTexture2DMesh.o: plTriangle.h plSeq.h plShader.h
objects/plTexture2DMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plTexture2DMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plTexture2DMesh.o: plPickingShader.h plMinimalShader.h
objects/plTexture2DMesh.o: plPhongShader.h plPickingTexture.h
objects/plTexture2DShader.o: plCommon.h plMinimalShader.h plShader.h plSeq.h
objects/plTexture2DShader.o: plMatrix44.h plVector3.h plString.h plVector4.h
objects/plTexture2DShader.o: plTriangle.h
objects/plTexture3D.o: plCommon.h plVector3.h plString.h plSeq.h
objects/plTexture3DMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plTexture3DMesh.o: plTriangle.h plSeq.h plShader.h
objects/plTexture3DMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plTexture3DMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plTexture3DMesh.o: plPickingShader.h plMinimalShader.h
objects/plTexture3DMesh.o: plPhongShader.h plPickingTexture.h
objects/plTexture3DShader.o: plCommon.h plMinimalShader.h plShader.h plSeq.h
objects/plTexture3DShader.o: plMatrix44.h plVector3.h plString.h plVector4.h
objects/plTexture3DShader.o: plTriangle.h
objects/plTrackedObject.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plTrackedObject.o: plDRBTransform.h plMatrix44.h plVector4.h
objects/plTrackedObject.o: plTriangle.h plSeq.h plShapeMesh.h plMesh.h
objects/plTrackedObject.o: plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plTrackedObject.o: plProjection.h plCamera.h plPickingShader.h
objects/plTrackedObject.o: plMinimalShader.h plPhongShader.h
objects/plTrackedObject.o: plPickingTexture.h
objects/plTransform.o: plCommon.h plSeq.h plString.h plVector3.h plMatrix44.h
objects/plTransform.o: plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plCommon.h plRenderable.h plVector3.h
objects/plTransparentRenderable.o: plString.h
objects/plTriangle.o: plCommon.h plString.h plVector3.h plSeq.h
objects/plUtility.o: plCommon.h plSeq.h
objects/plVector3.o: plCommon.h plString.h
objects/plVector4.o: plCommon.h plString.h plVector3.h
objects/plWindow.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plSeq.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h
objects/plWindow.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plWindow.o: plPhongShader.h plPickingTexture.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h
objects/PlannerWindow.o: plAutomaticPlanner.h plSeq.h plPlan.h plString.h
objects/PlannerWindow.o: plVector3.h plCSV.h plRenderable.h plGraft.h
objects/PlannerWindow.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h
objects/PlannerWindow.o: plMesh.h plShader.h plRenderingPipeline.h
objects/PlannerWindow.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/PlannerWindow.o: plProjection.h plCamera.h plPickingShader.h
objects/PlannerWindow.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/PlannerWindow.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/PlannerWindow.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/PlannerWindow.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/PlannerWindow.o: plTransform.h plTransparentRenderable.h plUtility.h
objects/PlannerWindow.o: plDefectSite.h plSpline.h plBoundary.h
objects/PlannerWindow.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/PlannerWindow.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/PlannerWindow.o: plIGuide.h plSiteGrid.h plPlannerStage0.h
objects/PlannerWindow.o: plPlannerStage0Shader.h plPlannerStage1.h
objects/PlannerWindow.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/PlannerWindow.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/PlannerWindow.o: plPlannerStage3Shader.h plBoundaryEditor.h
objects/PlannerWindow.o: plWindow.h plChessBoard.h plGraftEditor.h
objects/PlannerWindow.o: plRenderer.h plScan.h plScanPoint.h plScanVolume.h
objects/PlannerWindow.o: plTexture2D.h plTexture2DMesh.h plTexture2DShader.h
objects/PlannerWindow.o: plTexture3D.h plTexture3DMesh.h plTexture3DShader.h
objects/Window.o: Window.h pl.h plCommon.h plAutomaticPlanner.h plSeq.h
objects/Window.o: plPlan.h plString.h plVector3.h plCSV.h plRenderable.h
objects/Window.o: plGraft.h plEditable.h plTriangle.h plDraw.h plShapeMesh.h
objects/Window.o: plMesh.h plShader.h plRenderingPipeline.h plVector4.h
objects/Window.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/Window.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/Window.o: plPickingTexture.h plTrackedObject.h plDRBTransform.h
objects/Window.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/Window.o: plOctree.h plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/Window.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/Window.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/Window.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plSiteGrid.h plPlannerStage0.h
objects/Window.o: plPlannerStage0Shader.h plPlannerStage1.h
objects/Window.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/Window.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/Window.o: plPlannerStage3Shader.h plBoundaryEditor.h plWindow.h
objects/Window.o: plChessBoard.h plGraftEditor.h plRenderer.h plScan.h
objects/Window.o: plScanPoint.h plScanVolume.h plTexture2D.h
objects/Window.o: plTexture2DMesh.h plTexture2DShader.h plTexture3D.h
objects/Window.o: plTexture3DMesh.h plTexture3DShader.h
objects/main.o: PlannerWindow.h Window.h pl.h plCommon.h plAutomaticPlanner.h
objects/main.o: plSeq.h plPlan.h plString.h plVector3.h plCSV.h
objects/main.o: plRenderable.h plGraft.h plEditable.h plTriangle.h plDraw.h
objects/main.o: plShapeMesh.h plMesh.h plShader.h plRenderingPipeline.h
objects/main.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/main.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/main.o: plPhongShader.h plPickingTexture.h plTrackedObject.h
objects/main.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/main.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/main.o: plPolygon.h plLineMesh.h plTransform.h
objects/main.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/main.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/main.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/main.o: plIGuide.h plSiteGrid.h plPlannerStage0.h
objects/main.o: plPlannerStage0Shader.h plPlannerStage1.h
objects/main.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/main.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/main.o: plPlannerStage3Shader.h plBoundaryEditor.h plWindow.h
objects/main.o: plChessBoard.h plGraftEditor.h plRenderer.h plScan.h
objects/main.o: plScanPoint.h plScanVolume.h plTexture2D.h plTexture2DMesh.h
objects/main.o: plTexture2DShader.h plTexture3D.h plTexture3DMesh.h
objects/main.o: plTexture3DShader.h
objects/pl.o: pl.h plCommon.h plAutomaticPlanner.h plSeq.h plPlan.h
objects/pl.o: plString.h plVector3.h plCSV.h plRenderable.h plGraft.h
objects/pl.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/pl.o: plShader.h plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/pl.o: plMatrix44.h plProjection.h plCamera.h plPickingShader.h
objects/pl.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/pl.o: plTrackedObject.h plDRBTransform.h plPlug.h plModelSpecific.h
objects/pl.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h plPolygon.h
objects/pl.o: plLineMesh.h plTransform.h plTransparentRenderable.h
objects/pl.o: plUtility.h plDefectSite.h plSpline.h plBoundary.h
objects/pl.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuideSite.h
objects/pl.o: plMeshCutter.h plMeshExtruder.h plIGuide.h plSiteGrid.h
objects/pl.o: plPlannerStage0.h plPlannerStage0Shader.h plPlannerStage1.h
objects/pl.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/pl.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/pl.o: plPlannerStage3Shader.h plBoundaryEditor.h plWindow.h
objects/pl.o: plChessBoard.h plGraftEditor.h plRenderer.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plTexture2D.h plTexture2DMesh.h
objects/pl.o: plTexture2DShader.h plTexture3D.h plTexture3DMesh.h
objects/pl.o: plTexture3DShader.h
objects/plArthroscope.o: plArthroscope.h plCommon.h
objects/plAutomaticPlanner.o: plAutomaticPlanner.h plCommon.h plSeq.h
objects/plAutomaticPlanner.o: plPlan.h plString.h plVector3.h plCSV.h
objects/plAutomaticPlanner.o: plRenderable.h plGraft.h plEditable.h
objects/plAutomaticPlanner.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/plAutomaticPlanner.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plAutomaticPlanner.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plAutomaticPlanner.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plAutomaticPlanner.o: plPhongShader.h plPickingTexture.h
objects/plAutomaticPlanner.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/plAutomaticPlanner.o: plModelSpecific.h plBoneAndCartilage.h
objects/plAutomaticPlanner.o: plModel.h plOctree.h plMath.h plPolygon.h
objects/plAutomaticPlanner.o: plLineMesh.h plTransform.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plUtility.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plAutomaticPlanner.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plAutomaticPlanner.o: plIGuide.h plSiteGrid.h plPlannerStage0.h
objects/plAutomaticPlanner.o: plPlannerStage0Shader.h plPlannerStage1.h
objects/plAutomaticPlanner.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/plAutomaticPlanner.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plAutomaticPlanner.o: plPlannerStage3Shader.h
objects/plBoneAndCartilage.o: plBoneAndCartilage.h plCommon.h plModel.h
objects/plBoneAndCartilage.o: plSeq.h plMesh.h plVector3.h plString.h
objects/plBoneAndCartilage.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plBoneAndCartilage.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plBoneAndCartilage.o: plProjection.h plCamera.h plPickingShader.h
objects/plBoneAndCartilage.o: plMinimalShader.h plPhongShader.h
objects/plBoneAndCartilage.o: plPickingTexture.h plOctree.h plMath.h
objects/plBoneAndCartilage.o: plPolygon.h plLineMesh.h plRenderable.h
objects/plBoneAndCartilage.o: plTransform.h plTransparentRenderable.h
objects/plBoundary.o: plBoundary.h plCommon.h plMath.h plVector3.h plString.h
objects/plBoundary.o: plSeq.h plPolygon.h plTriangle.h plMatrix44.h
objects/plBoundary.o: plVector4.h plRenderable.h plEditable.h
objects/plBoundary.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plBoundary.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plBoundary.o: plMatrixStack.h plProjection.h plCamera.h
objects/plBoundary.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plBoundary.o: plOctree.h plLineMesh.h plTransform.h
objects/plBoundary.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plBoundary.o: plTrackedObject.h plDRBTransform.h
objects/plBoundaryEditor.o: plBoundaryEditor.h plCommon.h plSeq.h plVector3.h
objects/plBoundaryEditor.o: plString.h plPlan.h plCSV.h plRenderable.h
objects/plBoundaryEditor.o: plGraft.h plEditable.h plTriangle.h plDraw.h
objects/plBoundaryEditor.o: plShapeMesh.h plMesh.h plShader.h
objects/plBoundaryEditor.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plBoundaryEditor.o: plMatrix44.h plProjection.h plCamera.h
objects/plBoundaryEditor.o: plPickingShader.h plMinimalShader.h
objects/plBoundaryEditor.o: plPhongShader.h plPickingTexture.h
objects/plBoundaryEditor.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/plBoundaryEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plBoundaryEditor.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/plBoundaryEditor.o: plTransform.h plTransparentRenderable.h
objects/plBoundaryEditor.o: plUtility.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plBoundary.h plColourMesh.h plColourMap.h
objects/plBoundaryEditor.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plBoundaryEditor.o: plMeshExtruder.h plIGuide.h plWindow.h
objects/plCSV.o: plCSV.h plCommon.h plSeq.h plString.h
objects/plCamera.o: plCamera.h plCommon.h plSeq.h plVector3.h plString.h
objects/plCamera.o: plVector4.h plMatrix44.h plTriangle.h
objects/plChessBoard.o: plChessBoard.h plCommon.h plTransparentRenderable.h
objects/plChessBoard.o: plRenderable.h plVector3.h plString.h
objects/plChessBoard.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plChessBoard.o: plMatrix44.h plTriangle.h plSeq.h plProjection.h
objects/plChessBoard.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plChessBoard.o: plShader.h plPhongShader.h plPickingTexture.h
objects/plChessBoard.o: plTransform.h plColourMesh.h plMesh.h
objects/plChessBoard.o: plDRBTransform.h
objects/plColourMap.o: plColourMap.h plCommon.h plVector3.h plString.h
objects/plColourMesh.o: plColourMesh.h plCommon.h plVector3.h plString.h
objects/plColourMesh.o: plMesh.h plTriangle.h plSeq.h plShader.h
objects/plColourMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plColourMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plColourMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plColourMesh.o: plPickingTexture.h
objects/plDRBTransform.o: plDRBTransform.h plCommon.h plMatrix44.h
objects/plDRBTransform.o: plVector3.h plString.h plVector4.h plTriangle.h
objects/plDRBTransform.o: plSeq.h
objects/plDefectSite.o: plDefectSite.h plCommon.h plVector3.h plString.h
objects/plDefectSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDefectSite.o: plSeq.h plMesh.h plTriangle.h plShader.h
objects/plDefectSite.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plDefectSite.o: plMatrix44.h plProjection.h plCamera.h
objects/plDefectSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plDefectSite.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plDefectSite.o: plLineMesh.h plRenderable.h plTransform.h
objects/plDefectSite.o: plTransparentRenderable.h plSpline.h plBoundary.h
objects/plDefectSite.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plDefectSite.o: plDRBTransform.h plColourMesh.h plColourMap.h
objects/plDonorSite.o: plDonorSite.h plCommon.h plVector3.h plString.h
objects/plDonorSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDonorSite.o: plSeq.h plMesh.h plTriangle.h plShader.h
objects/plDonorSite.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plDonorSite.o: plMatrix44.h plProjection.h plCamera.h
objects/plDonorSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plDonorSite.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plDonorSite.o: plLineMesh.h plRenderable.h plTransform.h
objects/plDonorSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plDonorSite.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plDonorSite.o: plDRBTransform.h
objects/plDraw.o: plDraw.h plCommon.h plShapeMesh.h plMesh.h plVector3.h
objects/plDraw.o: plString.h plTriangle.h plSeq.h plShader.h
objects/plDraw.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plDraw.o: plMatrix44.h plProjection.h plCamera.h plPickingShader.h
objects/plDraw.o: plMinimalShader.h plPhongShader.h plPickingTexture.h
objects/plDraw.o: plTrackedObject.h plRenderable.h plDRBTransform.h
objects/plEditable.o: plEditable.h plCommon.h
objects/plGraft.o: plGraft.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plEditable.h plTriangle.h plSeq.h plDraw.h plShapeMesh.h
objects/plGraft.o: plMesh.h plShader.h plRenderingPipeline.h plVector4.h
objects/plGraft.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plGraft.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plGraft.o: plPickingTexture.h plTrackedObject.h plDRBTransform.h
objects/plGraft.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraft.o: plOctree.h plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plGraft.o: plTransparentRenderable.h plUtility.h
objects/plGraftEditor.o: plGraftEditor.h plCommon.h plSeq.h plVector3.h
objects/plGraftEditor.o: plString.h plTransform.h plMatrix44.h plVector4.h
objects/plGraftEditor.o: plTriangle.h plPlan.h plCSV.h plRenderable.h
objects/plGraftEditor.o: plGraft.h plEditable.h plDraw.h plShapeMesh.h
objects/plGraftEditor.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plGraftEditor.o: plMatrixStack.h plProjection.h plCamera.h
objects/plGraftEditor.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plGraftEditor.o: plPickingTexture.h plTrackedObject.h
objects/plGraftEditor.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plGraftEditor.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plGraftEditor.o: plPolygon.h plLineMesh.h plTransparentRenderable.h
objects/plGraftEditor.o: plUtility.h plDefectSite.h plSpline.h plBoundary.h
objects/plGraftEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plGraftEditor.o: plIGuide.h plWindow.h
objects/plIGuide.o: plIGuide.h plCommon.h plModelSpecific.h plVector3.h
objects/plIGuide.o: plString.h plBoneAndCartilage.h plModel.h plSeq.h
objects/plIGuide.o: plMesh.h plTriangle.h plShader.h plRenderingPipeline.h
objects/plIGuide.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plIGuide.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plIGuide.o: plPhongShader.h plPickingTexture.h plOctree.h plMath.h
objects/plIGuide.o: plPolygon.h plLineMesh.h plRenderable.h plTransform.h
objects/plIGuide.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuide.o: plDraw.h plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plIGuide.o: plGraft.h plPlug.h plUtility.h plIGuideSite.h
objects/plIGuide.o: plMeshCutter.h plMeshExtruder.h plSpline.h plColourMesh.h
objects/plIGuide.o: plColourMap.h
objects/plIGuideSite.o: plIGuideSite.h plCommon.h plVector3.h plString.h
objects/plIGuideSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plIGuideSite.o: plSeq.h plMesh.h plTriangle.h plShader.h
objects/plIGuideSite.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plIGuideSite.o: plMatrix44.h plProjection.h plCamera.h
objects/plIGuideSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plIGuideSite.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plIGuideSite.o: plLineMesh.h plRenderable.h plTransform.h
objects/plIGuideSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuideSite.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plIGuideSite.o: plDRBTransform.h plMeshCutter.h plMeshExtruder.h
objects/plLineMesh.o: plLineMesh.h plCommon.h plMesh.h plVector3.h plString.h
objects/plLineMesh.o: plTriangle.h plSeq.h plShader.h plRenderingPipeline.h
objects/plLineMesh.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plLineMesh.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plLineMesh.o: plPhongShader.h plPickingTexture.h
objects/plMath.o: plMath.h plCommon.h plVector3.h plString.h plSeq.h
objects/plMath.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plMatrix44.o: plMatrix44.h plCommon.h plVector3.h plString.h
objects/plMatrix44.o: plVector4.h plTriangle.h plSeq.h
objects/plMatrixStack.o: plMatrixStack.h plCommon.h plMatrix44.h plVector3.h
objects/plMatrixStack.o: plString.h plVector4.h plTriangle.h plSeq.h
objects/plMesh.o: plMesh.h plCommon.h plVector3.h plString.h plTriangle.h
objects/plMesh.o: plSeq.h plShader.h plRenderingPipeline.h plVector4.h
objects/plMesh.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMesh.o: plPickingTexture.h
objects/plMeshAlgorithm.o: plMeshAlgorithm.h plVector3.h plCommon.h
objects/plMeshAlgorithm.o: plString.h plPolygon.h plSeq.h plTriangle.h
objects/plMeshAlgorithm.o: plMeshConnectivityData.h
objects/plMeshConnectivityData.o: plMeshConnectivityData.h plSeq.h plCommon.h
objects/plMeshConnectivityData.o: plTriangle.h plString.h plVector3.h
objects/plMeshConnectivityData.o: plPolygon.h
objects/plMeshCutter.o: plMeshCutter.h plCommon.h plVector3.h plString.h
objects/plMeshCutter.o: plSeq.h plTriangle.h plPolygon.h plBoundary.h
objects/plMeshCutter.o: plMath.h plMatrix44.h plVector4.h plRenderable.h
objects/plMeshCutter.o: plEditable.h plPickingTexture.h plBoneAndCartilage.h
objects/plMeshCutter.o: plModel.h plMesh.h plShader.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h
objects/plMeshCutter.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMeshCutter.o: plOctree.h plLineMesh.h plTransform.h
objects/plMeshCutter.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plMeshCutter.o: plTrackedObject.h plDRBTransform.h
objects/plMeshExtruder.o: plMeshExtruder.h plTriangle.h plCommon.h plString.h
objects/plMeshExtruder.o: plVector3.h plSeq.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshIntersector.h plMeshAlgorithm.h
objects/plMeshIntersector.o: plVector3.h plCommon.h plString.h plPolygon.h
objects/plMeshIntersector.o: plSeq.h plTriangle.h plMeshConnectivityData.h
objects/plMeshIntersector.o: plMath.h plMatrix44.h plVector4.h
objects/plMinimalShader.o: plMinimalShader.h plCommon.h plShader.h plSeq.h
objects/plMinimalShader.o: plMatrix44.h plVector3.h plString.h plVector4.h
objects/plMinimalShader.o: plTriangle.h
objects/plModel.o: plModel.h plCommon.h plSeq.h plMesh.h plVector3.h
objects/plModel.o: plString.h plTriangle.h plShader.h plRenderingPipeline.h
objects/plModel.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plModel.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plModel.o: plPhongShader.h plPickingTexture.h plOctree.h plMath.h
objects/plModel.o: plPolygon.h plLineMesh.h plRenderable.h plTransform.h
objects/plModel.o: plTransparentRenderable.h
objects/plModelSpecific.o: plModelSpecific.h plCommon.h plVector3.h
objects/plModelSpecific.o: plString.h plBoneAndCartilage.h plModel.h plSeq.h
objects/plModelSpecific.o: plMesh.h plTriangle.h plShader.h
objects/plModelSpecific.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plModelSpecific.o: plMatrix44.h plProjection.h plCamera.h
objects/plModelSpecific.o: plPickingShader.h plMinimalShader.h
objects/plModelSpecific.o: plPhongShader.h plPickingTexture.h plOctree.h
objects/plModelSpecific.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plModelSpecific.o: plTransform.h plTransparentRenderable.h
objects/plOctree.o: plOctree.h plCommon.h plSeq.h plVector3.h plString.h
objects/plOctree.o: plMath.h plPolygon.h plTriangle.h plMatrix44.h
objects/plOctree.o: plVector4.h plLineMesh.h plMesh.h plShader.h
objects/plOctree.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctree.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plOctree.o: plPhongShader.h plPickingTexture.h plRenderable.h
objects/plOctree.o: plTransform.h
objects/plPhongShader.o: plPhongShader.h plCommon.h plMinimalShader.h
objects/plPhongShader.o: plShader.h plSeq.h plMatrix44.h plVector3.h
objects/plPhongShader.o: plString.h plVector4.h plTriangle.h
objects/plPickingShader.o: plPickingShader.h plCommon.h plMinimalShader.h
objects/plPickingShader.o: plShader.h plSeq.h plMatrix44.h plVector3.h
objects/plPickingShader.o: plString.h plVector4.h plTriangle.h
objects/plPickingTexture.o: plPickingTexture.h plCommon.h
objects/plPlan.o: plPlan.h plCommon.h plString.h plSeq.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plGraft.h plEditable.h plTriangle.h plDraw.h
objects/plPlan.o: plShapeMesh.h plMesh.h plShader.h plRenderingPipeline.h
objects/plPlan.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plPlan.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plPlan.o: plPhongShader.h plPickingTexture.h plTrackedObject.h
objects/plPlan.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plPlan.o: plPolygon.h plLineMesh.h plTransform.h
objects/plPlan.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plPlan.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plPlan.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage0.o: plPlannerStage0.h plCommon.h plUtility.h plSeq.h
objects/plPlannerStage0.o: plVector4.h plString.h plVector3.h plSiteGrid.h
objects/plPlannerStage0.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage0.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage0.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlannerStage0.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plPlannerStage0.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage0.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage0.o: plPhongShader.h plOctree.h plLineMesh.h
objects/plPlannerStage0.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage0.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage0.o: plMeshCutter.h plPlannerStage0Shader.h plSpline.h
objects/plPlannerStage0.o: plColourMesh.h plColourMap.h
objects/plPlannerStage0Shader.o: plPlannerStage0Shader.h plCommon.h
objects/plPlannerStage0Shader.o: plShader.h plSeq.h plSiteGrid.h plVector4.h
objects/plPlannerStage0Shader.o: plString.h plVector3.h plTriangle.h
objects/plPlannerStage0Shader.o: plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage0Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage0Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage0Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage0Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage0Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage0Shader.o: plPhongShader.h plOctree.h plLineMesh.h
objects/plPlannerStage0Shader.o: plTransform.h plTransparentRenderable.h
objects/plPlannerStage0Shader.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plPlannerStage0Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage0Shader.o: plColourMesh.h plColourMap.h
objects/plPlannerStage1.o: plPlannerStage1.h plCommon.h plSeq.h plVector4.h
objects/plPlannerStage1.o: plString.h plVector3.h plSiteGrid.h plTriangle.h
objects/plPlannerStage1.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage1.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage1.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage1.o: plLineMesh.h plTransform.h
objects/plPlannerStage1.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage1.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage1.o: plPlannerStage0.h plUtility.h
objects/plPlannerStage1.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage1.o: plColourMap.h plPlannerStage1Shader.h
objects/plPlannerStage1Shader.o: plPlannerStage1Shader.h plCommon.h
objects/plPlannerStage1Shader.o: plShader.h plSeq.h plSiteGrid.h plVector4.h
objects/plPlannerStage1Shader.o: plString.h plVector3.h plTriangle.h
objects/plPlannerStage1Shader.o: plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage1Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage1Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage1Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage1Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage1Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage1Shader.o: plPhongShader.h plOctree.h plLineMesh.h
objects/plPlannerStage1Shader.o: plTransform.h plTransparentRenderable.h
objects/plPlannerStage1Shader.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plPlannerStage1Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage1Shader.o: plColourMesh.h plColourMap.h
objects/plPlannerStage2.o: plPlannerStage2.h plCommon.h plSeq.h plVector4.h
objects/plPlannerStage2.o: plString.h plVector3.h plSiteGrid.h plTriangle.h
objects/plPlannerStage2.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage2.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage2.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage2.o: plLineMesh.h plTransform.h
objects/plPlannerStage2.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage2.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage2.o: plPlannerStage1.h plPlannerStage0.h plUtility.h
objects/plPlannerStage2.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage2.o: plColourMap.h plPlannerStage1Shader.h
objects/plPlannerStage2.o: plPlannerStage2Shader.h
objects/plPlannerStage2Shader.o: plPlannerStage2Shader.h plCommon.h
objects/plPlannerStage2Shader.o: plShader.h plSeq.h plSiteGrid.h plVector4.h
objects/plPlannerStage2Shader.o: plString.h plVector3.h plTriangle.h
objects/plPlannerStage2Shader.o: plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage2Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage2Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage2Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage2Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage2Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage2Shader.o: plPhongShader.h plOctree.h plLineMesh.h
objects/plPlannerStage2Shader.o: plTransform.h plTransparentRenderable.h
objects/plPlannerStage2Shader.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plPlannerStage2Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage2Shader.o: plColourMesh.h plColourMap.h
objects/plPlannerStage3.o: plPlannerStage3.h plCommon.h plSeq.h plVector4.h
objects/plPlannerStage3.o: plString.h plVector3.h plSiteGrid.h plTriangle.h
objects/plPlannerStage3.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage3.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage3.o: plMinimalShader.h plPhongShader.h plOctree.h
objects/plPlannerStage3.o: plLineMesh.h plTransform.h
objects/plPlannerStage3.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plPlannerStage3.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plPlannerStage3.o: plPlannerStage0.h plUtility.h
objects/plPlannerStage3.o: plPlannerStage0Shader.h plSpline.h plColourMesh.h
objects/plPlannerStage3.o: plColourMap.h plPlannerStage2.h plPlannerStage1.h
objects/plPlannerStage3.o: plPlannerStage1Shader.h plPlannerStage2Shader.h
objects/plPlannerStage3.o: plPlannerStage3Shader.h
objects/plPlannerStage3Shader.o: plPlannerStage3Shader.h plCommon.h
objects/plPlannerStage3Shader.o: plShader.h plSeq.h plSiteGrid.h plVector4.h
objects/plPlannerStage3Shader.o: plString.h plVector3.h plTriangle.h
objects/plPlannerStage3Shader.o: plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage3Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage3Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage3Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage3Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage3Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage3Shader.o: plPhongShader.h plOctree.h plLineMesh.h
objects/plPlannerStage3Shader.o: plTransform.h plTransparentRenderable.h
objects/plPlannerStage3Shader.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plPlannerStage3Shader.o: plDRBTransform.h plMeshCutter.h plSpline.h
objects/plPlannerStage3Shader.o: plColourMesh.h plColourMap.h
objects/plPlug.o: plPlug.h plCommon.h plModelSpecific.h plVector3.h
objects/plPlug.o: plString.h plBoneAndCartilage.h plModel.h plSeq.h plMesh.h
objects/plPlug.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plPlug.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plPlug.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plPlug.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plPlug.o: plLineMesh.h plRenderable.h plTransform.h
objects/plPlug.o: plTransparentRenderable.h
objects/plPolygon.o: plPolygon.h plCommon.h plSeq.h plVector3.h plString.h
objects/plProjection.o: plProjection.h plCommon.h plMatrix44.h plVector3.h
objects/plProjection.o: plString.h plVector4.h plTriangle.h plSeq.h
objects/plRenderable.o: plRenderable.h plCommon.h plVector3.h plString.h
objects/plRenderer.o: plRenderer.h plCommon.h plSeq.h plVector3.h plString.h
objects/plRenderer.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plRenderer.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plRenderer.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plRenderer.o: plPhongShader.h plPickingTexture.h plPlan.h plCSV.h
objects/plRenderer.o: plRenderable.h plGraft.h plEditable.h plDraw.h
objects/plRenderer.o: plShapeMesh.h plMesh.h plTrackedObject.h
objects/plRenderer.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plRenderer.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plRenderer.o: plPolygon.h plLineMesh.h plTransform.h
objects/plRenderer.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plRenderer.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plRenderer.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plRenderer.o: plMeshExtruder.h plIGuide.h plGraftEditor.h plWindow.h
objects/plRenderer.o: plBoundaryEditor.h plAutomaticPlanner.h plSiteGrid.h
objects/plRenderer.o: plPlannerStage0.h plPlannerStage0Shader.h
objects/plRenderer.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plRenderer.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plRenderer.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plRenderer.o: plChessBoard.h plScan.h plScanPoint.h plScanVolume.h
objects/plRenderingPipeline.o: plRenderingPipeline.h plCommon.h plVector4.h
objects/plRenderingPipeline.o: plString.h plVector3.h plMatrixStack.h
objects/plRenderingPipeline.o: plMatrix44.h plTriangle.h plSeq.h
objects/plRenderingPipeline.o: plProjection.h plCamera.h plPickingShader.h
objects/plRenderingPipeline.o: plMinimalShader.h plShader.h plPhongShader.h
objects/plRenderingPipeline.o: plPickingTexture.h
objects/plScan.o: plScan.h plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plSeq.h plScanVolume.h plDraw.h plShapeMesh.h plMesh.h
objects/plScan.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plScan.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plScan.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plScan.o: plPickingTexture.h plTrackedObject.h plRenderable.h
objects/plScan.o: plDRBTransform.h plLineMesh.h
objects/plScanPoint.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScanPoint.o: plSeq.h
objects/plScanVolume.o: plScanVolume.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plSeq.h plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plScanVolume.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plScanVolume.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plScanVolume.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plScanVolume.o: plPhongShader.h plPickingTexture.h plTrackedObject.h
objects/plScanVolume.o: plRenderable.h plDRBTransform.h plLineMesh.h
objects/plShader.o: plShader.h plCommon.h plSeq.h
objects/plShapeMesh.o: plShapeMesh.h plCommon.h plMesh.h plVector3.h
objects/plShapeMesh.o: plString.h plTriangle.h plSeq.h plShader.h
objects/plShapeMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plShapeMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plShapeMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plShapeMesh.o: plPickingTexture.h
objects/plSiteGrid.o: plSiteGrid.h plCommon.h plSeq.h plVector4.h plString.h
objects/plSiteGrid.o: plVector3.h plTriangle.h plBoundary.h plMath.h
objects/plSiteGrid.o: plPolygon.h plMatrix44.h plRenderable.h plEditable.h
objects/plSiteGrid.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plSiteGrid.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plSiteGrid.o: plMatrixStack.h plProjection.h plCamera.h
objects/plSiteGrid.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plSiteGrid.o: plOctree.h plLineMesh.h plTransform.h
objects/plSiteGrid.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plSiteGrid.o: plTrackedObject.h plDRBTransform.h plMeshCutter.h
objects/plSpline.o: plSpline.h plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plSeq.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plEditable.h
objects/plSpline.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plSpline.o: plMesh.h plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plSpline.o: plProjection.h plCamera.h plPickingShader.h
objects/plSpline.o: plMinimalShader.h plPhongShader.h plOctree.h plLineMesh.h
objects/plSpline.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plSpline.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plSpline.o: plColourMesh.h plColourMap.h
objects/plString.o: plString.h plCommon.h
objects/plTexture2D.o: plTexture2D.h plCommon.h plVector3.h plString.h
objects/plTexture2D.o: plSeq.h
objects/plTexture2DMesh.o: plTexture2DMesh.h plCommon.h plMesh.h plVector3.h
objects/plTexture2DMesh.o: plString.h plTriangle.h plSeq.h plShader.h
objects/plTexture2DMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plTexture2DMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plTexture2DMesh.o: plPickingShader.h plMinimalShader.h
objects/plTexture2DMesh.o: plPhongShader.h plPickingTexture.h
objects/plTexture2DShader.o: plTexture2DShader.h plCommon.h plMinimalShader.h
objects/plTexture2DShader.o: plShader.h plSeq.h plMatrix44.h plVector3.h
objects/plTexture2DShader.o: plString.h plVector4.h plTriangle.h
objects/plTexture3D.o: plTexture3D.h plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plSeq.h
objects/plTexture3DMesh.o: plTexture3DMesh.h plCommon.h plMesh.h plVector3.h
objects/plTexture3DMesh.o: plString.h plTriangle.h plSeq.h plShader.h
objects/plTexture3DMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plTexture3DMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plTexture3DMesh.o: plPickingShader.h plMinimalShader.h
objects/plTexture3DMesh.o: plPhongShader.h plPickingTexture.h
objects/plTexture3DShader.o: plTexture3DShader.h plCommon.h plMinimalShader.h
objects/plTexture3DShader.o: plShader.h plSeq.h plMatrix44.h plVector3.h
objects/plTexture3DShader.o: plString.h plVector4.h plTriangle.h
objects/plThreadPool.o: plThreadPool.h
objects/plTrackedObject.o: plTrackedObject.h plCommon.h plRenderable.h
objects/plTrackedObject.o: plVector3.h plString.h plDRBTransform.h
objects/plTrackedObject.o: plMatrix44.h plVector4.h plTriangle.h plSeq.h
objects/plTrackedObject.o: plShapeMesh.h plMesh.h plShader.h
objects/plTrackedObject.o: plRenderingPipeline.h plMatrixStack.h
objects/plTrackedObject.o: plProjection.h plCamera.h plPickingShader.h
objects/plTrackedObject.o: plMinimalShader.h plPhongShader.h
objects/plTrackedObject.o: plPickingTexture.h
objects/plTransform.o: plTransform.h plCommon.h plSeq.h plString.h
objects/plTransform.o: plVector3.h plMatrix44.h plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plTransparentRenderable.h plCommon.h
objects/plTransparentRenderable.o: plRenderable.h plVector3.h plString.h
objects/plTriangle.o: plTriangle.h plCommon.h plString.h plVector3.h plSeq.h
objects/plUtility.o: plUtility.h plCommon.h plSeq.h
objects/plVector3.o: plVector3.h plCommon.h plString.h
objects/plVector4.o: plVector4.h plCommon.h plString.h plVector3.h
objects/plWindow.o: plWindow.h plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plSeq.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h
objects/plWindow.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plWindow.o: plPhongShader.h plPickingTexture.h
