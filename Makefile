
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
            plDisk.cpp \
            plDonorSite.cpp \
            plDraw.cpp \
            plDRBTransform.cpp \
            plEABO.cpp \
            plEditable.cpp \
            plEditor.cpp \
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
            plMeshSpecific.cpp \
            plMinimalTechnique.cpp \
            plModel.cpp \
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
objects/PlannerWindow.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/PlannerWindow.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/PlannerWindow.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/PlannerWindow.o: plCamera.h plPicking.h plRenderResources.h
objects/PlannerWindow.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/PlannerWindow.o: plFBO.h plGraft.h plArthroViewable.h plEditable.h
objects/PlannerWindow.o: plTriangle.h plMath.h plDraw.h plPlug.h
objects/PlannerWindow.o: plMeshSpecific.h plMesh.h plBoneAndCartilage.h
objects/PlannerWindow.o: plModel.h plOctreeMesh.h plOctree.h plTransform.h
objects/PlannerWindow.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/PlannerWindow.o: plWindow.h plScreenQuadTechnique.h
objects/PlannerWindow.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/PlannerWindow.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/PlannerWindow.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/PlannerWindow.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/PlannerWindow.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/PlannerWindow.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/PlannerWindow.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/PlannerWindow.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/PlannerWindow.o: plPlannerStage0.h plPlannerStructs.h
objects/PlannerWindow.o: plPlannerShader.h plPlannerStage1.h
objects/PlannerWindow.o: plPlannerStage2.h plPlannerStage3.h
objects/PlannerWindow.o: plBoundaryEditor.h plEditor.h plDisk.h
objects/PlannerWindow.o: plChessBoard.h plDRBTransform.h plGraftEditor.h
objects/PlannerWindow.o: plScan.h plScanPoint.h plScanVolume.h plTexture3D.h
objects/PlannerWindow.o: plTrackedObject.h
objects/Window.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/Window.o: plVector3.h plCSV.h plRenderable.h plRenderComponent.h
objects/Window.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/Window.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/Window.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/Window.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/Window.o: plUniform.h plVertexFragmentShader.h plFBO.h plGraft.h
objects/Window.o: plArthroViewable.h plEditable.h plTriangle.h plMath.h
objects/Window.o: plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/Window.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/Window.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/Window.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/Window.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/Window.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/Window.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/Window.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/Window.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/Window.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/Window.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/Window.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/Window.o: plBoundaryEditor.h plEditor.h plDisk.h plChessBoard.h
objects/Window.o: plDRBTransform.h plGraftEditor.h plScan.h plScanPoint.h
objects/Window.o: plScanVolume.h plTexture3D.h plTrackedObject.h
objects/pl.o: plCommon.h plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/pl.o: plCSV.h plRenderable.h plRenderComponent.h plVector4.h
objects/pl.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/pl.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/pl.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/pl.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/pl.o: plGraft.h plArthroViewable.h plEditable.h plTriangle.h plMath.h
objects/pl.o: plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/pl.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/pl.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/pl.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/pl.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/pl.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/pl.o: plTransparentRenderable.h plPolygon.h plUtility.h plSphere.h
objects/pl.o: plGraftCap.h plCone.h plCylinder.h plDefectSite.h plSpline.h
objects/pl.o: plBoundary.h plColourMap.h plDonorSite.h plIGuideSite.h
objects/pl.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/pl.o: plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/pl.o: plBufferObject.h plPlannerStage0.h plPlannerStructs.h
objects/pl.o: plPlannerShader.h plPlannerStage1.h plPlannerStage2.h
objects/pl.o: plPlannerStage3.h plBoundaryEditor.h plEditor.h plDisk.h
objects/pl.o: plChessBoard.h plDRBTransform.h plGraftEditor.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plTexture3D.h plTrackedObject.h
objects/plArthroCamTechnique.o: plCommon.h plRenderTechnique.h plRenderable.h
objects/plArthroCamTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plArthroCamTechnique.o: plVector4.h plMatrix44.h plTexture2D.h
objects/plArthroCamTechnique.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plArthroCamTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plArthroCamTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plArthroCamTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plArthroCamTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plArthroCamTechnique.o: plWindow.h
objects/plArthroViewable.o: plCommon.h plVector3.h plString.h
objects/plArthroViewable.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plArthroViewable.o: plTexture2D.h plVAO.h plVBO.h
objects/plArthroViewable.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plArthroViewable.o: plRenderingPipeline.h plMatrixStack.h
objects/plArthroViewable.o: plProjection.h plCamera.h plPicking.h
objects/plArthroViewable.o: plRenderResources.h plShader.h plUniform.h
objects/plArthroViewable.o: plVertexFragmentShader.h plFBO.h
objects/plArthroscope.o: plCommon.h plTrackedObject.h plRenderable.h
objects/plArthroscope.o: plVector3.h plString.h plRenderComponent.h
objects/plArthroscope.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plArthroscope.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plArthroscope.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plArthroscope.o: plCamera.h plPicking.h plRenderResources.h
objects/plArthroscope.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plArthroscope.o: plFBO.h plDRBTransform.h plCylinder.h plCone.h
objects/plArthroscope.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plArthroscope.o: plWindow.h plScreenQuadTechnique.h
objects/plArthroscope.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plArthroscope.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plAutomaticPlanner.o: plCommon.h plPlan.h plString.h plVector3.h
objects/plAutomaticPlanner.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plAutomaticPlanner.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plAutomaticPlanner.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plAutomaticPlanner.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plAutomaticPlanner.o: plMatrixStack.h plProjection.h plCamera.h
objects/plAutomaticPlanner.o: plPicking.h plRenderResources.h plShader.h
objects/plAutomaticPlanner.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plAutomaticPlanner.o: plGraft.h plArthroViewable.h plEditable.h
objects/plAutomaticPlanner.o: plTriangle.h plMath.h plDraw.h plPlug.h
objects/plAutomaticPlanner.o: plMeshSpecific.h plMesh.h plBoneAndCartilage.h
objects/plAutomaticPlanner.o: plModel.h plOctreeMesh.h plOctree.h
objects/plAutomaticPlanner.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plAutomaticPlanner.o: plRenderTechnique.h plWindow.h
objects/plAutomaticPlanner.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plAutomaticPlanner.o: plArthroCamTechnique.h
objects/plAutomaticPlanner.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plPolygon.h
objects/plAutomaticPlanner.o: plUtility.h plSphere.h plGraftCap.h plCone.h
objects/plAutomaticPlanner.o: plCylinder.h plDefectSite.h plSpline.h
objects/plAutomaticPlanner.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plAutomaticPlanner.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plAutomaticPlanner.o: plIGuide.h plPlanningBufferData.h
objects/plAutomaticPlanner.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/plAutomaticPlanner.o: plPlannerStage0.h plPlannerStructs.h
objects/plAutomaticPlanner.o: plPlannerShader.h plPlannerStage1.h
objects/plAutomaticPlanner.o: plPlannerStage2.h plPlannerStage3.h
objects/plBoneAndCartilage.o: plCommon.h plModel.h plOctreeMesh.h plMesh.h
objects/plBoneAndCartilage.o: plVector3.h plString.h plTriangle.h plMath.h
objects/plBoneAndCartilage.o: plOctree.h plRenderable.h plRenderComponent.h
objects/plBoneAndCartilage.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plBoneAndCartilage.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plBoneAndCartilage.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plBoneAndCartilage.o: plMatrixStack.h plProjection.h plCamera.h
objects/plBoneAndCartilage.o: plPicking.h plRenderResources.h plShader.h
objects/plBoneAndCartilage.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plBoneAndCartilage.o: plTransform.h plDraw.h plRenderer.h
objects/plBoneAndCartilage.o: plPlanTechnique.h plRenderTechnique.h
objects/plBoneAndCartilage.o: plWindow.h plScreenQuadTechnique.h
objects/plBoneAndCartilage.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plBoneAndCartilage.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plBoneAndCartilage.o: plTransparentRenderable.h plArthroViewable.h
objects/plBoundary.o: plCommon.h plMath.h plVector3.h plString.h
objects/plBoundary.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plBoundary.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plBoundary.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoundary.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plBoundary.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plBoundary.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plBoundary.o: plEditable.h plMeshSpecific.h plMesh.h plTriangle.h
objects/plBoundary.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plBoundary.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plBoundary.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plBoundary.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plBoundaryEditor.o: plCommon.h plVector3.h plString.h plPlan.h
objects/plBoundaryEditor.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plBoundaryEditor.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plBoundaryEditor.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plBoundaryEditor.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plBoundaryEditor.o: plMatrixStack.h plProjection.h plCamera.h
objects/plBoundaryEditor.o: plPicking.h plRenderResources.h plShader.h
objects/plBoundaryEditor.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plBoundaryEditor.o: plGraft.h plArthroViewable.h plEditable.h
objects/plBoundaryEditor.o: plTriangle.h plMath.h plDraw.h plPlug.h
objects/plBoundaryEditor.o: plMeshSpecific.h plMesh.h plBoneAndCartilage.h
objects/plBoundaryEditor.o: plModel.h plOctreeMesh.h plOctree.h plTransform.h
objects/plBoundaryEditor.o: plRenderer.h plPlanTechnique.h
objects/plBoundaryEditor.o: plRenderTechnique.h plWindow.h
objects/plBoundaryEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoundaryEditor.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plBoundaryEditor.o: plMinimalTechnique.h plTransparentRenderable.h
objects/plBoundaryEditor.o: plPolygon.h plUtility.h plSphere.h plGraftCap.h
objects/plBoundaryEditor.o: plCone.h plCylinder.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plEditor.h plDisk.h
objects/plBufferObject.o: plCommon.h
objects/plCSV.o: plCommon.h plString.h
objects/plCamera.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h
objects/plChessBoard.o: plCommon.h plTransparentRenderable.h plRenderable.h
objects/plChessBoard.o: plVector3.h plString.h plRenderComponent.h
objects/plChessBoard.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plChessBoard.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plChessBoard.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plChessBoard.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plChessBoard.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plChessBoard.o: plTransform.h plDRBTransform.h plRenderer.h
objects/plChessBoard.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plChessBoard.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plChessBoard.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plChessBoard.o: plMinimalTechnique.h
objects/plColourMap.o: plCommon.h plVector3.h plString.h
objects/plCone.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plCone.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plCone.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plCone.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plCone.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plCone.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plCylinder.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plCylinder.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plCylinder.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plCylinder.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plCylinder.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plCylinder.o: plRenderResources.h plShader.h plUniform.h
objects/plCylinder.o: plVertexFragmentShader.h plFBO.h
objects/plDRBTransform.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plDRBTransform.o: plVector4.h
objects/plDebugSphere.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugSphere.o: plDraw.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plDebugSphere.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plDebugSphere.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plDebugSphere.o: plCamera.h plPicking.h plRenderResources.h
objects/plDebugSphere.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plDebugSphere.o: plTexture2D.h plFBO.h
objects/plDebugTransform.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plDebugTransform.o: plDraw.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plDebugTransform.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plDebugTransform.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plDebugTransform.o: plCamera.h plPicking.h plRenderResources.h
objects/plDebugTransform.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plDebugTransform.o: plTexture2D.h plFBO.h
objects/plDefectSite.o: plCommon.h plVector3.h plString.h plMeshSpecific.h
objects/plDefectSite.o: plMesh.h plTriangle.h plMath.h plSpline.h plVector4.h
objects/plDefectSite.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plDefectSite.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plDefectSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDefectSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDefectSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plDefectSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDefectSite.o: plEditable.h plDraw.h plSphere.h plRenderer.h
objects/plDefectSite.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plDefectSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plDefectSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plDefectSite.o: plMinimalTechnique.h plColourMap.h plOctreeMesh.h
objects/plDefectSite.o: plOctree.h plTransform.h
objects/plDisk.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plDisk.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plDisk.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDisk.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDisk.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plDisk.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plCommon.h plVector3.h plString.h plMeshSpecific.h
objects/plDonorSite.o: plMesh.h plTriangle.h plMath.h plBoundary.h
objects/plDonorSite.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plDonorSite.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plDonorSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDonorSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDonorSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plDonorSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plEditable.h plDraw.h plSphere.h plRenderer.h
objects/plDonorSite.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plDonorSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plDonorSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plDonorSite.o: plMinimalTechnique.h
objects/plDraw.o: plCommon.h plVAO.h plVBO.h plVertexSpecBuffer.h plVector4.h
objects/plDraw.o: plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plDraw.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDraw.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plDraw.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plDraw.o: plTexture2D.h plFBO.h
objects/plEABO.o: plCommon.h plVertexSpecBuffer.h plOpenGLInfo.h
objects/plEditable.o: plCommon.h plRenderComponent.h plVector3.h plString.h
objects/plEditable.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plEditable.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plEditable.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plEditable.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plEditable.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plEditor.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plEditor.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plEditor.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plEditor.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plEditor.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plEditor.o: plRenderResources.h plShader.h plUniform.h
objects/plEditor.o: plVertexFragmentShader.h plFBO.h plPlan.h plCSV.h
objects/plEditor.o: plGraft.h plArthroViewable.h plEditable.h plTriangle.h
objects/plEditor.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plEditor.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/plEditor.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plEditor.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plEditor.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plEditor.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plEditor.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plEditor.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/plEditor.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/plEditor.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plEditor.o: plMeshExtruder.h plIGuide.h
objects/plFBO.o: plCommon.h plVector3.h plString.h plTexture2D.h
objects/plFBO.o: plOpenGLInfo.h
objects/plGraft.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plGraft.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plGraft.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plGraft.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plGraft.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraft.o: plArthroViewable.h plEditable.h plTriangle.h plMath.h
objects/plGraft.o: plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plGraft.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/plGraft.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plGraft.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraft.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plGraft.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plGraft.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plGraft.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/plGraftCap.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraftCap.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plGraftCap.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraftCap.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraftCap.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plGraftCap.o: plRenderResources.h plShader.h plUniform.h
objects/plGraftCap.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plGraftCap.o: plTriangle.h plMath.h plPolygon.h plUtility.h
objects/plGraftCap.o: plTransform.h plOctreeMesh.h plMesh.h plOctree.h
objects/plGraftCap.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plGraftCap.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraftCap.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plGraftCap.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plGraftCap.o: plArthroViewable.h
objects/plGraftEditor.o: plCommon.h plVector3.h plString.h plTransform.h
objects/plGraftEditor.o: plMatrix44.h plVector4.h plPlan.h plCSV.h
objects/plGraftEditor.o: plRenderable.h plRenderComponent.h plTexture2D.h
objects/plGraftEditor.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plGraftEditor.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plGraftEditor.o: plProjection.h plCamera.h plPicking.h
objects/plGraftEditor.o: plRenderResources.h plShader.h plUniform.h
objects/plGraftEditor.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plGraftEditor.o: plArthroViewable.h plEditable.h plTriangle.h
objects/plGraftEditor.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plGraftEditor.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h
objects/plGraftEditor.o: plOctree.h plRenderer.h plPlanTechnique.h
objects/plGraftEditor.o: plRenderTechnique.h plWindow.h
objects/plGraftEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plGraftEditor.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plGraftEditor.o: plMinimalTechnique.h plTransparentRenderable.h
objects/plGraftEditor.o: plPolygon.h plUtility.h plSphere.h plGraftCap.h
objects/plGraftEditor.o: plCone.h plCylinder.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plGraftEditor.o: plIGuide.h plDisk.h plEditor.h
objects/plIGuide.o: plCommon.h plMeshSpecific.h plMesh.h plVector3.h
objects/plIGuide.o: plString.h plTriangle.h plMath.h plVector4.h plBoundary.h
objects/plIGuide.o: plRenderable.h plRenderComponent.h plMatrix44.h
objects/plIGuide.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plIGuide.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plIGuide.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plIGuide.o: plRenderResources.h plShader.h plUniform.h
objects/plIGuide.o: plVertexFragmentShader.h plFBO.h plEditable.h plDraw.h
objects/plIGuide.o: plSphere.h plRenderer.h plPlanTechnique.h
objects/plIGuide.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plIGuide.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plIGuide.o: plTransparencyTechnique.h plMinimalTechnique.h plGraft.h
objects/plIGuide.o: plArthroViewable.h plPlug.h plBoneAndCartilage.h
objects/plIGuide.o: plModel.h plOctreeMesh.h plOctree.h plTransform.h
objects/plIGuide.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plIGuide.o: plGraftCap.h plCone.h plCylinder.h plIGuideSite.h
objects/plIGuide.o: plMeshCutter.h plMeshExtruder.h plSpline.h plColourMap.h
objects/plIGuideSite.o: plCommon.h plVector3.h plString.h plMeshSpecific.h
objects/plIGuideSite.o: plMesh.h plTriangle.h plMath.h plBoundary.h
objects/plIGuideSite.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plIGuideSite.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plIGuideSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plIGuideSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plIGuideSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plIGuideSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plIGuideSite.o: plEditable.h plDraw.h plSphere.h plRenderer.h
objects/plIGuideSite.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plIGuideSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plIGuideSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plIGuideSite.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plIGuideSite.o: plMeshExtruder.h
objects/plLaserLine.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plLaserLine.o: plRenderable.h plRenderComponent.h plMatrix44.h
objects/plLaserLine.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plLaserLine.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plLaserLine.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plLaserLine.o: plRenderResources.h plShader.h plUniform.h
objects/plLaserLine.o: plVertexFragmentShader.h plFBO.h plRenderer.h
objects/plLaserLine.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plLaserLine.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plLaserLine.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plLaserLine.o: plMinimalTechnique.h plCylinder.h
objects/plMath.o: plCommon.h
objects/plMatrix44.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plMatrixStack.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plMatrixStack.o: plVector4.h
objects/plMesh.o: plCommon.h plVector3.h plString.h plTriangle.h plMath.h
objects/plMeshAlgorithm.o: plCommon.h plVector3.h plString.h plPolygon.h
objects/plMeshAlgorithm.o: plTriangle.h plMath.h plMeshConnectivityData.h
objects/plMeshAlgorithm.o: plUtility.h
objects/plMeshConnectivityData.o: plTriangle.h plCommon.h plString.h
objects/plMeshConnectivityData.o: plVector3.h plMath.h plPolygon.h
objects/plMeshConnectivityData.o: plUtility.h
objects/plMeshCutter.o: plCommon.h plVector3.h plString.h plTriangle.h
objects/plMeshCutter.o: plMath.h plPolygon.h plBoundary.h plRenderable.h
objects/plMeshCutter.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plMeshCutter.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plMeshCutter.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plMeshCutter.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plMeshCutter.o: plRenderResources.h plShader.h plUniform.h
objects/plMeshCutter.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plMeshCutter.o: plMeshSpecific.h plMesh.h plDraw.h plSphere.h
objects/plMeshCutter.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plMeshCutter.o: plWindow.h plScreenQuadTechnique.h
objects/plMeshCutter.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plMeshCutter.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plMeshExtruder.o: plTriangle.h plCommon.h plString.h plVector3.h
objects/plMeshExtruder.o: plMath.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshIntersector.o: plString.h plPolygon.h plTriangle.h plMath.h
objects/plMeshIntersector.o: plMeshConnectivityData.h plUtility.h
objects/plMeshSpecific.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plMeshSpecific.o: plTriangle.h plMath.h
objects/plMinimalTechnique.o: plCommon.h plRenderTechnique.h plRenderable.h
objects/plMinimalTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plMinimalTechnique.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plMinimalTechnique.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plMinimalTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plMinimalTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plMinimalTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plMinimalTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plMinimalTechnique.o: plWindow.h
objects/plModel.o: plCommon.h plOctreeMesh.h plMesh.h plVector3.h plString.h
objects/plModel.o: plTriangle.h plMath.h plOctree.h plRenderable.h
objects/plModel.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plModel.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plModel.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plModel.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plModel.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plModel.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plModel.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plModel.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plModel.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plModel.o: plTransparentRenderable.h plArthroViewable.h
objects/plOctree.o: plCommon.h plVector3.h plString.h plMath.h plTriangle.h
objects/plOctree.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plOctree.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plOctree.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctree.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctree.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctree.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctree.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctree.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plOctree.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plOctree.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plOctreeMesh.o: plCommon.h plMesh.h plVector3.h plString.h
objects/plOctreeMesh.o: plTriangle.h plMath.h plOctree.h plRenderable.h
objects/plOctreeMesh.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plOctreeMesh.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plOctreeMesh.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plOctreeMesh.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plOctreeMesh.o: plRenderResources.h plShader.h plUniform.h
objects/plOctreeMesh.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plOctreeMesh.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctreeMesh.o: plRenderTechnique.h plWindow.h
objects/plOctreeMesh.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plOctreeMesh.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plOctreeMesh.o: plMinimalTechnique.h
objects/plOpenGLInfo.o: plCommon.h
objects/plOutlineTechnique.o: plCommon.h plRenderTechnique.h plRenderable.h
objects/plOutlineTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plOutlineTechnique.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plOutlineTechnique.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plOutlineTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plOutlineTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plOutlineTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plOutlineTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOutlineTechnique.o: plWindow.h
objects/plPicking.o: plCommon.h plRenderResources.h plShader.h plString.h
objects/plPicking.o: plOpenGLInfo.h plUniform.h plVertexFragmentShader.h
objects/plPicking.o: plMatrix44.h plVector3.h plVector4.h plTexture2D.h
objects/plPicking.o: plFBO.h
objects/plPlan.o: plCommon.h plString.h plVector3.h plCSV.h plRenderable.h
objects/plPlan.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plPlan.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlan.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlan.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlan.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlan.o: plGraft.h plArthroViewable.h plEditable.h plTriangle.h
objects/plPlan.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/plPlan.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plPlan.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plPlan.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plPlan.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plPlan.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plPlan.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/plPlan.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/plPlan.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlanTechnique.o: plCommon.h plRenderTechnique.h plRenderable.h
objects/plPlanTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plPlanTechnique.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plPlanTechnique.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlanTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanTechnique.o: plWindow.h
objects/plPlannerShader.o: plCommon.h plShader.h plString.h plOpenGLInfo.h
objects/plPlannerShader.o: plUniform.h plPlanningSite.h plVector4.h
objects/plPlannerShader.o: plVector3.h plTriangle.h plMath.h plBoundary.h
objects/plPlannerShader.o: plRenderable.h plRenderComponent.h plMatrix44.h
objects/plPlannerShader.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlannerShader.o: plEABO.h plRenderingPipeline.h plMatrixStack.h
objects/plPlannerShader.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerShader.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerShader.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerShader.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerShader.o: plRenderTechnique.h plWindow.h
objects/plPlannerShader.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerShader.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerShader.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerShader.o: plSSBO.h plBufferObject.h plPlannerStructs.h
objects/plPlannerShader.o: plUtility.h
objects/plPlannerStage0.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStage0.o: plVector3.h plPlannerStructs.h plSSBO.h
objects/plPlannerStage0.o: plBufferObject.h plPlannerShader.h plShader.h
objects/plPlannerStage0.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage0.o: plTriangle.h plMath.h plBoundary.h plRenderable.h
objects/plPlannerStage0.o: plRenderComponent.h plMatrix44.h plTexture2D.h
objects/plPlannerStage0.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage0.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage0.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage0.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage0.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage0.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage0.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage0.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage0.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage0.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage0.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage0.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage0.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage0.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage0.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage0.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage0.o: plIGuide.h
objects/plPlannerStage1.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage1.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage1.o: plBufferObject.h plPlannerStage0.h
objects/plPlannerStage1.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage1.o: plUniform.h plPlanningSite.h plTriangle.h plMath.h
objects/plPlannerStage1.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plPlannerStage1.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerStage1.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage1.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage1.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage1.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage1.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage1.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage1.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage1.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage1.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage1.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage1.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage1.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage1.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage1.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage1.o: plIGuide.h
objects/plPlannerStage2.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage2.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage2.o: plBufferObject.h plPlannerStage1.h
objects/plPlannerStage2.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage2.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage2.o: plTriangle.h plMath.h plBoundary.h plRenderable.h
objects/plPlannerStage2.o: plRenderComponent.h plMatrix44.h plTexture2D.h
objects/plPlannerStage2.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage2.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage2.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage2.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage2.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage2.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage2.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage2.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage2.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage2.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage2.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage2.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage2.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage2.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage2.o: plIGuide.h
objects/plPlannerStage3.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlannerStage3.o: plPlannerStructs.h plUtility.h plSSBO.h
objects/plPlannerStage3.o: plBufferObject.h plPlannerStage0.h
objects/plPlannerStage3.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage3.o: plUniform.h plPlanningSite.h plTriangle.h plMath.h
objects/plPlannerStage3.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plPlannerStage3.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerStage3.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage3.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage3.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage3.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage3.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage3.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage3.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage3.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage3.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage3.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage3.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage3.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage3.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage3.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage3.o: plIGuide.h plPlannerStage2.h plPlannerStage1.h
objects/plPlannerStructs.o: plCommon.h plUtility.h plVector4.h plString.h
objects/plPlannerStructs.o: plVector3.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningBufferData.o: plPlanningSite.h plTriangle.h plMath.h
objects/plPlanningBufferData.o: plBoundary.h plRenderable.h
objects/plPlanningBufferData.o: plRenderComponent.h plMatrix44.h
objects/plPlanningBufferData.o: plTexture2D.h plVAO.h plVBO.h
objects/plPlanningBufferData.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plPlanningBufferData.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningBufferData.o: plProjection.h plCamera.h plPicking.h
objects/plPlanningBufferData.o: plRenderResources.h plShader.h plUniform.h
objects/plPlanningBufferData.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plPlanningBufferData.o: plMeshSpecific.h plMesh.h plDraw.h plSphere.h
objects/plPlanningBufferData.o: plRenderer.h plPlanTechnique.h
objects/plPlanningBufferData.o: plRenderTechnique.h plWindow.h
objects/plPlanningBufferData.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlanningBufferData.o: plArthroCamTechnique.h
objects/plPlanningBufferData.o: plTransparencyTechnique.h
objects/plPlanningBufferData.o: plMinimalTechnique.h plMeshCutter.h
objects/plPlanningBufferData.o: plPolygon.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plDefectSite.h plSpline.h plColourMap.h
objects/plPlanningBufferData.o: plOctreeMesh.h plOctree.h plTransform.h
objects/plPlanningBufferData.o: plDonorSite.h
objects/plPlanningSite.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plPlanningSite.o: plTriangle.h plMath.h plBoundary.h plRenderable.h
objects/plPlanningSite.o: plRenderComponent.h plMatrix44.h plTexture2D.h
objects/plPlanningSite.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlanningSite.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanningSite.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanningSite.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanningSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanningSite.o: plEditable.h plMeshSpecific.h plMesh.h plDraw.h
objects/plPlanningSite.o: plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlanningSite.o: plRenderTechnique.h plWindow.h
objects/plPlanningSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlanningSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlanningSite.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlanningSite.o: plSSBO.h plBufferObject.h
objects/plPlug.o: plCommon.h plMeshSpecific.h plMesh.h plVector3.h plString.h
objects/plPlug.o: plTriangle.h plMath.h plBoneAndCartilage.h plModel.h
objects/plPlug.o: plOctreeMesh.h plOctree.h plRenderable.h
objects/plPlug.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plPlug.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlug.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlug.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlug.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlug.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plPlug.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plPlug.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plPlug.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plPlug.o: plTransparentRenderable.h plArthroViewable.h
objects/plPolygon.o: plCommon.h plVector3.h plString.h
objects/plProjection.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plProjection.o: plVector4.h
objects/plRenderComponent.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plRenderComponent.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plRenderComponent.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderComponent.o: plRenderingPipeline.h plMatrixStack.h
objects/plRenderComponent.o: plProjection.h plCamera.h plPicking.h
objects/plRenderComponent.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderComponent.o: plVertexFragmentShader.h plFBO.h
objects/plRenderResources.o: plCommon.h plShader.h plString.h plOpenGLInfo.h
objects/plRenderResources.o: plUniform.h plVertexFragmentShader.h
objects/plRenderResources.o: plMatrix44.h plVector3.h plVector4.h
objects/plRenderResources.o: plTexture2D.h plFBO.h
objects/plRenderTechnique.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plRenderTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plRenderTechnique.o: plTexture2D.h plVAO.h plVBO.h
objects/plRenderTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plRenderTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plRenderTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plRenderable.o: plCommon.h plVector3.h plString.h plRenderComponent.h
objects/plRenderable.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plRenderable.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderable.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plRenderable.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plRenderable.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plRenderer.o: plCommon.h plVector3.h plString.h plRenderingPipeline.h
objects/plRenderer.o: plVector4.h plMatrixStack.h plMatrix44.h plProjection.h
objects/plRenderer.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plRenderer.o: plOpenGLInfo.h plUniform.h plVertexFragmentShader.h
objects/plRenderer.o: plTexture2D.h plFBO.h plRenderable.h
objects/plRenderer.o: plRenderComponent.h plVAO.h plVBO.h
objects/plRenderer.o: plVertexSpecBuffer.h plEABO.h plPlanTechnique.h
objects/plRenderer.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plRenderer.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plRenderer.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plRenderingPipeline.o: plCommon.h plVector4.h plString.h plVector3.h
objects/plRenderingPipeline.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plRenderingPipeline.o: plCamera.h plPicking.h plRenderResources.h
objects/plRenderingPipeline.o: plShader.h plOpenGLInfo.h plUniform.h
objects/plRenderingPipeline.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plSSBO.o: plCommon.h plBufferObject.h
objects/plScan.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plVAO.h plVBO.h
objects/plScan.o: plVertexSpecBuffer.h plVector4.h plEABO.h plOpenGLInfo.h
objects/plScan.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScan.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plScan.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plScan.o: plTexture2D.h plFBO.h
objects/plScanPoint.o: plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plVector3.h plCommon.h plString.h plDraw.h plVAO.h
objects/plScanVolume.o: plVBO.h plVertexSpecBuffer.h plVector4.h plEABO.h
objects/plScanVolume.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plScanVolume.o: plMatrix44.h plProjection.h plCamera.h plPicking.h
objects/plScanVolume.o: plRenderResources.h plShader.h plUniform.h
objects/plScanVolume.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plScreenQuadTechnique.o: plCommon.h plRenderTechnique.h
objects/plScreenQuadTechnique.o: plRenderable.h plVector3.h plString.h
objects/plScreenQuadTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plScreenQuadTechnique.o: plTexture2D.h plVAO.h plVBO.h
objects/plScreenQuadTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plScreenQuadTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plScreenQuadTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plScreenQuadTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plScreenQuadTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plSeq.o: plCommon.h
objects/plShader.o: plCommon.h plString.h plOpenGLInfo.h plUniform.h
objects/plSphere.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plSphere.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plSphere.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plSphere.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plSphere.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plSphere.o: plRenderResources.h plShader.h plUniform.h
objects/plSphere.o: plVertexFragmentShader.h plFBO.h
objects/plSpline.o: plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plRenderable.h plRenderComponent.h
objects/plSpline.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plSpline.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plSpline.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plSpline.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plSpline.o: plUniform.h plVertexFragmentShader.h plFBO.h plEditable.h
objects/plSpline.o: plMeshSpecific.h plMesh.h plTriangle.h plDraw.h
objects/plSpline.o: plSphere.h plRenderer.h plPlanTechnique.h
objects/plSpline.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plSpline.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plSpline.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plSpline.o: plColourMap.h plOctreeMesh.h plOctree.h plTransform.h
objects/plString.o: plCommon.h
objects/plTexture2D.o: plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plCommon.h plVector3.h plString.h
objects/plTrackedObject.o: plCommon.h plRenderable.h plVector3.h plString.h
objects/plTrackedObject.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plTrackedObject.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plTrackedObject.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plTrackedObject.o: plMatrixStack.h plProjection.h plCamera.h
objects/plTrackedObject.o: plPicking.h plRenderResources.h plShader.h
objects/plTrackedObject.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plTrackedObject.o: plDRBTransform.h plCylinder.h plCone.h
objects/plTrackedObject.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plTrackedObject.o: plWindow.h plScreenQuadTechnique.h
objects/plTrackedObject.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plTrackedObject.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plTransform.o: plCommon.h plString.h plVector3.h plMatrix44.h
objects/plTransform.o: plVector4.h
objects/plTransparencyTechnique.o: plCommon.h plRenderTechnique.h
objects/plTransparencyTechnique.o: plRenderable.h plVector3.h plString.h
objects/plTransparencyTechnique.o: plRenderComponent.h plVector4.h
objects/plTransparencyTechnique.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plTransparencyTechnique.o: plVertexSpecBuffer.h plEABO.h
objects/plTransparencyTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plTransparencyTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plTransparencyTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plTransparencyTechnique.o: plUniform.h plVertexFragmentShader.h
objects/plTransparencyTechnique.o: plFBO.h plWindow.h
objects/plTransparentRenderable.o: plCommon.h plRenderable.h plVector3.h
objects/plTransparentRenderable.o: plString.h plRenderComponent.h plVector4.h
objects/plTransparentRenderable.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plTransparentRenderable.o: plVertexSpecBuffer.h plEABO.h
objects/plTransparentRenderable.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plTransparentRenderable.o: plMatrixStack.h plProjection.h plCamera.h
objects/plTransparentRenderable.o: plPicking.h plRenderResources.h plShader.h
objects/plTransparentRenderable.o: plUniform.h plVertexFragmentShader.h
objects/plTransparentRenderable.o: plFBO.h
objects/plTriangle.o: plCommon.h plString.h plVector3.h plMath.h
objects/plUniform.o: plCommon.h
objects/plUtility.o: plCommon.h
objects/plVAO.o: plCommon.h plVBO.h plVertexSpecBuffer.h plVector4.h
objects/plVAO.o: plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plVAO.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plVAO.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plVAO.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plVAO.o: plTexture2D.h plFBO.h
objects/plVBO.o: plCommon.h plVertexSpecBuffer.h plVector4.h plString.h
objects/plVBO.o: plVector3.h
objects/plVector3.o: plCommon.h plString.h
objects/plVector4.o: plCommon.h plString.h plVector3.h
objects/plVertexFragmentShader.o: plCommon.h plShader.h plString.h
objects/plVertexFragmentShader.o: plOpenGLInfo.h plUniform.h plMatrix44.h
objects/plVertexFragmentShader.o: plVector3.h plVector4.h plTexture2D.h
objects/plVertexSpecBuffer.o: plCommon.h
objects/plWindow.o: plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plRenderingPipeline.h plMatrixStack.h
objects/plWindow.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plWindow.o: plShader.h plOpenGLInfo.h plUniform.h
objects/plWindow.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/PlannerWindow.o: PlannerWindow.h Window.h pl.h plCommon.h
objects/PlannerWindow.o: plAutomaticPlanner.h plPlan.h plString.h plVector3.h
objects/PlannerWindow.o: plCSV.h plRenderable.h plRenderComponent.h
objects/PlannerWindow.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/PlannerWindow.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/PlannerWindow.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/PlannerWindow.o: plCamera.h plPicking.h plRenderResources.h
objects/PlannerWindow.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/PlannerWindow.o: plFBO.h plGraft.h plArthroViewable.h plEditable.h
objects/PlannerWindow.o: plTriangle.h plMath.h plDraw.h plPlug.h
objects/PlannerWindow.o: plMeshSpecific.h plMesh.h plBoneAndCartilage.h
objects/PlannerWindow.o: plModel.h plOctreeMesh.h plOctree.h plTransform.h
objects/PlannerWindow.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/PlannerWindow.o: plWindow.h plScreenQuadTechnique.h
objects/PlannerWindow.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/PlannerWindow.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/PlannerWindow.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/PlannerWindow.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/PlannerWindow.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/PlannerWindow.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/PlannerWindow.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/PlannerWindow.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/PlannerWindow.o: plPlannerStage0.h plPlannerStructs.h
objects/PlannerWindow.o: plPlannerShader.h plPlannerStage1.h
objects/PlannerWindow.o: plPlannerStage2.h plPlannerStage3.h
objects/PlannerWindow.o: plBoundaryEditor.h plEditor.h plDisk.h
objects/PlannerWindow.o: plChessBoard.h plDRBTransform.h plGraftEditor.h
objects/PlannerWindow.o: plScan.h plScanPoint.h plScanVolume.h plTexture3D.h
objects/PlannerWindow.o: plTrackedObject.h
objects/Window.o: Window.h pl.h plCommon.h plAutomaticPlanner.h plPlan.h
objects/Window.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/Window.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/Window.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/Window.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/Window.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/Window.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/Window.o: plGraft.h plArthroViewable.h plEditable.h plTriangle.h
objects/Window.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/Window.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/Window.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/Window.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/Window.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/Window.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/Window.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/Window.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/Window.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/Window.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/Window.o: plMeshExtruder.h plIGuide.h plPlanningBufferData.h
objects/Window.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/Window.o: plPlannerStage0.h plPlannerStructs.h plPlannerShader.h
objects/Window.o: plPlannerStage1.h plPlannerStage2.h plPlannerStage3.h
objects/Window.o: plBoundaryEditor.h plEditor.h plDisk.h plChessBoard.h
objects/Window.o: plDRBTransform.h plGraftEditor.h plScan.h plScanPoint.h
objects/Window.o: plScanVolume.h plTexture3D.h plTrackedObject.h
objects/main.o: PlannerWindow.h Window.h pl.h plCommon.h plAutomaticPlanner.h
objects/main.o: plPlan.h plString.h plVector3.h plCSV.h plRenderable.h
objects/main.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/main.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/main.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/main.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/main.o: plUniform.h plVertexFragmentShader.h plFBO.h plGraft.h
objects/main.o: plArthroViewable.h plEditable.h plTriangle.h plMath.h
objects/main.o: plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/main.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/main.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/main.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/main.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/main.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/main.o: plTransparentRenderable.h plPolygon.h plUtility.h plSphere.h
objects/main.o: plGraftCap.h plCone.h plCylinder.h plDefectSite.h plSpline.h
objects/main.o: plBoundary.h plColourMap.h plDonorSite.h plIGuideSite.h
objects/main.o: plMeshCutter.h plMeshExtruder.h plIGuide.h
objects/main.o: plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/main.o: plBufferObject.h plPlannerStage0.h plPlannerStructs.h
objects/main.o: plPlannerShader.h plPlannerStage1.h plPlannerStage2.h
objects/main.o: plPlannerStage3.h plBoundaryEditor.h plEditor.h plDisk.h
objects/main.o: plChessBoard.h plDRBTransform.h plGraftEditor.h plScan.h
objects/main.o: plScanPoint.h plScanVolume.h plTexture3D.h plTrackedObject.h
objects/pl.o: pl.h plCommon.h plAutomaticPlanner.h plPlan.h plString.h
objects/pl.o: plVector3.h plCSV.h plRenderable.h plRenderComponent.h
objects/pl.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/pl.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/pl.o: plRenderingPipeline.h plMatrixStack.h plProjection.h plCamera.h
objects/pl.o: plPicking.h plRenderResources.h plShader.h plUniform.h
objects/pl.o: plVertexFragmentShader.h plFBO.h plGraft.h plArthroViewable.h
objects/pl.o: plEditable.h plTriangle.h plMath.h plDraw.h plPlug.h
objects/pl.o: plMeshSpecific.h plMesh.h plBoneAndCartilage.h plModel.h
objects/pl.o: plOctreeMesh.h plOctree.h plTransform.h plRenderer.h
objects/pl.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/pl.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/pl.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/pl.o: plMinimalTechnique.h plTransparentRenderable.h plPolygon.h
objects/pl.o: plUtility.h plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/pl.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/pl.o: plDonorSite.h plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/pl.o: plIGuide.h plPlanningBufferData.h plPlanningSite.h plSSBO.h
objects/pl.o: plBufferObject.h plPlannerStage0.h plPlannerStructs.h
objects/pl.o: plPlannerShader.h plPlannerStage1.h plPlannerStage2.h
objects/pl.o: plPlannerStage3.h plBoundaryEditor.h plEditor.h plDisk.h
objects/pl.o: plChessBoard.h plDRBTransform.h plGraftEditor.h plScan.h
objects/pl.o: plScanPoint.h plScanVolume.h plTexture3D.h plTrackedObject.h
objects/plArthroCamTechnique.o: plArthroCamTechnique.h plCommon.h
objects/plArthroCamTechnique.o: plRenderTechnique.h plRenderable.h
objects/plArthroCamTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plArthroCamTechnique.o: plVector4.h plMatrix44.h plTexture2D.h
objects/plArthroCamTechnique.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plArthroCamTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plArthroCamTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plArthroCamTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plArthroCamTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plArthroCamTechnique.o: plWindow.h
objects/plArthroViewable.o: plArthroViewable.h plCommon.h plVector3.h
objects/plArthroViewable.o: plString.h plRenderComponent.h plVector4.h
objects/plArthroViewable.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plArthroViewable.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plArthroViewable.o: plRenderingPipeline.h plMatrixStack.h
objects/plArthroViewable.o: plProjection.h plCamera.h plPicking.h
objects/plArthroViewable.o: plRenderResources.h plShader.h plUniform.h
objects/plArthroViewable.o: plVertexFragmentShader.h plFBO.h
objects/plArthroscope.o: plArthroscope.h plCommon.h plTrackedObject.h
objects/plArthroscope.o: plRenderable.h plVector3.h plString.h
objects/plArthroscope.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plArthroscope.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plArthroscope.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plArthroscope.o: plMatrixStack.h plProjection.h plCamera.h
objects/plArthroscope.o: plPicking.h plRenderResources.h plShader.h
objects/plArthroscope.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plArthroscope.o: plDRBTransform.h plCylinder.h plCone.h plRenderer.h
objects/plArthroscope.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plArthroscope.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plArthroscope.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plArthroscope.o: plMinimalTechnique.h
objects/plAutomaticPlanner.o: plAutomaticPlanner.h plCommon.h plPlan.h
objects/plAutomaticPlanner.o: plString.h plVector3.h plCSV.h plRenderable.h
objects/plAutomaticPlanner.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plAutomaticPlanner.o: plTexture2D.h plVAO.h plVBO.h
objects/plAutomaticPlanner.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plAutomaticPlanner.o: plRenderingPipeline.h plMatrixStack.h
objects/plAutomaticPlanner.o: plProjection.h plCamera.h plPicking.h
objects/plAutomaticPlanner.o: plRenderResources.h plShader.h plUniform.h
objects/plAutomaticPlanner.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plAutomaticPlanner.o: plArthroViewable.h plEditable.h plTriangle.h
objects/plAutomaticPlanner.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h
objects/plAutomaticPlanner.o: plMesh.h plBoneAndCartilage.h plModel.h
objects/plAutomaticPlanner.o: plOctreeMesh.h plOctree.h plTransform.h
objects/plAutomaticPlanner.o: plRenderer.h plPlanTechnique.h
objects/plAutomaticPlanner.o: plRenderTechnique.h plWindow.h
objects/plAutomaticPlanner.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plAutomaticPlanner.o: plArthroCamTechnique.h
objects/plAutomaticPlanner.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plAutomaticPlanner.o: plTransparentRenderable.h plPolygon.h
objects/plAutomaticPlanner.o: plUtility.h plSphere.h plGraftCap.h plCone.h
objects/plAutomaticPlanner.o: plCylinder.h plDefectSite.h plSpline.h
objects/plAutomaticPlanner.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plAutomaticPlanner.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plAutomaticPlanner.o: plIGuide.h plPlanningBufferData.h
objects/plAutomaticPlanner.o: plPlanningSite.h plSSBO.h plBufferObject.h
objects/plAutomaticPlanner.o: plPlannerStage0.h plPlannerStructs.h
objects/plAutomaticPlanner.o: plPlannerShader.h plPlannerStage1.h
objects/plAutomaticPlanner.o: plPlannerStage2.h plPlannerStage3.h
objects/plBoneAndCartilage.o: plBoneAndCartilage.h plCommon.h plModel.h
objects/plBoneAndCartilage.o: plOctreeMesh.h plMesh.h plVector3.h plString.h
objects/plBoneAndCartilage.o: plTriangle.h plMath.h plOctree.h plRenderable.h
objects/plBoneAndCartilage.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plBoneAndCartilage.o: plTexture2D.h plVAO.h plVBO.h
objects/plBoneAndCartilage.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoneAndCartilage.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoneAndCartilage.o: plProjection.h plCamera.h plPicking.h
objects/plBoneAndCartilage.o: plRenderResources.h plShader.h plUniform.h
objects/plBoneAndCartilage.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plBoneAndCartilage.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plBoneAndCartilage.o: plRenderTechnique.h plWindow.h
objects/plBoneAndCartilage.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoneAndCartilage.o: plArthroCamTechnique.h
objects/plBoneAndCartilage.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plBoneAndCartilage.o: plTransparentRenderable.h plArthroViewable.h
objects/plBoundary.o: plBoundary.h plCommon.h plMath.h plVector3.h plString.h
objects/plBoundary.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plBoundary.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plBoundary.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoundary.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plBoundary.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plBoundary.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plBoundary.o: plEditable.h plMeshSpecific.h plMesh.h plTriangle.h
objects/plBoundary.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plBoundary.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plBoundary.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plBoundary.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plBoundaryEditor.o: plBoundaryEditor.h plCommon.h plVector3.h
objects/plBoundaryEditor.o: plString.h plPlan.h plCSV.h plRenderable.h
objects/plBoundaryEditor.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plBoundaryEditor.o: plTexture2D.h plVAO.h plVBO.h
objects/plBoundaryEditor.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plBoundaryEditor.o: plRenderingPipeline.h plMatrixStack.h
objects/plBoundaryEditor.o: plProjection.h plCamera.h plPicking.h
objects/plBoundaryEditor.o: plRenderResources.h plShader.h plUniform.h
objects/plBoundaryEditor.o: plVertexFragmentShader.h plFBO.h plGraft.h
objects/plBoundaryEditor.o: plArthroViewable.h plEditable.h plTriangle.h
objects/plBoundaryEditor.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h
objects/plBoundaryEditor.o: plMesh.h plBoneAndCartilage.h plModel.h
objects/plBoundaryEditor.o: plOctreeMesh.h plOctree.h plTransform.h
objects/plBoundaryEditor.o: plRenderer.h plPlanTechnique.h
objects/plBoundaryEditor.o: plRenderTechnique.h plWindow.h
objects/plBoundaryEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plBoundaryEditor.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plBoundaryEditor.o: plMinimalTechnique.h plTransparentRenderable.h
objects/plBoundaryEditor.o: plPolygon.h plUtility.h plSphere.h plGraftCap.h
objects/plBoundaryEditor.o: plCone.h plCylinder.h plDefectSite.h plSpline.h
objects/plBoundaryEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plBoundaryEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plBoundaryEditor.o: plIGuide.h plEditor.h plDisk.h
objects/plBufferObject.o: plBufferObject.h plCommon.h
objects/plCSV.o: plCSV.h plCommon.h plString.h
objects/plCamera.o: plCamera.h plCommon.h plVector3.h plString.h plVector4.h
objects/plCamera.o: plMatrix44.h
objects/plChessBoard.o: plChessBoard.h plCommon.h plTransparentRenderable.h
objects/plChessBoard.o: plRenderable.h plVector3.h plString.h
objects/plChessBoard.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plChessBoard.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plChessBoard.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plChessBoard.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plChessBoard.o: plRenderResources.h plShader.h plUniform.h
objects/plChessBoard.o: plVertexFragmentShader.h plFBO.h plTransform.h
objects/plChessBoard.o: plDRBTransform.h plRenderer.h plPlanTechnique.h
objects/plChessBoard.o: plRenderTechnique.h plWindow.h
objects/plChessBoard.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plChessBoard.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plChessBoard.o: plMinimalTechnique.h
objects/plColourMap.o: plColourMap.h plCommon.h plVector3.h plString.h
objects/plCone.o: plCone.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plCone.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plCone.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plCone.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plCone.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plCone.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plCylinder.o: plCylinder.h plCommon.h plRenderable.h plVector3.h
objects/plCylinder.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plCylinder.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plCylinder.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plCylinder.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plCylinder.o: plRenderResources.h plShader.h plUniform.h
objects/plCylinder.o: plVertexFragmentShader.h plFBO.h
objects/plDRBTransform.o: plDRBTransform.h plCommon.h plMatrix44.h
objects/plDRBTransform.o: plVector3.h plString.h plVector4.h
objects/plDebugSphere.o: plDebugSphere.h plCommon.h plVector3.h plString.h
objects/plDebugSphere.o: plVector4.h plDraw.h plVAO.h plVBO.h
objects/plDebugSphere.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDebugSphere.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDebugSphere.o: plProjection.h plCamera.h plPicking.h
objects/plDebugSphere.o: plRenderResources.h plShader.h plUniform.h
objects/plDebugSphere.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plDebugTransform.o: plDebugTransform.h plCommon.h plVector3.h
objects/plDebugTransform.o: plString.h plVector4.h plDraw.h plVAO.h plVBO.h
objects/plDebugTransform.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDebugTransform.o: plRenderingPipeline.h plMatrixStack.h
objects/plDebugTransform.o: plMatrix44.h plProjection.h plCamera.h
objects/plDebugTransform.o: plPicking.h plRenderResources.h plShader.h
objects/plDebugTransform.o: plUniform.h plVertexFragmentShader.h
objects/plDebugTransform.o: plTexture2D.h plFBO.h
objects/plDefectSite.o: plDefectSite.h plCommon.h plVector3.h plString.h
objects/plDefectSite.o: plMeshSpecific.h plMesh.h plTriangle.h plMath.h
objects/plDefectSite.o: plSpline.h plVector4.h plBoundary.h plRenderable.h
objects/plDefectSite.o: plRenderComponent.h plMatrix44.h plTexture2D.h
objects/plDefectSite.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDefectSite.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDefectSite.o: plProjection.h plCamera.h plPicking.h
objects/plDefectSite.o: plRenderResources.h plShader.h plUniform.h
objects/plDefectSite.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plDefectSite.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plDefectSite.o: plRenderTechnique.h plWindow.h
objects/plDefectSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plDefectSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plDefectSite.o: plMinimalTechnique.h plColourMap.h plOctreeMesh.h
objects/plDefectSite.o: plOctree.h plTransform.h
objects/plDisk.o: plDisk.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plDisk.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plDisk.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plDisk.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plDisk.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plDisk.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plDonorSite.h plCommon.h plVector3.h plString.h
objects/plDonorSite.o: plMeshSpecific.h plMesh.h plTriangle.h plMath.h
objects/plDonorSite.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plDonorSite.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plDonorSite.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plDonorSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plDonorSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plDonorSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plDonorSite.o: plEditable.h plDraw.h plSphere.h plRenderer.h
objects/plDonorSite.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plDonorSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plDonorSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plDonorSite.o: plMinimalTechnique.h
objects/plDraw.o: plDraw.h plCommon.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plDraw.o: plVector4.h plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plDraw.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plDraw.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plDraw.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plDraw.o: plTexture2D.h plFBO.h
objects/plEABO.o: plEABO.h plCommon.h plVertexSpecBuffer.h plOpenGLInfo.h
objects/plEditable.o: plEditable.h plCommon.h plRenderComponent.h plVector3.h
objects/plEditable.o: plString.h plVector4.h plMatrix44.h plTexture2D.h
objects/plEditable.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plEditable.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plEditable.o: plProjection.h plCamera.h plPicking.h
objects/plEditable.o: plRenderResources.h plShader.h plUniform.h
objects/plEditable.o: plVertexFragmentShader.h plFBO.h
objects/plEditor.o: plEditor.h plCommon.h plRenderable.h plVector3.h
objects/plEditor.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plEditor.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plEditor.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plEditor.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plEditor.o: plRenderResources.h plShader.h plUniform.h
objects/plEditor.o: plVertexFragmentShader.h plFBO.h plPlan.h plCSV.h
objects/plEditor.o: plGraft.h plArthroViewable.h plEditable.h plTriangle.h
objects/plEditor.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plEditor.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/plEditor.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plEditor.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plEditor.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plEditor.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plEditor.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plEditor.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/plEditor.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/plEditor.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plEditor.o: plMeshExtruder.h plIGuide.h
objects/plFBO.o: plFBO.h plCommon.h plVector3.h plString.h plTexture2D.h
objects/plFBO.o: plOpenGLInfo.h
objects/plGraft.o: plGraft.h plCommon.h plRenderable.h plVector3.h plString.h
objects/plGraft.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plGraft.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plGraft.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plGraft.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plGraft.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraft.o: plArthroViewable.h plEditable.h plTriangle.h plMath.h
objects/plGraft.o: plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plGraft.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/plGraft.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plGraft.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraft.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plGraft.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plGraft.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plGraft.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/plGraftCap.o: plGraftCap.h plCommon.h plRenderable.h plVector3.h
objects/plGraftCap.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plGraftCap.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraftCap.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraftCap.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plGraftCap.o: plRenderResources.h plShader.h plUniform.h
objects/plGraftCap.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plGraftCap.o: plTriangle.h plMath.h plPolygon.h plUtility.h
objects/plGraftCap.o: plTransform.h plOctreeMesh.h plMesh.h plOctree.h
objects/plGraftCap.o: plDraw.h plRenderer.h plPlanTechnique.h
objects/plGraftCap.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plGraftCap.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plGraftCap.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plGraftCap.o: plArthroViewable.h
objects/plGraftEditor.o: plGraftEditor.h plCommon.h plVector3.h plString.h
objects/plGraftEditor.o: plTransform.h plMatrix44.h plVector4.h plPlan.h
objects/plGraftEditor.o: plCSV.h plRenderable.h plRenderComponent.h
objects/plGraftEditor.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plGraftEditor.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plGraftEditor.o: plMatrixStack.h plProjection.h plCamera.h
objects/plGraftEditor.o: plPicking.h plRenderResources.h plShader.h
objects/plGraftEditor.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plGraftEditor.o: plGraft.h plArthroViewable.h plEditable.h
objects/plGraftEditor.o: plTriangle.h plMath.h plDraw.h plPlug.h
objects/plGraftEditor.o: plMeshSpecific.h plMesh.h plBoneAndCartilage.h
objects/plGraftEditor.o: plModel.h plOctreeMesh.h plOctree.h plRenderer.h
objects/plGraftEditor.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plGraftEditor.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plGraftEditor.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plGraftEditor.o: plMinimalTechnique.h plTransparentRenderable.h
objects/plGraftEditor.o: plPolygon.h plUtility.h plSphere.h plGraftCap.h
objects/plGraftEditor.o: plCone.h plCylinder.h plDefectSite.h plSpline.h
objects/plGraftEditor.o: plBoundary.h plColourMap.h plDonorSite.h
objects/plGraftEditor.o: plIGuideSite.h plMeshCutter.h plMeshExtruder.h
objects/plGraftEditor.o: plIGuide.h plDisk.h plEditor.h
objects/plIGuide.o: plIGuide.h plCommon.h plMeshSpecific.h plMesh.h
objects/plIGuide.o: plVector3.h plString.h plTriangle.h plMath.h plVector4.h
objects/plIGuide.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plIGuide.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plIGuide.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plIGuide.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plIGuide.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plIGuide.o: plUniform.h plVertexFragmentShader.h plFBO.h plEditable.h
objects/plIGuide.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plIGuide.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plIGuide.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plIGuide.o: plTransparencyTechnique.h plMinimalTechnique.h plGraft.h
objects/plIGuide.o: plArthroViewable.h plPlug.h plBoneAndCartilage.h
objects/plIGuide.o: plModel.h plOctreeMesh.h plOctree.h plTransform.h
objects/plIGuide.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plIGuide.o: plGraftCap.h plCone.h plCylinder.h plIGuideSite.h
objects/plIGuide.o: plMeshCutter.h plMeshExtruder.h plSpline.h plColourMap.h
objects/plIGuideSite.o: plIGuideSite.h plCommon.h plVector3.h plString.h
objects/plIGuideSite.o: plMeshSpecific.h plMesh.h plTriangle.h plMath.h
objects/plIGuideSite.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plIGuideSite.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plIGuideSite.o: plVBO.h plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plIGuideSite.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plIGuideSite.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plIGuideSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plIGuideSite.o: plEditable.h plDraw.h plSphere.h plRenderer.h
objects/plIGuideSite.o: plPlanTechnique.h plRenderTechnique.h plWindow.h
objects/plIGuideSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plIGuideSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plIGuideSite.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plIGuideSite.o: plMeshExtruder.h
objects/plLaserLine.o: plLaserLine.h plCommon.h plVector3.h plString.h
objects/plLaserLine.o: plVector4.h plRenderable.h plRenderComponent.h
objects/plLaserLine.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plLaserLine.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plLaserLine.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plLaserLine.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plLaserLine.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plLaserLine.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plLaserLine.o: plWindow.h plScreenQuadTechnique.h
objects/plLaserLine.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plLaserLine.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plLaserLine.o: plCylinder.h
objects/plMath.o: plMath.h plCommon.h plVector3.h plString.h plVector4.h
objects/plMath.o: plMatrix44.h plTriangle.h plPolygon.h
objects/plMatrix44.o: plMatrix44.h plCommon.h plVector3.h plString.h
objects/plMatrix44.o: plVector4.h plTriangle.h plMath.h
objects/plMatrixStack.o: plMatrixStack.h plCommon.h plMatrix44.h plVector3.h
objects/plMatrixStack.o: plString.h plVector4.h
objects/plMesh.o: plMesh.h plCommon.h plVector3.h plString.h plTriangle.h
objects/plMesh.o: plMath.h
objects/plMeshAlgorithm.o: plMeshAlgorithm.h plCommon.h plVector3.h
objects/plMeshAlgorithm.o: plString.h plPolygon.h plTriangle.h plMath.h
objects/plMeshAlgorithm.o: plMeshConnectivityData.h plUtility.h
objects/plMeshConnectivityData.o: plMeshConnectivityData.h plTriangle.h
objects/plMeshConnectivityData.o: plCommon.h plString.h plVector3.h plMath.h
objects/plMeshConnectivityData.o: plPolygon.h plUtility.h
objects/plMeshCutter.o: plMeshCutter.h plCommon.h plVector3.h plString.h
objects/plMeshCutter.o: plTriangle.h plMath.h plPolygon.h plBoundary.h
objects/plMeshCutter.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plMeshCutter.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plMeshCutter.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plMeshCutter.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plMeshCutter.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plMeshCutter.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plMeshCutter.o: plEditable.h plMeshSpecific.h plMesh.h plDraw.h
objects/plMeshCutter.o: plSphere.h plRenderer.h plPlanTechnique.h
objects/plMeshCutter.o: plRenderTechnique.h plWindow.h
objects/plMeshCutter.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plMeshCutter.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plMeshCutter.o: plMinimalTechnique.h
objects/plMeshExtruder.o: plMeshExtruder.h plTriangle.h plCommon.h plString.h
objects/plMeshExtruder.o: plVector3.h plMath.h plMatrix44.h plVector4.h
objects/plMeshIntersector.o: plMeshIntersector.h plMeshAlgorithm.h plCommon.h
objects/plMeshIntersector.o: plVector3.h plString.h plPolygon.h plTriangle.h
objects/plMeshIntersector.o: plMath.h plMeshConnectivityData.h plUtility.h
objects/plMeshSpecific.o: plMeshSpecific.h plCommon.h plMesh.h plVector3.h
objects/plMeshSpecific.o: plString.h plTriangle.h plMath.h
objects/plMinimalTechnique.o: plMinimalTechnique.h plCommon.h
objects/plMinimalTechnique.o: plRenderTechnique.h plRenderable.h plVector3.h
objects/plMinimalTechnique.o: plString.h plRenderComponent.h plVector4.h
objects/plMinimalTechnique.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plMinimalTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plMinimalTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plMinimalTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plMinimalTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plMinimalTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plModel.o: plModel.h plCommon.h plOctreeMesh.h plMesh.h plVector3.h
objects/plModel.o: plString.h plTriangle.h plMath.h plOctree.h plRenderable.h
objects/plModel.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plModel.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plModel.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plModel.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plModel.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plModel.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plModel.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plModel.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plModel.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plModel.o: plTransparentRenderable.h plArthroViewable.h
objects/plOctree.o: plOctree.h plCommon.h plVector3.h plString.h plMath.h
objects/plOctree.o: plTriangle.h plRenderable.h plRenderComponent.h
objects/plOctree.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plOctree.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctree.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctree.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctree.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctree.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctree.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plOctree.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plOctree.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plOctreeMesh.o: plOctreeMesh.h plCommon.h plMesh.h plVector3.h
objects/plOctreeMesh.o: plString.h plTriangle.h plMath.h plOctree.h
objects/plOctreeMesh.o: plRenderable.h plRenderComponent.h plVector4.h
objects/plOctreeMesh.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plOctreeMesh.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOctreeMesh.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plOctreeMesh.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plOctreeMesh.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plOctreeMesh.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plOctreeMesh.o: plRenderTechnique.h plWindow.h
objects/plOctreeMesh.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plOctreeMesh.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plOctreeMesh.o: plMinimalTechnique.h
objects/plOpenGLInfo.o: plOpenGLInfo.h plCommon.h
objects/plOutlineTechnique.o: plOutlineTechnique.h plCommon.h
objects/plOutlineTechnique.o: plRenderTechnique.h plRenderable.h plVector3.h
objects/plOutlineTechnique.o: plString.h plRenderComponent.h plVector4.h
objects/plOutlineTechnique.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plOutlineTechnique.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plOutlineTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plOutlineTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plOutlineTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plOutlineTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plPicking.o: plPicking.h plCommon.h plRenderResources.h plShader.h
objects/plPicking.o: plString.h plOpenGLInfo.h plUniform.h
objects/plPicking.o: plVertexFragmentShader.h plMatrix44.h plVector3.h
objects/plPicking.o: plVector4.h plTexture2D.h plFBO.h
objects/plPlan.o: plPlan.h plCommon.h plString.h plVector3.h plCSV.h
objects/plPlan.o: plRenderable.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plPlan.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlan.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlan.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlan.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlan.o: plGraft.h plArthroViewable.h plEditable.h plTriangle.h
objects/plPlan.o: plMath.h plDraw.h plPlug.h plMeshSpecific.h plMesh.h
objects/plPlan.o: plBoneAndCartilage.h plModel.h plOctreeMesh.h plOctree.h
objects/plPlan.o: plTransform.h plRenderer.h plPlanTechnique.h
objects/plPlan.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plPlan.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plPlan.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plPlan.o: plTransparentRenderable.h plPolygon.h plUtility.h
objects/plPlan.o: plSphere.h plGraftCap.h plCone.h plCylinder.h
objects/plPlan.o: plDefectSite.h plSpline.h plBoundary.h plColourMap.h
objects/plPlan.o: plDonorSite.h plIGuideSite.h plMeshCutter.h
objects/plPlan.o: plMeshExtruder.h plIGuide.h
objects/plPlanTechnique.o: plPlanTechnique.h plCommon.h plRenderTechnique.h
objects/plPlanTechnique.o: plRenderable.h plVector3.h plString.h
objects/plPlanTechnique.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plPlanTechnique.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlanTechnique.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanTechnique.o: plWindow.h
objects/plPlannerShader.o: plPlannerShader.h plCommon.h plShader.h plString.h
objects/plPlannerShader.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerShader.o: plVector4.h plVector3.h plTriangle.h plMath.h
objects/plPlannerShader.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plPlannerShader.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerShader.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerShader.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerShader.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerShader.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerShader.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerShader.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerShader.o: plRenderTechnique.h plWindow.h
objects/plPlannerShader.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerShader.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerShader.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerShader.o: plSSBO.h plBufferObject.h plPlannerStructs.h
objects/plPlannerShader.o: plUtility.h
objects/plPlannerStage0.o: plPlannerStage0.h plCommon.h plUtility.h
objects/plPlannerStage0.o: plVector4.h plString.h plVector3.h
objects/plPlannerStage0.o: plPlannerStructs.h plSSBO.h plBufferObject.h
objects/plPlannerStage0.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage0.o: plUniform.h plPlanningSite.h plTriangle.h plMath.h
objects/plPlannerStage0.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plPlannerStage0.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerStage0.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage0.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage0.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage0.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage0.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage0.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage0.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage0.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage0.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage0.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage0.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage0.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage0.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage0.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage0.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage0.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage0.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage0.o: plIGuide.h
objects/plPlannerStage1.o: plPlannerStage1.h plCommon.h plVector4.h
objects/plPlannerStage1.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage1.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage1.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage1.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage1.o: plTriangle.h plMath.h plBoundary.h plRenderable.h
objects/plPlannerStage1.o: plRenderComponent.h plMatrix44.h plTexture2D.h
objects/plPlannerStage1.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage1.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage1.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage1.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage1.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage1.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage1.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage1.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage1.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage1.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage1.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage1.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage1.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage1.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage1.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage1.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage1.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage1.o: plIGuide.h
objects/plPlannerStage2.o: plPlannerStage2.h plCommon.h plVector4.h
objects/plPlannerStage2.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage2.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage2.o: plPlannerStage1.h plPlannerStage0.h
objects/plPlannerStage2.o: plPlannerShader.h plShader.h plOpenGLInfo.h
objects/plPlannerStage2.o: plUniform.h plPlanningSite.h plTriangle.h plMath.h
objects/plPlannerStage2.o: plBoundary.h plRenderable.h plRenderComponent.h
objects/plPlannerStage2.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plPlannerStage2.o: plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage2.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage2.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage2.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage2.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage2.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage2.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage2.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage2.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage2.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage2.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage2.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage2.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage2.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage2.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage2.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage2.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage2.o: plIGuide.h
objects/plPlannerStage3.o: plPlannerStage3.h plCommon.h plVector4.h
objects/plPlannerStage3.o: plString.h plVector3.h plPlannerStructs.h
objects/plPlannerStage3.o: plUtility.h plSSBO.h plBufferObject.h
objects/plPlannerStage3.o: plPlannerStage0.h plPlannerShader.h plShader.h
objects/plPlannerStage3.o: plOpenGLInfo.h plUniform.h plPlanningSite.h
objects/plPlannerStage3.o: plTriangle.h plMath.h plBoundary.h plRenderable.h
objects/plPlannerStage3.o: plRenderComponent.h plMatrix44.h plTexture2D.h
objects/plPlannerStage3.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlannerStage3.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlannerStage3.o: plProjection.h plCamera.h plPicking.h
objects/plPlannerStage3.o: plRenderResources.h plVertexFragmentShader.h
objects/plPlannerStage3.o: plFBO.h plEditable.h plMeshSpecific.h plMesh.h
objects/plPlannerStage3.o: plDraw.h plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlannerStage3.o: plRenderTechnique.h plWindow.h
objects/plPlannerStage3.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlannerStage3.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlannerStage3.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlannerStage3.o: plPlanningBufferData.h plDefectSite.h plSpline.h
objects/plPlannerStage3.o: plColourMap.h plOctreeMesh.h plOctree.h
objects/plPlannerStage3.o: plTransform.h plDonorSite.h plPlan.h plCSV.h
objects/plPlannerStage3.o: plGraft.h plArthroViewable.h plPlug.h
objects/plPlannerStage3.o: plBoneAndCartilage.h plModel.h
objects/plPlannerStage3.o: plTransparentRenderable.h plGraftCap.h plCone.h
objects/plPlannerStage3.o: plCylinder.h plIGuideSite.h plMeshExtruder.h
objects/plPlannerStage3.o: plIGuide.h plPlannerStage2.h plPlannerStage1.h
objects/plPlannerStructs.o: plPlannerStructs.h plCommon.h plUtility.h
objects/plPlannerStructs.o: plVector4.h plString.h plVector3.h plSSBO.h
objects/plPlannerStructs.o: plBufferObject.h
objects/plPlanningBufferData.o: plPlanningBufferData.h plCommon.h plVector4.h
objects/plPlanningBufferData.o: plString.h plVector3.h plPlanningSite.h
objects/plPlanningBufferData.o: plTriangle.h plMath.h plBoundary.h
objects/plPlanningBufferData.o: plRenderable.h plRenderComponent.h
objects/plPlanningBufferData.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plPlanningBufferData.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plPlanningBufferData.o: plRenderingPipeline.h plMatrixStack.h
objects/plPlanningBufferData.o: plProjection.h plCamera.h plPicking.h
objects/plPlanningBufferData.o: plRenderResources.h plShader.h plUniform.h
objects/plPlanningBufferData.o: plVertexFragmentShader.h plFBO.h plEditable.h
objects/plPlanningBufferData.o: plMeshSpecific.h plMesh.h plDraw.h plSphere.h
objects/plPlanningBufferData.o: plRenderer.h plPlanTechnique.h
objects/plPlanningBufferData.o: plRenderTechnique.h plWindow.h
objects/plPlanningBufferData.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlanningBufferData.o: plArthroCamTechnique.h
objects/plPlanningBufferData.o: plTransparencyTechnique.h
objects/plPlanningBufferData.o: plMinimalTechnique.h plMeshCutter.h
objects/plPlanningBufferData.o: plPolygon.h plSSBO.h plBufferObject.h
objects/plPlanningBufferData.o: plDefectSite.h plSpline.h plColourMap.h
objects/plPlanningBufferData.o: plOctreeMesh.h plOctree.h plTransform.h
objects/plPlanningBufferData.o: plDonorSite.h
objects/plPlanningSite.o: plPlanningSite.h plCommon.h plVector4.h plString.h
objects/plPlanningSite.o: plVector3.h plTriangle.h plMath.h plBoundary.h
objects/plPlanningSite.o: plRenderable.h plRenderComponent.h plMatrix44.h
objects/plPlanningSite.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plPlanningSite.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plPlanningSite.o: plMatrixStack.h plProjection.h plCamera.h
objects/plPlanningSite.o: plPicking.h plRenderResources.h plShader.h
objects/plPlanningSite.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlanningSite.o: plEditable.h plMeshSpecific.h plMesh.h plDraw.h
objects/plPlanningSite.o: plSphere.h plRenderer.h plPlanTechnique.h
objects/plPlanningSite.o: plRenderTechnique.h plWindow.h
objects/plPlanningSite.o: plScreenQuadTechnique.h plOutlineTechnique.h
objects/plPlanningSite.o: plArthroCamTechnique.h plTransparencyTechnique.h
objects/plPlanningSite.o: plMinimalTechnique.h plMeshCutter.h plPolygon.h
objects/plPlanningSite.o: plSSBO.h plBufferObject.h
objects/plPlug.o: plPlug.h plCommon.h plMeshSpecific.h plMesh.h plVector3.h
objects/plPlug.o: plString.h plTriangle.h plMath.h plBoneAndCartilage.h
objects/plPlug.o: plModel.h plOctreeMesh.h plOctree.h plRenderable.h
objects/plPlug.o: plRenderComponent.h plVector4.h plMatrix44.h plTexture2D.h
objects/plPlug.o: plVAO.h plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plPlug.o: plOpenGLInfo.h plRenderingPipeline.h plMatrixStack.h
objects/plPlug.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plPlug.o: plShader.h plUniform.h plVertexFragmentShader.h plFBO.h
objects/plPlug.o: plTransform.h plDraw.h plRenderer.h plPlanTechnique.h
objects/plPlug.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plPlug.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plPlug.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plPlug.o: plTransparentRenderable.h plArthroViewable.h
objects/plPolygon.o: plPolygon.h plCommon.h plVector3.h plString.h
objects/plProjection.o: plProjection.h plCommon.h plMatrix44.h plVector3.h
objects/plProjection.o: plString.h plVector4.h
objects/plRenderComponent.o: plRenderComponent.h plCommon.h plVector3.h
objects/plRenderComponent.o: plString.h plVector4.h plMatrix44.h
objects/plRenderComponent.o: plTexture2D.h plVAO.h plVBO.h
objects/plRenderComponent.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plRenderComponent.o: plRenderingPipeline.h plMatrixStack.h
objects/plRenderComponent.o: plProjection.h plCamera.h plPicking.h
objects/plRenderComponent.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderComponent.o: plVertexFragmentShader.h plFBO.h
objects/plRenderResources.o: plRenderResources.h plCommon.h plShader.h
objects/plRenderResources.o: plString.h plOpenGLInfo.h plUniform.h
objects/plRenderResources.o: plVertexFragmentShader.h plMatrix44.h
objects/plRenderResources.o: plVector3.h plVector4.h plTexture2D.h plFBO.h
objects/plRenderTechnique.o: plRenderTechnique.h plCommon.h plRenderable.h
objects/plRenderTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plRenderTechnique.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plRenderTechnique.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plRenderTechnique.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plRenderTechnique.o: plMatrixStack.h plProjection.h plCamera.h
objects/plRenderTechnique.o: plPicking.h plRenderResources.h plShader.h
objects/plRenderTechnique.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plRenderTechnique.o: plWindow.h
objects/plRenderable.o: plRenderable.h plCommon.h plVector3.h plString.h
objects/plRenderable.o: plRenderComponent.h plVector4.h plMatrix44.h
objects/plRenderable.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plRenderable.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plRenderable.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plRenderable.o: plRenderResources.h plShader.h plUniform.h
objects/plRenderable.o: plVertexFragmentShader.h plFBO.h
objects/plRenderer.o: plRenderer.h plCommon.h plVector3.h plString.h
objects/plRenderer.o: plRenderingPipeline.h plVector4.h plMatrixStack.h
objects/plRenderer.o: plMatrix44.h plProjection.h plCamera.h plPicking.h
objects/plRenderer.o: plRenderResources.h plShader.h plOpenGLInfo.h
objects/plRenderer.o: plUniform.h plVertexFragmentShader.h plTexture2D.h
objects/plRenderer.o: plFBO.h plRenderable.h plRenderComponent.h plVAO.h
objects/plRenderer.o: plVBO.h plVertexSpecBuffer.h plEABO.h plPlanTechnique.h
objects/plRenderer.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plRenderer.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plRenderer.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plRenderingPipeline.o: plRenderingPipeline.h plCommon.h plVector4.h
objects/plRenderingPipeline.o: plString.h plVector3.h plMatrixStack.h
objects/plRenderingPipeline.o: plMatrix44.h plProjection.h plCamera.h
objects/plRenderingPipeline.o: plPicking.h plRenderResources.h plShader.h
objects/plRenderingPipeline.o: plOpenGLInfo.h plUniform.h
objects/plRenderingPipeline.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plSSBO.o: plSSBO.h plCommon.h plBufferObject.h
objects/plScan.o: plScan.h plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScan.o: plScanVolume.h plDraw.h plVAO.h plVBO.h
objects/plScan.o: plVertexSpecBuffer.h plVector4.h plEABO.h plOpenGLInfo.h
objects/plScan.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScan.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plScan.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plScan.o: plTexture2D.h plFBO.h
objects/plScanPoint.o: plScanPoint.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plScanVolume.h plVector3.h plCommon.h plString.h
objects/plScanVolume.o: plDraw.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plScanVolume.o: plVector4.h plEABO.h plOpenGLInfo.h
objects/plScanVolume.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plScanVolume.o: plProjection.h plCamera.h plPicking.h
objects/plScanVolume.o: plRenderResources.h plShader.h plUniform.h
objects/plScanVolume.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
objects/plScreenQuadTechnique.o: plScreenQuadTechnique.h plCommon.h
objects/plScreenQuadTechnique.o: plRenderTechnique.h plRenderable.h
objects/plScreenQuadTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plScreenQuadTechnique.o: plVector4.h plMatrix44.h plTexture2D.h
objects/plScreenQuadTechnique.o: plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plScreenQuadTechnique.o: plEABO.h plOpenGLInfo.h
objects/plScreenQuadTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plScreenQuadTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plScreenQuadTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plScreenQuadTechnique.o: plVertexFragmentShader.h plFBO.h plWindow.h
objects/plShader.o: plShader.h plCommon.h plString.h plOpenGLInfo.h
objects/plShader.o: plUniform.h
objects/plSphere.o: plSphere.h plCommon.h plRenderable.h plVector3.h
objects/plSphere.o: plString.h plRenderComponent.h plVector4.h plMatrix44.h
objects/plSphere.o: plTexture2D.h plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plSphere.o: plEABO.h plOpenGLInfo.h plRenderingPipeline.h
objects/plSphere.o: plMatrixStack.h plProjection.h plCamera.h plPicking.h
objects/plSphere.o: plRenderResources.h plShader.h plUniform.h
objects/plSphere.o: plVertexFragmentShader.h plFBO.h
objects/plSpline.o: plSpline.h plCommon.h plVector3.h plString.h plVector4.h
objects/plSpline.o: plBoundary.h plMath.h plRenderable.h plRenderComponent.h
objects/plSpline.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plSpline.o: plVertexSpecBuffer.h plEABO.h plOpenGLInfo.h
objects/plSpline.o: plRenderingPipeline.h plMatrixStack.h plProjection.h
objects/plSpline.o: plCamera.h plPicking.h plRenderResources.h plShader.h
objects/plSpline.o: plUniform.h plVertexFragmentShader.h plFBO.h plEditable.h
objects/plSpline.o: plMeshSpecific.h plMesh.h plTriangle.h plDraw.h
objects/plSpline.o: plSphere.h plRenderer.h plPlanTechnique.h
objects/plSpline.o: plRenderTechnique.h plWindow.h plScreenQuadTechnique.h
objects/plSpline.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plSpline.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plSpline.o: plColourMap.h plOctreeMesh.h plOctree.h plTransform.h
objects/plString.o: plString.h plCommon.h
objects/plTexture2D.o: plTexture2D.h plCommon.h plVector3.h plString.h
objects/plTexture3D.o: plTexture3D.h plCommon.h plVector3.h plString.h
objects/plThreadPool.o: plThreadPool.h
objects/plTrackedObject.o: plTrackedObject.h plCommon.h plRenderable.h
objects/plTrackedObject.o: plVector3.h plString.h plRenderComponent.h
objects/plTrackedObject.o: plVector4.h plMatrix44.h plTexture2D.h plVAO.h
objects/plTrackedObject.o: plVBO.h plVertexSpecBuffer.h plEABO.h
objects/plTrackedObject.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plTrackedObject.o: plMatrixStack.h plProjection.h plCamera.h
objects/plTrackedObject.o: plPicking.h plRenderResources.h plShader.h
objects/plTrackedObject.o: plUniform.h plVertexFragmentShader.h plFBO.h
objects/plTrackedObject.o: plDRBTransform.h plCylinder.h plCone.h
objects/plTrackedObject.o: plRenderer.h plPlanTechnique.h plRenderTechnique.h
objects/plTrackedObject.o: plWindow.h plScreenQuadTechnique.h
objects/plTrackedObject.o: plOutlineTechnique.h plArthroCamTechnique.h
objects/plTrackedObject.o: plTransparencyTechnique.h plMinimalTechnique.h
objects/plTransform.o: plTransform.h plCommon.h plString.h plVector3.h
objects/plTransform.o: plMatrix44.h plVector4.h
objects/plTransparencyTechnique.o: plTransparencyTechnique.h plCommon.h
objects/plTransparencyTechnique.o: plRenderTechnique.h plRenderable.h
objects/plTransparencyTechnique.o: plVector3.h plString.h plRenderComponent.h
objects/plTransparencyTechnique.o: plVector4.h plMatrix44.h plTexture2D.h
objects/plTransparencyTechnique.o: plVAO.h plVBO.h plVertexSpecBuffer.h
objects/plTransparencyTechnique.o: plEABO.h plOpenGLInfo.h
objects/plTransparencyTechnique.o: plRenderingPipeline.h plMatrixStack.h
objects/plTransparencyTechnique.o: plProjection.h plCamera.h plPicking.h
objects/plTransparencyTechnique.o: plRenderResources.h plShader.h plUniform.h
objects/plTransparencyTechnique.o: plVertexFragmentShader.h plFBO.h
objects/plTransparencyTechnique.o: plWindow.h
objects/plTransparentRenderable.o: plTransparentRenderable.h plCommon.h
objects/plTransparentRenderable.o: plRenderable.h plVector3.h plString.h
objects/plTransparentRenderable.o: plRenderComponent.h plVector4.h
objects/plTransparentRenderable.o: plMatrix44.h plTexture2D.h plVAO.h plVBO.h
objects/plTransparentRenderable.o: plVertexSpecBuffer.h plEABO.h
objects/plTransparentRenderable.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plTransparentRenderable.o: plMatrixStack.h plProjection.h plCamera.h
objects/plTransparentRenderable.o: plPicking.h plRenderResources.h plShader.h
objects/plTransparentRenderable.o: plUniform.h plVertexFragmentShader.h
objects/plTransparentRenderable.o: plFBO.h
objects/plTriangle.o: plTriangle.h plCommon.h plString.h plVector3.h plMath.h
objects/plUniform.o: plUniform.h plCommon.h
objects/plUtility.o: plUtility.h plCommon.h
objects/plVAO.o: plVAO.h plCommon.h plVBO.h plVertexSpecBuffer.h plVector4.h
objects/plVAO.o: plString.h plVector3.h plEABO.h plOpenGLInfo.h
objects/plVAO.o: plRenderingPipeline.h plMatrixStack.h plMatrix44.h
objects/plVAO.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plVAO.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plVAO.o: plTexture2D.h plFBO.h
objects/plVBO.o: plVBO.h plCommon.h plVertexSpecBuffer.h plVector4.h
objects/plVBO.o: plString.h plVector3.h
objects/plVector3.o: plVector3.h plCommon.h plString.h plVector4.h
objects/plVector4.o: plVector4.h plCommon.h plString.h plVector3.h
objects/plVertexFragmentShader.o: plVertexFragmentShader.h plCommon.h
objects/plVertexFragmentShader.o: plShader.h plString.h plOpenGLInfo.h
objects/plVertexFragmentShader.o: plUniform.h plMatrix44.h plVector3.h
objects/plVertexFragmentShader.o: plVector4.h plTexture2D.h
objects/plVertexSpecBuffer.o: plVertexSpecBuffer.h plCommon.h plVAO.h plVBO.h
objects/plVertexSpecBuffer.o: plVector4.h plString.h plVector3.h plEABO.h
objects/plVertexSpecBuffer.o: plOpenGLInfo.h plRenderingPipeline.h
objects/plVertexSpecBuffer.o: plMatrixStack.h plMatrix44.h plProjection.h
objects/plVertexSpecBuffer.o: plCamera.h plPicking.h plRenderResources.h
objects/plVertexSpecBuffer.o: plShader.h plUniform.h plVertexFragmentShader.h
objects/plVertexSpecBuffer.o: plTexture2D.h plFBO.h
objects/plWindow.o: plWindow.h plCommon.h plMatrix44.h plVector3.h plString.h
objects/plWindow.o: plVector4.h plRenderingPipeline.h plMatrixStack.h
objects/plWindow.o: plProjection.h plCamera.h plPicking.h plRenderResources.h
objects/plWindow.o: plShader.h plOpenGLInfo.h plUniform.h
objects/plWindow.o: plVertexFragmentShader.h plTexture2D.h plFBO.h
