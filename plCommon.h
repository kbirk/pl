#ifndef _PL_GLOBAL_HEADERS_H__
#define _PL_GLOBAL_HEADERS_H__

#ifdef WIN32
    #include <windows.h>  
#endif

#ifdef LINUX
    //#include <sys/timeb.h>	// includes ftime (to return current time)
    //#include <unistd.h>		// includes usleep (to sleep for some time)
    //#include <values.h>
#endif

#ifdef __APPLE_CC__
    #include <glew.h>    // include before gl.h
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/glew.h> // include before gl.h
    #include <GL/glut.h>
    #include <GL/glu.h>
    #include <GL/gl.h>
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <cfloat>
#include <cmath>
#include <vector>


#endif
