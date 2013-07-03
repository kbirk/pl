LIBS = -Llib -lGLEW -lglut -lGLU -lGL -lm 
#LIBS = -framework OpenGL -framework GLUT -framework Cocoa

INCS = 

CXXFLAGS = -O3 -std=c++0x -DLINUX $(INCS)

CXX = g++

OBJECTS = $(patsubst %.cpp,objects/%.o,$(SOURCES))

SOURCES = main.cpp Window.cpp ArcballWindow.cpp PlannerWindow.cpp pl.cpp \
	plVector3.cpp plVector4.cpp plString.cpp plMath.cpp plDraw.cpp plMesh.cpp \
	plColourMesh.cpp plColourMap.cpp plProjection.cpp plState.cpp \
	plMatrix44.cpp plPickingTexture.cpp plTriangle.cpp plModel.cpp plGraft.cpp \
	plTransform.cpp plPlan.cpp plBoundary.cpp plSpline.cpp plCSV.cpp \
	plCamera.cpp plRenderable.cpp plError.cpp plDonorSite.cpp plIGuide.cpp
       
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
	makedepend -Y *.h *.cpp

