
OPENGL_DRIVER_DIR = /usr/lib/nvidia-331

LIBS = -L$(OPENGL_DRIVER_DIR) -lglut -lGL 

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES =   main.cpp \
            PlannerWindow.cpp \
            pl.cpp \
            plAutomaticPlanner.cpp \
            plBoneAndCartilage.cpp \
            plBoundary.cpp \
            plBoundaryEditor.cpp \
            plCamera.cpp \
            plChessBoard.cpp \
            plColourMap.cpp \
            plColourMesh.cpp \
            plCSV.cpp \
            plDebugTransform.cpp \
            plDebugSphere.cpp \
            plDefectSite.cpp \
            plDonorSite.cpp \
            plDraw.cpp \
            plDRBTransform.cpp \
            plEditable.cpp \
            plGraft.cpp \
            plGraftEditor.cpp \
            plIGuide.cpp \
            plIGuideSite.cpp \
            plLineMesh.cpp \
            plMath.cpp \
            plMatrix44.cpp \
            plMatrixStack.cpp \
            plMesh.cpp \
            plMeshAlgorithm.cpp \
            plMeshConnectivityData.cpp \
            plMeshCutter.cpp \
            plMeshExtruder.cpp \
            plMeshIntersector.cpp \
            plMinimalShader.cpp \
            plModel.cpp \
            plModelSpecific.cpp \
            plOctree.cpp \
            plPhongShader.cpp \
            plPickingShader.cpp \
            plPickingTexture.cpp \
            plPlan.cpp \
            plPlannerStage0.cpp \
            plPlannerStage1.cpp \
            plPlannerStage2.cpp \
            plPlannerStage3.cpp \
            plPlannerStage0Shader.cpp \
            plPlannerStage1Shader.cpp \
            plPlannerStage2Shader.cpp \
            plPlannerStage3Shader.cpp \
            plPlannerStructs.cpp \
            plPlanningBufferData.cpp \
            plPlanningSite.cpp \
            plPlug.cpp \
            plPolygon.cpp \
            plProjection.cpp \
            plRenderable.cpp \
            plRenderer.cpp \
            plRenderingPipeline.cpp \
            plScan.cpp \
            plScanPoint.cpp \
            plScanVolume.cpp \
            plShader.cpp \
            plShapeMesh.cpp \
            plSpline.cpp \
            plSSBO.cpp \
            plString.cpp \
            plTexture2D.cpp \
            plTexture2DMesh.cpp \
            plTexture2DShader.cpp \
            plTexture3D.cpp \
            plTexture3DMesh.cpp \
            plTexture3DShader.cpp \
            plTrackedObject.cpp \
            plTransform.cpp \
            plTransparentRenderable.cpp \
            plTriangle.cpp \
            plUtility.cpp \
            plVector3.cpp \
            plVector4.cpp \
            plWindow.cpp \
            Window.cpp
       
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
objects/PlannerWindow.o: plPlan.h plString.h plVector3.h plCSV.h
objects/PlannerWindow.o: plRenderable.h plGraft.h plEditable.h plTriangle.h
objects/PlannerWindow.o: plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/PlannerWindow.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/PlannerWindow.o: plMatrix44.h plProjection.h plCamera.h
objects/PlannerWindow.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/PlannerWindow.o: plTexture2DShader.h plPickingTexture.h
objects/PlannerWindow.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/PlannerWindow.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/PlannerWindow.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/PlannerWindow.o: plTransform.h plTransparentRenderable.h plUtility.h
objects/PlannerWindow.o: plDefectSite.h plSpline.h plBoundary.h
objects/PlannerWindow.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/PlannerWindow.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/PlannerWindow.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h
objects/PlannerWindow.o: plSSBO.h plPlannerStage0.h plPlannerStructs.h
objects/PlannerWindow.o: plPlannerStage0Shader.h plRenderer.h plGraftEditor.h
objects/PlannerWindow.o: plWindow.h plBoundaryEditor.h plTexture2DMesh.h
objects/PlannerWindow.o: plTexture2D.h plChessBoard.h plScan.h plScanPoint.h
objects/PlannerWindow.o: plScanVolume.h plDebugSphere.h plDebugTransform.h
objects/PlannerWindow.o: plLaserLine.h plPlannerStage1.h
objects/PlannerWindow.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/PlannerWindow.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/PlannerWindow.o: plPlannerStage3Shader.h plTexture3D.h
objects/PlannerWindow.o: plTexture3DMesh.h plTexture3DShader.h
objects/Window.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/Window.o: plVector3.h plCSV.h plRenderable.h plGraft.h plEditable.h
objects/Window.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/Window.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/Window.o: plMatrix44.h plProjection.h plCamera.h plPickingShader.h
objects/Window.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/Window.o: plPickingTexture.h plTrackedObject.h plDRBTransform.h
objects/Window.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/Window.o: plOctree.h plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/Window.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/Window.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/Window.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/Window.o: plPlannerStructs.h plPlannerStage0Shader.h plRenderer.h
objects/Window.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/Window.o: plTexture2DMesh.h plTexture2D.h plChessBoard.h plScan.h
objects/Window.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/Window.o: plDebugTransform.h plLaserLine.h plPlannerStage1.h
objects/Window.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/Window.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/Window.o: plPlannerStage3Shader.h plTexture3D.h plTexture3DMesh.h
objects/Window.o: plTexture3DShader.h
objects/pl.o: plCommon.h plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/pl.o: plCSV.h plRenderable.h plGraft.h plEditable.h plTriangle.h
objects/pl.o: plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/pl.o: plRenderingPipeline.h plVector4.h plMatrixStack.h plMatrix44.h
objects/pl.o: plProjection.h plCamera.h plPickingShader.h plMinimalShader.h
objects/pl.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/pl.o: plTrackedObject.h plDRBTransform.h plPlug.h plModelSpecific.h
objects/pl.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h plPolygon.h
objects/pl.o: plLineMesh.h plTransform.h plTransparentRenderable.h
objects/pl.o: plUtility.h plDefectSite.h plSpline.h plBoundary.h
objects/pl.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuideSite.h
objects/pl.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/pl.o: plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/pl.o: plPlannerStage0.h plPlannerStructs.h plPlannerStage0Shader.h
objects/pl.o: plRenderer.h plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/pl.o: plTexture2DMesh.h plTexture2D.h plChessBoard.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plDebugSphere.h plDebugTransform.h
objects/pl.o: plLaserLine.h plPlannerStage1.h plPlannerStage1Shader.h
objects/pl.o: plPlannerStage2.h plPlannerStage2Shader.h plPlannerStage3.h
objects/pl.o: plPlannerStage3Shader.h plTexture3D.h plTexture3DMesh.h
objects/pl.o: plTexture3DShader.h
objects/plArthroscope.o: plCommon.h
objects/plAutomaticPlanner.o: plCommon.h plPlan.h plString.h plVector3.h
objects/plAutomaticPlanner.o: plCSV.h plRenderable.h plGraft.h plEditable.h
objects/plAutomaticPlanner.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/plAutomaticPlanner.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plAutomaticPlanner.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plAutomaticPlanner.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plAutomaticPlanner.o: plPhongShader.h plTexture2DShader.h
objects/plAutomaticPlanner.o: plPickingTexture.h plTrackedObject.h
objects/plAutomaticPlanner.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plAutomaticPlanner.o: plBoneAndCartilage.h plModel.h plOctree.h
objects/plAutomaticPlanner.o: plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plUtility.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plAutomaticPlanner.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plAutomaticPlanner.o: plIGuide.h plPlanningBufferData.h
objects/plAutomaticPlanner.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/plAutomaticPlanner.o: plPlannerStructs.h plPlannerStage0Shader.h
objects/plAutomaticPlanner.o: plRenderer.h plGraftEditor.h plWindow.h
objects/plAutomaticPlanner.o: plBoundaryEditor.h plTexture2DMesh.h
objects/plAutomaticPlanner.o: plTexture2D.h plAutomaticPlanner.h
objects/plAutomaticPlanner.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plAutomaticPlanner.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plAutomaticPlanner.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plAutomaticPlanner.o: plChessBoard.h plScan.h plScanPoint.h
objects/plAutomaticPlanner.o: plScanVolume.h plDebugSphere.h
objects/plAutomaticPlanner.o: plDebugTransform.h plLaserLine.h
objects/plBoneAndCartilage.o: plCommon.h plModel.h plMesh.h plVector3.h
objects/plBoneAndCartilage.o: plString.h plTriangle.h plShader.h
objects/plBoneAndCartilage.o: plRenderingPipeline.h plVector4.h
objects/plBoneAndCartilage.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plBoneAndCartilage.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plBoneAndCartilage.o: plPhongShader.h plTexture2DShader.h
objects/plBoneAndCartilage.o: plPickingTexture.h plOctree.h plMath.h
objects/plBoneAndCartilage.o: plPolygon.h plLineMesh.h plRenderable.h
objects/plBoneAndCartilage.o: plTransform.h plTransparentRenderable.h
objects/plBoundary.o: plCommon.h plMath.h plVector3.h plString.h plPolygon.h
objects/plBoundary.o: plTriangle.h plMatrix44.h plVector4.h plRenderable.h
objects/plBoundary.o: plEditable.h plPickingTexture.h plBoneAndCartilage.h
objects/plBoundary.o: plModel.h plMesh.h plShader.h plRenderingPipeline.h
objects/plBoundary.o: plMatrixStack.h plProjection.h plCamera.h
objects/plBoundary.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plBoundary.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plBoundary.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plBoundary.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plBoundaryEditor.o: plCommon.h plVector3.h plString.h plPlan.h
objects/plBoundaryEditor.o: plCSV.h plRenderable.h plGraft.h plEditable.h
objects/plBoundaryEditor.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/plBoundaryEditor.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plBoundaryEditor.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plBoundaryEditor.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plBoundaryEditor.o: plPhongShader.h plTexture2DShader.h
objects/plBoundaryEditor.o: plPickingTexture.h plTrackedObject.h
objects/plBoundaryEditor.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plBoundaryEditor.o: plBoneAndCartilage.h plModel.h plOctree.h
objects/plBoundaryEditor.o: plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plBoundaryEditor.o: plTransparentRenderable.h plUtility.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plBoundary.h
objects/plBoundaryEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plWindow.h
objects/plCSV.o: plCommon.h plString.h
objects/plCamera.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plCommon.h plTransparentRenderable.h plRenderable.h
objects/plChessBoard.o: plVector3.h plString.h plRenderingPipeline.h
objects/plChessBoard.o: plVector4.h plMatrixStack.h plMatrix44.h plTriangle.h
objects/plChessBoard.o: plProjection.h plCamera.h plPickingShader.h
objects/plChessBoard.o: plMinimalShader.h plShader.h plPhongShader.h
objects/plChessBoard.o: plTexture2DShader.h plPickingTexture.h plTransform.h
objects/plChessBoard.o: plColourMesh.h plMesh.h plDRBTransform.h
objects/plColourMap.o: plCommon.h plVector3.h plString.h
objects/plColourMesh.o: plCommon.h plVector3.h plString.h plMesh.h
objects/plColourMesh.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plColourMesh.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plColourMesh.o: plProjection.h plCamera.h plPickingShader.h
objects/plColourMesh.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plColourMesh.o: plPickingTexture.h
objects/plDRBTransform.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plDRBTransform.o: plVector4.h plTriangle.h
objects/plDebugSphere.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugSphere.o: plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plDebugSphere.o: plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plDebugSphere.o: plMatrix44.h plProjection.h plCamera.h
objects/plDebugSphere.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plDebugSphere.o: plTexture2DShader.h plPickingTexture.h
objects/plDebugSphere.o: plTrackedObject.h plRenderable.h plDRBTransform.h
objects/plDebugTransform.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugTransform.o: plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plDebugTransform.o: plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plDebugTransform.o: plMatrix44.h plProjection.h plCamera.h
objects/plDebugTransform.o: plPickingShader.h plMinimalShader.h
objects/plDebugTransform.o: plPhongShader.h plTexture2DShader.h
objects/plDebugTransform.o: plPickingTexture.h plTrackedObject.h
objects/plDebugTransform.o: plRenderable.h plDRBTransform.h
objects/plDefectSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDefectSite.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plDefectSite.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plDefectSite.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plDefectSite.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plDefectSite.o: plPhongShader.h plTexture2DShader.h
objects/plDefectSite.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plDefectSite.o: plLineMesh.h plRenderable.h plTransform.h
objects/plDefectSite.o: plTransparentRenderable.h plSpline.h plBoundary.h
objects/plDefectSite.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plDefectSite.o: plDRBTransform.h plColourMesh.h plColourMap.h
objects/plDonorSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDonorSite.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plDonorSite.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plDonorSite.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plDonorSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plDonorSite.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plDonorSite.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plDonorSite.o: plTransform.h plTransparentRenderable.h plBoundary.h
objects/plDonorSite.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plDonorSite.o: plDRBTransform.h
objects/plDraw.o: plCommon.h plShapeMesh.h plMesh.h plVector3.h plString.h
objects/plDraw.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plDraw.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plDraw.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plDraw.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/plDraw.o: plRenderable.h plDRBTransform.h
objects/plEditable.o: plCommon.h
objects/plGraft.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/plGraft.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plGraft.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plGraft.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plGraft.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/plGraft.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plGraft.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plGraft.o: plPolygon.h plLineMesh.h plTransform.h
objects/plGraft.o: plTransparentRenderable.h plUtility.h
objects/plGraftEditor.o: plCommon.h plVector3.h plString.h plTransform.h
objects/plGraftEditor.o: plMatrix44.h plVector4.h plTriangle.h plPlan.h
objects/plGraftEditor.o: plCSV.h plRenderable.h plGraft.h plEditable.h
objects/plGraftEditor.o: plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/plGraftEditor.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plGraftEditor.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plGraftEditor.o: plPhongShader.h plTexture2DShader.h
objects/plGraftEditor.o: plPickingTexture.h plTrackedObject.h
objects/plGraftEditor.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plGraftEditor.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plGraftEditor.o: plPolygon.h plLineMesh.h plTransparentRenderable.h
objects/plGraftEditor.o: plUtility.h plDefectSite.h plSpline.h plBoundary.h
objects/plGraftEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plGraftEditor.o: plIGuide.h plWindow.h
objects/plIGuide.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plIGuide.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plIGuide.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plIGuide.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plIGuide.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plIGuide.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plIGuide.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plIGuide.o: plTransform.h plTransparentRenderable.h plBoundary.h
objects/plIGuide.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plIGuide.o: plDRBTransform.h plGraft.h plPlug.h plUtility.h
objects/plIGuide.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h plSpline.h
objects/plIGuide.o: plColourMesh.h plColourMap.h
objects/plIGuideSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plIGuideSite.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plIGuideSite.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plIGuideSite.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plIGuideSite.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plIGuideSite.o: plPhongShader.h plTexture2DShader.h
objects/plIGuideSite.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plIGuideSite.o: plLineMesh.h plRenderable.h plTransform.h
objects/plIGuideSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuideSite.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plIGuideSite.o: plDRBTransform.h plMeshCutter.h plMeshExtruder.h
objects/plLaserLine.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plLineMesh.o: plCommon.h plMesh.h plVector3.h plString.h plTriangle.h
objects/plLineMesh.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plLineMesh.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plLineMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plLineMesh.o: plTexture2DShader.h plPickingTexture.h
objects/plMath.o: plCommon.h plVector3.h plString.h plPolygon.h plTriangle.h
objects/plMath.o: plMatrix44.h plVector4.h
objects/plMatrix44.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plMatrix44.o: plTriangle.h
objects/plMatrixStack.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plMatrixStack.o: plVector4.h plTriangle.h
objects/plMesh.o: plCommon.h plVector3.h plString.h plTriangle.h plShader.h
objects/plMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plMesh.o: plMatrix44.h plProjection.h plCamera.h plPickingShader.h
objects/plMesh.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plMesh.o: plPickingTexture.h
objects/plMeshAlgorithm.o: plCommon.h plVector3.h plString.h plPolygon.h
objects/plMeshAlgorithm.o: plTriangle.h plMeshConnectivityData.h plUtility.h
objects/plMeshConnectivityData.o: plTriangle.h plCommon.h plString.h
objects/plMeshConnectivityData.o: plVector3.h plPolygon.h plUtility.h
objects/plMeshCutter.o: plCommon.h plVector3.h plString.h plTriangle.h
objects/plMeshCutter.o: plPolygon.h plBoundary.h plMath.h plMatrix44.h
objects/plMeshCutter.o: plVector4.h plRenderable.h plEditable.h
objects/plMeshCutter.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plMeshCutter.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h
objects/plMeshCutter.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMeshCutter.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plMeshCutter.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plMeshCutter.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plMeshExtruder.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plMeshExtruder.o: plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshIntersector.o: plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h plMath.h
objects/plMeshIntersector.o: plMatrix44.h plVector4.h
objects/plMinimalShader.o: plCommon.h plShader.h plString.h plMatrix44.h
objects/plMinimalShader.o: plVector3.h plVector4.h plTriangle.h
objects/plModel.o: plCommon.h plMesh.h plVector3.h plString.h plTriangle.h
objects/plModel.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plModel.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plModel.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plModel.o: plTexture2DShader.h plPickingTexture.h plOctree.h plMath.h
objects/plModel.o: plPolygon.h plLineMesh.h plRenderable.h plTransform.h
objects/plModel.o: plTransparentRenderable.h
objects/plModelSpecific.o: plCommon.h plVector3.h plString.h
objects/plModelSpecific.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plModelSpecific.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plModelSpecific.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plModelSpecific.o: plProjection.h plCamera.h plPickingShader.h
objects/plModelSpecific.o: plMinimalShader.h plPhongShader.h
objects/plModelSpecific.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plModelSpecific.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plModelSpecific.o: plTransform.h plTransparentRenderable.h
objects/plOctree.o: plCommon.h plVector3.h plString.h plMath.h plPolygon.h
objects/plOctree.o: plTriangle.h plMatrix44.h plVector4.h plLineMesh.h
objects/plOctree.o: plMesh.h plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plOctree.o: plProjection.h plCamera.h plPickingShader.h
objects/plOctree.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plOctree.o: plPickingTexture.h plRenderable.h plTransform.h
objects/plPhongShader.o: plCommon.h plMinimalShader.h plShader.h plString.h
objects/plPhongShader.o: plMatrix44.h plVector3.h plVector4.h plTriangle.h
objects/plPickingShader.o: plCommon.h plMinimalShader.h plShader.h plString.h
objects/plPickingShader.o: plMatrix44.h plVector3.h plVector4.h plTriangle.h
objects/plPickingTexture.o: plCommon.h
objects/plPlan.o: plCommon.h plString.h plVector3.h plCSV.h plRenderable.h
objects/plPlan.o: plGraft.h plEditable.h plTriangle.h plDraw.h plShapeMesh.h
objects/plPlan.o: plMesh.h plShader.h plRenderingPipeline.h plVector4.h
objects/plPlan.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plPlan.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plPlan.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/plPlan.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plPlan.o: plPolygon.h plLineMesh.h plTransform.h
objects/plPlan.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plPlan.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plPlan.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage0.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStage0.o: plVector3.h plPlannerStructs.h plSSBO.h
objects/plPlannerStage0.o: plPlannerStage0Shader.h plShader.h plSpline.h
objects/plPlannerStage0.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage0.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage0.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlannerStage0.o: plMesh.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage0.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage0.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage0.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage0.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage0.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage0.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage0.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage0.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage0.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage0.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage0.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage0.o: plAutomaticPlanner.h plPlannerStage0.h
objects/plPlannerStage0.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plPlannerStage0.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plPlannerStage0.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plPlannerStage0.o: plChessBoard.h plScan.h plScanPoint.h
objects/plPlannerStage0.o: plScanVolume.h plDebugSphere.h plDebugTransform.h
objects/plPlannerStage0.o: plLaserLine.h
objects/plPlannerStage0Shader.o: plCommon.h plShader.h plString.h plSpline.h
objects/plPlannerStage0Shader.o: plVector3.h plVector4.h plBoundary.h
objects/plPlannerStage0Shader.o: plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage0Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage0Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage0Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage0Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage0Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage0Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage0Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage0Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage0Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage0Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage0Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage0Shader.o: plMeshCutter.h plSSBO.h
objects/plPlannerStage1.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage1.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage1.o: plPlannerStage0.h plPlannerStage0Shader.h
objects/plPlannerStage1.o: plShader.h plSpline.h plBoundary.h plMath.h
objects/plPlannerStage1.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage1.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage1.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage1.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage1.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage1.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage1.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage1.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage1.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage1.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage1.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage1.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage1.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage1.o: plAutomaticPlanner.h plPlannerStage1.h
objects/plPlannerStage1.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/plPlannerStage1.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plPlannerStage1.o: plPlannerStage3Shader.h plChessBoard.h plScan.h
objects/plPlannerStage1.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage1.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage1Shader.o: plCommon.h plShader.h plString.h plSpline.h
objects/plPlannerStage1Shader.o: plVector3.h plVector4.h plBoundary.h
objects/plPlannerStage1Shader.o: plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage1Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage1Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage1Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage1Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage1Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage1Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage1Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage1Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage1Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage1Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage1Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage1Shader.o: plMeshCutter.h plSSBO.h plPlannerStructs.h
objects/plPlannerStage1Shader.o: plUtility.h
objects/plPlannerStage2.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage2.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage2.o: plPlannerStage1.h plPlannerStage0.h
objects/plPlannerStage2.o: plPlannerStage0Shader.h plShader.h plSpline.h
objects/plPlannerStage2.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage2.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage2.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlannerStage2.o: plMesh.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage2.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage2.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage2.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage2.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage2.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage2.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage2.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage2.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage2.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage2.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage2.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage2.o: plAutomaticPlanner.h plPlannerStage2.h
objects/plPlannerStage2.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plPlannerStage2.o: plPlannerStage3Shader.h plChessBoard.h plScan.h
objects/plPlannerStage2.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage2.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage2.o: plPlannerStage1Shader.h
objects/plPlannerStage2Shader.o: plCommon.h plShader.h plString.h plSpline.h
objects/plPlannerStage2Shader.o: plVector3.h plVector4.h plBoundary.h
objects/plPlannerStage2Shader.o: plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage2Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage2Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage2Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage2Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage2Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage2Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage2Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage2Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage2Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage2Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage2Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage2Shader.o: plMeshCutter.h plSSBO.h plPlannerStructs.h
objects/plPlannerStage2Shader.o: plUtility.h
objects/plPlannerStage3.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage3.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage3.o: plPlannerStage0.h plPlannerStage0Shader.h
objects/plPlannerStage3.o: plShader.h plSpline.h plBoundary.h plMath.h
objects/plPlannerStage3.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage3.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage3.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage3.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage3.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage3.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage3.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage3.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage3.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage3.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage3.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage3.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage3.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage3.o: plAutomaticPlanner.h plPlannerStage1.h
objects/plPlannerStage3.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/plPlannerStage3.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plPlannerStage3.o: plPlannerStage3Shader.h plChessBoard.h plScan.h
objects/plPlannerStage3.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage3.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage3Shader.o: plCommon.h plShader.h plString.h plSpline.h
objects/plPlannerStage3Shader.o: plVector3.h plVector4.h plBoundary.h
objects/plPlannerStage3Shader.o: plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage3Shader.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage3Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage3Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage3Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage3Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage3Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage3Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage3Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage3Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage3Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage3Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage3Shader.o: plMeshCutter.h plSSBO.h plPlannerStructs.h
objects/plPlannerStage3Shader.o: plUtility.h
objects/plPlannerStructs.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStructs.o: plVector3.h plSSBO.h
objects/plPlanningBufferData.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningBufferData.o: plPlanningSite.h plTriangle.h plBoundary.h
objects/plPlanningBufferData.o: plMath.h plPolygon.h plMatrix44.h
objects/plPlanningBufferData.o: plRenderable.h plEditable.h
objects/plPlanningBufferData.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlanningBufferData.o: plModel.h plMesh.h plShader.h
objects/plPlanningBufferData.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningBufferData.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlanningBufferData.o: plMinimalShader.h plPhongShader.h
objects/plPlanningBufferData.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlanningBufferData.o: plTransform.h plTransparentRenderable.h
objects/plPlanningBufferData.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plPlanningBufferData.o: plDRBTransform.h plMeshCutter.h plSSBO.h
objects/plPlanningBufferData.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlanningBufferData.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plPlanningSite.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningSite.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlanningSite.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlanningSite.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlanningSite.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plPlanningSite.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanningSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plPlanningSite.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlanningSite.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlanningSite.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlanningSite.o: plMeshCutter.h plSSBO.h
objects/plPlug.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plPlug.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plPlug.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plPlug.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plPlug.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plPlug.o: plTexture2DShader.h plPickingTexture.h plOctree.h plMath.h
objects/plPlug.o: plPolygon.h plLineMesh.h plRenderable.h plTransform.h
objects/plPlug.o: plTransparentRenderable.h
objects/plPolygon.o: plCommon.h plVector3.h plString.h
objects/plProjection.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plProjection.o: plVector4.h plTriangle.h
objects/plRenderable.o: plCommon.h plVector3.h plString.h
objects/plRenderer.o: plCommon.h plVector3.h plString.h plRenderingPipeline.h
objects/plRenderer.o: plVector4.h plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderer.o: plProjection.h plCamera.h plPickingShader.h
objects/plRenderer.o: plMinimalShader.h plShader.h plPhongShader.h
objects/plRenderer.o: plTexture2DShader.h plPickingTexture.h plPlan.h plCSV.h
objects/plRenderer.o: plRenderable.h plGraft.h plEditable.h plDraw.h
objects/plRenderer.o: plShapeMesh.h plMesh.h plTrackedObject.h
objects/plRenderer.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plRenderer.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plRenderer.o: plPolygon.h plLineMesh.h plTransform.h
objects/plRenderer.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plRenderer.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plRenderer.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plRenderer.o: plMeshExtruder.h plIGuide.h plGraftEditor.h plWindow.h
objects/plRenderer.o: plBoundaryEditor.h plTexture2DMesh.h plTexture2D.h
objects/plRenderer.o: plAutomaticPlanner.h plPlanningBufferData.h
objects/plRenderer.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/plRenderer.o: plPlannerStructs.h plPlannerStage0Shader.h plRenderer.h
objects/plRenderer.o: plChessBoard.h plScan.h plScanPoint.h plScanVolume.h
objects/plRenderer.o: plDebugSphere.h plDebugTransform.h plLaserLine.h
objects/plRenderer.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plRenderer.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plRenderer.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plRenderingPipeline.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plRenderingPipeline.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderingPipeline.o: plProjection.h plCamera.h plPickingShader.h
objects/plRenderingPipeline.o: plMinimalShader.h plShader.h plPhongShader.h
objects/plRenderingPipeline.o: plTexture2DShader.h plPickingTexture.h
objects/plSSBO.o: plCommon.h
objects/plScan.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plScan.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plScan.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plScan.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plScan.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/plScan.o: plRenderable.h plDRBTransform.h plLineMesh.h
objects/plScanPoint.o: plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plVector3.h plCommon.h plString.h plDraw.h
objects/plScanVolume.o: plShapeMesh.h plMesh.h plTriangle.h plShader.h
objects/plScanVolume.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plScanVolume.o: plMatrix44.h plProjection.h plCamera.h
objects/plScanVolume.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plScanVolume.o: plTexture2DShader.h plPickingTexture.h
objects/plScanVolume.o: plTrackedObject.h plRenderable.h plDRBTransform.h
objects/plScanVolume.o: plLineMesh.h
objects/plSeq.o: plCommon.h
objects/plShader.o: plCommon.h plString.h
objects/plShapeMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plShapeMesh.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plShapeMesh.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plShapeMesh.o: plProjection.h plCamera.h plPickingShader.h
objects/plShapeMesh.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plShapeMesh.o: plPickingTexture.h
objects/plSpline.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plEditable.h
objects/plSpline.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plSpline.o: plMesh.h plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plSpline.o: plProjection.h plCamera.h plPickingShader.h
objects/plSpline.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plSpline.o: plOctree.h plLineMesh.h plTransform.h
objects/plSpline.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plSpline.o: plTrackedObject.h plDRBTransform.h plColourMesh.h
objects/plSpline.o: plColourMap.h
objects/plString.o: plCommon.h
objects/plTexture2D.o: plCommon.h plVector3.h plString.h
objects/plTexture2DMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plTexture2DMesh.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plTexture2DMesh.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plTexture2DMesh.o: plProjection.h plCamera.h plPickingShader.h
objects/plTexture2DMesh.o: plMinimalShader.h plPhongShader.h
objects/plTexture2DMesh.o: plTexture2DShader.h plPickingTexture.h
objects/plTexture2DMesh.o: plTexture2D.h
objects/plTexture2DShader.o: plCommon.h plMinimalShader.h plShader.h
objects/plTexture2DShader.o: plString.h plMatrix44.h plVector3.h plVector4.h
objects/plTexture2DShader.o: plTriangle.h
objects/plTexture3D.o: plCommon.h plVector3.h plString.h
objects/plTexture3DMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plTexture3DMesh.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plTexture3DMesh.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plTexture3DMesh.o: plProjection.h plCamera.h plPickingShader.h
objects/plTexture3DMesh.o: plMinimalShader.h plPhongShader.h
objects/plTexture3DMesh.o: plTexture2DShader.h plPickingTexture.h
objects/plTexture3DMesh.o: plTexture3D.h
objects/plTexture3DShader.o: plCommon.h plMinimalShader.h plShader.h
objects/plTexture3DShader.o: plString.h plMatrix44.h plVector3.h plVector4.h
objects/plTexture3DShader.o: plTriangle.h
objects/plTrackedObject.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plTrackedObject.o: plDRBTransform.h plMatrix44.h plVector4.h
objects/plTrackedObject.o: plTriangle.h plShapeMesh.h plMesh.h plShader.h
objects/plTrackedObject.o: plRenderingPipeline.h plMatrixStack.h
objects/plTrackedObject.o: plProjection.h plCamera.h plPickingShader.h
objects/plTrackedObject.o: plMinimalShader.h plPhongShader.h
objects/plTrackedObject.o: plTexture2DShader.h plPickingTexture.h
objects/plTransform.o: plCommon.h plString.h plVector3.h plMatrix44.h
objects/plTransform.o: plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plCommon.h plRenderable.h plVector3.h
objects/plTransparentRenderable.o: plString.h
objects/plTriangle.o: plCommon.h plString.h plVector3.h
objects/plUtility.o: plCommon.h
objects/plVector3.o: plCommon.h plString.h
objects/plVector4.o: plCommon.h plString.h plVector3.h
objects/plWindow.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h
objects/plWindow.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plWindow.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h
objects/PlannerWindow.o: plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/PlannerWindow.o: plCSV.h plRenderable.h plGraft.h plEditable.h
objects/PlannerWindow.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/PlannerWindow.o: plShader.h plRenderingPipeline.h plVector4.h
objects/PlannerWindow.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/PlannerWindow.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/PlannerWindow.o: plPhongShader.h plTexture2DShader.h
objects/PlannerWindow.o: plPickingTexture.h plTrackedObject.h
objects/PlannerWindow.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/PlannerWindow.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/PlannerWindow.o: plPolygon.h plLineMesh.h plTransform.h
objects/PlannerWindow.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/PlannerWindow.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/PlannerWindow.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/PlannerWindow.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/PlannerWindow.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/PlannerWindow.o: plPlannerStructs.h plPlannerStage0Shader.h
objects/PlannerWindow.o: plRenderer.h plGraftEditor.h plWindow.h
objects/PlannerWindow.o: plBoundaryEditor.h plTexture2DMesh.h plTexture2D.h
objects/PlannerWindow.o: plChessBoard.h plScan.h plScanPoint.h plScanVolume.h
objects/PlannerWindow.o: plDebugSphere.h plDebugTransform.h plLaserLine.h
objects/PlannerWindow.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/PlannerWindow.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/PlannerWindow.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/PlannerWindow.o: plTexture3D.h plTexture3DMesh.h plTexture3DShader.h
objects/Window.o: Window.h pl.h plCommon.h plAutomaticPlanner.h plPlan.h
objects/Window.o: plString.h plVector3.h plCSV.h plRenderable.h plGraft.h
objects/Window.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/Window.o: plShader.h plRenderingPipeline.h plVector4.h
objects/Window.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/Window.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/Window.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/Window.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/Window.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/Window.o: plPolygon.h plLineMesh.h plTransform.h
objects/Window.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/Window.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/Window.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/Window.o: plPlannerStructs.h plPlannerStage0Shader.h plRenderer.h
objects/Window.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/Window.o: plTexture2DMesh.h plTexture2D.h plChessBoard.h plScan.h
objects/Window.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/Window.o: plDebugTransform.h plLaserLine.h plPlannerStage1.h
objects/Window.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/Window.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/Window.o: plPlannerStage3Shader.h plTexture3D.h plTexture3DMesh.h
objects/Window.o: plTexture3DShader.h
objects/main.o: PlannerWindow.h Window.h pl.h plCommon.h plAutomaticPlanner.h
objects/main.o: plPlan.h plString.h plVector3.h plCSV.h plRenderable.h
objects/main.o: plGraft.h plEditable.h plTriangle.h plDraw.h plShapeMesh.h
objects/main.o: plMesh.h plShader.h plRenderingPipeline.h plVector4.h
objects/main.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/main.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/main.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/main.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/main.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/main.o: plPolygon.h plLineMesh.h plTransform.h
objects/main.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/main.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/main.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/main.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/main.o: plPlannerStage0.h plPlannerStructs.h plPlannerStage0Shader.h
objects/main.o: plRenderer.h plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/main.o: plTexture2DMesh.h plTexture2D.h plChessBoard.h plScan.h
objects/main.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/main.o: plDebugTransform.h plLaserLine.h plPlannerStage1.h
objects/main.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/main.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/main.o: plPlannerStage3Shader.h plTexture3D.h plTexture3DMesh.h
objects/main.o: plTexture3DShader.h
objects/pl.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/pl.o: plVector3.h plCSV.h plRenderable.h plGraft.h plEditable.h
objects/pl.o: plTriangle.h plDraw.h plShapeMesh.h plMesh.h plShader.h
objects/pl.o: plRenderingPipeline.h plVector4.h plMatrixStack.h plMatrix44.h
objects/pl.o: plProjection.h plCamera.h plPickingShader.h plMinimalShader.h
objects/pl.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/pl.o: plTrackedObject.h plDRBTransform.h plPlug.h plModelSpecific.h
objects/pl.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h plPolygon.h
objects/pl.o: plLineMesh.h plTransform.h plTransparentRenderable.h
objects/pl.o: plUtility.h plDefectSite.h plSpline.h plBoundary.h
objects/pl.o: plColourMesh.h plColourMap.h plDonorSite.h plIGuideSite.h
objects/pl.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/pl.o: plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/pl.o: plPlannerStage0.h plPlannerStructs.h plPlannerStage0Shader.h
objects/pl.o: plRenderer.h plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/pl.o: plTexture2DMesh.h plTexture2D.h plChessBoard.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plDebugSphere.h plDebugTransform.h
objects/pl.o: plLaserLine.h plPlannerStage1.h plPlannerStage1Shader.h
objects/pl.o: plPlannerStage2.h plPlannerStage2Shader.h plPlannerStage3.h
objects/pl.o: plPlannerStage3Shader.h plTexture3D.h plTexture3DMesh.h
objects/pl.o: plTexture3DShader.h
objects/plArthroscope.o: plArthroscope.h plCommon.h
objects/plAutomaticPlanner.o: plAutomaticPlanner.h plCommon.h plPlan.h
objects/plAutomaticPlanner.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/plAutomaticPlanner.o: plGraft.h plEditable.h plTriangle.h plDraw.h
objects/plAutomaticPlanner.o: plShapeMesh.h plMesh.h plShader.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plVector4.h
objects/plAutomaticPlanner.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plAutomaticPlanner.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plAutomaticPlanner.o: plPhongShader.h plTexture2DShader.h
objects/plAutomaticPlanner.o: plPickingTexture.h plTrackedObject.h
objects/plAutomaticPlanner.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plAutomaticPlanner.o: plBoneAndCartilage.h plModel.h plOctree.h
objects/plAutomaticPlanner.o: plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plUtility.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plAutomaticPlanner.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plAutomaticPlanner.o: plIGuide.h plPlanningBufferData.h
objects/plAutomaticPlanner.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/plAutomaticPlanner.o: plPlannerStructs.h plPlannerStage0Shader.h
objects/plAutomaticPlanner.o: plRenderer.h plGraftEditor.h plWindow.h
objects/plAutomaticPlanner.o: plBoundaryEditor.h plTexture2DMesh.h
objects/plAutomaticPlanner.o: plTexture2D.h plChessBoard.h plScan.h
objects/plAutomaticPlanner.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plAutomaticPlanner.o: plDebugTransform.h plLaserLine.h
objects/plAutomaticPlanner.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plAutomaticPlanner.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plAutomaticPlanner.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plBoneAndCartilage.o: plBoneAndCartilage.h plCommon.h plModel.h
objects/plBoneAndCartilage.o: plMesh.h plVector3.h plString.h plTriangle.h
objects/plBoneAndCartilage.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plBoneAndCartilage.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plBoneAndCartilage.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plBoneAndCartilage.o: plPhongShader.h plTexture2DShader.h
objects/plBoneAndCartilage.o: plPickingTexture.h plOctree.h plMath.h
objects/plBoneAndCartilage.o: plPolygon.h plLineMesh.h plRenderable.h
objects/plBoneAndCartilage.o: plTransform.h plTransparentRenderable.h
objects/plBoundary.o: plBoundary.h plCommon.h plMath.h plVector3.h plString.h
objects/plBoundary.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plBoundary.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plBoundary.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plBoundary.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plBoundary.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plBoundary.o: plPhongShader.h plTexture2DShader.h plOctree.h
objects/plBoundary.o: plLineMesh.h plTransform.h plTransparentRenderable.h
objects/plBoundary.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plBoundary.o: plDRBTransform.h
objects/plBoundaryEditor.o: plBoundaryEditor.h plCommon.h plVector3.h
objects/plBoundaryEditor.o: plString.h plPlan.h plCSV.h plRenderable.h
objects/plBoundaryEditor.o: plGraft.h plEditable.h plTriangle.h plDraw.h
objects/plBoundaryEditor.o: plShapeMesh.h plMesh.h plShader.h
objects/plBoundaryEditor.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plBoundaryEditor.o: plMatrix44.h plProjection.h plCamera.h
objects/plBoundaryEditor.o: plPickingShader.h plMinimalShader.h
objects/plBoundaryEditor.o: plPhongShader.h plTexture2DShader.h
objects/plBoundaryEditor.o: plPickingTexture.h plTrackedObject.h
objects/plBoundaryEditor.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plBoundaryEditor.o: plBoneAndCartilage.h plModel.h plOctree.h
objects/plBoundaryEditor.o: plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plBoundaryEditor.o: plTransparentRenderable.h plUtility.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plBoundary.h
objects/plBoundaryEditor.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plWindow.h
objects/plCSV.o: plCSV.h plCommon.h plString.h
objects/plCamera.o: plCamera.h plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plChessBoard.h plCommon.h plTransparentRenderable.h
objects/plChessBoard.o: plRenderable.h plVector3.h plString.h
objects/plChessBoard.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plChessBoard.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plChessBoard.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plChessBoard.o: plPhongShader.h plTexture2DShader.h
objects/plChessBoard.o: plPickingTexture.h plTransform.h plColourMesh.h
objects/plChessBoard.o: plMesh.h plDRBTransform.h
objects/plColourMap.o: plColourMap.h plCommon.h plVector3.h plString.h
objects/plColourMesh.o: plColourMesh.h plCommon.h plVector3.h plString.h
objects/plColourMesh.o: plMesh.h plTriangle.h plShader.h
objects/plColourMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plColourMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plColourMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plColourMesh.o: plTexture2DShader.h plPickingTexture.h
objects/plDRBTransform.o: plDRBTransform.h plCommon.h plMatrix44.h
objects/plDRBTransform.o: plVector3.h plString.h plVector4.h plTriangle.h
objects/plDebugSphere.o: plDebugSphere.h plCommon.h plVector3.h plString.h
objects/plDebugSphere.o: plVector4.h plDraw.h plShapeMesh.h plMesh.h
objects/plDebugSphere.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plDebugSphere.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plDebugSphere.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plDebugSphere.o: plPhongShader.h plTexture2DShader.h
objects/plDebugSphere.o: plPickingTexture.h plTrackedObject.h plRenderable.h
objects/plDebugSphere.o: plDRBTransform.h
objects/plDebugTransform.o: plDebugTransform.h plCommon.h plVector3.h
objects/plDebugTransform.o: plString.h plVector4.h plDraw.h plShapeMesh.h
objects/plDebugTransform.o: plMesh.h plTriangle.h plShader.h
objects/plDebugTransform.o: plRenderingPipeline.h plMatrixStack.h
objects/plDebugTransform.o: plMatrix44.h plProjection.h plCamera.h
objects/plDebugTransform.o: plPickingShader.h plMinimalShader.h
objects/plDebugTransform.o: plPhongShader.h plTexture2DShader.h
objects/plDebugTransform.o: plPickingTexture.h plTrackedObject.h
objects/plDebugTransform.o: plRenderable.h plDRBTransform.h
objects/plDefectSite.o: plDefectSite.h plCommon.h plVector3.h plString.h
objects/plDefectSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDefectSite.o: plMesh.h plTriangle.h plShader.h
objects/plDefectSite.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plDefectSite.o: plMatrix44.h plProjection.h plCamera.h
objects/plDefectSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plDefectSite.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plDefectSite.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plDefectSite.o: plTransform.h plTransparentRenderable.h plSpline.h
objects/plDefectSite.o: plBoundary.h plEditable.h plDraw.h plShapeMesh.h
objects/plDefectSite.o: plTrackedObject.h plDRBTransform.h plColourMesh.h
objects/plDefectSite.o: plColourMap.h
objects/plDonorSite.o: plDonorSite.h plCommon.h plVector3.h plString.h
objects/plDonorSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDonorSite.o: plMesh.h plTriangle.h plShader.h plRenderingPipeline.h
objects/plDonorSite.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plDonorSite.o: plProjection.h plCamera.h plPickingShader.h
objects/plDonorSite.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plDonorSite.o: plPickingTexture.h plOctree.h plMath.h plPolygon.h
objects/plDonorSite.o: plLineMesh.h plRenderable.h plTransform.h
objects/plDonorSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plDonorSite.o: plDraw.h plShapeMesh.h plTrackedObject.h
objects/plDonorSite.o: plDRBTransform.h
objects/plDraw.o: plDraw.h plCommon.h plShapeMesh.h plMesh.h plVector3.h
objects/plDraw.o: plString.h plTriangle.h plShader.h plRenderingPipeline.h
objects/plDraw.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plDraw.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plDraw.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/plDraw.o: plTrackedObject.h plRenderable.h plDRBTransform.h
objects/plEditable.o: plEditable.h plCommon.h
objects/plGraft.o: plGraft.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plEditable.h plTriangle.h plDraw.h plShapeMesh.h plMesh.h
objects/plGraft.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plGraft.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plGraft.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plGraft.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/plGraft.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plGraft.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plGraft.o: plPolygon.h plLineMesh.h plTransform.h
objects/plGraft.o: plTransparentRenderable.h plUtility.h
objects/plGraftEditor.o: plGraftEditor.h plCommon.h plVector3.h plString.h
objects/plGraftEditor.o: plTransform.h plMatrix44.h plVector4.h plTriangle.h
objects/plGraftEditor.o: plPlan.h plCSV.h plRenderable.h plGraft.h
objects/plGraftEditor.o: plEditable.h plDraw.h plShapeMesh.h plMesh.h
objects/plGraftEditor.o: plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plGraftEditor.o: plProjection.h plCamera.h plPickingShader.h
objects/plGraftEditor.o: plMinimalShader.h plPhongShader.h
objects/plGraftEditor.o: plTexture2DShader.h plPickingTexture.h
objects/plGraftEditor.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/plGraftEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraftEditor.o: plOctree.h plMath.h plPolygon.h plLineMesh.h
objects/plGraftEditor.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plGraftEditor.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plGraftEditor.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plGraftEditor.o: plMeshExtruder.h plIGuide.h plWindow.h
objects/plIGuide.o: plIGuide.h plCommon.h plModelSpecific.h plVector3.h
objects/plIGuide.o: plString.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plIGuide.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plIGuide.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plIGuide.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plIGuide.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plIGuide.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plIGuide.o: plTransform.h plTransparentRenderable.h plBoundary.h
objects/plIGuide.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plIGuide.o: plDRBTransform.h plGraft.h plPlug.h plUtility.h
objects/plIGuide.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h plSpline.h
objects/plIGuide.o: plColourMesh.h plColourMap.h
objects/plIGuideSite.o: plIGuideSite.h plCommon.h plVector3.h plString.h
objects/plIGuideSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plIGuideSite.o: plMesh.h plTriangle.h plShader.h
objects/plIGuideSite.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plIGuideSite.o: plMatrix44.h plProjection.h plCamera.h
objects/plIGuideSite.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plIGuideSite.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plIGuideSite.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plIGuideSite.o: plTransform.h plTransparentRenderable.h plBoundary.h
objects/plIGuideSite.o: plEditable.h plDraw.h plShapeMesh.h plTrackedObject.h
objects/plIGuideSite.o: plDRBTransform.h plMeshCutter.h plMeshExtruder.h
objects/plLaserLine.o: plLaserLine.h plCommon.h plVector3.h plString.h
objects/plLaserLine.o: plVector4.h
objects/plLineMesh.o: plLineMesh.h plCommon.h plMesh.h plVector3.h plString.h
objects/plLineMesh.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plLineMesh.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plLineMesh.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plLineMesh.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/plMath.o: plMath.h plCommon.h plVector3.h plString.h plPolygon.h
objects/plMath.o: plTriangle.h plMatrix44.h plVector4.h
objects/plMatrix44.o: plMatrix44.h plCommon.h plVector3.h plString.h
objects/plMatrix44.o: plVector4.h plTriangle.h
objects/plMatrixStack.o: plMatrixStack.h plCommon.h plMatrix44.h plVector3.h
objects/plMatrixStack.o: plString.h plVector4.h plTriangle.h
objects/plMesh.o: plMesh.h plCommon.h plVector3.h plString.h plTriangle.h
objects/plMesh.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plMesh.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMesh.o: plTexture2DShader.h plPickingTexture.h
objects/plMeshAlgorithm.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshAlgorithm.o: plString.h plPolygon.h plTriangle.h
objects/plMeshAlgorithm.o: plMeshConnectivityData.h plUtility.h
objects/plMeshConnectivityData.o: plMeshConnectivityData.h plTriangle.h
objects/plMeshConnectivityData.o: plCommon.h plString.h plVector3.h
objects/plMeshConnectivityData.o: plPolygon.h plUtility.h
objects/plMeshCutter.o: plMeshCutter.h plCommon.h plVector3.h plString.h
objects/plMeshCutter.o: plTriangle.h plPolygon.h plBoundary.h plMath.h
objects/plMeshCutter.o: plMatrix44.h plVector4.h plRenderable.h plEditable.h
objects/plMeshCutter.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plMeshCutter.o: plMesh.h plShader.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h
objects/plMeshCutter.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plMeshCutter.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plMeshCutter.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plMeshCutter.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plMeshExtruder.o: plMeshExtruder.h plTriangle.h plCommon.h plString.h
objects/plMeshExtruder.o: plVector3.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshIntersector.h plMeshAlgorithm.h plCommon.h
objects/plMeshIntersector.o: plVector3.h plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h plMath.h
objects/plMeshIntersector.o: plMatrix44.h plVector4.h
objects/plMinimalShader.o: plMinimalShader.h plCommon.h plShader.h plString.h
objects/plMinimalShader.o: plMatrix44.h plVector3.h plVector4.h plTriangle.h
objects/plModel.o: plModel.h plCommon.h plMesh.h plVector3.h plString.h
objects/plModel.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plModel.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plModel.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plModel.o: plTexture2DShader.h plPickingTexture.h plOctree.h plMath.h
objects/plModel.o: plPolygon.h plLineMesh.h plRenderable.h plTransform.h
objects/plModel.o: plTransparentRenderable.h
objects/plModelSpecific.o: plModelSpecific.h plCommon.h plVector3.h
objects/plModelSpecific.o: plString.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plModelSpecific.o: plTriangle.h plShader.h plRenderingPipeline.h
objects/plModelSpecific.o: plVector4.h plMatrixStack.h plMatrix44.h
objects/plModelSpecific.o: plProjection.h plCamera.h plPickingShader.h
objects/plModelSpecific.o: plMinimalShader.h plPhongShader.h
objects/plModelSpecific.o: plTexture2DShader.h plPickingTexture.h plOctree.h
objects/plModelSpecific.o: plMath.h plPolygon.h plLineMesh.h plRenderable.h
objects/plModelSpecific.o: plTransform.h plTransparentRenderable.h
objects/plOctree.o: plOctree.h plCommon.h plVector3.h plString.h plMath.h
objects/plOctree.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plOctree.o: plLineMesh.h plMesh.h plShader.h plRenderingPipeline.h
objects/plOctree.o: plMatrixStack.h plProjection.h plCamera.h
objects/plOctree.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plOctree.o: plTexture2DShader.h plPickingTexture.h plRenderable.h
objects/plOctree.o: plTransform.h
objects/plPhongShader.o: plPhongShader.h plCommon.h plMinimalShader.h
objects/plPhongShader.o: plShader.h plString.h plMatrix44.h plVector3.h
objects/plPhongShader.o: plVector4.h plTriangle.h
objects/plPickingShader.o: plPickingShader.h plCommon.h plMinimalShader.h
objects/plPickingShader.o: plShader.h plString.h plMatrix44.h plVector3.h
objects/plPickingShader.o: plVector4.h plTriangle.h
objects/plPickingTexture.o: plPickingTexture.h plCommon.h
objects/plPlan.o: plPlan.h plCommon.h plString.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plGraft.h plEditable.h plTriangle.h plDraw.h
objects/plPlan.o: plShapeMesh.h plMesh.h plShader.h plRenderingPipeline.h
objects/plPlan.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plPlan.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plPlan.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/plPlan.o: plTrackedObject.h plDRBTransform.h plPlug.h
objects/plPlan.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plOctree.h
objects/plPlan.o: plMath.h plPolygon.h plLineMesh.h plTransform.h
objects/plPlan.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plPlan.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plPlan.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage0.o: plPlannerStage0.h plCommon.h plUtility.h
objects/plPlannerStage0.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage0.o: plPlannerStructs.h plSSBO.h
objects/plPlannerStage0.o: plPlannerStage0Shader.h plShader.h plSpline.h
objects/plPlannerStage0.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage0.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage0.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlannerStage0.o: plMesh.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage0.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage0.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage0.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage0.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage0.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage0.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage0.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage0.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage0.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage0.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage0.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage0.o: plAutomaticPlanner.h plPlannerStage1.h
objects/plPlannerStage0.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/plPlannerStage0.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plPlannerStage0.o: plPlannerStage3Shader.h plChessBoard.h plScan.h
objects/plPlannerStage0.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage0.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage0Shader.o: plPlannerStage0Shader.h plCommon.h
objects/plPlannerStage0Shader.o: plShader.h plString.h plSpline.h plVector3.h
objects/plPlannerStage0Shader.o: plVector4.h plBoundary.h plMath.h
objects/plPlannerStage0Shader.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage0Shader.o: plRenderable.h plEditable.h
objects/plPlannerStage0Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage0Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage0Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage0Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage0Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage0Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage0Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage0Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage0Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage0Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage0Shader.o: plMeshCutter.h plSSBO.h
objects/plPlannerStage1.o: plPlannerStage1.h plCommon.h plVector4.h
objects/plPlannerStage1.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage1.o: plUtility.h plSSBO.h plPlannerStage0.h
objects/plPlannerStage1.o: plPlannerStage0Shader.h plShader.h plSpline.h
objects/plPlannerStage1.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage1.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage1.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlannerStage1.o: plMesh.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage1.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage1.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage1.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage1.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage1.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage1.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage1.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage1.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage1.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage1.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage1.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage1.o: plAutomaticPlanner.h plPlannerStage2.h
objects/plPlannerStage1.o: plPlannerStage2Shader.h plPlannerStage3.h
objects/plPlannerStage1.o: plPlannerStage3Shader.h plChessBoard.h plScan.h
objects/plPlannerStage1.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage1.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage1.o: plPlannerStage1Shader.h
objects/plPlannerStage1Shader.o: plPlannerStage1Shader.h plCommon.h
objects/plPlannerStage1Shader.o: plShader.h plString.h plSpline.h plVector3.h
objects/plPlannerStage1Shader.o: plVector4.h plBoundary.h plMath.h
objects/plPlannerStage1Shader.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage1Shader.o: plRenderable.h plEditable.h
objects/plPlannerStage1Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage1Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage1Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage1Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage1Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage1Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage1Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage1Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage1Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage1Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage1Shader.o: plMeshCutter.h plSSBO.h plPlannerStructs.h
objects/plPlannerStage1Shader.o: plUtility.h
objects/plPlannerStage2.o: plPlannerStage2.h plCommon.h plVector4.h
objects/plPlannerStage2.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage2.o: plUtility.h plSSBO.h plPlannerStage1.h
objects/plPlannerStage2.o: plPlannerStage0.h plPlannerStage0Shader.h
objects/plPlannerStage2.o: plShader.h plSpline.h plBoundary.h plMath.h
objects/plPlannerStage2.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage2.o: plRenderable.h plEditable.h plPickingTexture.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage2.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage2.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage2.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage2.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage2.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage2.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage2.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage2.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage2.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage2.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage2.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage2.o: plAutomaticPlanner.h plPlannerStage3.h
objects/plPlannerStage2.o: plPlannerStage3Shader.h plChessBoard.h plScan.h
objects/plPlannerStage2.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage2.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage2.o: plPlannerStage1Shader.h plPlannerStage2Shader.h
objects/plPlannerStage2Shader.o: plPlannerStage2Shader.h plCommon.h
objects/plPlannerStage2Shader.o: plShader.h plString.h plSpline.h plVector3.h
objects/plPlannerStage2Shader.o: plVector4.h plBoundary.h plMath.h
objects/plPlannerStage2Shader.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage2Shader.o: plRenderable.h plEditable.h
objects/plPlannerStage2Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage2Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage2Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage2Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage2Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage2Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage2Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage2Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage2Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage2Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage2Shader.o: plMeshCutter.h plSSBO.h plPlannerStructs.h
objects/plPlannerStage2Shader.o: plUtility.h
objects/plPlannerStage3.o: plPlannerStage3.h plCommon.h plVector4.h
objects/plPlannerStage3.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage3.o: plUtility.h plSSBO.h plPlannerStage0.h
objects/plPlannerStage3.o: plPlannerStage0Shader.h plShader.h plSpline.h
objects/plPlannerStage3.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plPlannerStage3.o: plMatrix44.h plRenderable.h plEditable.h
objects/plPlannerStage3.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plPlannerStage3.o: plMesh.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlannerStage3.o: plMinimalShader.h plPhongShader.h
objects/plPlannerStage3.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlannerStage3.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlannerStage3.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlannerStage3.o: plColourMesh.h plColourMap.h plPlanningSite.h
objects/plPlannerStage3.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage3.o: plDefectSite.h plModelSpecific.h plDonorSite.h
objects/plPlannerStage3.o: plRenderer.h plPlan.h plCSV.h plGraft.h plPlug.h
objects/plPlannerStage3.o: plIGuideSite.h plMeshExtruder.h plIGuide.h
objects/plPlannerStage3.o: plGraftEditor.h plWindow.h plBoundaryEditor.h
objects/plPlannerStage3.o: plTexture2DMesh.h plTexture2D.h
objects/plPlannerStage3.o: plAutomaticPlanner.h plPlannerStage1.h
objects/plPlannerStage3.o: plPlannerStage1Shader.h plPlannerStage2.h
objects/plPlannerStage3.o: plPlannerStage2Shader.h plChessBoard.h plScan.h
objects/plPlannerStage3.o: plScanPoint.h plScanVolume.h plDebugSphere.h
objects/plPlannerStage3.o: plDebugTransform.h plLaserLine.h
objects/plPlannerStage3.o: plPlannerStage3Shader.h
objects/plPlannerStage3Shader.o: plPlannerStage3Shader.h plCommon.h
objects/plPlannerStage3Shader.o: plShader.h plString.h plSpline.h plVector3.h
objects/plPlannerStage3Shader.o: plVector4.h plBoundary.h plMath.h
objects/plPlannerStage3Shader.o: plPolygon.h plTriangle.h plMatrix44.h
objects/plPlannerStage3Shader.o: plRenderable.h plEditable.h
objects/plPlannerStage3Shader.o: plPickingTexture.h plBoneAndCartilage.h
objects/plPlannerStage3Shader.o: plModel.h plMesh.h plRenderingPipeline.h
objects/plPlannerStage3Shader.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlannerStage3Shader.o: plPickingShader.h plMinimalShader.h
objects/plPlannerStage3Shader.o: plPhongShader.h plTexture2DShader.h
objects/plPlannerStage3Shader.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlannerStage3Shader.o: plTransparentRenderable.h plDraw.h
objects/plPlannerStage3Shader.o: plShapeMesh.h plTrackedObject.h
objects/plPlannerStage3Shader.o: plDRBTransform.h plColourMesh.h
objects/plPlannerStage3Shader.o: plColourMap.h plPlanningSite.h
objects/plPlannerStage3Shader.o: plMeshCutter.h plSSBO.h plPlannerStructs.h
objects/plPlannerStage3Shader.o: plUtility.h
objects/plPlannerStructs.o: plPlannerStructs.h plCommon.h plUtility.h
objects/plPlannerStructs.o: plVector4.h plString.h plVector3.h plSSBO.h
objects/plPlanningBufferData.o: plPlanningBufferData.h plCommon.h plVector4.h
objects/plPlanningBufferData.o: plString.h plVector3.h plPlanningSite.h
objects/plPlanningBufferData.o: plTriangle.h plBoundary.h plMath.h
objects/plPlanningBufferData.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlanningBufferData.o: plEditable.h plPickingTexture.h
objects/plPlanningBufferData.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlanningBufferData.o: plShader.h plRenderingPipeline.h
objects/plPlanningBufferData.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanningBufferData.o: plPickingShader.h plMinimalShader.h
objects/plPlanningBufferData.o: plPhongShader.h plTexture2DShader.h
objects/plPlanningBufferData.o: plOctree.h plLineMesh.h plTransform.h
objects/plPlanningBufferData.o: plTransparentRenderable.h plDraw.h
objects/plPlanningBufferData.o: plShapeMesh.h plTrackedObject.h
objects/plPlanningBufferData.o: plDRBTransform.h plMeshCutter.h plSSBO.h
objects/plPlanningBufferData.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlanningBufferData.o: plColourMesh.h plColourMap.h plDonorSite.h
objects/plPlanningSite.o: plPlanningSite.h plCommon.h plVector4.h plString.h
objects/plPlanningSite.o: plVector3.h plTriangle.h plBoundary.h plMath.h
objects/plPlanningSite.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlanningSite.o: plEditable.h plPickingTexture.h
objects/plPlanningSite.o: plBoneAndCartilage.h plModel.h plMesh.h plShader.h
objects/plPlanningSite.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningSite.o: plProjection.h plCamera.h plPickingShader.h
objects/plPlanningSite.o: plMinimalShader.h plPhongShader.h
objects/plPlanningSite.o: plTexture2DShader.h plOctree.h plLineMesh.h
objects/plPlanningSite.o: plTransform.h plTransparentRenderable.h plDraw.h
objects/plPlanningSite.o: plShapeMesh.h plTrackedObject.h plDRBTransform.h
objects/plPlanningSite.o: plMeshCutter.h plSSBO.h
objects/plPlug.o: plPlug.h plCommon.h plModelSpecific.h plVector3.h
objects/plPlug.o: plString.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlug.o: plTriangle.h plShader.h plRenderingPipeline.h plVector4.h
objects/plPlug.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plPlug.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plPlug.o: plTexture2DShader.h plPickingTexture.h plOctree.h plMath.h
objects/plPlug.o: plPolygon.h plLineMesh.h plRenderable.h plTransform.h
objects/plPlug.o: plTransparentRenderable.h
objects/plPolygon.o: plPolygon.h plCommon.h plVector3.h plString.h
objects/plProjection.o: plProjection.h plCommon.h plMatrix44.h plVector3.h
objects/plProjection.o: plString.h plVector4.h plTriangle.h
objects/plRenderable.o: plRenderable.h plCommon.h plVector3.h plString.h
objects/plRenderer.o: plRenderer.h plCommon.h plVector3.h plString.h
objects/plRenderer.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plRenderer.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plRenderer.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plRenderer.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
objects/plRenderer.o: plPlan.h plCSV.h plRenderable.h plGraft.h plEditable.h
objects/plRenderer.o: plDraw.h plShapeMesh.h plMesh.h plTrackedObject.h
objects/plRenderer.o: plDRBTransform.h plPlug.h plModelSpecific.h
objects/plRenderer.o: plBoneAndCartilage.h plModel.h plOctree.h plMath.h
objects/plRenderer.o: plPolygon.h plLineMesh.h plTransform.h
objects/plRenderer.o: plTransparentRenderable.h plUtility.h plDefectSite.h
objects/plRenderer.o: plSpline.h plBoundary.h plColourMesh.h plColourMap.h
objects/plRenderer.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plRenderer.o: plMeshExtruder.h plIGuide.h plGraftEditor.h plWindow.h
objects/plRenderer.o: plBoundaryEditor.h plTexture2DMesh.h plTexture2D.h
objects/plRenderer.o: plAutomaticPlanner.h plPlanningBufferData.h
objects/plRenderer.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/plRenderer.o: plPlannerStructs.h plPlannerStage0Shader.h
objects/plRenderer.o: plPlannerStage1.h plPlannerStage1Shader.h
objects/plRenderer.o: plPlannerStage2.h plPlannerStage2Shader.h
objects/plRenderer.o: plPlannerStage3.h plPlannerStage3Shader.h
objects/plRenderer.o: plChessBoard.h plScan.h plScanPoint.h plScanVolume.h
objects/plRenderer.o: plDebugSphere.h plDebugTransform.h plLaserLine.h
objects/plRenderingPipeline.o: plRenderingPipeline.h plCommon.h plVector4.h
objects/plRenderingPipeline.o: plString.h plVector3.h plMatrixStack.h
objects/plRenderingPipeline.o: plMatrix44.h plTriangle.h plProjection.h
objects/plRenderingPipeline.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plRenderingPipeline.o: plShader.h plPhongShader.h plTexture2DShader.h
objects/plRenderingPipeline.o: plPickingTexture.h
objects/plSSBO.o: plSSBO.h plCommon.h
objects/plScan.o: plScan.h plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plScan.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plScan.o: plMatrixStack.h plMatrix44.h plProjection.h plCamera.h
objects/plScan.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plScan.o: plTexture2DShader.h plPickingTexture.h plTrackedObject.h
objects/plScan.o: plRenderable.h plDRBTransform.h plLineMesh.h
objects/plScanPoint.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plScanVolume.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plDraw.h plShapeMesh.h plMesh.h plTriangle.h
objects/plScanVolume.o: plShader.h plRenderingPipeline.h plVector4.h
objects/plScanVolume.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plScanVolume.o: plCamera.h plPickingShader.h plMinimalShader.h
objects/plScanVolume.o: plPhongShader.h plTexture2DShader.h
objects/plScanVolume.o: plPickingTexture.h plTrackedObject.h plRenderable.h
objects/plScanVolume.o: plDRBTransform.h plLineMesh.h
objects/plShader.o: plShader.h plCommon.h plString.h
objects/plShapeMesh.o: plShapeMesh.h plCommon.h plMesh.h plVector3.h
objects/plShapeMesh.o: plString.h plTriangle.h plShader.h
objects/plShapeMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plShapeMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plShapeMesh.o: plPickingShader.h plMinimalShader.h plPhongShader.h
objects/plShapeMesh.o: plTexture2DShader.h plPickingTexture.h
objects/plSpline.o: plSpline.h plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plEditable.h
objects/plSpline.o: plPickingTexture.h plBoneAndCartilage.h plModel.h
objects/plSpline.o: plMesh.h plShader.h plRenderingPipeline.h plMatrixStack.h
objects/plSpline.o: plProjection.h plCamera.h plPickingShader.h
objects/plSpline.o: plMinimalShader.h plPhongShader.h plTexture2DShader.h
objects/plSpline.o: plOctree.h plLineMesh.h plTransform.h
objects/plSpline.o: plTransparentRenderable.h plDraw.h plShapeMesh.h
objects/plSpline.o: plTrackedObject.h plDRBTransform.h plColourMesh.h
objects/plSpline.o: plColourMap.h
objects/plString.o: plString.h plCommon.h
objects/plTexture2D.o: plTexture2D.h plCommon.h plVector3.h plString.h
objects/plTexture2DMesh.o: plTexture2DMesh.h plCommon.h plMesh.h plVector3.h
objects/plTexture2DMesh.o: plString.h plTriangle.h plShader.h
objects/plTexture2DMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plTexture2DMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plTexture2DMesh.o: plPickingShader.h plMinimalShader.h
objects/plTexture2DMesh.o: plPhongShader.h plTexture2DShader.h
objects/plTexture2DMesh.o: plPickingTexture.h plTexture2D.h
objects/plTexture2DShader.o: plTexture2DShader.h plCommon.h plMinimalShader.h
objects/plTexture2DShader.o: plShader.h plString.h plMatrix44.h plVector3.h
objects/plTexture2DShader.o: plVector4.h plTriangle.h
objects/plTexture3D.o: plTexture3D.h plCommon.h plVector3.h plString.h
objects/plTexture3DMesh.o: plTexture3DMesh.h plCommon.h plMesh.h plVector3.h
objects/plTexture3DMesh.o: plString.h plTriangle.h plShader.h
objects/plTexture3DMesh.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plTexture3DMesh.o: plMatrix44.h plProjection.h plCamera.h
objects/plTexture3DMesh.o: plPickingShader.h plMinimalShader.h
objects/plTexture3DMesh.o: plPhongShader.h plTexture2DShader.h
objects/plTexture3DMesh.o: plPickingTexture.h plTexture3D.h
objects/plTexture3DShader.o: plTexture3DShader.h plCommon.h plMinimalShader.h
objects/plTexture3DShader.o: plShader.h plString.h plMatrix44.h plVector3.h
objects/plTexture3DShader.o: plVector4.h plTriangle.h
objects/plThreadPool.o: plThreadPool.h
objects/plTrackedObject.o: plTrackedObject.h plCommon.h plRenderable.h
objects/plTrackedObject.o: plVector3.h plString.h plDRBTransform.h
objects/plTrackedObject.o: plMatrix44.h plVector4.h plTriangle.h
objects/plTrackedObject.o: plShapeMesh.h plMesh.h plShader.h
objects/plTrackedObject.o: plRenderingPipeline.h plMatrixStack.h
objects/plTrackedObject.o: plProjection.h plCamera.h plPickingShader.h
objects/plTrackedObject.o: plMinimalShader.h plPhongShader.h
objects/plTrackedObject.o: plTexture2DShader.h plPickingTexture.h
objects/plTransform.o: plTransform.h plCommon.h plString.h plVector3.h
objects/plTransform.o: plMatrix44.h plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plTransparentRenderable.h plCommon.h
objects/plTransparentRenderable.o: plRenderable.h plVector3.h plString.h
objects/plTriangle.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plUtility.o: plUtility.h plCommon.h
objects/plVector3.o: plVector3.h plCommon.h plString.h plVector4.h
objects/plVector4.o: plVector4.h plCommon.h plString.h plVector3.h
objects/plWindow.o: plWindow.h plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h
objects/plWindow.o: plPickingShader.h plMinimalShader.h plShader.h
objects/plWindow.o: plPhongShader.h plTexture2DShader.h plPickingTexture.h
