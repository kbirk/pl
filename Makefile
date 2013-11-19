
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
            plBufferObject.cpp \
            plCamera.cpp \
            plChessBoard.cpp \
            plColourMap.cpp \
            plCSV.cpp \
            plDebugTransform.cpp \
            plDebugSphere.cpp \
            plDefectSite.cpp \
            plDonorSite.cpp \
            plDraw.cpp \
            plDRBTransform.cpp \
            plEABO.cpp \
            plEditable.cpp \
            plFBO.cpp \
            plGraft.cpp \
            plGraftEditor.cpp \
            plIGuide.cpp \
            plIGuideSite.cpp \
            plMath.cpp \
            plMatrix44.cpp \
            plMatrixStack.cpp \
            plMesh.cpp \
            plMeshAlgorithm.cpp \
            plMeshConnectivityData.cpp \
            plMeshCutter.cpp \
            plMeshExtruder.cpp \
            plMeshIntersector.cpp \
            plModel.cpp \
            plModelSpecific.cpp \
            plOctree.cpp \
            plOctreeMesh.cpp \
            plOpenGLInfo.cpp \
            plPickingTexture.cpp \
            plPlan.cpp \
            plPlannerShader.cpp \
            plPlannerStage0.cpp \
            plPlannerStage1.cpp \
            plPlannerStage2.cpp \
            plPlannerStage3.cpp \
            plPlannerStructs.cpp \
            plPlanningBufferData.cpp \
            plPlanningSite.cpp \
            plPlug.cpp \
            plPolygon.cpp \
            plProjection.cpp \
            plRenderComponent.cpp \
            plRenderable.cpp \
            plRenderer.cpp \
            plRenderingPipeline.cpp \
            plScan.cpp \
            plScanPoint.cpp \
            plScanVolume.cpp \
            plShader.cpp \
            plSphere.cpp \
            plSpline.cpp \
            plSSBO.cpp \
            plString.cpp \
            plTexture2D.cpp \
            plTexture3D.cpp \
            plTrackedObject.cpp \
            plTransform.cpp \
            plTransparentRenderable.cpp \
            plTriangle.cpp \
            plUtility.cpp \
            plUniform.cpp \
            plVAO.cpp \
            plVBO.cpp \
            plVector3.cpp \
            plVector4.cpp \
            plVertexFragmentShader.cpp \
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
objects/PlannerWindow.o: plRenderable.h plRenderComponent.h plVector4.h
objects/PlannerWindow.o: plMatrix44.h plTriangle.h plTexture2D.h
objects/PlannerWindow.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/PlannerWindow.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/PlannerWindow.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/PlannerWindow.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/PlannerWindow.o: plGraft.h plEditable.h plDraw.h plPlug.h
objects/PlannerWindow.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/PlannerWindow.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/PlannerWindow.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/PlannerWindow.o: plUtility.h plRenderer.h plSphere.h plDefectSite.h
objects/PlannerWindow.o: plSpline.h plBoundary.h plColourMap.h plDonorSite.h
objects/PlannerWindow.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/PlannerWindow.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h
objects/PlannerWindow.o: plSSBO.h plPlannerStage0.h plPlannerStructs.h
objects/PlannerWindow.o: plPlannerShader.h plPlannerStage1.h
objects/PlannerWindow.o: plPlannerStage2.h plPlannerStage3.h
objects/PlannerWindow.o: plBoundaryEditor.h plWindow.h plDisk.h
objects/PlannerWindow.o: plChessBoard.h plDRBTransform.h plGraftEditor.h
objects/PlannerWindow.o: plScan.h plScanPoint.h plScanVolume.h plTexture3D.h
objects/PlannerWindow.o: plTrackedObject.h
objects/Window.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/Window.o: plVector3.h plCSV.h plRenderable.h plRenderComponent.h
objects/Window.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/Window.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/Window.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/Window.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/Window.o: plVertexFragmentShader.h plShader.h plUniform.h plGraft.h
objects/Window.o: plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/Window.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/Window.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/Window.o: plMath.h plPolygon.h plTransform.h plUtility.h plRenderer.h
objects/Window.o: plSphere.h plDefectSite.h plSpline.h plBoundary.h
objects/Window.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/Window.o: plPlannerStructs.h plPlannerShader.h plPlannerStage1.h
objects/Window.o: plPlannerStage2.h plPlannerStage3.h plBoundaryEditor.h
objects/Window.o: plWindow.h plDisk.h plChessBoard.h plDRBTransform.h
objects/Window.o: plGraftEditor.h plScan.h plScanPoint.h plScanVolume.h
objects/Window.o: plTexture3D.h plTrackedObject.h
objects/pl.o: plCommon.h plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/pl.o: plCSV.h plRenderable.h plRenderComponent.h plVector4.h
objects/pl.o: plMatrix44.h plTriangle.h plTexture2D.h plPickingTexture.h
objects/pl.o: plVAO.h plVBO.h plBufferObject.h plEABO.h plRenderingPipeline.h
objects/pl.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/pl.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h plUniform.h
objects/pl.o: plGraft.h plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/pl.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/pl.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h plMath.h
objects/pl.o: plPolygon.h plTransform.h plUtility.h plRenderer.h plSphere.h
objects/pl.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/pl.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/pl.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/pl.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/pl.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/pl.o: plBoundaryEditor.h plWindow.h plDisk.h plChessBoard.h
objects/pl.o: plDRBTransform.h plGraftEditor.h plScan.h plScanPoint.h
objects/pl.o: plScanVolume.h plTexture3D.h plTrackedObject.h
objects/plArthroscope.o: plCommon.h
objects/plAutomaticPlanner.o: plCommon.h plPlan.h plString.h plVector3.h
objects/plAutomaticPlanner.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plAutomaticPlanner.o: plVector4.h plMatrix44.h plTriangle.h
objects/plAutomaticPlanner.o: plTexture2D.h plPickingTexture.h plVAO.h
objects/plAutomaticPlanner.o: plVBO.h plBufferObject.h plEABO.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plMatrixStack.h
objects/plAutomaticPlanner.o: plProjection.h plCamera.h plFBO.h
objects/plAutomaticPlanner.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plAutomaticPlanner.o: plShader.h plUniform.h plGraft.h plEditable.h
objects/plAutomaticPlanner.o: plDraw.h plPlug.h plModelSpecific.h
objects/plAutomaticPlanner.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plOctreeMesh.h
objects/plAutomaticPlanner.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/plAutomaticPlanner.o: plUtility.h plRenderer.h plSphere.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plAutomaticPlanner.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plAutomaticPlanner.o: plPlanningBufferData.h plPlanningSite.h
objects/plAutomaticPlanner.o: plSSBO.h plPlannerStage0.h plPlannerStructs.h
objects/plAutomaticPlanner.o: plPlannerShader.h plPlannerStage1.h
objects/plAutomaticPlanner.o: plPlannerStage2.h plPlannerStage3.h
objects/plBoneAndCartilage.o: plCommon.h plModel.h plMesh.h plVector3.h
objects/plBoneAndCartilage.o: plString.h plTriangle.h
objects/plBoneAndCartilage.o: plTransparentRenderable.h plRenderable.h
objects/plBoneAndCartilage.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plBoneAndCartilage.o: plTexture2D.h plPickingTexture.h plVAO.h
objects/plBoneAndCartilage.o: plVBO.h plBufferObject.h plEABO.h
objects/plBoneAndCartilage.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoneAndCartilage.o: plProjection.h plCamera.h plFBO.h
objects/plBoneAndCartilage.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plBoneAndCartilage.o: plShader.h plUniform.h plOctreeMesh.h
objects/plBoneAndCartilage.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/plBoneAndCartilage.o: plDraw.h
objects/plBoundary.o: plCommon.h plMath.h plVector3.h plString.h plPolygon.h
objects/plBoundary.o: plTriangle.h plMatrix44.h plVector4.h plRenderable.h
objects/plBoundary.o: plRenderComponent.h plTexture2D.h plPickingTexture.h
objects/plBoundary.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plBoundary.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plBoundary.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plBoundary.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plBoundary.o: plEditable.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plBoundary.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plBoundary.o: plTransform.h plDraw.h plSphere.h plRenderer.h
objects/plBoundaryEditor.o: plCommon.h plVector3.h plString.h plPlan.h
objects/plBoundaryEditor.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plBoundaryEditor.o: plVector4.h plMatrix44.h plTriangle.h
objects/plBoundaryEditor.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plBoundaryEditor.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plBoundaryEditor.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plBoundaryEditor.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plBoundaryEditor.o: plShader.h plUniform.h plGraft.h plEditable.h
objects/plBoundaryEditor.o: plDraw.h plPlug.h plModelSpecific.h
objects/plBoundaryEditor.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plBoundaryEditor.o: plTransparentRenderable.h plOctreeMesh.h
objects/plBoundaryEditor.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/plBoundaryEditor.o: plUtility.h plRenderer.h plSphere.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plBoundary.h
objects/plBoundaryEditor.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plBoundaryEditor.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plBoundaryEditor.o: plWindow.h plDisk.h
objects/plBufferObject.o: plCommon.h
objects/plCSV.o: plCommon.h plString.h
objects/plCamera.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plCommon.h plTransparentRenderable.h plRenderable.h
objects/plChessBoard.o: plVector3.h plString.h plRenderComponent.h
objects/plChessBoard.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/plChessBoard.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plChessBoard.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plChessBoard.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plChessBoard.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plChessBoard.o: plTransform.h plDRBTransform.h
objects/plColourMap.o: plCommon.h plVector3.h plString.h
objects/plDRBTransform.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plDRBTransform.o: plVector4.h plTriangle.h
objects/plDebugSphere.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugSphere.o: plDraw.h plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plDebugSphere.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDebugSphere.o: plTriangle.h plProjection.h plCamera.h
objects/plDebugSphere.o: plPickingTexture.h plFBO.h plTexture2D.h
objects/plDebugSphere.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plDebugSphere.o: plUniform.h
objects/plDebugTransform.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugTransform.o: plDraw.h plVAO.h plVBO.h plBufferObject.h
objects/plDebugTransform.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plDebugTransform.o: plMatrix44.h plTriangle.h plProjection.h
objects/plDebugTransform.o: plCamera.h plPickingTexture.h plFBO.h
objects/plDebugTransform.o: plTexture2D.h plOpenGLInfo.h
objects/plDebugTransform.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plDefectSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDefectSite.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plDefectSite.o: plTransparentRenderable.h plRenderable.h
objects/plDefectSite.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plDefectSite.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plDefectSite.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plDefectSite.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plDefectSite.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plDefectSite.o: plUniform.h plOctreeMesh.h plOctree.h plMath.h
objects/plDefectSite.o: plPolygon.h plTransform.h plDraw.h plSpline.h
objects/plDefectSite.o: plBoundary.h plEditable.h plSphere.h plRenderer.h
objects/plDefectSite.o: plColourMap.h
objects/plDisk.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plDisk.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plDisk.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plDisk.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plDisk.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plDisk.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plDisk.o: plUniform.h
objects/plDonorSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDonorSite.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plDonorSite.o: plTransparentRenderable.h plRenderable.h
objects/plDonorSite.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plDonorSite.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plDonorSite.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plDonorSite.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plDonorSite.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plDonorSite.o: plUniform.h plOctreeMesh.h plOctree.h plMath.h
objects/plDonorSite.o: plPolygon.h plTransform.h plDraw.h plBoundary.h
objects/plDonorSite.o: plEditable.h plSphere.h plRenderer.h
objects/plDraw.o: plCommon.h plVAO.h plVBO.h plBufferObject.h plVector4.h
objects/plDraw.o: plString.h plVector3.h plEABO.h plRenderingPipeline.h
objects/plDraw.o: plMatrixStack.h plMatrix44.h plTriangle.h plProjection.h
objects/plDraw.o: plCamera.h plPickingTexture.h plFBO.h plTexture2D.h
objects/plDraw.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plDraw.o: plUniform.h
objects/plEABO.o: plCommon.h plBufferObject.h
objects/plEditable.o: plCommon.h
objects/plFBO.o: plCommon.h plVector3.h plString.h plTexture2D.h
objects/plFBO.o: plOpenGLInfo.h
objects/plGraft.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plGraft.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plGraft.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plGraft.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plGraft.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plGraft.o: plUniform.h plEditable.h plDraw.h plPlug.h
objects/plGraft.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plGraft.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plGraft.o: plMath.h plPolygon.h plTransform.h plUtility.h
objects/plGraft.o: plRenderer.h plSphere.h
objects/plGraftEditor.o: plCommon.h plVector3.h plString.h plTransform.h
objects/plGraftEditor.o: plMatrix44.h plVector4.h plTriangle.h plPlan.h
objects/plGraftEditor.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plGraftEditor.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plGraftEditor.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plGraftEditor.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plGraftEditor.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plGraftEditor.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/plGraftEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraftEditor.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/plGraftEditor.o: plOctree.h plMath.h plPolygon.h plUtility.h
objects/plGraftEditor.o: plRenderer.h plSphere.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plGraftEditor.o: plIGuide.h plWindow.h
objects/plIGuide.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plIGuide.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plIGuide.o: plTransparentRenderable.h plRenderable.h
objects/plIGuide.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plIGuide.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plIGuide.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plIGuide.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plIGuide.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plIGuide.o: plUniform.h plOctreeMesh.h plOctree.h plMath.h
objects/plIGuide.o: plPolygon.h plTransform.h plDraw.h plBoundary.h
objects/plIGuide.o: plEditable.h plSphere.h plRenderer.h plGraft.h plPlug.h
objects/plIGuide.o: plUtility.h plIGuideSite.h plMeshCutter.h
objects/plIGuide.o: plMeshExtruder.h plSpline.h plColourMap.h
objects/plIGuideSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plIGuideSite.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plIGuideSite.o: plTransparentRenderable.h plRenderable.h
objects/plIGuideSite.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plIGuideSite.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plIGuideSite.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plIGuideSite.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plIGuideSite.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plIGuideSite.o: plUniform.h plOctreeMesh.h plOctree.h plMath.h
objects/plIGuideSite.o: plPolygon.h plTransform.h plDraw.h plBoundary.h
objects/plIGuideSite.o: plEditable.h plSphere.h plRenderer.h plMeshCutter.h
objects/plIGuideSite.o: plMeshExtruder.h
objects/plLaserLine.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plMath.o: plCommon.h plVector3.h plString.h plPolygon.h plTriangle.h
objects/plMath.o: plMatrix44.h plVector4.h
objects/plMatrix44.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plMatrix44.o: plTriangle.h
objects/plMatrixStack.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plMatrixStack.o: plVector4.h plTriangle.h
objects/plMesh.o: plCommon.h plVector3.h plString.h plTriangle.h
objects/plMeshAlgorithm.o: plCommon.h plVector3.h plString.h plPolygon.h
objects/plMeshAlgorithm.o: plTriangle.h plMeshConnectivityData.h plUtility.h
objects/plMeshConnectivityData.o: plTriangle.h plCommon.h plString.h
objects/plMeshConnectivityData.o: plVector3.h plPolygon.h plUtility.h
objects/plMeshCutter.o: plCommon.h plVector3.h plString.h plTriangle.h
objects/plMeshCutter.o: plPolygon.h plBoundary.h plMath.h plMatrix44.h
objects/plMeshCutter.o: plVector4.h plRenderable.h plRenderComponent.h
objects/plMeshCutter.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plMeshCutter.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plMeshCutter.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plMeshCutter.o: plUniform.h plEditable.h plBoneAndCartilage.h
objects/plMeshCutter.o: plModel.h plMesh.h plTransparentRenderable.h
objects/plMeshCutter.o: plOctreeMesh.h plOctree.h plTransform.h plDraw.h
objects/plMeshCutter.o: plSphere.h plRenderer.h
objects/plMeshExtruder.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plMeshExtruder.o: plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshIntersector.o: plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h plMath.h
objects/plMeshIntersector.o: plMatrix44.h plVector4.h
objects/plModel.o: plCommon.h plMesh.h plVector3.h plString.h plTriangle.h
objects/plModel.o: plTransparentRenderable.h plRenderable.h
objects/plModel.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plModel.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plModel.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plModel.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plModel.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plModel.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plModel.o: plTransform.h plDraw.h
objects/plModelSpecific.o: plCommon.h plVector3.h plString.h
objects/plModelSpecific.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plModelSpecific.o: plTriangle.h plTransparentRenderable.h
objects/plModelSpecific.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plModelSpecific.o: plMatrix44.h plTexture2D.h plPickingTexture.h
objects/plModelSpecific.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plModelSpecific.o: plRenderingPipeline.h plMatrixStack.h
objects/plModelSpecific.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plModelSpecific.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plModelSpecific.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plModelSpecific.o: plTransform.h plDraw.h
objects/plOctree.o: plCommon.h plVector3.h plString.h plMath.h plPolygon.h
objects/plOctree.o: plTriangle.h plMatrix44.h plVector4.h plRenderable.h
objects/plOctree.o: plRenderComponent.h plTexture2D.h plPickingTexture.h
objects/plOctree.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plOctree.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctree.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plOctree.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plOctree.o: plTransform.h plDraw.h
objects/plOctreeMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plOctreeMesh.o: plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plOctreeMesh.o: plMatrix44.h plVector4.h plRenderable.h
objects/plOctreeMesh.o: plRenderComponent.h plTexture2D.h plPickingTexture.h
objects/plOctreeMesh.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plOctreeMesh.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctreeMesh.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plOctreeMesh.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plOctreeMesh.o: plTransform.h plDraw.h
objects/plOpenGLInfo.o: plCommon.h
objects/plPickingTexture.o: plCommon.h
objects/plPlan.o: plCommon.h plString.h plVector3.h plCSV.h plRenderable.h
objects/plPlan.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plPlan.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plPlan.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plPlan.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plPlan.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plPlan.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/plPlan.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlan.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plPlan.o: plMath.h plPolygon.h plTransform.h plUtility.h plRenderer.h
objects/plPlan.o: plSphere.h plDefectSite.h plSpline.h plBoundary.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlannerShader.o: plCommon.h plShader.h plString.h plOpenGLInfo.h
objects/plPlannerShader.o: plUniform.h plPlanningSite.h plVector4.h
objects/plPlannerShader.o: plVector3.h plTriangle.h plBoundary.h plMath.h
objects/plPlannerShader.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerShader.o: plRenderComponent.h plTexture2D.h
objects/plPlannerShader.o: plPickingTexture.h plVAO.h plVBO.h
objects/plPlannerShader.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plPlannerShader.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plPlannerShader.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerShader.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerShader.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerShader.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerShader.o: plRenderer.h plMeshCutter.h plSSBO.h
objects/plPlannerShader.o: plPlannerStructs.h plUtility.h
objects/plPlannerStage0.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStage0.o: plVector3.h plPlannerStructs.h plSSBO.h
objects/plPlannerStage0.o: plBufferObject.h plPlannerShader.h plShader.h
objects/plPlannerStage0.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage0.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage0.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage0.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plPlannerStage0.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage0.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage0.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage0.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage0.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage0.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage0.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage0.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage0.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage0.o: plIGuide.h
objects/plPlannerStage1.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage1.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage1.o: plBufferObject.h plPlannerStage0.h
objects/plPlannerStage1.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage1.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage1.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage1.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage1.o: plPickingTexture.h plVAO.h plVBO.h plEABO.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage1.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage1.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage1.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage1.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage1.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage1.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage1.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage1.o: plIGuide.h
objects/plPlannerStage2.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage2.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage2.o: plBufferObject.h plPlannerStage1.h
objects/plPlannerStage2.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage2.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage2.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage2.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage2.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plPlannerStage2.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage2.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage2.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage2.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage2.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage2.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage2.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage2.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage2.o: plIGuide.h
objects/plPlannerStage3.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage3.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage3.o: plBufferObject.h plPlannerStage0.h
objects/plPlannerStage3.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage3.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage3.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage3.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage3.o: plPickingTexture.h plVAO.h plVBO.h plEABO.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage3.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage3.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage3.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage3.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage3.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage3.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage3.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage3.o: plIGuide.h plPlannerStage2.h plPlannerStage1.h
objects/plPlannerStructs.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStructs.o: plVector3.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningBufferData.o: plPlanningSite.h plTriangle.h plBoundary.h
objects/plPlanningBufferData.o: plMath.h plPolygon.h plMatrix44.h
objects/plPlanningBufferData.o: plRenderable.h plRenderComponent.h
objects/plPlanningBufferData.o: plTexture2D.h plPickingTexture.h plVAO.h
objects/plPlanningBufferData.o: plVBO.h plBufferObject.h plEABO.h
objects/plPlanningBufferData.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningBufferData.o: plProjection.h plCamera.h plFBO.h
objects/plPlanningBufferData.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plPlanningBufferData.o: plShader.h plUniform.h plEditable.h
objects/plPlanningBufferData.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlanningBufferData.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlanningBufferData.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlanningBufferData.o: plRenderer.h plMeshCutter.h plSSBO.h
objects/plPlanningBufferData.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlanningBufferData.o: plColourMap.h plDonorSite.h
objects/plPlanningSite.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningSite.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlanningSite.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlanningSite.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plPlanningSite.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plPlanningSite.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plPlanningSite.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plPlanningSite.o: plUniform.h plEditable.h plBoneAndCartilage.h
objects/plPlanningSite.o: plModel.h plMesh.h plTransparentRenderable.h
objects/plPlanningSite.o: plOctreeMesh.h plOctree.h plTransform.h plDraw.h
objects/plPlanningSite.o: plSphere.h plRenderer.h plMeshCutter.h plSSBO.h
objects/plPlug.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plPlug.o: plBoneAndCartilage.h plModel.h plMesh.h plTriangle.h
objects/plPlug.o: plTransparentRenderable.h plRenderable.h
objects/plPlug.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plPlug.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plPlug.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlug.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plPlug.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plPlug.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plPlug.o: plTransform.h plDraw.h
objects/plPolygon.o: plCommon.h plVector3.h plString.h
objects/plProjection.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plProjection.o: plVector4.h plTriangle.h
objects/plRenderComponent.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plRenderComponent.o: plMatrix44.h plTriangle.h plTexture2D.h
objects/plRenderComponent.o: plPickingTexture.h plVAO.h plVBO.h
objects/plRenderComponent.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plRenderComponent.o: plMatrixStack.h plProjection.h plCamera.h
objects/plRenderComponent.o: plFBO.h plOpenGLInfo.h plVertexFragmentShader.h
objects/plRenderComponent.o: plShader.h plUniform.h
objects/plRenderable.o: plCommon.h plVector3.h plString.h plRenderComponent.h
objects/plRenderable.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/plRenderable.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plRenderable.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plRenderable.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plRenderable.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plRenderer.o: plCommon.h plVector3.h plString.h plRenderingPipeline.h
objects/plRenderer.o: plVector4.h plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderer.o: plProjection.h plCamera.h plPickingTexture.h plFBO.h
objects/plRenderer.o: plTexture2D.h plOpenGLInfo.h plVertexFragmentShader.h
objects/plRenderer.o: plShader.h plUniform.h plRenderable.h
objects/plRenderer.o: plRenderComponent.h plVAO.h plVBO.h plBufferObject.h
objects/plRenderer.o: plEABO.h
objects/plRenderingPipeline.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plRenderingPipeline.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderingPipeline.o: plProjection.h plCamera.h plPickingTexture.h
objects/plRenderingPipeline.o: plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plRenderingPipeline.o: plVertexFragmentShader.h plShader.h
objects/plRenderingPipeline.o: plUniform.h
objects/plSSBO.o: plCommon.h plBufferObject.h
objects/plScan.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plVAO.h plVBO.h plBufferObject.h
objects/plScan.o: plVector4.h plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plScan.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plScan.o: plPickingTexture.h plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plScan.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plScanPoint.o: plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plVector3.h plCommon.h plString.h plDraw.h plVAO.h
objects/plScanVolume.o: plVBO.h plBufferObject.h plVector4.h plEABO.h
objects/plScanVolume.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScanVolume.o: plTriangle.h plProjection.h plCamera.h
objects/plScanVolume.o: plPickingTexture.h plFBO.h plTexture2D.h
objects/plScanVolume.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plScanVolume.o: plUniform.h
objects/plSeq.o: plCommon.h
objects/plShader.o: plCommon.h plString.h plOpenGLInfo.h plUniform.h
objects/plSphere.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plSphere.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plSphere.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plSphere.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plSphere.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plSphere.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plSphere.o: plUniform.h
objects/plSpline.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plSpline.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plSpline.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plSpline.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plSpline.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plSpline.o: plUniform.h plEditable.h plBoneAndCartilage.h plModel.h
objects/plSpline.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/plSpline.o: plOctree.h plTransform.h plDraw.h plSphere.h plRenderer.h
objects/plSpline.o: plColourMap.h
objects/plString.o: plCommon.h
objects/plTexture2D.o: plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plCommon.h plVector3.h plString.h
objects/plTrackedObject.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plTrackedObject.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plTrackedObject.o: plTriangle.h plTexture2D.h plPickingTexture.h
objects/plTrackedObject.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plTrackedObject.o: plRenderingPipeline.h plMatrixStack.h
objects/plTrackedObject.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plTrackedObject.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plTrackedObject.o: plDRBTransform.h plDraw.h
objects/plTransform.o: plCommon.h plString.h plVector3.h plMatrix44.h
objects/plTransform.o: plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plCommon.h plRenderable.h plVector3.h
objects/plTransparentRenderable.o: plString.h plRenderComponent.h plVector4.h
objects/plTransparentRenderable.o: plMatrix44.h plTriangle.h plTexture2D.h
objects/plTransparentRenderable.o: plPickingTexture.h plVAO.h plVBO.h
objects/plTransparentRenderable.o: plBufferObject.h plEABO.h
objects/plTransparentRenderable.o: plRenderingPipeline.h plMatrixStack.h
objects/plTransparentRenderable.o: plProjection.h plCamera.h plFBO.h
objects/plTransparentRenderable.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plTransparentRenderable.o: plShader.h plUniform.h
objects/plTriangle.o: plCommon.h plString.h plVector3.h
objects/plUniform.o: plCommon.h
objects/plUtility.o: plCommon.h
objects/plVAO.o: plCommon.h plVBO.h plBufferObject.h plVector4.h plString.h
objects/plVAO.o: plVector3.h plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plVAO.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plVAO.o: plPickingTexture.h plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plVAO.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plVBO.o: plCommon.h plBufferObject.h plVector4.h plString.h
objects/plVBO.o: plVector3.h
objects/plVector3.o: plCommon.h plString.h
objects/plVector4.o: plCommon.h plString.h plVector3.h
objects/plVertexFragmentShader.o: plCommon.h plShader.h plString.h
objects/plVertexFragmentShader.o: plOpenGLInfo.h plUniform.h plMatrix44.h
objects/plVertexFragmentShader.o: plVector3.h plVector4.h plTriangle.h
objects/plVertexFragmentShader.o: plTexture2D.h
objects/plWindow.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h
objects/plWindow.o: plPickingTexture.h plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plWindow.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plWindow.o: plRenderer.h plRenderable.h plRenderComponent.h plVAO.h
objects/plWindow.o: plVBO.h plBufferObject.h plEABO.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h
objects/PlannerWindow.o: plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/PlannerWindow.o: plCSV.h plRenderable.h plRenderComponent.h
objects/PlannerWindow.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/PlannerWindow.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/PlannerWindow.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/PlannerWindow.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/PlannerWindow.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/PlannerWindow.o: plGraft.h plEditable.h plDraw.h plPlug.h
objects/PlannerWindow.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/PlannerWindow.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/PlannerWindow.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/PlannerWindow.o: plUtility.h plRenderer.h plSphere.h plDefectSite.h
objects/PlannerWindow.o: plSpline.h plBoundary.h plColourMap.h plDonorSite.h
objects/PlannerWindow.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/PlannerWindow.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h
objects/PlannerWindow.o: plSSBO.h plPlannerStage0.h plPlannerStructs.h
objects/PlannerWindow.o: plPlannerShader.h plPlannerStage1.h
objects/PlannerWindow.o: plPlannerStage2.h plPlannerStage3.h
objects/PlannerWindow.o: plBoundaryEditor.h plWindow.h plDisk.h
objects/PlannerWindow.o: plChessBoard.h plDRBTransform.h plGraftEditor.h
objects/PlannerWindow.o: plScan.h plScanPoint.h plScanVolume.h plTexture3D.h
objects/PlannerWindow.o: plTrackedObject.h
objects/Window.o: Window.h pl.h plCommon.h plAutomaticPlanner.h plPlan.h
objects/Window.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/Window.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/Window.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/Window.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/Window.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/Window.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/Window.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/Window.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/Window.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/Window.o: plMath.h plPolygon.h plTransform.h plUtility.h plRenderer.h
objects/Window.o: plSphere.h plDefectSite.h plSpline.h plBoundary.h
objects/Window.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plPlannerStage0.h
objects/Window.o: plPlannerStructs.h plPlannerShader.h plPlannerStage1.h
objects/Window.o: plPlannerStage2.h plPlannerStage3.h plBoundaryEditor.h
objects/Window.o: plWindow.h plDisk.h plChessBoard.h plDRBTransform.h
objects/Window.o: plGraftEditor.h plScan.h plScanPoint.h plScanVolume.h
objects/Window.o: plTexture3D.h plTrackedObject.h
objects/main.o: PlannerWindow.h Window.h pl.h plCommon.h plAutomaticPlanner.h
objects/main.o: plPlan.h plString.h plVector3.h plCSV.h plRenderable.h
objects/main.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/main.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/main.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/main.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/main.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/main.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/main.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/main.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h plMath.h
objects/main.o: plPolygon.h plTransform.h plUtility.h plRenderer.h plSphere.h
objects/main.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/main.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/main.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/main.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/main.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/main.o: plBoundaryEditor.h plWindow.h plDisk.h plChessBoard.h
objects/main.o: plDRBTransform.h plGraftEditor.h plScan.h plScanPoint.h
objects/main.o: plScanVolume.h plTexture3D.h plTrackedObject.h
objects/pl.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/pl.o: plVector3.h plCSV.h plRenderable.h plRenderComponent.h
objects/pl.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/pl.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/pl.o: plRenderingPipeline.h plMatrixStack.h plProjection.h plCamera.h
objects/pl.o: plFBO.h plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/pl.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/pl.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/pl.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h plMath.h
objects/pl.o: plPolygon.h plTransform.h plUtility.h plRenderer.h plSphere.h
objects/pl.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/pl.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/pl.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/pl.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/pl.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/pl.o: plBoundaryEditor.h plWindow.h plDisk.h plChessBoard.h
objects/pl.o: plDRBTransform.h plGraftEditor.h plScan.h plScanPoint.h
objects/pl.o: plScanVolume.h plTexture3D.h plTrackedObject.h
objects/plArthroscope.o: plArthroscope.h plCommon.h
objects/plAutomaticPlanner.o: plAutomaticPlanner.h plCommon.h plPlan.h
objects/plAutomaticPlanner.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/plAutomaticPlanner.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plAutomaticPlanner.o: plTriangle.h plTexture2D.h plPickingTexture.h
objects/plAutomaticPlanner.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plMatrixStack.h
objects/plAutomaticPlanner.o: plProjection.h plCamera.h plFBO.h
objects/plAutomaticPlanner.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plAutomaticPlanner.o: plShader.h plUniform.h plGraft.h plEditable.h
objects/plAutomaticPlanner.o: plDraw.h plPlug.h plModelSpecific.h
objects/plAutomaticPlanner.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plOctreeMesh.h
objects/plAutomaticPlanner.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/plAutomaticPlanner.o: plUtility.h plRenderer.h plSphere.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plAutomaticPlanner.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plAutomaticPlanner.o: plPlanningBufferData.h plPlanningSite.h
objects/plAutomaticPlanner.o: plSSBO.h plPlannerStage0.h plPlannerStructs.h
objects/plAutomaticPlanner.o: plPlannerShader.h plPlannerStage1.h
objects/plAutomaticPlanner.o: plPlannerStage2.h plPlannerStage3.h
objects/plBoneAndCartilage.o: plBoneAndCartilage.h plCommon.h plModel.h
objects/plBoneAndCartilage.o: plMesh.h plVector3.h plString.h plTriangle.h
objects/plBoneAndCartilage.o: plTransparentRenderable.h plRenderable.h
objects/plBoneAndCartilage.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plBoneAndCartilage.o: plTexture2D.h plPickingTexture.h plVAO.h
objects/plBoneAndCartilage.o: plVBO.h plBufferObject.h plEABO.h
objects/plBoneAndCartilage.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoneAndCartilage.o: plProjection.h plCamera.h plFBO.h
objects/plBoneAndCartilage.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plBoneAndCartilage.o: plShader.h plUniform.h plOctreeMesh.h
objects/plBoneAndCartilage.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/plBoneAndCartilage.o: plDraw.h
objects/plBoundary.o: plBoundary.h plCommon.h plMath.h plVector3.h plString.h
objects/plBoundary.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plBoundary.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plBoundary.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plBoundary.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plBoundary.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plBoundary.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plBoundary.o: plEditable.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plBoundary.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plBoundary.o: plTransform.h plDraw.h plSphere.h plRenderer.h
objects/plBoundaryEditor.o: plBoundaryEditor.h plCommon.h plVector3.h
objects/plBoundaryEditor.o: plString.h plPlan.h plCSV.h plRenderable.h
objects/plBoundaryEditor.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plBoundaryEditor.o: plTriangle.h plTexture2D.h plPickingTexture.h
objects/plBoundaryEditor.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plBoundaryEditor.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoundaryEditor.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plBoundaryEditor.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plBoundaryEditor.o: plGraft.h plEditable.h plDraw.h plPlug.h
objects/plBoundaryEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plBoundaryEditor.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/plBoundaryEditor.o: plOctree.h plMath.h plPolygon.h plTransform.h
objects/plBoundaryEditor.o: plUtility.h plRenderer.h plSphere.h
objects/plBoundaryEditor.o: plDefectSite.h plSpline.h plBoundary.h
objects/plBoundaryEditor.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plBoundaryEditor.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plBoundaryEditor.o: plWindow.h plDisk.h
objects/plBufferObject.o: plBufferObject.h plCommon.h
objects/plCSV.o: plCSV.h plCommon.h plString.h
objects/plCamera.o: plCamera.h plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plChessBoard.h plCommon.h plTransparentRenderable.h
objects/plChessBoard.o: plRenderable.h plVector3.h plString.h
objects/plChessBoard.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plChessBoard.o: plTriangle.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plChessBoard.o: plVBO.h plBufferObject.h plEABO.h
objects/plChessBoard.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plChessBoard.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plChessBoard.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plChessBoard.o: plTransform.h plDRBTransform.h
objects/plColourMap.o: plColourMap.h plCommon.h plVector3.h plString.h
objects/plDRBTransform.o: plDRBTransform.h plCommon.h plMatrix44.h
objects/plDRBTransform.o: plVector3.h plString.h plVector4.h plTriangle.h
objects/plDebugSphere.o: plDebugSphere.h plCommon.h plVector3.h plString.h
objects/plDebugSphere.o: plVector4.h plDraw.h plVAO.h plVBO.h
objects/plDebugSphere.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plDebugSphere.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plDebugSphere.o: plProjection.h plCamera.h plPickingTexture.h plFBO.h
objects/plDebugSphere.o: plTexture2D.h plOpenGLInfo.h
objects/plDebugSphere.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plDebugTransform.o: plDebugTransform.h plCommon.h plVector3.h
objects/plDebugTransform.o: plString.h plVector4.h plDraw.h plVAO.h plVBO.h
objects/plDebugTransform.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plDebugTransform.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plDebugTransform.o: plProjection.h plCamera.h plPickingTexture.h
objects/plDebugTransform.o: plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plDebugTransform.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plDefectSite.o: plDefectSite.h plCommon.h plVector3.h plString.h
objects/plDefectSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDefectSite.o: plMesh.h plTriangle.h plTransparentRenderable.h
objects/plDefectSite.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plDefectSite.o: plMatrix44.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plDefectSite.o: plVBO.h plBufferObject.h plEABO.h
objects/plDefectSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDefectSite.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plDefectSite.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plDefectSite.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plDefectSite.o: plTransform.h plDraw.h plSpline.h plBoundary.h
objects/plDefectSite.o: plEditable.h plSphere.h plRenderer.h plColourMap.h
objects/plDisk.o: plDisk.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plDisk.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plDisk.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plDisk.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plDisk.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plDisk.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plDisk.o: plUniform.h
objects/plDonorSite.o: plDonorSite.h plCommon.h plVector3.h plString.h
objects/plDonorSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDonorSite.o: plMesh.h plTriangle.h plTransparentRenderable.h
objects/plDonorSite.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plDonorSite.o: plMatrix44.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plDonorSite.o: plVBO.h plBufferObject.h plEABO.h
objects/plDonorSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDonorSite.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plDonorSite.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plDonorSite.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plDonorSite.o: plTransform.h plDraw.h plBoundary.h plEditable.h
objects/plDonorSite.o: plSphere.h plRenderer.h
objects/plDraw.o: plDraw.h plCommon.h plVAO.h plVBO.h plBufferObject.h
objects/plDraw.o: plVector4.h plString.h plVector3.h plEABO.h
objects/plDraw.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDraw.o: plTriangle.h plProjection.h plCamera.h plPickingTexture.h
objects/plDraw.o: plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plDraw.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plEABO.o: plEABO.h plCommon.h plBufferObject.h
objects/plEditable.o: plEditable.h plCommon.h
objects/plFBO.o: plFBO.h plCommon.h plVector3.h plString.h plTexture2D.h
objects/plFBO.o: plOpenGLInfo.h
objects/plGraft.o: plGraft.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plGraft.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plGraft.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plGraft.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plGraft.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plGraft.o: plUniform.h plEditable.h plDraw.h plPlug.h
objects/plGraft.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plGraft.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plGraft.o: plMath.h plPolygon.h plTransform.h plUtility.h
objects/plGraft.o: plRenderer.h plSphere.h
objects/plGraftEditor.o: plGraftEditor.h plCommon.h plVector3.h plString.h
objects/plGraftEditor.o: plTransform.h plMatrix44.h plVector4.h plTriangle.h
objects/plGraftEditor.o: plPlan.h plCSV.h plRenderable.h plRenderComponent.h
objects/plGraftEditor.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plGraftEditor.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plGraftEditor.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plGraftEditor.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plGraftEditor.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/plGraftEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraftEditor.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/plGraftEditor.o: plOctree.h plMath.h plPolygon.h plUtility.h
objects/plGraftEditor.o: plRenderer.h plSphere.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plGraftEditor.o: plIGuide.h plWindow.h
objects/plIGuide.o: plIGuide.h plCommon.h plModelSpecific.h plVector3.h
objects/plIGuide.o: plString.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plIGuide.o: plTriangle.h plTransparentRenderable.h plRenderable.h
objects/plIGuide.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plIGuide.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plIGuide.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plIGuide.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plIGuide.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plIGuide.o: plUniform.h plOctreeMesh.h plOctree.h plMath.h
objects/plIGuide.o: plPolygon.h plTransform.h plDraw.h plBoundary.h
objects/plIGuide.o: plEditable.h plSphere.h plRenderer.h plGraft.h plPlug.h
objects/plIGuide.o: plUtility.h plIGuideSite.h plMeshCutter.h
objects/plIGuide.o: plMeshExtruder.h plSpline.h plColourMap.h
objects/plIGuideSite.o: plIGuideSite.h plCommon.h plVector3.h plString.h
objects/plIGuideSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plIGuideSite.o: plMesh.h plTriangle.h plTransparentRenderable.h
objects/plIGuideSite.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plIGuideSite.o: plMatrix44.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plIGuideSite.o: plVBO.h plBufferObject.h plEABO.h
objects/plIGuideSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plIGuideSite.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plIGuideSite.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plIGuideSite.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plIGuideSite.o: plTransform.h plDraw.h plBoundary.h plEditable.h
objects/plIGuideSite.o: plSphere.h plRenderer.h plMeshCutter.h
objects/plIGuideSite.o: plMeshExtruder.h
objects/plLaserLine.o: plLaserLine.h plCommon.h plVector3.h plString.h
objects/plLaserLine.o: plVector4.h
objects/plMath.o: plMath.h plCommon.h plVector3.h plString.h plPolygon.h
objects/plMath.o: plTriangle.h plMatrix44.h plVector4.h
objects/plMatrix44.o: plMatrix44.h plCommon.h plVector3.h plString.h
objects/plMatrix44.o: plVector4.h plTriangle.h
objects/plMatrixStack.o: plMatrixStack.h plCommon.h plMatrix44.h plVector3.h
objects/plMatrixStack.o: plString.h plVector4.h plTriangle.h
objects/plMesh.o: plMesh.h plCommon.h plVector3.h plString.h plTriangle.h
objects/plMeshAlgorithm.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshAlgorithm.o: plString.h plPolygon.h plTriangle.h
objects/plMeshAlgorithm.o: plMeshConnectivityData.h plUtility.h
objects/plMeshConnectivityData.o: plMeshConnectivityData.h plTriangle.h
objects/plMeshConnectivityData.o: plCommon.h plString.h plVector3.h
objects/plMeshConnectivityData.o: plPolygon.h plUtility.h
objects/plMeshCutter.o: plMeshCutter.h plCommon.h plVector3.h plString.h
objects/plMeshCutter.o: plTriangle.h plPolygon.h plBoundary.h plMath.h
objects/plMeshCutter.o: plMatrix44.h plVector4.h plRenderable.h
objects/plMeshCutter.o: plRenderComponent.h plTexture2D.h plPickingTexture.h
objects/plMeshCutter.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plMeshCutter.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plMeshCutter.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plMeshCutter.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plMeshCutter.o: plEditable.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plMeshCutter.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plMeshCutter.o: plTransform.h plDraw.h plSphere.h plRenderer.h
objects/plMeshExtruder.o: plMeshExtruder.h plTriangle.h plCommon.h plString.h
objects/plMeshExtruder.o: plVector3.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshIntersector.h plMeshAlgorithm.h plCommon.h
objects/plMeshIntersector.o: plVector3.h plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h plMath.h
objects/plMeshIntersector.o: plMatrix44.h plVector4.h
objects/plModel.o: plModel.h plCommon.h plMesh.h plVector3.h plString.h
objects/plModel.o: plTriangle.h plTransparentRenderable.h plRenderable.h
objects/plModel.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plModel.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plModel.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plModel.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plModel.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plModel.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plModel.o: plTransform.h plDraw.h
objects/plModelSpecific.o: plModelSpecific.h plCommon.h plVector3.h
objects/plModelSpecific.o: plString.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plModelSpecific.o: plTriangle.h plTransparentRenderable.h
objects/plModelSpecific.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plModelSpecific.o: plMatrix44.h plTexture2D.h plPickingTexture.h
objects/plModelSpecific.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plModelSpecific.o: plRenderingPipeline.h plMatrixStack.h
objects/plModelSpecific.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plModelSpecific.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plModelSpecific.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plModelSpecific.o: plTransform.h plDraw.h
objects/plOctree.o: plOctree.h plCommon.h plVector3.h plString.h plMath.h
objects/plOctree.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plOctree.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plOctree.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plOctree.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plOctree.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plOctree.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plOctree.o: plTransform.h plDraw.h
objects/plOctreeMesh.o: plOctreeMesh.h plCommon.h plMesh.h plVector3.h
objects/plOctreeMesh.o: plString.h plTriangle.h plOctree.h plMath.h
objects/plOctreeMesh.o: plPolygon.h plMatrix44.h plVector4.h plRenderable.h
objects/plOctreeMesh.o: plRenderComponent.h plTexture2D.h plPickingTexture.h
objects/plOctreeMesh.o: plVAO.h plVBO.h plBufferObject.h plEABO.h
objects/plOctreeMesh.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctreeMesh.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plOctreeMesh.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plOctreeMesh.o: plTransform.h plDraw.h
objects/plOpenGLInfo.o: plOpenGLInfo.h plCommon.h
objects/plPickingTexture.o: plPickingTexture.h plCommon.h
objects/plPlan.o: plPlan.h plCommon.h plString.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plPlan.o: plTriangle.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plPlan.o: plVBO.h plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plPlan.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plPlan.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plPlan.o: plUniform.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/plPlan.o: plModelSpecific.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlan.o: plTransparentRenderable.h plOctreeMesh.h plOctree.h
objects/plPlan.o: plMath.h plPolygon.h plTransform.h plUtility.h plRenderer.h
objects/plPlan.o: plSphere.h plDefectSite.h plSpline.h plBoundary.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlannerShader.o: plPlannerShader.h plCommon.h plShader.h plString.h
objects/plPlannerShader.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerShader.o: plVector4.h plVector3.h plTriangle.h plBoundary.h
objects/plPlannerShader.o: plMath.h plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerShader.o: plRenderComponent.h plTexture2D.h
objects/plPlannerShader.o: plPickingTexture.h plVAO.h plVBO.h
objects/plPlannerShader.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plPlannerShader.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plPlannerShader.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerShader.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerShader.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerShader.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerShader.o: plRenderer.h plMeshCutter.h plSSBO.h
objects/plPlannerShader.o: plPlannerStructs.h plUtility.h
objects/plPlannerStage0.o: plPlannerStage0.h plCommon.h plUtility.h
objects/plPlannerStage0.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage0.o: plPlannerStructs.h plSSBO.h plBufferObject.h
objects/plPlannerStage0.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage0.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage0.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage0.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage0.o: plPickingTexture.h plVAO.h plVBO.h plEABO.h
objects/plPlannerStage0.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage0.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage0.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage0.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage0.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage0.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage0.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage0.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage0.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage0.o: plIGuide.h
objects/plPlannerStage1.o: plPlannerStage1.h plCommon.h plVector4.h
objects/plPlannerStage1.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage1.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage1.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage1.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage1.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage1.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage1.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plPlannerStage1.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage1.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage1.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage1.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage1.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage1.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage1.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage1.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage1.o: plIGuide.h
objects/plPlannerStage2.o: plPlannerStage2.h plCommon.h plVector4.h
objects/plPlannerStage2.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage2.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage2.o: plPlannerStage1.h plPlannerStage0.h
objects/plPlannerStage2.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage2.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage2.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage2.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage2.o: plPickingTexture.h plVAO.h plVBO.h plEABO.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage2.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage2.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage2.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage2.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage2.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage2.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage2.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage2.o: plIGuide.h
objects/plPlannerStage3.o: plPlannerStage3.h plCommon.h plVector4.h
objects/plPlannerStage3.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage3.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage3.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage3.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage3.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage3.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage3.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plPlannerStage3.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plFBO.h
objects/plPlannerStage3.o: plVertexFragmentShader.h plEditable.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlannerStage3.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlannerStage3.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlannerStage3.o: plRenderer.h plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage3.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage3.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage3.o: plGraft.h plPlug.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage3.o: plIGuide.h plPlannerStage2.h plPlannerStage1.h
objects/plPlannerStructs.o: plPlannerStructs.h plCommon.h plUtility.h
objects/plPlannerStructs.o: plVector4.h plString.h plVector3.h plSSBO.h
objects/plPlannerStructs.o: plBufferObject.h
objects/plPlanningBufferData.o: plPlanningBufferData.h plCommon.h plVector4.h
objects/plPlanningBufferData.o: plString.h plVector3.h plPlanningSite.h
objects/plPlanningBufferData.o: plTriangle.h plBoundary.h plMath.h
objects/plPlanningBufferData.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlanningBufferData.o: plRenderComponent.h plTexture2D.h
objects/plPlanningBufferData.o: plPickingTexture.h plVAO.h plVBO.h
objects/plPlanningBufferData.o: plBufferObject.h plEABO.h
objects/plPlanningBufferData.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningBufferData.o: plProjection.h plCamera.h plFBO.h
objects/plPlanningBufferData.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plPlanningBufferData.o: plShader.h plUniform.h plEditable.h
objects/plPlanningBufferData.o: plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlanningBufferData.o: plTransparentRenderable.h plOctreeMesh.h
objects/plPlanningBufferData.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlanningBufferData.o: plRenderer.h plMeshCutter.h plSSBO.h
objects/plPlanningBufferData.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlanningBufferData.o: plColourMap.h plDonorSite.h
objects/plPlanningSite.o: plPlanningSite.h plCommon.h plVector4.h plString.h
objects/plPlanningSite.o: plVector3.h plTriangle.h plBoundary.h plMath.h
objects/plPlanningSite.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlanningSite.o: plRenderComponent.h plTexture2D.h
objects/plPlanningSite.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plPlanningSite.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlanningSite.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plPlanningSite.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plPlanningSite.o: plEditable.h plBoneAndCartilage.h plModel.h
objects/plPlanningSite.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/plPlanningSite.o: plOctree.h plTransform.h plDraw.h plSphere.h
objects/plPlanningSite.o: plRenderer.h plMeshCutter.h plSSBO.h
objects/plPlug.o: plPlug.h plCommon.h plModelSpecific.h plVector3.h
objects/plPlug.o: plString.h plBoneAndCartilage.h plModel.h plMesh.h
objects/plPlug.o: plTriangle.h plTransparentRenderable.h plRenderable.h
objects/plPlug.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plPlug.o: plPickingTexture.h plVAO.h plVBO.h plBufferObject.h
objects/plPlug.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlug.o: plProjection.h plCamera.h plFBO.h plOpenGLInfo.h
objects/plPlug.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plPlug.o: plOctreeMesh.h plOctree.h plMath.h plPolygon.h
objects/plPlug.o: plTransform.h plDraw.h
objects/plPolygon.o: plPolygon.h plCommon.h plVector3.h plString.h
objects/plProjection.o: plProjection.h plCommon.h plMatrix44.h plVector3.h
objects/plProjection.o: plString.h plVector4.h plTriangle.h
objects/plRenderComponent.o: plRenderComponent.h plCommon.h plVector3.h
objects/plRenderComponent.o: plString.h plVector4.h plMatrix44.h plTriangle.h
objects/plRenderComponent.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plRenderComponent.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plRenderComponent.o: plMatrixStack.h plProjection.h plCamera.h
objects/plRenderComponent.o: plFBO.h plOpenGLInfo.h plVertexFragmentShader.h
objects/plRenderComponent.o: plShader.h plUniform.h
objects/plRenderable.o: plRenderable.h plCommon.h plVector3.h plString.h
objects/plRenderable.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plRenderable.o: plTriangle.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plRenderable.o: plVBO.h plBufferObject.h plEABO.h
objects/plRenderable.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plRenderable.o: plCamera.h plFBO.h plOpenGLInfo.h
objects/plRenderable.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plRenderer.o: plRenderer.h plCommon.h plVector3.h plString.h
objects/plRenderer.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plRenderer.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plRenderer.o: plPickingTexture.h plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plRenderer.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plRenderer.o: plRenderable.h plRenderComponent.h plVAO.h plVBO.h
objects/plRenderer.o: plBufferObject.h plEABO.h
objects/plRenderingPipeline.o: plRenderingPipeline.h plCommon.h plVector4.h
objects/plRenderingPipeline.o: plString.h plVector3.h plMatrixStack.h
objects/plRenderingPipeline.o: plMatrix44.h plTriangle.h plProjection.h
objects/plRenderingPipeline.o: plCamera.h plPickingTexture.h plFBO.h
objects/plRenderingPipeline.o: plTexture2D.h plOpenGLInfo.h
objects/plRenderingPipeline.o: plVertexFragmentShader.h plShader.h
objects/plRenderingPipeline.o: plUniform.h
objects/plSSBO.o: plSSBO.h plCommon.h plBufferObject.h
objects/plScan.o: plScan.h plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plVAO.h plVBO.h plBufferObject.h
objects/plScan.o: plVector4.h plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plScan.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plScan.o: plPickingTexture.h plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plScan.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plScanPoint.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plScanVolume.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plDraw.h plVAO.h plVBO.h plBufferObject.h plVector4.h
objects/plScanVolume.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plScanVolume.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plScanVolume.o: plPickingTexture.h plFBO.h plTexture2D.h
objects/plScanVolume.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plScanVolume.o: plUniform.h
objects/plShader.o: plShader.h plCommon.h plString.h plOpenGLInfo.h
objects/plShader.o: plUniform.h
objects/plSphere.o: plSphere.h plCommon.h plRenderable.h plVector3.h
objects/plSphere.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plSphere.o: plTriangle.h plTexture2D.h plPickingTexture.h plVAO.h
objects/plSphere.o: plVBO.h plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plSphere.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plSphere.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plSphere.o: plUniform.h
objects/plSpline.o: plSpline.h plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plSpline.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plSpline.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plSpline.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plSpline.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plSpline.o: plUniform.h plEditable.h plBoneAndCartilage.h plModel.h
objects/plSpline.o: plMesh.h plTransparentRenderable.h plOctreeMesh.h
objects/plSpline.o: plOctree.h plTransform.h plDraw.h plSphere.h plRenderer.h
objects/plSpline.o: plColourMap.h
objects/plString.o: plString.h plCommon.h
objects/plTexture2D.o: plTexture2D.h plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plTexture3D.h plCommon.h plVector3.h plString.h
objects/plThreadPool.o: plThreadPool.h
objects/plTrackedObject.o: plTrackedObject.h plCommon.h plRenderable.h
objects/plTrackedObject.o: plVector3.h plString.h plRenderComponent.h
objects/plTrackedObject.o: plVector4.h plMatrix44.h plTriangle.h
objects/plTrackedObject.o: plTexture2D.h plPickingTexture.h plVAO.h plVBO.h
objects/plTrackedObject.o: plBufferObject.h plEABO.h plRenderingPipeline.h
objects/plTrackedObject.o: plMatrixStack.h plProjection.h plCamera.h plFBO.h
objects/plTrackedObject.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plTrackedObject.o: plUniform.h plDRBTransform.h plDraw.h
objects/plTransform.o: plTransform.h plCommon.h plString.h plVector3.h
objects/plTransform.o: plMatrix44.h plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plTransparentRenderable.h plCommon.h
objects/plTransparentRenderable.o: plRenderable.h plVector3.h plString.h
objects/plTransparentRenderable.o: plRenderComponent.h plVector4.h
objects/plTransparentRenderable.o: plMatrix44.h plTriangle.h plTexture2D.h
objects/plTransparentRenderable.o: plPickingTexture.h plVAO.h plVBO.h
objects/plTransparentRenderable.o: plBufferObject.h plEABO.h
objects/plTransparentRenderable.o: plRenderingPipeline.h plMatrixStack.h
objects/plTransparentRenderable.o: plProjection.h plCamera.h plFBO.h
objects/plTransparentRenderable.o: plOpenGLInfo.h plVertexFragmentShader.h
objects/plTransparentRenderable.o: plShader.h plUniform.h
objects/plTriangle.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plUniform.o: plUniform.h plCommon.h
objects/plUtility.o: plUtility.h plCommon.h
objects/plVAO.o: plVAO.h plCommon.h plVBO.h plBufferObject.h plVector4.h
objects/plVAO.o: plString.h plVector3.h plEABO.h plRenderingPipeline.h
objects/plVAO.o: plMatrixStack.h plMatrix44.h plTriangle.h plProjection.h
objects/plVAO.o: plCamera.h plPickingTexture.h plFBO.h plTexture2D.h
objects/plVAO.o: plOpenGLInfo.h plVertexFragmentShader.h plShader.h
objects/plVAO.o: plUniform.h
objects/plVBO.o: plVBO.h plCommon.h plBufferObject.h plVector4.h plString.h
objects/plVBO.o: plVector3.h
objects/plVector3.o: plVector3.h plCommon.h plString.h plVector4.h
objects/plVector4.o: plVector4.h plCommon.h plString.h plVector3.h
objects/plVertexFragmentShader.o: plVertexFragmentShader.h plCommon.h
objects/plVertexFragmentShader.o: plShader.h plString.h plOpenGLInfo.h
objects/plVertexFragmentShader.o: plUniform.h plMatrix44.h plVector3.h
objects/plVertexFragmentShader.o: plVector4.h plTriangle.h plTexture2D.h
objects/plWindow.o: plWindow.h plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h
objects/plWindow.o: plPickingTexture.h plFBO.h plTexture2D.h plOpenGLInfo.h
objects/plWindow.o: plVertexFragmentShader.h plShader.h plUniform.h
objects/plWindow.o: plRenderer.h plRenderable.h plRenderComponent.h plVAO.h
objects/plWindow.o: plVBO.h plBufferObject.h plEABO.h
