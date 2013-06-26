
LIBS = -Llib -lGLEW -lglut -lGLU -lGL -lm 
#LIBS = -framework OpenGL -framework GLUT -framework Cocoa

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJS = main.o Window.o ArcballWindow.o PlannerWindow.o pl.o \
       plVector3.o plVector4.o plString.o plMath.o plDraw.o plMesh.o plColourMesh.o plColourMap.o plProjection.o plState.o \
       plMatrix44.o plPickingTexture.o plTriangle.o plModel.o plGraft.o plTransform.o plPlan.o plBoundary.o plSpline.o plCSV.o \
       plCamera.o plRenderable.o plError.o
       
EXE  = planner

Window:	$(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS) $(LIBS)

.C.o:
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f *.o *~ core $(EXE) Makefile.bak

depend:	
	makedepend -Y *.h *.cpp

