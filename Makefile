
OPENGL_DRIVER_DIR = /usr/lib/nvidia-331

LIBS = -L$(OPENGL_DRIVER_DIR) -lglut -lGL 

INCS = 

CXXFLAGS = -Wfatal-errors -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++ 

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES =   main.cpp \
            PlannerWindow.cpp \
            pl.cpp \
            plArthroCamTechnique.cpp \
            plArthroViewable.cpp \
            plAutomaticPlanner.cpp \
            plBoneAndCartilage.cpp \
            plBoundary.cpp \
            plBoundaryEditor.cpp \
            plBufferObject.cpp \
            plCamera.cpp \
            plChessBoard.cpp \
            plColourMap.cpp \
            plCone.cpp \
            plCSV.cpp \
            plCylinder.cpp \
            plDefectSite.cpp \
            plDonorSite.cpp \
            plDraw.cpp \
            plDRBTransform.cpp \
            plEABO.cpp \
            plEditable.cpp \
            plFBO.cpp \
            plGraft.cpp \
            plGraftCap.cpp \
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
            plOutlineTechnique.cpp \
            plPicking.cpp \
            plPlan.cpp \
            plPlannerShader.cpp \
            plPlannerStage0.cpp \
            plPlannerStage1.cpp \
            plPlannerStage2.cpp \
            plPlannerStage3.cpp \
            plPlannerStructs.cpp \
            plPlanningBufferData.cpp \
            plPlanningSite.cpp \
            plPlanTechnique.cpp \
            plPlug.cpp \
            plPolygon.cpp \
            plProjection.cpp \
            plRenderComponent.cpp \
            plRenderable.cpp \
            plRenderer.cpp \
            plRenderingPipeline.cpp \
            plRenderResources.cpp \
            plRenderTechnique.cpp \
            plScan.cpp \
            plScanPoint.cpp \
            plScanVolume.cpp \
            plScreenQuadTechnique.cpp \
            plShader.cpp \
            plSphere.cpp \
            plSpline.cpp \
            plSSBO.cpp \
            plString.cpp \
            plTexture2D.cpp \
            plTexture3D.cpp \
            plTrackedObject.cpp \
            plTransform.cpp \
            plTransparencyTechnique.cpp \
            plTransparentRenderable.cpp \
            plTriangle.cpp \
            plUtility.cpp \
            plUniform.cpp \
            plVAO.cpp \
            plVBO.cpp \
            plVector3.cpp \
            plVector4.cpp \
            plVertexFragmentShader.cpp \
            plVertexSpecBuffer.cpp \
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
objects/PlannerWindow.o: plMatrix44.h plTriangle.h plTexture2D.h plVAO.h
objects/PlannerWindow.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/PlannerWindow.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/PlannerWindow.o: plCamera.h plPicking.h plRenderResources.h
objects/PlannerWindow.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/PlannerWindow.o: plFBO.h plGraft.h plEditable.h plDraw.h plPlug.h
objects/PlannerWindow.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/PlannerWindow.o: plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/PlannerWindow.o: plPolygon.h plTransform.h plRenderer.h
objects/PlannerWindow.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/PlannerWindow.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/PlannerWindow.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/PlannerWindow.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/PlannerWindow.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/PlannerWindow.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/PlannerWindow.o: plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/PlannerWindow.o: plBufferObject.h plPlannerStage0.h
objects/PlannerWindow.o: plPlannerStructs.h plPlannerShader.h
objects/PlannerWindow.o: plPlannerStage1.h plPlannerStage2.h
objects/PlannerWindow.o: plPlannerStage3.h plBoundaryEditor.h plDisk.h
objects/PlannerWindow.o: plChessBoard.h plDRBTransform.h plGraftEditor.h
objects/PlannerWindow.o: plScan.h plScanPoint.h plScanVolume.h plTexture3D.h
objects/PlannerWindow.o: plTrackedObject.h
objects/Window.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/Window.o: plVector3.h plCSV.h plRenderable.h plRenderComponent.h
objects/Window.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h plVAO.h
objects/Window.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/Window.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/Window.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/Window.o: plUniform.h plVertexFragmentShader.h plFBO.h plGraft.h
objects/Window.o: plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/Window.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/Window.o: plOctree.h plMath.h plPolygon.h plTransform.h plRenderer.h
objects/Window.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/Window.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/Window.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/Window.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/Window.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/Window.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/Window.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/Window.o: plBoundaryEditor.h plDisk.h plChessBoard.h plDRBTransform.h
objects/Window.o: plGraftEditor.h plScan.h plScanPoint.h plScanVolume.h
objects/Window.o: plTexture3D.h plTrackedObject.h
objects/pl.o: plCommon.h plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/pl.o: plCSV.h plRenderable.h plRenderComponent.h plVector4.h
objects/pl.o: plMatrix44.h plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/pl.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/pl.o: plRenderingPipeline.h plMatrixStack.h plProjection.h plCamera.h
objects/pl.o: plPicking.h plRenderResources.h plShader.h plUniform.h
objects/pl.o: plVertexFragmentShader.h plFBO.h plGraft.h plEditable.h
objects/pl.o: plDraw.h plPlug.h plModelSpecific.h plBoneAndCartilage.h
objects/pl.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/pl.o: plPolygon.h plTransform.h plRenderer.h plPlanTechnique.h
objects/pl.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/pl.o: plOutlineTechnique.h plTransparentRenderable.h plUtility.h
objects/pl.o: plSphere.h plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/pl.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/pl.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/pl.o: plPlanningSite.h plSSBO.h plBufferObject.h plPlannerStage0.h
objects/pl.o: plPlannerStructs.h plPlannerShader.h plPlannerStage1.h
objects/pl.o: plPlannerStage2.h plPlannerStage3.h plBoundaryEditor.h plDisk.h
objects/pl.o: plChessBoard.h plDRBTransform.h plGraftEditor.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plTexture3D.h plTrackedObject.h
objects/plArthroCamTechnique.o: plCommon.h plScreenQuadTechnique.h
objects/plArthroCamTechnique.o: plRenderTechnique.h plRenderable.h
objects/plArthroCamTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plArthroCamTechnique.o: plVector4.h plMatrix44.h plTriangle.h
objects/plArthroCamTechnique.o: plTexture2D.h plVAO.h plVBO.h
objects/plArthroCamTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plArthroCamTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plArthroCamTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plArthroCamTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plArthroCamTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plArthroscope.o: plCommon.h
objects/plAutomaticPlanner.o: plCommon.h plPlan.h plString.h plVector3.h
objects/plAutomaticPlanner.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plAutomaticPlanner.o: plVector4.h plMatrix44.h plTriangle.h
objects/plAutomaticPlanner.o: plTexture2D.h plVAO.h plVBO.h
objects/plAutomaticPlanner.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plMatrixStack.h
objects/plAutomaticPlanner.o: plProjection.h plCamera.h plPicking.h
objects/plAutomaticPlanner.o: plRenderResources.h plShader.h plUniform.h
objects/plAutomaticPlanner.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plAutomaticPlanner.o: plEditable.h plDraw.h plPlug.h
objects/plAutomaticPlanner.o: plModelSpecific.h plBoneAndCartilage.h
objects/plAutomaticPlanner.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plAutomaticPlanner.o: plMath.h plPolygon.h plTransform.h plRenderer.h
objects/plAutomaticPlanner.o: plPlanTechnique.h plRenderTechnique.h
objects/plAutomaticPlanner.o: plWindow.h plScreenQuadTechnique.h
objects/plAutomaticPlanner.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plAutomaticPlanner.o: plUtility.h plSphere.h plGraftCap.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plAutomaticPlanner.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plAutomaticPlanner.o: plPlanningBufferData.h plPlanningSite.h
objects/plAutomaticPlanner.o: plSSBO.h plBufferObject.h plPlannerStage0.h
objects/plAutomaticPlanner.o: plPlannerStructs.h plPlannerShader.h
objects/plAutomaticPlanner.o: plPlannerStage1.h plPlannerStage2.h
objects/plAutomaticPlanner.o: plPlannerStage3.h
objects/plBoneAndCartilage.o: plCommon.h plModel.h plOctreeMesh.h plMesh.h
objects/plBoneAndCartilage.o: plVector3.h plString.h plTriangle.h plOctree.h
objects/plBoneAndCartilage.o: plMath.h plPolygon.h plMatrix44.h plVector4.h
objects/plBoneAndCartilage.o: plRenderable.h plRenderComponent.h
objects/plBoneAndCartilage.o: plTexture2D.h plVAO.h plVBO.h
objects/plBoneAndCartilage.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoneAndCartilage.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoneAndCartilage.o: plProjection.h plCamera.h plPicking.h
objects/plBoneAndCartilage.o: plRenderResources.h plShader.h plUniform.h
objects/plBoneAndCartilage.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plBoneAndCartilage.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plBoneAndCartilage.o: plRenderTechnique.h plWindow.h
objects/plBoneAndCartilage.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoneAndCartilage.o: plTransparentRenderable.h
objects/plBoundary.o: plCommon.h plMath.h plVector3.h plString.h plPolygon.h
objects/plBoundary.o: plTriangle.h plMatrix44.h plVector4.h plRenderable.h
objects/plBoundary.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plBoundary.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoundary.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plBoundary.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plBoundary.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plBoundary.o: plEditable.h plBoneAndCartilage.h plModel.h
objects/plBoundary.o: plOctreeMesh.h plMesh.h plOctree.h plTransform.h
objects/plBoundary.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plBoundary.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plBoundary.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plBoundary.o: plSphere.h
objects/plBoundaryEditor.o: plCommon.h plVector3.h plString.h plPlan.h
objects/plBoundaryEditor.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plBoundaryEditor.o: plVector4.h plMatrix44.h plTriangle.h
objects/plBoundaryEditor.o: plTexture2D.h plVAO.h plVBO.h
objects/plBoundaryEditor.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoundaryEditor.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoundaryEditor.o: plProjection.h plCamera.h plPicking.h
objects/plBoundaryEditor.o: plRenderResources.h plShader.h plUniform.h
objects/plBoundaryEditor.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plBoundaryEditor.o: plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/plBoundaryEditor.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plBoundaryEditor.o: plMesh.h plOctree.h plMath.h plPolygon.h
objects/plBoundaryEditor.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plBoundaryEditor.o: plRenderTechnique.h plWindow.h
objects/plBoundaryEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoundaryEditor.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/plBoundaryEditor.o: plGraftCap.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plDisk.h
objects/plBufferObject.o: plCommon.h
objects/plCSV.o: plCommon.h plString.h
objects/plCamera.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plCommon.h plTransparentRenderable.h plRenderable.h
objects/plChessBoard.o: plVector3.h plString.h plRenderComponent.h
objects/plChessBoard.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/plChessBoard.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plChessBoard.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plChessBoard.o: plProjection.h plCamera.h plPicking.h
objects/plChessBoard.o: plRenderResources.h plShader.h plUniform.h
objects/plChessBoard.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plChessBoard.o: plDRBTransform.h
objects/plColourMap.o: plCommon.h plVector3.h plString.h
objects/plDRBTransform.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plDRBTransform.o: plVector4.h plTriangle.h
objects/plDebugSphere.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugSphere.o: plDraw.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plDebugSphere.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plDebugSphere.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plDebugSphere.o: plProjection.h plCamera.h plPicking.h
objects/plDebugSphere.o: plRenderResources.h plShader.h plUniform.h
objects/plDebugSphere.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plDebugTransform.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugTransform.o: plDraw.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plDebugTransform.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plDebugTransform.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plDebugTransform.o: plProjection.h plCamera.h plPicking.h
objects/plDebugTransform.o: plRenderResources.h plShader.h plUniform.h
objects/plDebugTransform.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plDefectSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDefectSite.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plDefectSite.o: plMesh.h plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plDefectSite.o: plMatrix44.h plVector4.h plRenderable.h
objects/plDefectSite.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plDefectSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDefectSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDefectSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plDefectSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDefectSite.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plDefectSite.o: plRenderTechnique.h plWindow.h
objects/plDefectSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plDefectSite.o: plTransparentRenderable.h plSpline.h plBoundary.h
objects/plDefectSite.o: plEditable.h plSphere.h plColourMap.h
objects/plDisk.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plDisk.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plDisk.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDisk.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDisk.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plDisk.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plDonorSite.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plDonorSite.o: plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plDonorSite.o: plMatrix44.h plVector4.h plRenderable.h
objects/plDonorSite.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plDonorSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDonorSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDonorSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plDonorSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plDonorSite.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plDonorSite.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plDonorSite.o: plBoundary.h plEditable.h plSphere.h
objects/plDraw.o: plCommon.h plVAO.h plVBO.h plVertexSpecBuffer.h plVector4.h
objects/plDraw.o: plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plDraw.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDraw.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plDraw.o: plRenderResources.h plShader.h plUniform.h
objects/plDraw.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plEABO.o: plCommon.h plVertexSpecBuffer.h plOpenGLInfo.h
objects/plEditable.o: plCommon.h plRenderComponent.h plVector3.h plString.h
objects/plEditable.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/plEditable.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plEditable.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plEditable.o: plProjection.h plCamera.h plPicking.h
objects/plEditable.o: plRenderResources.h plShader.h plUniform.h
objects/plEditable.o: plVertexFragmentShader.h plFBO.h
objects/plFBO.o: plCommon.h plVector3.h plString.h plTexture2D.h
objects/plFBO.o: plOpenGLInfo.h
objects/plGraft.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plGraft.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraft.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraft.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plGraft.o: plRenderResources.h plShader.h plUniform.h
objects/plGraft.o: plVertexFragmentShader.h plFBO.h plEditable.h plDraw.h
objects/plGraft.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraft.o: plOctreeMesh.h plMesh.h plOctree.h plMath.h plPolygon.h
objects/plGraft.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plGraft.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraft.o: plOutlineTechnique.h plTransparentRenderable.h plUtility.h
objects/plGraft.o: plSphere.h plGraftCap.h
objects/plGraftCap.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraftCap.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plGraftCap.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plGraftCap.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plGraftCap.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plGraftCap.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plGraftCap.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraftCap.o: plEditable.h plPolygon.h plUtility.h plTransform.h
objects/plGraftCap.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/plGraftCap.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plGraftCap.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraftCap.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plGraftEditor.o: plCommon.h plVector3.h plString.h plTransform.h
objects/plGraftEditor.o: plMatrix44.h plVector4.h plTriangle.h plPlan.h
objects/plGraftEditor.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plGraftEditor.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraftEditor.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraftEditor.o: plMatrixStack.h plProjection.h plCamera.h
objects/plGraftEditor.o: plPicking.h plRenderResources.h plShader.h
objects/plGraftEditor.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraftEditor.o: plGraft.h plEditable.h plDraw.h plPlug.h
objects/plGraftEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraftEditor.o: plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/plGraftEditor.o: plPolygon.h plRenderer.h plPlanTechnique.h
objects/plGraftEditor.o: plRenderTechnique.h plWindow.h
objects/plGraftEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plGraftEditor.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/plGraftEditor.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/plGraftEditor.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plGraftEditor.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plIGuide.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plIGuide.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plIGuide.o: plTriangle.h plOctree.h plMath.h plPolygon.h plMatrix44.h
objects/plIGuide.o: plVector4.h plRenderable.h plRenderComponent.h
objects/plIGuide.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plIGuide.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plIGuide.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plIGuide.o: plRenderResources.h plShader.h plUniform.h
objects/plIGuide.o: plVertexFragmentShader.h plFBO.h plTransform.h plDraw.h
objects/plIGuide.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plIGuide.o: plWindow.h plScreenQuadTechnique.h plOutlineTechnique.h
objects/plIGuide.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuide.o: plSphere.h plGraft.h plPlug.h plUtility.h plGraftCap.h
objects/plIGuide.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h plSpline.h
objects/plIGuide.o: plColourMap.h
objects/plIGuideSite.o: plCommon.h plVector3.h plString.h plModelSpecific.h
objects/plIGuideSite.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plIGuideSite.o: plMesh.h plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plIGuideSite.o: plMatrix44.h plVector4.h plRenderable.h
objects/plIGuideSite.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plIGuideSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plIGuideSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plIGuideSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plIGuideSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plIGuideSite.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plIGuideSite.o: plRenderTechnique.h plWindow.h
objects/plIGuideSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plIGuideSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuideSite.o: plSphere.h plMeshCutter.h plMeshExtruder.h
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
objects/plMeshCutter.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plMeshCutter.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plMeshCutter.o: plRenderResources.h plShader.h plUniform.h
objects/plMeshCutter.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plMeshCutter.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plMeshCutter.o: plMesh.h plOctree.h plTransform.h plDraw.h
objects/plMeshCutter.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plMeshCutter.o: plWindow.h plScreenQuadTechnique.h
objects/plMeshCutter.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plMeshCutter.o: plSphere.h
objects/plMeshExtruder.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plMeshExtruder.o: plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshIntersector.o: plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h plMath.h
objects/plMeshIntersector.o: plMatrix44.h plVector4.h
objects/plModel.o: plCommon.h plOctreeMesh.h plMesh.h plVector3.h plString.h
objects/plModel.o: plTriangle.h plOctree.h plMath.h plPolygon.h plMatrix44.h
objects/plModel.o: plVector4.h plRenderable.h plRenderComponent.h
objects/plModel.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plModel.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plModel.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plModel.o: plRenderResources.h plShader.h plUniform.h
objects/plModel.o: plVertexFragmentShader.h plFBO.h plTransform.h plDraw.h
objects/plModel.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plModel.o: plWindow.h plScreenQuadTechnique.h plOutlineTechnique.h
objects/plModel.o: plTransparentRenderable.h
objects/plModelSpecific.o: plCommon.h plVector3.h plString.h
objects/plModelSpecific.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plModelSpecific.o: plMesh.h plTriangle.h plOctree.h plMath.h
objects/plModelSpecific.o: plPolygon.h plMatrix44.h plVector4.h
objects/plModelSpecific.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plModelSpecific.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plModelSpecific.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plModelSpecific.o: plMatrixStack.h plProjection.h plCamera.h
objects/plModelSpecific.o: plPicking.h plRenderResources.h plShader.h
objects/plModelSpecific.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plModelSpecific.o: plTransform.h plDraw.h plRenderer.h
objects/plModelSpecific.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plModelSpecific.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plModelSpecific.o: plTransparentRenderable.h
objects/plOctree.o: plCommon.h plVector3.h plString.h plMath.h plPolygon.h
objects/plOctree.o: plTriangle.h plMatrix44.h plVector4.h plRenderable.h
objects/plOctree.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plOctree.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctree.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctree.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctree.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctree.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctree.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plOctree.o: plOutlineTechnique.h
objects/plOctreeMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plOctreeMesh.o: plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plOctreeMesh.o: plMatrix44.h plVector4.h plRenderable.h
objects/plOctreeMesh.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plOctreeMesh.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctreeMesh.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctreeMesh.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctreeMesh.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctreeMesh.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctreeMesh.o: plRenderTechnique.h plWindow.h
objects/plOctreeMesh.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plOpenGLInfo.o: plCommon.h
objects/plOutlineTechnique.o: plCommon.h plRenderTechnique.h plRenderable.h
objects/plOutlineTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plOutlineTechnique.o: plVector4.h plMatrix44.h plTriangle.h
objects/plOutlineTechnique.o: plTexture2D.h plVAO.h plVBO.h
objects/plOutlineTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOutlineTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plOutlineTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plOutlineTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plOutlineTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plPicking.o: plCommon.h plRenderResources.h plShader.h plString.h
objects/plPicking.o: plOpenGLInfo.h plUniform.h plVertexFragmentShader.h
objects/plPicking.o: plMatrix44.h plVector3.h plVector4.h plTriangle.h
objects/plPicking.o: plTexture2D.h plFBO.h
objects/plPlan.o: plCommon.h plString.h plVector3.h plCSV.h plRenderable.h
objects/plPlan.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plPlan.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlan.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlan.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlan.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlan.o: plGraft.h plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plPlan.o: plOctree.h plMath.h plPolygon.h plTransform.h plRenderer.h
objects/plPlan.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlan.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlan.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/plPlan.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlanTechnique.o: plCommon.h plRenderTechnique.h plRenderable.h
objects/plPlanTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plPlanTechnique.o: plVector4.h plMatrix44.h plTriangle.h
objects/plPlanTechnique.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlanTechnique.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanTechnique.o: plWindow.h
objects/plPlannerShader.o: plCommon.h plShader.h plString.h plOpenGLInfo.h
objects/plPlannerShader.o: plUniform.h plPlanningSite.h plVector4.h
objects/plPlannerShader.o: plVector3.h plTriangle.h plBoundary.h plMath.h
objects/plPlannerShader.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerShader.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerShader.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerShader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerShader.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerShader.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerShader.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerShader.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerShader.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerShader.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerShader.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerShader.o: plTransparentRenderable.h plSphere.h
objects/plPlannerShader.o: plMeshCutter.h plSSBO.h plBufferObject.h
objects/plPlannerShader.o: plPlannerStructs.h plUtility.h
objects/plPlannerStage0.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStage0.o: plVector3.h plPlannerStructs.h plSSBO.h
objects/plPlannerStage0.o: plBufferObject.h plPlannerShader.h plShader.h
objects/plPlannerStage0.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage0.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage0.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage0.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlannerStage0.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage0.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage0.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage0.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage0.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage0.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage0.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage0.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage0.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage0.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage0.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage0.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage0.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage1.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage1.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage1.o: plBufferObject.h plPlannerStage0.h
objects/plPlannerStage1.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage1.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage1.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage1.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage1.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage1.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage1.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage1.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage1.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage1.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage1.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage1.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage1.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage1.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage1.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage1.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage1.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage2.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage2.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage2.o: plBufferObject.h plPlannerStage1.h
objects/plPlannerStage2.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage2.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage2.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage2.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage2.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlannerStage2.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage2.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage2.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage2.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage2.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage2.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage2.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage2.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage2.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage2.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage2.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage2.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage2.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage3.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage3.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage3.o: plBufferObject.h plPlannerStage0.h
objects/plPlannerStage3.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage3.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage3.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage3.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage3.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage3.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage3.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage3.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage3.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage3.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage3.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage3.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage3.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage3.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage3.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage3.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage3.o: plMeshExtruder.h plIGuide.h plPlannerStage2.h
objects/plPlannerStage3.o: plPlannerStage1.h
objects/plPlannerStructs.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStructs.o: plVector3.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningBufferData.o: plPlanningSite.h plTriangle.h plBoundary.h
objects/plPlanningBufferData.o: plMath.h plPolygon.h plMatrix44.h
objects/plPlanningBufferData.o: plRenderable.h plRenderComponent.h
objects/plPlanningBufferData.o: plTexture2D.h plVAO.h plVBO.h
objects/plPlanningBufferData.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plPlanningBufferData.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningBufferData.o: plProjection.h plCamera.h plPicking.h
objects/plPlanningBufferData.o: plRenderResources.h plShader.h plUniform.h
objects/plPlanningBufferData.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plPlanningBufferData.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plPlanningBufferData.o: plMesh.h plOctree.h plTransform.h plDraw.h
objects/plPlanningBufferData.o: plRenderer.h plPlanTechnique.h
objects/plPlanningBufferData.o: plRenderTechnique.h plWindow.h
objects/plPlanningBufferData.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlanningBufferData.o: plTransparentRenderable.h plSphere.h
objects/plPlanningBufferData.o: plMeshCutter.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlanningBufferData.o: plColourMap.h plDonorSite.h
objects/plPlanningSite.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningSite.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlanningSite.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlanningSite.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlanningSite.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanningSite.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanningSite.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanningSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanningSite.o: plEditable.h plBoneAndCartilage.h plModel.h
objects/plPlanningSite.o: plOctreeMesh.h plMesh.h plOctree.h plTransform.h
objects/plPlanningSite.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plPlanningSite.o: plRenderTechnique.h plWindow.h
objects/plPlanningSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlanningSite.o: plTransparentRenderable.h plSphere.h plMeshCutter.h
objects/plPlanningSite.o: plSSBO.h plBufferObject.h
objects/plPlug.o: plCommon.h plModelSpecific.h plVector3.h plString.h
objects/plPlug.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plPlug.o: plTriangle.h plOctree.h plMath.h plPolygon.h plMatrix44.h
objects/plPlug.o: plVector4.h plRenderable.h plRenderComponent.h
objects/plPlug.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlug.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlug.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlug.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlug.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plPlug.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plPlug.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plPolygon.o: plCommon.h plVector3.h plString.h
objects/plProjection.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plProjection.o: plVector4.h plTriangle.h
objects/plRenderComponent.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plRenderComponent.o: plMatrix44.h plTriangle.h plTexture2D.h plVAO.h
objects/plRenderComponent.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plRenderComponent.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plRenderComponent.o: plMatrixStack.h plProjection.h plCamera.h
objects/plRenderComponent.o: plPicking.h plRenderResources.h plShader.h
objects/plRenderComponent.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plRenderResources.o: plCommon.h plShader.h plString.h plOpenGLInfo.h
objects/plRenderResources.o: plUniform.h plVertexFragmentShader.h
objects/plRenderResources.o: plMatrix44.h plVector3.h plVector4.h
objects/plRenderResources.o: plTriangle.h plTexture2D.h plFBO.h
objects/plRenderTechnique.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plRenderTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plRenderTechnique.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plRenderTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plRenderTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plRenderTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plRenderable.o: plCommon.h plVector3.h plString.h plRenderComponent.h
objects/plRenderable.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/plRenderable.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plRenderable.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plRenderable.o: plProjection.h plCamera.h plPicking.h
objects/plRenderable.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderable.o: plVertexFragmentShader.h plFBO.h
objects/plRenderer.o: plCommon.h plVector3.h plString.h plRenderingPipeline.h
objects/plRenderer.o: plVector4.h plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderer.o: plProjection.h plCamera.h plPicking.h
objects/plRenderer.o: plRenderResources.h plShader.h plOpenGLInfo.h
objects/plRenderer.o: plUniform.h plVertexFragmentShader.h plTexture2D.h
objects/plRenderer.o: plFBO.h plRenderable.h plRenderComponent.h plVAO.h
objects/plRenderer.o: plVBO.h plVertexSpecBuffer.h plEABO.h plPlanTechnique.h
objects/plRenderer.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plRenderer.o: plOutlineTechnique.h
objects/plRenderingPipeline.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plRenderingPipeline.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plRenderingPipeline.o: plProjection.h plCamera.h plPicking.h
objects/plRenderingPipeline.o: plRenderResources.h plShader.h plOpenGLInfo.h
objects/plRenderingPipeline.o: plUniform.h plVertexFragmentShader.h
objects/plRenderingPipeline.o: plTexture2D.h plFBO.h
objects/plSSBO.o: plCommon.h plBufferObject.h
objects/plScan.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plVAO.h plVBO.h
objects/plScan.o: plVertexSpecBuffer.h plVector4.h plEABO.h plOpenGLInfo.h
objects/plScan.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScan.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plScan.o: plRenderResources.h plShader.h plUniform.h
objects/plScan.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plScanPoint.o: plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plVector3.h plCommon.h plString.h plDraw.h plVAO.h
objects/plScanVolume.o: plVBO.h plVertexSpecBuffer.h plVector4.h plEABO.h
objects/plScanVolume.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plScanVolume.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plScanVolume.o: plPicking.h plRenderResources.h plShader.h
objects/plScanVolume.o: plUniform.h plVertexFragmentShader.h plTexture2D.h
objects/plScanVolume.o: plFBO.h
objects/plScreenQuadTechnique.o: plCommon.h plRenderTechnique.h
objects/plScreenQuadTechnique.o: plRenderable.h plVector3.h plString.h
objects/plScreenQuadTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plScreenQuadTechnique.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plScreenQuadTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plScreenQuadTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plScreenQuadTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plScreenQuadTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plScreenQuadTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plSeq.o: plCommon.h
objects/plShader.o: plCommon.h plString.h plOpenGLInfo.h plUniform.h
objects/plSphere.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plSphere.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plSphere.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plSphere.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plSphere.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plSphere.o: plRenderResources.h plShader.h plUniform.h
objects/plSphere.o: plVertexFragmentShader.h plFBO.h
objects/plSpline.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plSpline.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plSpline.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plSpline.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plSpline.o: plRenderResources.h plShader.h plUniform.h
objects/plSpline.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plSpline.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plSpline.o: plOctree.h plTransform.h plDraw.h plRenderer.h
objects/plSpline.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plSpline.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plSpline.o: plTransparentRenderable.h plSphere.h plColourMap.h
objects/plString.o: plCommon.h
objects/plTexture2D.o: plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plCommon.h plVector3.h plString.h
objects/plTrackedObject.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plTrackedObject.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plTrackedObject.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plTrackedObject.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plTrackedObject.o: plRenderingPipeline.h plMatrixStack.h
objects/plTrackedObject.o: plProjection.h plCamera.h plPicking.h
objects/plTrackedObject.o: plRenderResources.h plShader.h plUniform.h
objects/plTrackedObject.o: plVertexFragmentShader.h plFBO.h plDRBTransform.h
objects/plTrackedObject.o: plDraw.h
objects/plTransform.o: plCommon.h plString.h plVector3.h plMatrix44.h
objects/plTransform.o: plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plCommon.h plRenderable.h plVector3.h
objects/plTransparentRenderable.o: plString.h plRenderComponent.h plVector4.h
objects/plTransparentRenderable.o: plMatrix44.h plTriangle.h plTexture2D.h
objects/plTransparentRenderable.o: plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plTransparentRenderable.o: plEABO.h plOpenGLInfo.h
objects/plTransparentRenderable.o: plRenderingPipeline.h plMatrixStack.h
objects/plTransparentRenderable.o: plProjection.h plCamera.h plPicking.h
objects/plTransparentRenderable.o: plRenderResources.h plShader.h plUniform.h
objects/plTransparentRenderable.o: plVertexFragmentShader.h plFBO.h
objects/plTriangle.o: plCommon.h plString.h plVector3.h
objects/plUniform.o: plCommon.h
objects/plUtility.o: plCommon.h
objects/plVAO.o: plCommon.h plVBO.h plVertexSpecBuffer.h plVector4.h
objects/plVAO.o: plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plVAO.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plVAO.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plVAO.o: plRenderResources.h plShader.h plUniform.h
objects/plVAO.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plVBO.o: plCommon.h plVertexSpecBuffer.h plVector4.h plString.h
objects/plVBO.o: plVector3.h
objects/plVector3.o: plCommon.h plString.h
objects/plVector4.o: plCommon.h plString.h plVector3.h
objects/plVertexFragmentShader.o: plCommon.h plShader.h plString.h
objects/plVertexFragmentShader.o: plOpenGLInfo.h plUniform.h plMatrix44.h
objects/plVertexFragmentShader.o: plVector3.h plVector4.h plTriangle.h
objects/plVertexFragmentShader.o: plTexture2D.h
objects/plVertexSpecBuffer.o: plCommon.h
objects/plWindow.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plWindow.o: plRenderResources.h plShader.h plOpenGLInfo.h plUniform.h
objects/plWindow.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h
objects/PlannerWindow.o: plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/PlannerWindow.o: plCSV.h plRenderable.h plRenderComponent.h
objects/PlannerWindow.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h
objects/PlannerWindow.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/PlannerWindow.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/PlannerWindow.o: plProjection.h plCamera.h plPicking.h
objects/PlannerWindow.o: plRenderResources.h plShader.h plUniform.h
objects/PlannerWindow.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/PlannerWindow.o: plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/PlannerWindow.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/PlannerWindow.o: plMesh.h plOctree.h plMath.h plPolygon.h
objects/PlannerWindow.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/PlannerWindow.o: plRenderTechnique.h plWindow.h
objects/PlannerWindow.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/PlannerWindow.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/PlannerWindow.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/PlannerWindow.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/PlannerWindow.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/PlannerWindow.o: plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/PlannerWindow.o: plBufferObject.h plPlannerStage0.h
objects/PlannerWindow.o: plPlannerStructs.h plPlannerShader.h
objects/PlannerWindow.o: plPlannerStage1.h plPlannerStage2.h
objects/PlannerWindow.o: plPlannerStage3.h plBoundaryEditor.h plDisk.h
objects/PlannerWindow.o: plChessBoard.h plDRBTransform.h plGraftEditor.h
objects/PlannerWindow.o: plScan.h plScanPoint.h plScanVolume.h plTexture3D.h
objects/PlannerWindow.o: plTrackedObject.h
objects/Window.o: Window.h pl.h plCommon.h plAutomaticPlanner.h plPlan.h
objects/Window.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/Window.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/Window.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/Window.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/Window.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/Window.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/Window.o: plGraft.h plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/Window.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/Window.o: plOctree.h plMath.h plPolygon.h plTransform.h plRenderer.h
objects/Window.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/Window.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/Window.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/Window.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/Window.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/Window.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/Window.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/Window.o: plBoundaryEditor.h plDisk.h plChessBoard.h plDRBTransform.h
objects/Window.o: plGraftEditor.h plScan.h plScanPoint.h plScanVolume.h
objects/Window.o: plTexture3D.h plTrackedObject.h
objects/main.o: PlannerWindow.h Window.h pl.h plCommon.h plAutomaticPlanner.h
objects/main.o: plPlan.h plString.h plVector3.h plCSV.h plRenderable.h
objects/main.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/main.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/main.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/main.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/main.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/main.o: plGraft.h plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/main.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/main.o: plOctree.h plMath.h plPolygon.h plTransform.h plRenderer.h
objects/main.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/main.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/main.o: plTransparentRenderable.h plUtility.h plSphere.h plGraftCap.h
objects/main.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/main.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/main.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/main.o: plBufferObject.h plPlannerStage0.h plPlannerStructs.h
objects/main.o: plPlannerShader.h plPlannerStage1.h plPlannerStage2.h
objects/main.o: plPlannerStage3.h plBoundaryEditor.h plDisk.h plChessBoard.h
objects/main.o: plDRBTransform.h plGraftEditor.h plScan.h plScanPoint.h
objects/main.o: plScanVolume.h plTexture3D.h plTrackedObject.h
objects/pl.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/pl.o: plVector3.h plCSV.h plRenderable.h plRenderComponent.h
objects/pl.o: plVector4.h plMatrix44.h plTriangle.h plTexture2D.h plVAO.h
objects/pl.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/pl.o: plRenderingPipeline.h plMatrixStack.h plProjection.h plCamera.h
objects/pl.o: plPicking.h plRenderResources.h plShader.h plUniform.h
objects/pl.o: plVertexFragmentShader.h plFBO.h plGraft.h plEditable.h
objects/pl.o: plDraw.h plPlug.h plModelSpecific.h plBoneAndCartilage.h
objects/pl.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/pl.o: plPolygon.h plTransform.h plRenderer.h plPlanTechnique.h
objects/pl.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/pl.o: plOutlineTechnique.h plTransparentRenderable.h plUtility.h
objects/pl.o: plSphere.h plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/pl.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/pl.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/pl.o: plPlanningSite.h plSSBO.h plBufferObject.h plPlannerStage0.h
objects/pl.o: plPlannerStructs.h plPlannerShader.h plPlannerStage1.h
objects/pl.o: plPlannerStage2.h plPlannerStage3.h plBoundaryEditor.h plDisk.h
objects/pl.o: plChessBoard.h plDRBTransform.h plGraftEditor.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plTexture3D.h plTrackedObject.h
objects/plArthoCamTechnique.o: plArthroCamTechnique.h plCommon.h
objects/plArthoCamTechnique.o: plScreenQuadTechnique.h plRenderTechnique.h
objects/plArthoCamTechnique.o: plRenderable.h plVector3.h plString.h
objects/plArthoCamTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plArthoCamTechnique.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plArthoCamTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plArthoCamTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plArthoCamTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plArthoCamTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plArthoCamTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plArthroscope.o: plArthroscope.h plCommon.h
objects/plAutomaticPlanner.o: plAutomaticPlanner.h plCommon.h plPlan.h
objects/plAutomaticPlanner.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/plAutomaticPlanner.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plAutomaticPlanner.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plAutomaticPlanner.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plMatrixStack.h
objects/plAutomaticPlanner.o: plProjection.h plCamera.h plPicking.h
objects/plAutomaticPlanner.o: plRenderResources.h plShader.h plUniform.h
objects/plAutomaticPlanner.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plAutomaticPlanner.o: plEditable.h plDraw.h plPlug.h
objects/plAutomaticPlanner.o: plModelSpecific.h plBoneAndCartilage.h
objects/plAutomaticPlanner.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plAutomaticPlanner.o: plMath.h plPolygon.h plTransform.h plRenderer.h
objects/plAutomaticPlanner.o: plPlanTechnique.h plRenderTechnique.h
objects/plAutomaticPlanner.o: plWindow.h plScreenQuadTechnique.h
objects/plAutomaticPlanner.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plAutomaticPlanner.o: plUtility.h plSphere.h plGraftCap.h
objects/plAutomaticPlanner.o: plDefectSite.h plSpline.h plBoundary.h
objects/plAutomaticPlanner.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plAutomaticPlanner.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plAutomaticPlanner.o: plPlanningBufferData.h plPlanningSite.h
objects/plAutomaticPlanner.o: plSSBO.h plBufferObject.h plPlannerStage0.h
objects/plAutomaticPlanner.o: plPlannerStructs.h plPlannerShader.h
objects/plAutomaticPlanner.o: plPlannerStage1.h plPlannerStage2.h
objects/plAutomaticPlanner.o: plPlannerStage3.h
objects/plBoneAndCartilage.o: plBoneAndCartilage.h plCommon.h plModel.h
objects/plBoneAndCartilage.o: plOctreeMesh.h plMesh.h plVector3.h plString.h
objects/plBoneAndCartilage.o: plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plBoneAndCartilage.o: plMatrix44.h plVector4.h plRenderable.h
objects/plBoneAndCartilage.o: plRenderComponent.h plTexture2D.h plVAO.h
objects/plBoneAndCartilage.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plBoneAndCartilage.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plBoneAndCartilage.o: plMatrixStack.h plProjection.h plCamera.h
objects/plBoneAndCartilage.o: plPicking.h plRenderResources.h plShader.h
objects/plBoneAndCartilage.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plBoneAndCartilage.o: plTransform.h plDraw.h plRenderer.h
objects/plBoneAndCartilage.o: plPlanTechnique.h plRenderTechnique.h
objects/plBoneAndCartilage.o: plWindow.h plScreenQuadTechnique.h
objects/plBoneAndCartilage.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plBoundary.o: plBoundary.h plCommon.h plMath.h plVector3.h plString.h
objects/plBoundary.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plBoundary.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plBoundary.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plBoundary.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plBoundary.o: plProjection.h plCamera.h plPicking.h
objects/plBoundary.o: plRenderResources.h plShader.h plUniform.h
objects/plBoundary.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plBoundary.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plBoundary.o: plOctree.h plTransform.h plDraw.h plRenderer.h
objects/plBoundary.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plBoundary.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoundary.o: plTransparentRenderable.h plSphere.h
objects/plBoundaryEditor.o: plBoundaryEditor.h plCommon.h plVector3.h
objects/plBoundaryEditor.o: plString.h plPlan.h plCSV.h plRenderable.h
objects/plBoundaryEditor.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plBoundaryEditor.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plBoundaryEditor.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoundaryEditor.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoundaryEditor.o: plProjection.h plCamera.h plPicking.h
objects/plBoundaryEditor.o: plRenderResources.h plShader.h plUniform.h
objects/plBoundaryEditor.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plBoundaryEditor.o: plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/plBoundaryEditor.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plBoundaryEditor.o: plMesh.h plOctree.h plMath.h plPolygon.h
objects/plBoundaryEditor.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plBoundaryEditor.o: plRenderTechnique.h plWindow.h
objects/plBoundaryEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoundaryEditor.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/plBoundaryEditor.o: plGraftCap.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plDisk.h
objects/plBufferObject.o: plBufferObject.h plCommon.h
objects/plCSV.o: plCSV.h plCommon.h plString.h
objects/plCamera.o: plCamera.h plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h plTriangle.h
objects/plChessBoard.o: plChessBoard.h plCommon.h plTransparentRenderable.h
objects/plChessBoard.o: plRenderable.h plVector3.h plString.h
objects/plChessBoard.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plChessBoard.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plChessBoard.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plChessBoard.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plChessBoard.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plChessBoard.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plChessBoard.o: plTransform.h plDRBTransform.h
objects/plColourMap.o: plColourMap.h plCommon.h plVector3.h plString.h
objects/plDRBTransform.o: plDRBTransform.h plCommon.h plMatrix44.h
objects/plDRBTransform.o: plVector3.h plString.h plVector4.h plTriangle.h
objects/plDebugSphere.o: plDebugSphere.h plCommon.h plVector3.h plString.h
objects/plDebugSphere.o: plVector4.h plDraw.h plVAO.h plVBO.h
objects/plDebugSphere.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDebugSphere.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDebugSphere.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plDebugSphere.o: plRenderResources.h plShader.h plUniform.h
objects/plDebugSphere.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plDebugTransform.o: plDebugTransform.h plCommon.h plVector3.h
objects/plDebugTransform.o: plString.h plVector4.h plDraw.h plVAO.h plVBO.h
objects/plDebugTransform.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDebugTransform.o: plRenderingPipeline.h plMatrixStack.h
objects/plDebugTransform.o: plMatrix44.h plTriangle.h plProjection.h
objects/plDebugTransform.o: plCamera.h plPicking.h plRenderResources.h
objects/plDebugTransform.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plDebugTransform.o: plTexture2D.h plFBO.h
objects/plDefectSite.o: plDefectSite.h plCommon.h plVector3.h plString.h
objects/plDefectSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDefectSite.o: plOctreeMesh.h plMesh.h plTriangle.h plOctree.h
objects/plDefectSite.o: plMath.h plPolygon.h plMatrix44.h plVector4.h
objects/plDefectSite.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plDefectSite.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDefectSite.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDefectSite.o: plProjection.h plCamera.h plPicking.h
objects/plDefectSite.o: plRenderResources.h plShader.h plUniform.h
objects/plDefectSite.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plDefectSite.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plDefectSite.o: plRenderTechnique.h plWindow.h
objects/plDefectSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plDefectSite.o: plTransparentRenderable.h plSpline.h plBoundary.h
objects/plDefectSite.o: plEditable.h plSphere.h plColourMap.h
objects/plDisk.o: plDisk.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plDisk.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plDisk.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDisk.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDisk.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plDisk.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plDonorSite.h plCommon.h plVector3.h plString.h
objects/plDonorSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plDonorSite.o: plOctreeMesh.h plMesh.h plTriangle.h plOctree.h
objects/plDonorSite.o: plMath.h plPolygon.h plMatrix44.h plVector4.h
objects/plDonorSite.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plDonorSite.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDonorSite.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDonorSite.o: plProjection.h plCamera.h plPicking.h
objects/plDonorSite.o: plRenderResources.h plShader.h plUniform.h
objects/plDonorSite.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plDonorSite.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plDonorSite.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plDonorSite.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plDonorSite.o: plBoundary.h plEditable.h plSphere.h
objects/plDraw.o: plDraw.h plCommon.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plDraw.o: plVector4.h plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plDraw.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDraw.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plDraw.o: plRenderResources.h plShader.h plUniform.h
objects/plDraw.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plEABO.o: plEABO.h plCommon.h plVertexSpecBuffer.h plOpenGLInfo.h
objects/plEditable.o: plEditable.h plCommon.h plRenderComponent.h plVector3.h
objects/plEditable.o: plString.h plVector4.h plMatrix44.h plTriangle.h
objects/plEditable.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plEditable.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plEditable.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plEditable.o: plRenderResources.h plShader.h plUniform.h
objects/plEditable.o: plVertexFragmentShader.h plFBO.h
objects/plFBO.o: plFBO.h plCommon.h plVector3.h plString.h plTexture2D.h
objects/plFBO.o: plOpenGLInfo.h
objects/plGraft.o: plGraft.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plRenderComponent.h plVector4.h plMatrix44.h plTriangle.h
objects/plGraft.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraft.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraft.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plGraft.o: plRenderResources.h plShader.h plUniform.h
objects/plGraft.o: plVertexFragmentShader.h plFBO.h plEditable.h plDraw.h
objects/plGraft.o: plPlug.h plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraft.o: plOctreeMesh.h plMesh.h plOctree.h plMath.h plPolygon.h
objects/plGraft.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plGraft.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraft.o: plOutlineTechnique.h plTransparentRenderable.h plUtility.h
objects/plGraft.o: plSphere.h plGraftCap.h
objects/plGraftCap.o: plGraftCap.h plCommon.h plRenderable.h plVector3.h
objects/plGraftCap.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plGraftCap.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plGraftCap.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plGraftCap.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plGraftCap.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plGraftCap.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraftCap.o: plEditable.h plPolygon.h plUtility.h plTransform.h
objects/plGraftCap.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/plGraftCap.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plGraftCap.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraftCap.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plGraftEditor.o: plGraftEditor.h plCommon.h plVector3.h plString.h
objects/plGraftEditor.o: plTransform.h plMatrix44.h plVector4.h plTriangle.h
objects/plGraftEditor.o: plPlan.h plCSV.h plRenderable.h plRenderComponent.h
objects/plGraftEditor.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraftEditor.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraftEditor.o: plMatrixStack.h plProjection.h plCamera.h
objects/plGraftEditor.o: plPicking.h plRenderResources.h plShader.h
objects/plGraftEditor.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraftEditor.o: plGraft.h plEditable.h plDraw.h plPlug.h
objects/plGraftEditor.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plGraftEditor.o: plOctreeMesh.h plMesh.h plOctree.h plMath.h
objects/plGraftEditor.o: plPolygon.h plRenderer.h plPlanTechnique.h
objects/plGraftEditor.o: plRenderTechnique.h plWindow.h
objects/plGraftEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plGraftEditor.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/plGraftEditor.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/plGraftEditor.o: plColourMap.h plDonorSite.h plIGuideSite.h
objects/plGraftEditor.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/plIGuide.o: plIGuide.h plCommon.h plModelSpecific.h plVector3.h
objects/plIGuide.o: plString.h plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plIGuide.o: plMesh.h plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plIGuide.o: plMatrix44.h plVector4.h plRenderable.h
objects/plIGuide.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plIGuide.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plIGuide.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plIGuide.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plIGuide.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plIGuide.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plIGuide.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plIGuide.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plIGuide.o: plBoundary.h plEditable.h plSphere.h plGraft.h plPlug.h
objects/plIGuide.o: plUtility.h plGraftCap.h plIGuideSite.h plMeshCutter.h
objects/plIGuide.o: plMeshExtruder.h plSpline.h plColourMap.h
objects/plIGuideSite.o: plIGuideSite.h plCommon.h plVector3.h plString.h
objects/plIGuideSite.o: plModelSpecific.h plBoneAndCartilage.h plModel.h
objects/plIGuideSite.o: plOctreeMesh.h plMesh.h plTriangle.h plOctree.h
objects/plIGuideSite.o: plMath.h plPolygon.h plMatrix44.h plVector4.h
objects/plIGuideSite.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plIGuideSite.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plIGuideSite.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plIGuideSite.o: plProjection.h plCamera.h plPicking.h
objects/plIGuideSite.o: plRenderResources.h plShader.h plUniform.h
objects/plIGuideSite.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plIGuideSite.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plIGuideSite.o: plRenderTechnique.h plWindow.h
objects/plIGuideSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plIGuideSite.o: plTransparentRenderable.h plBoundary.h plEditable.h
objects/plIGuideSite.o: plSphere.h plMeshCutter.h plMeshExtruder.h
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
objects/plMeshCutter.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plMeshCutter.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plMeshCutter.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plMeshCutter.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plMeshCutter.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plMeshCutter.o: plEditable.h plBoneAndCartilage.h plModel.h
objects/plMeshCutter.o: plOctreeMesh.h plMesh.h plOctree.h plTransform.h
objects/plMeshCutter.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plMeshCutter.o: plRenderTechnique.h plWindow.h
objects/plMeshCutter.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plMeshCutter.o: plTransparentRenderable.h plSphere.h
objects/plMeshExtruder.o: plMeshExtruder.h plTriangle.h plCommon.h plString.h
objects/plMeshExtruder.o: plVector3.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshIntersector.h plMeshAlgorithm.h plCommon.h
objects/plMeshIntersector.o: plVector3.h plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h plMath.h
objects/plMeshIntersector.o: plMatrix44.h plVector4.h
objects/plModel.o: plModel.h plCommon.h plOctreeMesh.h plMesh.h plVector3.h
objects/plModel.o: plString.h plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plModel.o: plMatrix44.h plVector4.h plRenderable.h
objects/plModel.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plModel.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plModel.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plModel.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plModel.o: plUniform.h plVertexFragmentShader.h plFBO.h plTransform.h
objects/plModel.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plModel.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plModel.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plModelSpecific.o: plModelSpecific.h plCommon.h plVector3.h
objects/plModelSpecific.o: plString.h plBoneAndCartilage.h plModel.h
objects/plModelSpecific.o: plOctreeMesh.h plMesh.h plTriangle.h plOctree.h
objects/plModelSpecific.o: plMath.h plPolygon.h plMatrix44.h plVector4.h
objects/plModelSpecific.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plModelSpecific.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plModelSpecific.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plModelSpecific.o: plMatrixStack.h plProjection.h plCamera.h
objects/plModelSpecific.o: plPicking.h plRenderResources.h plShader.h
objects/plModelSpecific.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plModelSpecific.o: plTransform.h plDraw.h plRenderer.h
objects/plModelSpecific.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plModelSpecific.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plModelSpecific.o: plTransparentRenderable.h
objects/plOctree.o: plOctree.h plCommon.h plVector3.h plString.h plMath.h
objects/plOctree.o: plPolygon.h plTriangle.h plMatrix44.h plVector4.h
objects/plOctree.o: plRenderable.h plRenderComponent.h plTexture2D.h plVAO.h
objects/plOctree.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctree.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctree.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctree.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctree.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctree.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plOctree.o: plOutlineTechnique.h
objects/plOctreeMesh.o: plOctreeMesh.h plCommon.h plMesh.h plVector3.h
objects/plOctreeMesh.o: plString.h plTriangle.h plOctree.h plMath.h
objects/plOctreeMesh.o: plPolygon.h plMatrix44.h plVector4.h plRenderable.h
objects/plOctreeMesh.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plOctreeMesh.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctreeMesh.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctreeMesh.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctreeMesh.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctreeMesh.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctreeMesh.o: plRenderTechnique.h plWindow.h
objects/plOctreeMesh.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plOpenGLInfo.o: plOpenGLInfo.h plCommon.h
objects/plOutlineTechnique.o: plOutlineTechnique.h plCommon.h
objects/plOutlineTechnique.o: plRenderTechnique.h plRenderable.h plVector3.h
objects/plOutlineTechnique.o: plString.h plRenderComponent.h plVector4.h
objects/plOutlineTechnique.o: plMatrix44.h plTriangle.h plTexture2D.h plVAO.h
objects/plOutlineTechnique.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plOutlineTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plOutlineTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plOutlineTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plOutlineTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOutlineTechnique.o: plWindow.h
objects/plPicking.o: plPicking.h plCommon.h plRenderResources.h plShader.h
objects/plPicking.o: plString.h plOpenGLInfo.h plUniform.h
objects/plPicking.o: plVertexFragmentShader.h plMatrix44.h plVector3.h
objects/plPicking.o: plVector4.h plTriangle.h plTexture2D.h plFBO.h
objects/plPlan.o: plPlan.h plCommon.h plString.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plPlan.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plPlan.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plPlan.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plPlan.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plPlan.o: plUniform.h plVertexFragmentShader.h plFBO.h plGraft.h
objects/plPlan.o: plEditable.h plDraw.h plPlug.h plModelSpecific.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plPlan.o: plOctree.h plMath.h plPolygon.h plTransform.h plRenderer.h
objects/plPlan.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlan.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlan.o: plTransparentRenderable.h plUtility.h plSphere.h
objects/plPlan.o: plGraftCap.h plDefectSite.h plSpline.h plBoundary.h
objects/plPlan.o: plColourMap.h plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlanTechnique.o: plPlanTechnique.h plCommon.h plRenderTechnique.h
objects/plPlanTechnique.o: plRenderable.h plVector3.h plString.h
objects/plPlanTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plPlanTechnique.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plPlanTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plPlanTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plPlanTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plPlanTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plPlannerShader.o: plPlannerShader.h plCommon.h plShader.h plString.h
objects/plPlannerShader.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerShader.o: plVector4.h plVector3.h plTriangle.h plBoundary.h
objects/plPlannerShader.o: plMath.h plPolygon.h plMatrix44.h plRenderable.h
objects/plPlannerShader.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerShader.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerShader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerShader.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerShader.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerShader.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerShader.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerShader.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerShader.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerShader.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerShader.o: plTransparentRenderable.h plSphere.h
objects/plPlannerShader.o: plMeshCutter.h plSSBO.h plBufferObject.h
objects/plPlannerShader.o: plPlannerStructs.h plUtility.h
objects/plPlannerStage0.o: plPlannerStage0.h plCommon.h plUtility.h
objects/plPlannerStage0.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage0.o: plPlannerStructs.h plSSBO.h plBufferObject.h
objects/plPlannerStage0.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage0.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage0.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage0.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage0.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage0.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage0.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage0.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage0.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage0.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage0.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage0.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage0.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage0.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage0.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage0.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage0.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage0.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage1.o: plPlannerStage1.h plCommon.h plVector4.h
objects/plPlannerStage1.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage1.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage1.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage1.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage1.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage1.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage1.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlannerStage1.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage1.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage1.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage1.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage1.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage1.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage1.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage1.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage1.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage1.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage1.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage1.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage1.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage2.o: plPlannerStage2.h plCommon.h plVector4.h
objects/plPlannerStage2.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage2.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage2.o: plPlannerStage1.h plPlannerStage0.h
objects/plPlannerStage2.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage2.o: plUniform.h plPlanningSite.h plTriangle.h
objects/plPlannerStage2.o: plBoundary.h plMath.h plPolygon.h plMatrix44.h
objects/plPlannerStage2.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plPlannerStage2.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage2.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage2.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage2.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage2.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage2.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage2.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage2.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage2.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage2.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage2.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage2.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage2.o: plMeshExtruder.h plIGuide.h
objects/plPlannerStage3.o: plPlannerStage3.h plCommon.h plVector4.h
objects/plPlannerStage3.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage3.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage3.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage3.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage3.o: plTriangle.h plBoundary.h plMath.h plPolygon.h
objects/plPlannerStage3.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plPlannerStage3.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlannerStage3.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage3.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage3.o: plFBO.h plEditable.h plBoneAndCartilage.h
objects/plPlannerStage3.o: plModel.h plOctreeMesh.h plMesh.h plOctree.h
objects/plPlannerStage3.o: plTransform.h plDraw.h plRenderer.h
objects/plPlannerStage3.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plPlannerStage3.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage3.o: plTransparentRenderable.h plSphere.h
objects/plPlannerStage3.o: plMeshCutter.h plPlanningBufferData.h
objects/plPlannerStage3.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlannerStage3.o: plColourMap.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage3.o: plGraft.h plPlug.h plGraftCap.h plIGuideSite.h
objects/plPlannerStage3.o: plMeshExtruder.h plIGuide.h plPlannerStage2.h
objects/plPlannerStage3.o: plPlannerStage1.h
objects/plPlannerStructs.o: plPlannerStructs.h plCommon.h plUtility.h
objects/plPlannerStructs.o: plVector4.h plString.h plVector3.h plSSBO.h
objects/plPlannerStructs.o: plBufferObject.h
objects/plPlanningBufferData.o: plPlanningBufferData.h plCommon.h plVector4.h
objects/plPlanningBufferData.o: plString.h plVector3.h plPlanningSite.h
objects/plPlanningBufferData.o: plTriangle.h plBoundary.h plMath.h
objects/plPlanningBufferData.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlanningBufferData.o: plRenderComponent.h plTexture2D.h plVAO.h
objects/plPlanningBufferData.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlanningBufferData.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanningBufferData.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanningBufferData.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanningBufferData.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanningBufferData.o: plEditable.h plBoneAndCartilage.h plModel.h
objects/plPlanningBufferData.o: plOctreeMesh.h plMesh.h plOctree.h
objects/plPlanningBufferData.o: plTransform.h plDraw.h plRenderer.h
objects/plPlanningBufferData.o: plPlanTechnique.h plRenderTechnique.h
objects/plPlanningBufferData.o: plWindow.h plScreenQuadTechnique.h
objects/plPlanningBufferData.o: plOutlineTechnique.h
objects/plPlanningBufferData.o: plTransparentRenderable.h plSphere.h
objects/plPlanningBufferData.o: plMeshCutter.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plDefectSite.h plModelSpecific.h plSpline.h
objects/plPlanningBufferData.o: plColourMap.h plDonorSite.h
objects/plPlanningSite.o: plPlanningSite.h plCommon.h plVector4.h plString.h
objects/plPlanningSite.o: plVector3.h plTriangle.h plBoundary.h plMath.h
objects/plPlanningSite.o: plPolygon.h plMatrix44.h plRenderable.h
objects/plPlanningSite.o: plRenderComponent.h plTexture2D.h plVAO.h plVBO.h
objects/plPlanningSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plPlanningSite.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningSite.o: plProjection.h plCamera.h plPicking.h
objects/plPlanningSite.o: plRenderResources.h plShader.h plUniform.h
objects/plPlanningSite.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plPlanningSite.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plPlanningSite.o: plMesh.h plOctree.h plTransform.h plDraw.h
objects/plPlanningSite.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plPlanningSite.o: plWindow.h plScreenQuadTechnique.h
objects/plPlanningSite.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plPlanningSite.o: plSphere.h plMeshCutter.h plSSBO.h plBufferObject.h
objects/plPlug.o: plPlug.h plCommon.h plModelSpecific.h plVector3.h
objects/plPlug.o: plString.h plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plPlug.o: plMesh.h plTriangle.h plOctree.h plMath.h plPolygon.h
objects/plPlug.o: plMatrix44.h plVector4.h plRenderable.h plRenderComponent.h
objects/plPlug.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlug.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlug.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlug.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlug.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plPlug.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plPlug.o: plOutlineTechnique.h plTransparentRenderable.h
objects/plPolygon.o: plPolygon.h plCommon.h plVector3.h plString.h
objects/plProjection.o: plProjection.h plCommon.h plMatrix44.h plVector3.h
objects/plProjection.o: plString.h plVector4.h plTriangle.h
objects/plRenderComponent.o: plRenderComponent.h plCommon.h plVector3.h
objects/plRenderComponent.o: plString.h plVector4.h plMatrix44.h plTriangle.h
objects/plRenderComponent.o: plTexture2D.h plVAO.h plVBO.h
objects/plRenderComponent.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderComponent.o: plRenderingPipeline.h plMatrixStack.h
objects/plRenderComponent.o: plProjection.h plCamera.h plPicking.h
objects/plRenderComponent.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderComponent.o: plVertexFragmentShader.h plFBO.h
objects/plRenderResources.o: plRenderResources.h plCommon.h plShader.h
objects/plRenderResources.o: plString.h plOpenGLInfo.h plUniform.h
objects/plRenderResources.o: plVertexFragmentShader.h plMatrix44.h
objects/plRenderResources.o: plVector3.h plVector4.h plTriangle.h
objects/plRenderResources.o: plTexture2D.h plFBO.h
objects/plRenderTechnique.o: plRenderTechnique.h plCommon.h plRenderable.h
objects/plRenderTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plRenderTechnique.o: plVector4.h plMatrix44.h plTriangle.h
objects/plRenderTechnique.o: plTexture2D.h plVAO.h plVBO.h
objects/plRenderTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plRenderTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plRenderTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plRenderable.o: plRenderable.h plCommon.h plVector3.h plString.h
objects/plRenderable.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plRenderable.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plRenderable.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderable.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plRenderable.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plRenderable.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plRenderer.o: plRenderer.h plCommon.h plVector3.h plString.h
objects/plRenderer.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plRenderer.o: plMatrix44.h plTriangle.h plProjection.h plCamera.h
objects/plRenderer.o: plPicking.h plRenderResources.h plShader.h
objects/plRenderer.o: plOpenGLInfo.h plUniform.h plVertexFragmentShader.h
objects/plRenderer.o: plTexture2D.h plFBO.h plRenderable.h
objects/plRenderer.o: plRenderComponent.h plVAO.h plVBO.h
objects/plRenderer.o: plVertexSpecBuffer.h plEABO.h plPlanTechnique.h
objects/plRenderer.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plRenderer.o: plOutlineTechnique.h
objects/plRenderingPipeline.o: plRenderingPipeline.h plCommon.h plVector4.h
objects/plRenderingPipeline.o: plString.h plVector3.h plMatrixStack.h
objects/plRenderingPipeline.o: plMatrix44.h plTriangle.h plProjection.h
objects/plRenderingPipeline.o: plCamera.h plPicking.h plRenderResources.h
objects/plRenderingPipeline.o: plShader.h plOpenGLInfo.h plUniform.h
objects/plRenderingPipeline.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plSSBO.o: plSSBO.h plCommon.h plBufferObject.h
objects/plScan.o: plScan.h plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plVAO.h plVBO.h
objects/plScan.o: plVertexSpecBuffer.h plVector4.h plEABO.h plOpenGLInfo.h
objects/plScan.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScan.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plScan.o: plRenderResources.h plShader.h plUniform.h
objects/plScan.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plScanPoint.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plScanVolume.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plDraw.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plScanVolume.o: plVector4.h plEABO.h plOpenGLInfo.h
objects/plScanVolume.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScanVolume.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plScanVolume.o: plRenderResources.h plShader.h plUniform.h
objects/plScanVolume.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plScreenQuadTechnique.o: plScreenQuadTechnique.h plCommon.h
objects/plScreenQuadTechnique.o: plRenderTechnique.h plRenderable.h
objects/plScreenQuadTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plScreenQuadTechnique.o: plVector4.h plMatrix44.h plTriangle.h
objects/plScreenQuadTechnique.o: plTexture2D.h plVAO.h plVBO.h
objects/plScreenQuadTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plScreenQuadTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plScreenQuadTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plScreenQuadTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plScreenQuadTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plShader.o: plShader.h plCommon.h plString.h plOpenGLInfo.h
objects/plShader.o: plUniform.h
objects/plSphere.o: plSphere.h plCommon.h plRenderable.h plVector3.h
objects/plSphere.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plSphere.o: plTriangle.h plTexture2D.h plVAO.h plVBO.h
objects/plSphere.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plSphere.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plSphere.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plSphere.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plSpline.o: plSpline.h plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plPolygon.h plTriangle.h
objects/plSpline.o: plMatrix44.h plRenderable.h plRenderComponent.h
objects/plSpline.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plSpline.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plSpline.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plSpline.o: plRenderResources.h plShader.h plUniform.h
objects/plSpline.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plSpline.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plMesh.h
objects/plSpline.o: plOctree.h plTransform.h plDraw.h plRenderer.h
objects/plSpline.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plSpline.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plSpline.o: plTransparentRenderable.h plSphere.h plColourMap.h
objects/plString.o: plString.h plCommon.h
objects/plTexture2D.o: plTexture2D.h plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plTexture3D.h plCommon.h plVector3.h plString.h
objects/plThreadPool.o: plThreadPool.h
objects/plTrackedObject.o: plTrackedObject.h plCommon.h plRenderable.h
objects/plTrackedObject.o: plVector3.h plString.h plRenderComponent.h
objects/plTrackedObject.o: plVector4.h plMatrix44.h plTriangle.h
objects/plTrackedObject.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plTrackedObject.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plTrackedObject.o: plMatrixStack.h plProjection.h plCamera.h
objects/plTrackedObject.o: plPicking.h plRenderResources.h plShader.h
objects/plTrackedObject.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plTrackedObject.o: plDRBTransform.h plDraw.h
objects/plTransform.o: plTransform.h plCommon.h plString.h plVector3.h
objects/plTransform.o: plMatrix44.h plVector4.h plTriangle.h
objects/plTransparentRenderable.o: plTransparentRenderable.h plCommon.h
objects/plTransparentRenderable.o: plRenderable.h plVector3.h plString.h
objects/plTransparentRenderable.o: plRenderComponent.h plVector4.h
objects/plTransparentRenderable.o: plMatrix44.h plTriangle.h plTexture2D.h
objects/plTransparentRenderable.o: plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plTransparentRenderable.o: plEABO.h plOpenGLInfo.h
objects/plTransparentRenderable.o: plRenderingPipeline.h plMatrixStack.h
objects/plTransparentRenderable.o: plProjection.h plCamera.h plPicking.h
objects/plTransparentRenderable.o: plRenderResources.h plShader.h plUniform.h
objects/plTransparentRenderable.o: plVertexFragmentShader.h plFBO.h
objects/plTriangle.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plUniform.o: plUniform.h plCommon.h
objects/plUtility.o: plUtility.h plCommon.h
objects/plVAO.o: plVAO.h plCommon.h plVBO.h plVertexSpecBuffer.h plVector4.h
objects/plVAO.o: plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plVAO.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plVAO.o: plTriangle.h plProjection.h plCamera.h plPicking.h
objects/plVAO.o: plRenderResources.h plShader.h plUniform.h
objects/plVAO.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plVBO.o: plVBO.h plCommon.h plVertexSpecBuffer.h plVector4.h
objects/plVBO.o: plString.h plVector3.h
objects/plVector3.o: plVector3.h plCommon.h plString.h plVector4.h
objects/plVector4.o: plVector4.h plCommon.h plString.h plVector3.h
objects/plVertexFragmentShader.o: plVertexFragmentShader.h plCommon.h
objects/plVertexFragmentShader.o: plShader.h plString.h plOpenGLInfo.h
objects/plVertexFragmentShader.o: plUniform.h plMatrix44.h plVector3.h
objects/plVertexFragmentShader.o: plVector4.h plTriangle.h plTexture2D.h
objects/plVertexSpecBuffer.o: plVertexSpecBuffer.h plCommon.h plVAO.h plVBO.h
objects/plVertexSpecBuffer.o: plVector4.h plString.h plVector3.h plEABO.h
objects/plVertexSpecBuffer.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plVertexSpecBuffer.o: plMatrixStack.h plMatrix44.h plTriangle.h
objects/plVertexSpecBuffer.o: plProjection.h plCamera.h plPicking.h
objects/plVertexSpecBuffer.o: plRenderResources.h plShader.h plUniform.h
objects/plVertexSpecBuffer.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plWindow.o: plWindow.h plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plTriangle.h plRenderingPipeline.h
objects/plWindow.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plWindow.o: plRenderResources.h plShader.h plOpenGLInfo.h plUniform.h
objects/plWindow.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
